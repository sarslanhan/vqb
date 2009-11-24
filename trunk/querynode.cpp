#include "querynode.h"
//#include "querythread.h"
#include "combobox.h"
#include "vqbglobal.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSize>
#include <QList>
#include <QPushButton>

#include <KPushButton>
#include <KDebug>
#include <KRandom>

static const QSize GlobalSize(200, 20);//global size for most screen elements
static const int IndentSize = 40;
static const QStringList RelationList( QStringList() << "contains" << "equals" );

class QueryNode::Private
{
public:
    Private(QString _parentClass)
            : predicateCB(0), relationCB(0), objectCB(0), addBtn(0), removeBtn(0), subjectLayout(0),  restrictionLayout(0)
    {
        parentClass  = _parentClass;
    }
    ~Private()
    {
        QList<QObject*> objects;
        objects << predicateCB << relationCB << objectCB << restrictionLayout << subjectLayout << addBtn << removeBtn;
        foreach(QObject *o, objects) {
            if(o != 0) {
                o->deleteLater();
            }
        }
    }

    QComboBox *predicateCB;
    QComboBox *relationCB;
    ComboBox *objectCB;
    KPushButton *addBtn;
    KPushButton *removeBtn;
    QHBoxLayout *subjectLayout;

    QList<QueryNode*> restrictions;
    QVBoxLayout *restrictionLayout;

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
        d->predicateCB->setStatusTip("Predicate of the RDF triple");
        findPredicates();//FIXME: get predicates from parent

        d->subjectLayout->addWidget( d->predicateCB );
        connect(d->predicateCB, SIGNAL(currentIndexChanged(int)), this, SLOT(addObjectToLayout()));
        connect(d->predicateCB, SIGNAL(currentIndexChanged(int)), this, SLOT(updateQueryPart()));
    }    

    //Restriction Layout 
    d->restrictionLayout = new QVBoxLayout();
    d->restrictionLayout->setAlignment(Qt::AlignLeft);
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
    connect(qt, SIGNAL(queryDone(QList<QStringPair>)), this, SLOT(addSubjects(QList<QStringPair>)));
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
    connect(qt, SIGNAL(queryDone(QList<QStringPair>)), this, SLOT(addPredicates(QList<QStringPair>)));

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

void QueryNode::addSubjects(QList<QStringPair> subjects)
{
    //cleanup
    removeRestrictions();

    d->objectCB->clear();


    //add subjects
    //foreach(QStringPairsp, subjects) {
    QStringPair sp;
    for(int i=0; i<subjects.count(); i++) {
        sp = subjects[i];
        d->objectCB->addItem(sp.first, sp.second);  //add s2 as the data associated to the item
        //kDebug() << "++**++ Added item" << sp.first << sp.second;
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
            d->relationCB->setStatusTip("Constraint relation");
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
        if(d->relationCB != 0) {//relation CB
            d->subjectLayout->removeWidget(d->relationCB);
            d->relationCB->deleteLater();
            d->relationCB = 0;
        }
        d->objectCB->setEditable(false);

        if(d->addBtn == 0) {//add button
            d->addBtn = new KPushButton(KStandardGuiItem::add().icon(), "");
            d->addBtn->setToolTip("Add Restriction to " + d->objectCB->currentText());
            d->addBtn->setStatusTip("Adds property restriction to element " + d->objectCB->currentText());
            d->addBtn->setBaseSize(20, 20);
            d->addBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            connect(d->addBtn, SIGNAL(clicked()),this, SLOT(addRestriction()));
            d->restrictionLayout->addWidget(d->addBtn);
        }
    }

    if(d->removeBtn == 0) {//remove button
        d->removeBtn = new KPushButton(KStandardGuiItem::remove().icon(), "");
        d->removeBtn->setToolTip("Remove Restriction");
        d->removeBtn->setStatusTip("Removes restriction");
        d->removeBtn->setBaseSize(20, 20);
        d->removeBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        connect(d->removeBtn, SIGNAL(clicked()),this, SLOT(emitRemove()));
        d->subjectLayout->addWidget(d->removeBtn);
    }

    updateQueryPart();
}

void QueryNode::addPredicates(QList<QStringPair > predicates)
{
    d->predicateCB->blockSignals(true);  //don't trigger object population
    d->predicateCB->clear();
    //foreach(QStringPair  sp, predicates) {
    QStringPair sp;
    for(int i=0; i<predicates.count(); i++) {
        sp = predicates[i];
        //FIXME: if label is empty, parse (I don't know what I meant by this)
        d->predicateCB->addItem(sp.first.isEmpty() ? sp.second : sp.first, sp.second);  //add s2 as the data associated to the item
    }
    d->predicateCB->setCurrentIndex(-1);  //insertItem( 0, "?p" );
    d->predicateCB->blockSignals(false);
}

void QueryNode::addObjectToLayout()
{
    //add the objectCB to the layout
    //FIXME: sometimes one of the CBs is longer
    if(d->objectCB == 0) {
        d->objectCB = new ComboBox();
        d->objectCB->setStatusTip("Object of the RDF triple");
        d->objectCB->resize( GlobalSize );
        d->objectCB->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        d->objectCB->setEditable(true);

        connect(d->objectCB, SIGNAL(currentIndexChanged(int)), this, SLOT(updateQueryPart()));
        connect(d->objectCB, SIGNAL(editTextChanged(QString)), this, SLOT(updateQueryPart()));
        connect(d->objectCB, SIGNAL(addVarToOutput(QString)), this, SIGNAL(addVarToOutput(QString)));
        connect(d->objectCB, SIGNAL(removeVarFromOutput(QString)), this, SIGNAL(removeVarFromOutput(QString)));
        connect(d->objectCB, SIGNAL(currentIndexChanged(int)), this, SLOT(removeRestrictions()));

        findObjects();

        d->subjectLayout->addWidget(d->objectCB);
    }
    else {
        findObjects();
    }
}

void QueryNode::addRestriction()
{
    QueryNode *qn = new QueryNode(d->objectCB->itemData(d->objectCB->currentIndex()).toString());
    d->restrictions.append(qn);
    d->restrictionLayout->insertLayout(d->restrictionLayout->count()-1, qn);
    connect(qn, SIGNAL(queryPartChanged(QString)), this, SLOT(updateQueryPart()));
    connect(qn, SIGNAL(addVarToOutput(QString)), this, SIGNAL(addVarToOutput(QString)));
    connect(qn, SIGNAL(removeVarFromOutput(QString)), this, SIGNAL(removeVarFromOutput(QString)));
    connect(qn, SIGNAL(removeClicked(QueryNode*)), this, SLOT(removeRestriction(QueryNode*)));
}

void QueryNode::removeRestrictions()
{
    foreach(QueryNode *qn, d->restrictions) {
        d->restrictionLayout->removeItem(qn);
        qn->deleteLater();
    }
    d->restrictions.clear();
}

void QueryNode::removeRestriction(QueryNode *qn)
{
    kDebug() << "Removing Restriction...";
    d->restrictionLayout->removeItem(qn);
    d->restrictions.removeAt(d->restrictions.indexOf(qn));
    qn->deleteLater();

    updateQueryPart();//refresh
}

void QueryNode::emitRemove()
{
    kDebug() << "Emitting remove";
    emit removeClicked(this);
}

/****** QUERY COMPUTATION *******/

QString QueryNode::queryPart()
{
    //WISH: only the changed query part returns its query string:
    //       use that, and don't recompute anything

    //FIXME: use vars - based on the relation combobox (have a "variable" option).

    if(d->objectCB == 0) {
        return QString();
    }

    //variable
    QString objectVar;
    QRegExp rx(VqbGlobal::typeRegExp("var"));
    if(rx.exactMatch(d->objectCB->currentText())) {
        objectVar = d->objectCB->currentText();
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
