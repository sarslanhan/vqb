#ifndef QUERYTHREAD_H
#define QUERYTHREAD_H

#include <QThread>
class QString;
class QStringList;

namespace Soprano {
    class Model;
}

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
    void setQuery( QString query );

signals:
    void queryDone( QList<StringPair> );

private:
    class Private;
    Private * const d;

    Soprano::Model* nepomukMainModel();

};

#endif // QUERYTHREAD_H
