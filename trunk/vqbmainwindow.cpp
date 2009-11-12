#include "vqbmainwindow.h"
#include "ui_vqbmainwindow.h"
#include "vqbform.h"
#include "vqbglobal.h"
#include "vqbschemaform.h"
#include "vqbinstancesform.h"
#include "sparqlhighlighter.h"

#include <QProcess>
#include <QInputDialog>

#include <KDebug>


VqbMainWindow::VqbMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::VqbMainWindow)
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
    //kDebug() << KIconLoader::global()->queryIcons(KIconLoader::Desktop);

    new SparqlHighlighter(m_ui->queryViewer);

    connect(m_ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

    //FIXME: get this on a different thread?
    QString choice = QInputDialog::getItem(this, "Please choose interface mode", "Interface choser", QStringList() << "Schema-based" << "Instance-based", 0, false);

    //FIXME: test and refine this
    VqbForm *vqbForm;
    if(choice == "Schema-based") {
        vqbForm = new VqbSchemaForm(this);
    }
    else {
        vqbForm = new VqbInstancesForm(this);
    }

    m_ui->scrollArea->setWidget(vqbForm);
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
