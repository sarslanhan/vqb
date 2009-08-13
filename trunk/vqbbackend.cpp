#include "vqbbackend.h"
#include "vqbform.h"
#include "querythread.h"

#include <QString>
#include <QStringList>

class VqbBackend::Private
{
    public:
        Private() {}
        VqbForm *parent;
};

VqbBackend::VqbBackend( VqbForm *parent )
        : d( new Private )
{
    d->parent = parent;
    //url regexp: /^(https?:\/\/)?([\da-z\.-]+)\.([a-z\.]{2,6})([\/\w \.-]*)*\/?$/
    //html tag regexp: /^<([a-z]+)([^<]+)*(?:>(.*)<\/\1>|\s+\/>)$/

}

void VqbBackend::findSubjectsWithLabels()
{
    QueryThread *qt = new QueryThread( this );
    connect( qt, SIGNAL(queryDone( QStringList )),
             d->parent, SLOT(addSubjects(QStringList)));
    qt->setQuery( "?resource a ?class .  ?class <http://www.w3.org/2000/01/rdf-schema#label> ?label", "label" );
    qt->start();
}

void VqbBackend::findPredicatesForSubject( QString subject )
{
}

void VqbBackend::findObjectsForSP( QString subject, QString predicate )
{
}

#include "vqbbackend.moc"
