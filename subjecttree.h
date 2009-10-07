#ifndef SUBJECTTREE_H
#define SUBJECTTREE_H

#include <QGroupBox>

class SubjectTree : public QGroupBox
{
public:
    SubjectTree( int constraintNo, QWidget * parent = 0 );

private:
    class Private;
    Private * const d;
};

#endif // SUBJECTTREE_H
