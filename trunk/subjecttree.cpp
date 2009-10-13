#include "subjecttree.h"
#include "querynode.h"
#include "vqbform.h"

#include <QVBoxLayout>

#include <KAction>
#include <KStandardAction>



SubjectTree::SubjectTree(int treeNumber, QWidget * parent)
        : QGroupBox(parent)
{
    setTreeNumber(treeNumber);
    init();
}

void SubjectTree::init()
{
    m_root = new QueryNode();
    QVBoxLayout *l = new QVBoxLayout();
    this->setLayout(l);
    l->addLayout(m_root);

    connect(m_root, SIGNAL(queryPartChanged(QString)), this, SLOT(updateQueryTree(QString)));
    connect(m_root, SIGNAL(addVarToOutput(QString)), parent(), SLOT(addVarToOutput(QString)));
    connect(m_root, SIGNAL(removeClicked(QueryNode*)), this, SLOT(close()));

    QAction *removeAction = KStandardAction::close(this, SLOT(close()), this);
    removeAction->setText(tr("Remove &subject tree"));
    removeAction->setStatusTip(tr("Removes this constraint from the query and GUI"));
    removeAction->setShortcut(0);
    /*QAction *refresh = KStandardAction::redisplay(this, SLOT(rebuildQueryTree()), this);
    refresh->setText(tr("&Refresh query"));
    refresh->setShortcut(0);
    this->addAction(refresh);
    */
    this->addAction(removeAction);
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
    setAttribute(Qt::WA_DeleteOnClose);   //delete when closed
}

void SubjectTree::updateQueryTree(QString queryTree)
{
    emit queryTreeChanged(m_treeNumber, queryTree);
}

void SubjectTree::setTreeNumber(int treeNumber)
{
    m_treeNumber = treeNumber;
    setTitle("Subject " + QString::number(m_treeNumber + 1));
}

void SubjectTree::close()
{
    emit queryTreeDeleted(m_treeNumber);
    QGroupBox::close();
}

#include "subjecttree.moc"
