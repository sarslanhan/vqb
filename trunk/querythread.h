#ifndef QUERYTHREAD_H
#define QUERYTHREAD_H

#include <QThread>
#include <QPair>

typedef QPair<QString, QString> QStringPair;

namespace Soprano
{
class Model;
}

/*! Thread class that executes queries (and performs some utility functions)
*/

class QueryThread : public QThread
{
    Q_OBJECT

public:
    enum QueryMode{ SingleQuery, IncrementalQuery };

    QueryThread(QObject *parent);

    /** Runs the query:
        - for SingleQuery: extracts the first two bindings
        - for IncrementalQuery: extracts the binding corresponding to varName
     */
    void run();

    /** Sets the query to be run on the RDF repository with the run() function
    */
    void setQuery(QString query, QString varName = QString(), QueryMode queryMode = QueryThread::SingleQuery);

    /** SYNCHRONOUS utility function.
       Returns query results of a simple SELECT query
       query - the query to run
       freeVar - the variable to get bindings for
    */
    static QStringList queryResults( QString query, QString freeVar );

    /** SYNCHRONOUS utility function.
        Counts the query results of a simple SELECT query
    */
    static int countQueryResults( QString query );

signals:
    void queryDone(QList<QStringPair>);
    void resultFound(QString item);

private:
    void singleQuery();//fires results at the end
    void incrementalQuery();//fires each result item in turn
    static Soprano::Model* nepomukMainModel();

    QString m_query;
    QString m_varName;
    QueryMode m_queryMode;
};

#endif // QUERYTHREAD_H
