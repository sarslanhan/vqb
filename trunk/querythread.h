#ifndef QUERYTHREAD_H
#define QUERYTHREAD_H

#include <QThread>
class QString;
class QStringList;

namespace Soprano
{
class Model;
}

//FIXME:: QPair makes this redundant
struct StringPair {
public:
    QString s1, s2;
};

/*! Thread class that executes queries (and performs some utility functions)
*/

class QueryThread : public QThread
{
    Q_OBJECT

public:
    QueryThread(QObject *parent);

    /** Runs the query and extracts the first two bindings
     *  (if the second one is invalid, it returns invalid nodes).
     */
    void run();

    /** Sets the query to be run on the RDF repository
    */
    void setQuery(QString query);


    /** Static SYNCHRONOUS utility function.
       Returns query results of a simple SELECT query
       query - the query to run
       freeVar - the variable to get bindings for
    */
    static QStringList queryResults( QString query, QString freeVar );

    /** Static SYNCHRONOUS utility function.
        Counts the query results of a simple SELECT query
    */
    static int countQueryResults( QString query );

signals:
    void queryDone(QList<StringPair>);

private:
    QString m_query;

    static Soprano::Model* nepomukMainModel();

};

#endif // QUERYTHREAD_H
