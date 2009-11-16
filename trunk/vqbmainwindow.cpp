/* pastebin part license: Thomas Georgiou <TAGeorgiou@gmail.com> */

#include "vqbmainwindow.h"
#include "ui_vqbmainwindow.h"
#include "vqbform.h"
#include "vqbglobal.h"
#include "vqbschemaform.h"
#include "vqbinstancesform.h"
#include "sparqlhighlighter.h"

#include <QProcess>
#include <QClipboard>
#include <QTimer>

#include <KDebug>
#include <KFileDialog>
#include <KSaveFile>
#include <KUrl>
#include <KConfigGroup>
#include <KInputDialog>


VqbMainWindow::VqbMainWindow(QWidget *parent) :
    QMainWindow(parent), m_mainForm(0), m_ui(new Ui::VqbMainWindow), m_pastebinApiKey("6bqWgxxnkOQpahdgoD3vuG05aNwazhan")
{
    m_ui->setupUi(this);
    init();
}

VqbMainWindow::~VqbMainWindow()
{
    delete m_ui;
}

void VqbMainWindow::init()
{
    m_ui->buttonUp->setIcon(KIcon("arrow-up"));
    connect(m_ui->buttonUp, SIGNAL(clicked()), this, SLOT(moveOutputUp()));
    m_ui->buttonDown->setIcon(KIcon("arrow-down"));
    connect(m_ui->buttonDown, SIGNAL(clicked()), this, SLOT(moveOutputDown()));
    m_ui->buttonRemove->setIcon(KStandardGuiItem::remove().icon());
    connect(m_ui->buttonRemove, SIGNAL(clicked()), this, SLOT(removeOutput()));

    new SparqlHighlighter(m_ui->queryViewer);

    connect(m_ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

    QTimer::singleShot(0, this, SLOT(initStartupMenu()));

}

void VqbMainWindow::initStartupMenu()
{
    showStartupMenu(true);
}

void VqbMainWindow::addVarToOutput(QString var)
{
    m_ui->outputList->addItem(var);
    refreshQuery();
}

void VqbMainWindow::queryChanged(QString queryPart)
{
    m_queryPart = queryPart;
    refreshQuery();
}

void VqbMainWindow::refreshQuery()
{
    QString query = "SELECT DISTINCT ";
    foreach(QListWidgetItem *item, m_ui->outputList->findItems(QString(), Qt::MatchContains)) {
        query.append(item->text() + " ");
    }
    query.append(" \n WHERE { \n");
    query.append(m_queryPart);
    query.append("}\n");
    m_ui->queryViewer->setText(VqbGlobal::addPrefixes(query));
}

void VqbMainWindow::moveOutputUp()
{
    QListWidget* lw = m_ui->outputList;
    int i = lw->currentRow();
    if(i > 0) {
        QListWidgetItem *item = lw->takeItem(i);
        lw->insertItem(i-1, item);
        lw->setCurrentRow(i-1);
    }
    refreshQuery();

}
void VqbMainWindow::moveOutputDown()
{
    QListWidget* lw = m_ui->outputList;
    int i = lw->currentRow();
    if(i < lw->count()-1) {
        QListWidgetItem *item = lw->takeItem(i);
        lw->insertItem(i+1, item);
        lw->setCurrentRow(i+1);
    }
    refreshQuery();
}
void VqbMainWindow::removeOutput()
{
    m_ui->outputList->takeItem( m_ui->outputList->currentRow() );
    refreshQuery();
}


void VqbMainWindow::tabChanged(int index)
{
    QString q;
    if (index == 1) {
        q = m_ui->queryViewer->toPlainText();
        q.replace("\n", " ");

        QString cmd = "sopranocmd --dbus org.kde.NepomukStorage --model main --querylang SPARQL query \"";
        cmd.append(q);

        QProcess script(this);
        script.start(cmd);

        if (!script.waitForStarted()) {
            kDebug() << "Query could not be started.";
            //return;
        }

        if (!script.waitForFinished()) {
            kDebug() << "Query could not be finished.";
            //return;
        }

        kDebug() << "Ran query: " << q;
        QString result = QString(script.readAllStandardOutput().data()) + (QString) script.readAllStandardError().data();
        m_ui->queryResults->setText(result);
    }
}

/***************************** Menu actions ********************************/


void VqbMainWindow::on_actionSave_triggered()
{
    QString outputFileName = KFileDialog::getSaveFileName();//KUrl(), QString("*.*|All files"), this, QString());

    KSaveFile file(outputFileName);
    file.open();
    QByteArray outputByteArray;
    outputByteArray.append(m_ui->queryViewer->toPlainText().toUtf8());
    file.write(outputByteArray);
    if( file.finalize() ) {
        m_ui->statusBar->showMessage("File " + outputFileName + " saved successfully.");//show it for 30 seconds
    }
    else {
        m_ui->statusBar->showMessage("Error saving file " + outputFileName + "!");//show it for 30 seconds
    }
    file.close();
}


void VqbMainWindow::on_actionExit_triggered()
{
    close();
}


void VqbMainWindow::on_action_Return_to_Startup_triggered()
{
    showStartupMenu(false);
}

void VqbMainWindow::showStartupMenu(bool exitOnCancel)
{
    hide();
    if(m_mainForm) {
        m_mainForm->hide();
    }

    QString choice = KInputDialog::getItem("Please choose interface mode", "VQB Startup", QStringList() << "Schema-based" << "Instance-based", 0, false);

    if(choice == "Schema-based") {
        if(m_mainForm) {
            m_mainForm->hide();
            m_mainForm->deleteLater();
        }
        m_mainForm = new VqbSchemaForm(this);
    }
    else if (choice == "Instance-based") {
        if(m_mainForm) {
            m_mainForm->hide();
            m_mainForm->deleteLater();
        }
        m_mainForm = new VqbInstancesForm(this);
    }
    else if (exitOnCancel) { //Cancel at first startup
        close();
        return;
    }
    else { //Cancel
        show();
        m_mainForm->show();
        return;
    }

    //remove old VqbForm
    show();
    m_mainForm->show();
    m_ui->scrollArea->setWidget(m_mainForm);
    m_ui->queryViewer->clear();
}

void VqbMainWindow::on_actionPostToPastebin_triggered()
{
    QByteArray bytearray = "content=";
    bytearray.append(QUrl::toPercentEncoding(m_ui->queryViewer->toPlainText(),"/"));
    bytearray.append("&api=");
    bytearray.append(m_pastebinApiKey);
    bytearray.append("&description=&type=16&expiry=1%20day&name=");//SQL statement, with a 1 day expiry period

    m_pastebinData.clear();

    KIO::TransferJob *tf = KIO::http_post(KUrl("http://pastebin.ca/quiet-paste.php"),bytearray,KIO::HideProgressInfo);
    tf->addMetaData("content-type","Content-Type: application/x-www-form-urlencoded");
    connect(tf, SIGNAL(data(KIO::Job*, const QByteArray&)),
            this, SLOT(readKIOData(KIO::Job*, const QByteArray&)));

    connect(tf, SIGNAL(result(KJob *)), this, SLOT(finished(KJob *)));
}

void VqbMainWindow::finished(KJob *job)
{
    Q_UNUSED(job);

    if (m_pastebinData.length() == 0 || m_pastebinData.contains("FAIL")
        || !m_pastebinData.contains("SUCCESS")) {
        m_ui->statusBar->showMessage("Error posting to pastebin.ca");
        return;
    }

    QString url(m_pastebinData);
    url.remove("SUCCESS:");
    url.prepend(QString("http://pastebin.ca/"));
    //FIXME: success action
    m_ui->statusBar->showMessage("Successfully posted to " + url + " (link copied to clipboard).");
    QApplication::clipboard()->setText(url);
}

void VqbMainWindow::readKIOData(KIO::Job *job, const QByteArray &data)
{
    Q_UNUSED(job);

    if (data.length() == 0) {
        return;
    }

    m_pastebinData.append(data);
}
