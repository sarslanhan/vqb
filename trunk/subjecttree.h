#ifndef SUBJECTTREE_H
#define SUBJECTTREE_H

#include <QGroupBox>
class QueryNode;

class SubjectTree : public QGroupBox
{
    Q_OBJECT
public:
    SubjectTree(int constraintNo, QWidget* parent = 0);

public slots:
    void updateQueryTree(QString queryPart);
    void setTreeNumber(int treeNumber);
    void close();

signals:
    void queryTreeChanged(int, QString);
    void queryTreeDeleted(int);

private:
    void init();

    int m_treeNumber;
    QueryNode *m_root;
};

#endif // SUBJECTTREE_H
