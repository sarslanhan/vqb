/* pastebin part license: Thomas Georgiou <TAGeorgiou@gmail.com> */

#include "vqbmainwindow.h"
#include "ui_vqbmainwindow.h"
#include "vqbform.h"
#include "vqbglobal.h"
#include "vqbschemaselect.h"
#include "vqbschemaconstruct.h"
#include "vqbinstancesselect.h"
#include "vqbinstancesconstruct.h"
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
#include <KMessageBox>


VqbMainWindow::VqbMainWindow(QWidget *parent) :
    QMainWindow(parent), m_mainForm(0), m_ui(new Ui::VqbMainWindow), m_pastebinApiKey("c4bnx61QpMPA6gxK8BCh0qR3og4utbPQ")
{
    showStartupMenu(true);
    m_ui->setupUi(this);
    
    new SparqlHighlighter(m_ui->queryViewer);
    connect(m_ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

    //m_ui->dockWidget->resize(751, 221);

    initMainForm();
}

VqbMainWindow::~VqbMainWindow()
{
    delete m_ui;
}

void VqbMainWindow::initMainForm()
{
    m_ui->scrollArea->setWidget(m_mainForm);
    m_ui->queryViewer->clear();
}

void VqbMainWindow::initStartupMenu()
{
    showStartupMenu(true);
}

void VqbMainWindow::queryChanged(QString query)
{
    m_ui->queryViewer->setText(VqbGlobal::addPrefixes(query));
    //FIXME: convert all URIs to prefixes
}

void VqbMainWindow::tabChanged(int index)
{
    //FIXME: remove tabs. Have a live result previewer?
    QString q;
    if (index == 1) {
        q = m_ui->queryViewer->toPlainText();
        q.replace("\n", " ");
        q.append(" LIMIT 100");

        QString cmd = "sopranocmd --dbus org.kde.NepomukStorage --model main --querylang SPARQL query \"";
        cmd.append(q);

        QProcess script(this);
        script.start(cmd);

        if (!script.waitForStarted()) {
            //kDebug() << "Query could not be started.";
            //return;
        }

        if (!script.waitForFinished()) {
            //kDebug() << "Query could not be finished.";
            //return;
        }

        //kDebug() << "Ran query: " << q;
        QString result = QString(script.readAllStandardOutput().data()) + (QString) script.readAllStandardError().data();
        m_ui->queryResults->setText(result);
    }
}

void VqbMainWindow::postErrorMessage(QString message)
{
    m_ui->statusBar->showMessage(message);
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
    int answer = KMessageBox::questionYesNoCancel(this, "Returning to startup will result in losing all previouse work. Continue?",
                    "Confirm Return to Startup");

    if(answer == KMessageBox::Yes) {
        showStartupMenu(false);
        initMainForm();
    }
    //otherwise do nothing
}

void VqbMainWindow::showStartupMenu(bool exitOnCancel)
{
    Q_UNUSED(exitOnCancel);

    hide();
    if(m_mainForm) {
        m_mainForm->hide();
        m_mainForm->deleteLater();
    }

    QStringList options;
    options << "Schema-based SELECT" << "Schema-based CONSTRUCT" << "Instance-based SELECT" << "Instance-based CONSTRUCT";
    QString choice = KInputDialog::getItem("Please choose query building mode", "VQB Startup", options , 0, false);
        
    if(choice == options[0]) {
        m_mainForm = new VqbSchemaSelect(this);
    }
    else if (choice == options[1]) {
        m_mainForm = new VqbSchemaConstruct(this);
    }    
    else if (choice == options[2]) {
        m_mainForm = new VqbInstancesSelect(this);
    }
    else if (choice == options[3]) {
        m_mainForm = new VqbInstancesConstruct(this);
    }
    else {
        close();
        return;
    }

    show();
    m_mainForm->show();    
    m_mainForm->activateWindow();
    m_mainForm->setFocus(Qt::OtherFocusReason);
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

void VqbMainWindow::on_dockWidget_topLevelChanged(bool topLevel)
{
    if(topLevel) {
        //kDebug() << "Toplevel";
        resize(size()-QSize(0, m_ui->dockWidget->size().height()));
    }
    else {
        resize(size()+QSize(0, m_ui->dockWidget->size().height()));
    }
}
