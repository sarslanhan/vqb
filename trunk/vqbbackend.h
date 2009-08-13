#ifndef VQBBACKEND_H
#define VQBBACKEND_H

#include <QObject>

namespace Soprano {
    class Model;
}

class QString;
class QStringList;
class VqbForm;

/** Asynchronous (thread-based) backend executing queries through Soprano
*/

class VqbBackend : public QObject
{

    Q_OBJECT

public:
    VqbBackend( VqbForm *parent );

public Q_SLOTS:
    void findSubjectsWithLabels();
    void findPredicatesForSubject( QString subject );
    void findObjectsForSP( QString subject, QString predicate );

private:
    class Private;
    Private * const d;


};

#endif // VQBBACKEND_H
