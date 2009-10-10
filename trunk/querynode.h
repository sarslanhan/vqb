#ifndef QUERYNODE_H
#define QUERYNODE_H

#include <QVBoxLayout>

class StringPair;

class QueryNode : public QVBoxLayout
{
    Q_OBJECT
public:
    QueryNode(QString parentClass = QString());//filtering class

    /** Returns the query part contributed by this node
      */
    QString queryPart();

public slots:
    void addSubjects(QList<StringPair> subjectList);
    void addPredicates(QList<StringPair> predicateList);

signals:
    void queryPartChanged(QString queryPart);
    void addVarToOutput(QString varName);

private:
    void init();

    class Private;
    Private * const d;

private slots:
    void addRestriction();
    void addObjectToLayout();

    void findObjects();
    void findPredicates();

    void updateQueryPart();
};

#endif // QUERYNODE_H
