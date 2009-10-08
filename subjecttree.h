#ifndef SUBJECTTREE_H
#define SUBJECTTREE_H

#include <QGroupBox>
class QueryNode;

class SubjectTree : public QGroupBox
{
    Q_OBJECT
public:
    SubjectTree(int constraintNo, QWidget * parent = 0);

public slots:
    void rebuildQueryPart();

signals:
    void queryPartChanged(int, QString);

private:
    void init();

    int m_constraintNo;
    QueryNode *m_root;
};

#endif // SUBJECTTREE_H
