#ifndef QUERYNODE_H
#define QUERYNODE_H

#include <QVBoxLayout>
#include "querythread.h"

class QueryNode : public QVBoxLayout
{
    Q_OBJECT
public:
    QueryNode(QString parentClass = QString());//filtering class
    ~QueryNode();

    /** Returns the query part contributed by this node
      */
    QString queryPart();

public slots:
    void addSubjects(QList<QStringPair> subjectList);
    void addPredicates(QList<QStringPair > predicateList);

signals:
    void queryPartChanged(QString queryPart);
    void addVarToOutput(QString varName);
    void removeClicked(QueryNode *qn);

private:
    void init();

    class Private;
    Private * const d;

private slots:
    void addRestriction();
    void addObjectToLayout();

    void emitRemove();
    void removeRestrictions();
    void removeRestriction(QueryNode *qn);


    void findObjects();
    void findPredicates();

    void updateQueryPart();
};

#endif // QUERYNODE_H
