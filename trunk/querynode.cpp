#include "querynode.h"
#include "querythread.h"
#include "combobox.h"

#include <QHBoxLayout>
#include <QSize>
#include <QList>
#include <QPushButton>

#include <KDebug>
#include <KRandom>

static const QSize GlobalSize(120, 20);//global size for most screen elements
static const int IndentSize = 20;
static const QStringList RelationList( QStringList() << "contains" << "equals" );

class QueryNode::Private
{
public:
    Private(QString _parentClass)
            : predicateCB(0), relationCB(0), objectCB(0), addBtn(0), subjectLayout(0),  restrictionLayout(0)
    {
        parentClass  = _parentClass;
    }
    ~Private()
    {
        QList<QObject*> objects;
        objects << predicateCB << relationCB << objectCB << restrictionLayout << subjectLayout << addBtn;
        foreach(QObject *o, objects) {
            if(o != 0) {
                o->deleteLater();
            }
        }
    }
    QComboBox *predicateCB;
    QComboBox *relationCB;
    ComboBox *objectCB;
    QPushButton *addBtn;
    QHBoxLayout *subjectLayout;

    QList<QueryNode*> restrictions;
    QVBoxLayout *restrictionLayout;

    //QString objectVariable;//variable representing the object
    QString parentClass;//URI of parent predicate
};



QueryNode::QueryNode(QString parentClass)
        : QVBoxLayout(), d(new Private(parentClass))
{
    init();
}

void QueryNode::init()
{
    //Subject Layout
    d->subjectLayout = new QHBoxLayout();
    d->subjectLayout->setAlignment(Qt::AlignLeft);

    if( d->parentClass.isEmpty() ) { //root node
        addObjectToLayout();
    }
    else { //child node
        d->predicateCB = new QComboBox();
        findPredicates();//FIXME: get predicates from parent

        d->subjectLayout->addWidget( d->predicateCB );
        connect(d->predicateCB, SIGNAL(currentIndexChanged(int)),
                this, SLOT(addObjectToLayout()));
        connect(d->predicateCB, SIGNAL(currentIndexChanged(int)),
                this, SLOT(updateQueryPart()));
    }    
    //Restriction Layout 
    d->restrictionLayout = new QVBoxLayout();
    QHBoxLayout *container = new QHBoxLayout();//to indent restrictionLayout
    container->addSpacing(IndentSize);
    container->addLayout(d->restrictionLayout);
    this->addLayout(d->subjectLayout);
    this->addLayout(container);
}

QueryNode::~QueryNode()
{
    delete d;
}

/**** BACKEND QUERYING  ******/

void QueryNode::findObjects()
{
    QueryThread *qt = new QueryThread(this);
    connect(qt, SIGNAL(queryDone(QList<StringPair>)),
            this, SLOT(addSubjects(QList<StringPair>)));
    QString query;
    if(!d->parentClass.isEmpty()) { //if not root node
        kDebug() << (int) d->predicateCB;
        QString predicate = d->predicateCB->itemData( d->predicateCB->currentIndex() ).toString();
        query = QString("SELECT DISTINCT ?label ?class WHERE {"
                            "<%1> <http://www.w3.org/2000/01/rdf-schema#range> ?class ."
                            "OPTIONAL { ?class <http://www.w3.org/2000/01/rdf-schema#label> ?label } }")
                    .arg(predicate);
    }
    else { //root node
    query = "SELECT DISTINCT ?label ?class WHERE"
                    "{ ?resource a ?class .  ?class <http://www.w3.org/2000/01/rdf-schema#label> ?label }";
    }
    qt->setQuery(query);
    qt->start();
}

void QueryNode::findPredicates()
{
    QueryThread *qt = new QueryThread(this);
    connect(qt, SIGNAL(queryDone(QList<StringPair>)),
            this, SLOT(addPredicates(QList<StringPair>)));

    QString query = QString("SELECT DISTINCT ?label ?property WHERE {"
                            " { "
                            " ?property <http://www.w3.org/2000/01/rdf-schema#domain> <%1> "
                            " OPTIONAL { ?property  <http://www.w3.org/2000/01/rdf-schema#label> ?label }"
                            " } UNION {"
                            " ?instance a <%1> . "
                            " ?instance ?property ?object "
                            " OPTIONAL { ?property  <http://www.w3.org/2000/01/rdf-schema#label> ?label } "
                            " } } "
                            ).arg(d->parentClass);
    qt->setQuery( query );
    qt->start();
}

/**** INTERFACE AND TREE MANIPULATIONS ******/

void QueryNode::addSubjects(QList<StringPair> subjects)
{
    //cleanup
    d->restrictions.clear();
    for(int i=0; i<d->restrictionLayout->count(); i++) {
        QueryNode *qn = (QueryNode*)d->restrictionLayout->itemAt(i);
        d->restrictionLayout->removeItem(qn);
        qn->deleteLater();
    }

    d->objectCB->clear();


    //add subjects
    foreach(StringPair sp, subjects) {
        d->objectCB->addItem(sp.s1, sp.s2);  //add s2 as the data associated to the item
        //kDebug() << "++**++ Added item" << sp.s1 << sp.s2;
    }

    if(d->objectCB->count() == 1) {
        d->objectCB->setCurrentIndex(0);
    }

    //change interface
    QString object = d->objectCB->itemData( d->objectCB->currentIndex() ).toString();
    if (object == "http://www.w3.org/2001/XMLSchema#string" ||
        object == "http://www.w3.org/2000/01/rdf-schema#Literal") { //Literal

        if(d->relationCB == 0) {
            d->relationCB = new ComboBox();
            connect(d->relationCB, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(updateQueryPart()));
            d->relationCB->resize( GlobalSize );
            d->relationCB->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            d->relationCB->insertItems(0, RelationList);
            d->subjectLayout->insertWidget(1, d->relationCB );
        }

        d->objectCB->clear();
        d->objectCB->setEditable( true );
    }
    else { //not literal (root node or not)        
        if(d->relationCB != 0) {
            d->subjectLayout->removeWidget(d->relationCB);
            d->relationCB->deleteLater();
            d->relationCB = 0;
        }
        d->objectCB->setEditable(false);
        if(d->addBtn == 0) {
            d->addBtn = new QPushButton("+");
            d->addBtn->setBaseSize(20, 20);
            connect(d->addBtn, SIGNAL(clicked()),this, SLOT(addRestriction()));
            d->subjectLayout->addWidget(d->addBtn);
        }
    }
    updateQueryPart();
}

void QueryNode::addPredicates(QList<StringPair> predicates)
{
    d->predicateCB->blockSignals(true);  //don't trigger object population
    d->predicateCB->clear();
    foreach(StringPair sp, predicates) {
        //FIXME: if label is empty, parse (I don't know what I meant by this)
        d->predicateCB->addItem(sp.s1.isEmpty() ? sp.s2 : sp.s1, sp.s2);  //add s2 as the data associated to the item
    }
    d->predicateCB->setCurrentIndex(-1);  //insertItem( 0, "?p" );
    d->predicateCB->blockSignals(false);
}

void QueryNode::addRestriction()
{
    QueryNode *qn = new QueryNode(d->objectCB->itemData(d->objectCB->currentIndex()).toString());
    d->restrictions.append(qn);
    d->restrictionLayout->addLayout(qn);
    connect(qn, SIGNAL(queryPartChanged(QString)), this, SLOT(updateQueryPart()));
    connect(qn, SIGNAL(addVarToOutput(QString)), this, SIGNAL(addVarToOutput(QString)));
}

void QueryNode::addObjectToLayout()
{
    //add the objectCB to the layout

    if(d->objectCB == 0) {
        d->objectCB = new ComboBox();
        d->objectCB->resize( GlobalSize );
        d->objectCB->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        connect(d->objectCB, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(updateQueryPart()));
        connect(d->objectCB, SIGNAL(editTextChanged(QString)),
                    this, SLOT(updateQueryPart()));
        connect(d->objectCB, SIGNAL(addVarToOutput(QString)),
                this, SIGNAL(addVarToOutput(QString)));
        //FIXME: connect to removeRestrictions() method.
        connect(d->objectCB, SIGNAL(currentIndexChanged(int)),
                this, SLOT(removeRestrictions()));

        findObjects();

        d->subjectLayout->addWidget(d->objectCB);
    }
    else {
        //kDebug() << "It's not null! " << QString::number(d->objectCB->count());
        findObjects();
    }
}

void QueryNode::removeRestrictions()
{
    for(int i=0; i<d->restrictionLayout->count(); i++) {
        QueryNode *qn = (QueryNode*) d->restrictionLayout->itemAt(i);
        d->restrictionLayout->removeItem(qn);
        qn->deleteLater();
    }
    d->restrictions.clear();
}

/****** QUERY COMPUTATION *******/

QString QueryNode::queryPart()
{
    //FIXME(not urgent): the only changed query part returns its query string:
    //       use this, and don't recompute anything
    if(d->objectCB == 0) {
        return QString();
    }

    QString var = d->objectCB->varName();

    if(d->parentClass.isEmpty()) { //root node    
        QString classUri = d->objectCB->itemData(d->objectCB->currentIndex()).toString();
        QString childrenQueryParts;
        foreach(QueryNode *node, d->restrictions) {
            childrenQueryParts.append(var + " " + node->queryPart());
        }
        return QString(var + " a <" + classUri + "> . " + childrenQueryParts);
    }    

    if(d->objectCB->isEditable()) { //Literal node
        QString relStr = QString("contains");
        if(d->relationCB) {
            relStr = d->relationCB->currentText();
        }
        //FIXME: wrong output if no value is entered
        QString predUri = d->predicateCB->itemData(d->predicateCB->currentIndex()).toString();
        QString filterStr;
        QString object = d->objectCB->currentText();
        if (relStr == "equals") {
            filterStr = QString(". FILTER regex(" + var + ", '^" + object + "$', 'i') . ");
        } else if (relStr == "contains") {
            filterStr = QString(". FILTER regex(" + var + ", '" + object + "', 'i') . ") ;
        }
        return QString("<" + predUri + "> " + var + filterStr);
    }

    else { //Resource node
        QString predUri = d->predicateCB->itemData(d->predicateCB->currentIndex()).toString();
        QString classUri = d->objectCB->itemData(d->objectCB->currentIndex()).toString();
        QString childrenQueryParts;
        foreach(QueryNode *node, d->restrictions) {
            childrenQueryParts.append(var + " " + node->queryPart());
        }
        return QString("<" + predUri + "> " + var + " . "
                       + var + " a <" + classUri +
                       + "> . " + childrenQueryParts);
    }
}

void QueryNode::updateQueryPart()
{
    emit queryPartChanged(queryPart());
}


#include "querynode.moc"
