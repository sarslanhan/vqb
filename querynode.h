#ifndef QUERYNODE_H
#define QUERYNODE_H

#include <QVBoxLayout>

class StringPair;

class QueryNode : public QVBoxLayout
{
    Q_OBJECT
public:
    QueryNode(QString parentClass = QString());//filtering class

public slots:
    void addSubjects( QList<StringPair> subjectList );
    void addPredicates( QList<StringPair> predicateList );

private:
    void init();

    class Private;
    Private * const d;

private slots:
    void addRestriction();
    void addObjectToLayout();

    void findObjects();
    void findPredicates();

};

#endif // QUERYNODE_H
