#include "subjecttree.h"
#include "querynode.h"

#include <QVBoxLayout>

#include <KAction>
#include <KStandardAction>



SubjectTree::SubjectTree(int constraintNo, QWidget * parent)
        : QGroupBox(parent)
{
    m_constraintNo = constraintNo;
    init();
}

void SubjectTree::init()
{
    setTitle("Subject " + QString::number(m_constraintNo + 1));

    m_root = new QueryNode();
    QVBoxLayout *l = new QVBoxLayout();
    this->setLayout(l);
    l->addLayout( m_root );

    QAction *removeAction = KStandardAction::close(this, SLOT(close()), this);
    removeAction->setText(tr("Remove &subject tree"));
    removeAction->setStatusTip(tr("Removes this constraint from the query and GUI"));
    removeAction->setShortcut(0);
    QAction *refresh = KStandardAction::redisplay(this, SLOT(rebuildQueryPart()), this);
    refresh->setText(tr("&Refresh query"));
    refresh->setShortcut(0);

    this->addAction(removeAction);
    this->addAction(refresh);
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
    setAttribute(Qt::WA_DeleteOnClose);   //delete when closed
}

void SubjectTree::rebuildQueryPart()
{
    emit queryPartChanged(m_constraintNo, "Oszkar a Human");
}

#include "subjecttree.moc"
