#include "querythread.h"
#include "vqbglobal.h"
#include <QString>
#include <QList>
#include <QStringList>
#include <QMutex>
#include <QTimer>
#include <QSet>
#include <QPair>

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
    qRegisterMetaType<QList<QStringPair> >("QList<QStringPair>");//user-defined types need to be registered to be used as signals
}

void QueryThread::run()
{
    if(m_queryMode == QueryThread::SingleQuery) {
        singleQuery();
    }
    else if(m_queryMode == QueryThread::IncrementalQuery) {
        incrementalQuery();
    }
}

void QueryThread::singleQuery()
{
    //kDebug() << "\n\n *** Querying: " << m_query;
    Soprano::Model* m = QueryThread::nepomukMainModel();
    Soprano::QueryResultIterator it = m->executeQuery(m_query, Soprano::Query::QueryLanguageSparql);

    QList<QStringPair> res;
    QList<Soprano::BindingSet> allStatements = it.allBindings();

    QPair<QString,QString>   val;
    Soprano::Node n;
    foreach(Soprano::BindingSet s, allStatements) {

        n = s.value(0);
        if (n.isResource()) {
            val.first = n.uri().toString();
            //val = VisualQueryBuilderConsts::getPrefixForm( val );
        } else if (n.isLiteral()) {
            //val = "\"" + n.literal().toString() + "\"^^<"+ n.literal().dataTypeUri().toString()+">";
            val.first = n.literal().toString();
            //kDebug() << "__||__ Datatype: " << n.dataType();
        }

        n = s.value(1);
        if (n.isResource()) {
            val.second = n.uri().toString();
            val.second = VqbGlobal::prefixForm( val.second );
        } else if (n.isLiteral()) {
            //val = "\"" + n.literal().toString() + "\"^^<"+ n.literal().dataTypeUri().toString()+">";
            val.second = n.literal().toString();
            //kDebug() << "__||__ Datatype: " << n.dataType();
        }

        //kDebug() << "Found: " << val.s1 << val.s2;
        res.append(val);
    }

    //kDebug() << "\n\n ===> Results: " << ": \n";
    emit queryDone(res);
}

void QueryThread::incrementalQuery()
{
    //NOTE: for fast results, just select DISTINCT with LIMIT 100
    QString s = "SELECT DISTINCT " + this->m_varName + " WHERE { " + this->m_query + " } LIMIT 50";
    //add prefixes
    s = VqbGlobal::addPrefixes( s );
    kDebug() << "---000--- Running query: " << s;

    Soprano::Model* m = QueryThread::nepomukMainModel();
    Soprano::QueryResultIterator it = m->executeQuery( s, Soprano::Query::QueryLanguageSparql );
    QSet<QString> results;
    QList<Soprano::BindingSet> allStatements = it.allBindings();


    //kDebug() << "---000--- " << allStatements.count();
    QString val;
    foreach (Soprano::BindingSet s, allStatements ) {
              Soprano::Node n = s.value(this->m_varName.replace("?", ""));
              if ( n.isResource() ) {
                  val = n.uri().toString();
                  val = VqbGlobal::prefixForm( val );
              } else if ( n.isLiteral() ) {
                  QString dtUri = n.literal().dataTypeUri().toString();
                  val = "\"" + n.literal().toString() + "\"" + (dtUri.isEmpty() ? "" : "^^<"+ dtUri +">");
              }

              if(!results.contains(val)) {
                  //kDebug() << "Item found: " << val;
                  emit(resultFound(val));//notification of a new item
                  results.insert(val);
              }
    }
    //kDebug() << "---000--- Query done.";
}

void QueryThread::setQuery(QString query, QString varName, QueryMode queryMode)
{
    m_query = query;
    m_varName = varName;
    m_queryMode = queryMode;
}

/**************** utility functions ************/

static Soprano::Model *s_model = 0;
//static Soprano::Inference::InferenceModel *s_im = 0;

Soprano::Model* QueryThread::nepomukMainModel()
{
    // we use a dummy test model here
    if (!s_model) {
        static Soprano::Client::DBusClient client("org.kde.NepomukServer");
        s_model = client.createModel("main");
    }

    if (!s_model) {
        s_model = new Soprano::Util::DummyModel();
    }

    /*if (!s_im) {
        s_im = new Soprano::Inference::InferenceModel(s_model);
    //s_im->addStatements( s_model->listStatements().allStatements() );
        s_im->performInference();
    }
    */

    //return s_im;
    return s_model;
}

QStringList QueryThread::queryResults( QString query, QString freeVar )
{
    QString s = "SELECT DISTINCT " + freeVar + " WHERE { " + query + " } LIMIT 100";//"SELECT * WHERE {" + query + "} LIMIT 1";
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
                  QString dtUri = n.literal().dataTypeUri().toString();
                  val = "\"" + n.literal().toString() + "\"" + (dtUri.isEmpty() ? "" : "^^<"+ dtUri +">");
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

