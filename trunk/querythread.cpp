#include "querythread.h"

#include <QString>
#include <QList>
#include <QStringList>

#include <Soprano/Soprano>
#include <Soprano/Model>
#include <Soprano/Client/DBusClient>
#include <Soprano/Client/DBusModel>
#include <Soprano/Util/DummyModel>
#include <Soprano/QueryResultIterator>

#include <KDebug>

class QueryThread::Private
{
    public:
        Private() {}
        QString query;
        QString freeVar;
};

QueryThread::QueryThread(QObject *parent)
        : QThread( parent ),
          d( new Private )
{
    d->query = "?s ?p ?o";
    d->freeVar = "s";
}

void QueryThread::run()
{
    QString q =  "SELECT DISTINCT ?" + d->freeVar + " WHERE { " + d->query + " } LIMIT 50";
    kDebug() << "\n\n *** Querying: " << q << ": \n";

    Soprano::Model* m = nepomukMainModel();
    Soprano::QueryResultIterator it = m->executeQuery( q, Soprano::Query::QueryLanguageSparql );

    //QList<Soprano::Node> resNodes;
    QStringList res;
    QList<Soprano::BindingSet> allStatements = it.allBindings();

    QString val;
    foreach (Soprano::BindingSet s, allStatements ) {
              Soprano::Node n = s.value(d->freeVar);
              if ( n.isResource() ) {
                  val = n.uri().toString();
                  //val = VisualQueryBuilderConsts::getPrefixForm( val );
              } else if ( n.isLiteral() ) {
                  //val = "\"" + n.literal().toString() + "\"^^<"+ n.literal().dataTypeUri().toString()+">";
                  val = n.literal().toString();
              }
              //kDebug() << "--- Found: " << val;
              res << val;
              //resNodes << n;
    }
    kDebug() << "\n\n ===> Results: " << ": \n" << res;
    //res << "Label1" << "Label2" << "Label3" << "Label4" << "Label5";
    emit queryDone( res );
    //emit queryDoneNodes( resNodes );
}

void QueryThread::setQuery( QString query, QString freeVar )
{
    d->query = query;
    d->freeVar = freeVar;
}

static Soprano::Model *s_model = 0;

Soprano::Model* QueryThread::nepomukMainModel()
{
    // we use a dummy test model here
    if ( !s_model ) {
        static Soprano::Client::DBusClient client( "org.kde.NepomukServer" );
        s_model = client.createModel( "main" );
    }

    if ( !s_model ) {
        s_model = new Soprano::Util::DummyModel();
    }

    return s_model;
}

#include "querythread.moc"

