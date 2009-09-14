#ifndef QUERYTHREAD_H
#define QUERYTHREAD_H

#include <QThread>
class QString;
class QStringList;

namespace Soprano {
    class Model;
}

/*! Thread class that executes queries (and performs some utility functions)
*/

class QueryThread : public QThread
{
    Q_OBJECT

public:
    QueryThread(QObject *parent);

    /** Runs thread
    */
    void run();

    /** Sets the query to be run on the RDF repository
    */
    void setQuery( QString query, QString freeVar );

signals:
    void queryDone( QStringList );

private:
    class Private;
    Private * const d;

    Soprano::Model* nepomukMainModel();

};

#endif // QUERYTHREAD_H
