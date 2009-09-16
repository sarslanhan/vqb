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
};

QueryThread::QueryThread(QObject *parent)
        : QThread( parent ),
          d( new Private )
{
    qRegisterMetaType<QList<StringPair> >("QList<StringPair>");//user-defined types need to be registered to be used as signals
    d->query = "SELECT DISTINCT ?s WHERE { ?s ?p ?o } LIMIT 50";
}

void QueryThread::run()
{
    kDebug() << "\n\n *** Querying: " << d->query << ": \n";

    Soprano::Model* m = nepomukMainModel();
    Soprano::QueryResultIterator it = m->executeQuery( d->query, Soprano::Query::QueryLanguageSparql );

    //QList<Soprano::Node> resNodes;
    QList<StringPair> res;
    QList<Soprano::BindingSet> allStatements = it.allBindings();

    StringPair  val;
    Soprano::Node n;
    foreach (Soprano::BindingSet s, allStatements ) {

              n = s.value( 0 );
              if ( n.isResource() ) {
                  val.s1 = n.uri().toString();
                  //val = VisualQueryBuilderConsts::getPrefixForm( val );
              } else if ( n.isLiteral() ) {
                  //val = "\"" + n.literal().toString() + "\"^^<"+ n.literal().dataTypeUri().toString()+">";
                  val.s1 = n.literal().toString();
                  kDebug() << "__||__ Datatype: " << n.dataType();
              }

              n = s.value( 1 );
              if ( n.isResource() ) {
                  val.s2 = n.uri().toString();
                  //val = VisualQueryBuilderConsts::getPrefixForm( val );
              } else if ( n.isLiteral() ) {
                  //val = "\"" + n.literal().toString() + "\"^^<"+ n.literal().dataTypeUri().toString()+">";
                  val.s2 = n.literal().toString();
                  kDebug() << "__||__ Datatype: " << n.dataType();
              }

              //kDebug() << "--- Found: " << val;
              kDebug() << "Found: " << val.s1 << val.s2;
              res << val;
              //resNodes << n;
    }

    //kDebug() << "\n\n ===> Results: " << ": \n";
    //foreach( StringPair s, res ) { kDebug() << s.s1 << s.s2 << endl; }


    emit queryDone( res );
    //emit queryDoneNodes( resNodes );
}

void QueryThread::setQuery( QString query )
{
    d->query = query;
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

