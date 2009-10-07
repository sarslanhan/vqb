#include "subjecttree.h"
#include "subjectnode.h"

#include <QVBoxLayout>

SubjectTree::SubjectTree( int constraintNo, QWidget * parent = 0 )
        : QGroupBox( parent )
{
    d->constraintNo = constraintNo;
    this->setTitle( "Subject " + QString::number( constraintNo ) );

    d->root = new SubjectNode();

    this->setLayout( new QVBoxLayout() );

    //add root's subjectCB and restrictionLayout to this->layout()

}

class SubjectTree::Private
{
public:
    int constraintNo;

    SubjectNode *root;
};

#include "subjecttree.moc"
