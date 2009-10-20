#include "querythread.h"
#include "vqbglobal.h"

#include <QString>
#include <QList>
#include <QStringList>
#include <QMutex>

#include <Soprano/Soprano>
#include <Soprano/Model>
#include <Soprano/Client/DBusClient>
#include <Soprano/Client/DBusModel>
#include <Soprano/Util/DummyModel>
#include <Soprano/QueryResultIterator>
#include <Soprano/Inference/InferenceModel>

#include <kdebug.h>

QueryThread::QueryThread(QObject *parent)
        : QThread(parent)
{
    qRegisterMetaType<QList<StringPair> >("QList<StringPair>");//user-defined types need to be registered to be used as signals
    m_query = "SELECT DISTINCT ?s WHERE { ?s ?p ?o } LIMIT 50";
}

void QueryThread::run()
{
    //kDebug() << "\n\n *** Querying: " << m_query;
    Soprano::Model* m = QueryThread::nepomukMainModel();
    //FIXME: see if there's a query syntax checker in Soprano
    Soprano::QueryResultIterator it = m->executeQuery(m_query, Soprano::Query::QueryLanguageSparql);

    //QList<Soprano::Node> resNodes;
    QList<StringPair> res;
    QList<Soprano::BindingSet> allStatements = it.allBindings();

    StringPair  val;
    Soprano::Node n;
    foreach(Soprano::BindingSet s, allStatements) {

        n = s.value(0);
        if (n.isResource()) {
            val.s1 = n.uri().toString();
            //val = VisualQueryBuilderConsts::getPrefixForm( val );
        } else if (n.isLiteral()) {
            //val = "\"" + n.literal().toString() + "\"^^<"+ n.literal().dataTypeUri().toString()+">";
            val.s1 = n.literal().toString();
            //kDebug() << "__||__ Datatype: " << n.dataType();
        }

        n = s.value(1);
        if (n.isResource()) {
            val.s2 = n.uri().toString();
            //val = VisualQueryBuilderConsts::getPrefixForm( val );
        } else if (n.isLiteral()) {
            //val = "\"" + n.literal().toString() + "\"^^<"+ n.literal().dataTypeUri().toString()+">";
            val.s2 = n.literal().toString();
            //kDebug() << "__||__ Datatype: " << n.dataType();
        }

        //kDebug() << "Found: " << val.s1 << val.s2;
        res << val;
        //resNodes << n;
    }

    //kDebug() << "\n\n ===> Results: " << ": \n";
    //foreach( StringPair s, res ) { kDebug() << s.s1 << s.s2 << endl; }


    emit queryDone(res);
    //emit queryDoneNodes( resNodes );
}

void QueryThread::setQuery(QString query)
{
    m_query = query;
}

static Soprano::Model *s_model = 0;
static Soprano::Inference::InferenceModel *s_im = 0;

Soprano::Model* QueryThread::nepomukMainModel()
{
    QMutex mutex;//this might not be necessary
    mutex.lock();

    // we use a dummy test model here
    if (!s_model) {
        static Soprano::Client::DBusClient client("org.kde.NepomukServer");
        s_model = client.createModel("main");
    }

    if (!s_model) {
        s_model = new Soprano::Util::DummyModel();
    }

    //return s_model;
    if (!s_im) {
        s_im = new Soprano::Inference::InferenceModel(s_model);
    //s_im->addStatements( s_model->listStatements().allStatements() );
        s_im->performInference();
    }

    mutex.unlock();
    return s_im;
}

/**************** SYCHRONOUS utility methods ************/


QStringList QueryThread::queryResults( QString query, QString freeVar )
{
    QString s = "SELECT DISTINCT " + freeVar + " WHERE { " + query + " } LIMIT 50";//"SELECT * WHERE {" + query + "} LIMIT 1";
    //add prefixes
    s = VqbGlobal::addPrefixes( s );
    //kDebug() << "--- Running query: " << s;

    Soprano::Model* m = QueryThread::nepomukMainModel();
    Soprano::QueryResultIterator it = m->executeQuery( s, Soprano::Query::QueryLanguageSparql );
    QStringList res;
    QList<Soprano::BindingSet> allStatements = it.allBindings();

    QString val;
    foreach (Soprano::BindingSet s, allStatements ) {
              Soprano::Node n = s.value(freeVar.replace("?", ""));
              if ( n.isResource() ) {
                  val = n.uri().toString();
                  val = VqbGlobal::prefixForm( val );
              } else if ( n.isLiteral() ) {
                  val = "\"" + n.literal().toString() + "\"^^<"+ n.literal().dataTypeUri().toString()+">";
              }
              //kDebug() << "--- Found: " << val;
              res << val;
    }

    return res;
}

int QueryThread::countQueryResults( QString query )
{
    query = VqbGlobal::addPrefixes(query);
    Soprano::QueryResultIterator it = QueryThread::nepomukMainModel()->executeQuery( query, Soprano::Query::QueryLanguageSparql );
    return it.allBindings().count();
}

#include "querythread.moc"

