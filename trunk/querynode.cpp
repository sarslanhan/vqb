#include "querynode.h"
#include "querythread.h"
#include "combobox.h"

#include <QHBoxLayout>
#include <QSize>
#include <QList>
#include <QPushButton>

#include <KDebug>
#include <KRandom>

static const QSize GlobalSize(100, 20);//global size for most screen elements
static const int IndentSize = 20;
static const QStringList RelationList( QStringList() << "contains" << "equals" );

class QueryNode::Private
{
public:
    QComboBox *predicateCB;
    QComboBox *relationCB;
    ComboBox *objectCB;
    QHBoxLayout *subjectLayout;

    QList<QueryNode*> restrictions;
    QVBoxLayout *restrictionLayout;

    QString objectVariable;//variable representing the object
    QString parentClass;//URI of parent predicate
};

QueryNode::QueryNode(QString parentClass)
        : QVBoxLayout(), d(new Private)
{
    d->parentClass = parentClass;
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
                            " ?property <http://www.w3.org/2000/01/rdf-schema#domain> <%1> ."
                            " OPTIONAL { ?property  <http://www.w3.org/2000/01/rdf-schema#label> ?label } }"
                            ).arg(d->parentClass);
    qt->setQuery( query );
    qt->start();
}

void QueryNode::addSubjects(QList<StringPair> subjects)
{
    d->restrictions.clear();
    //FIXME: remove restrictions from layout

    foreach(StringPair sp, subjects) {
        d->objectCB->addItem(sp.s1, sp.s2);  //add s2 as the data associated to the item
        //kDebug() << "++**++ Added item" << sp.s1 << sp.s2;
    }

    if(d->objectCB->count() == 1) {
        d->objectCB->setCurrentIndex(0);
    }

    QString object = d->objectCB->itemData( d->objectCB->currentIndex() ).toString();
    if (object == "http://www.w3.org/2001/XMLSchema#string" ||
        object == "http://www.w3.org/2000/01/rdf-schema#Literal") { //Literal
        d->relationCB = new ComboBox();
        connect(d->relationCB, SIGNAL(currentIndexChanged(int)),
                this, SLOT(updateQueryPart()));
        d->relationCB->resize( GlobalSize );
        d->relationCB->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        d->relationCB->insertItems(0, RelationList);
        d->subjectLayout->insertWidget(1, d->relationCB );

        d->objectCB->clear();
        d->objectCB->setEditable( true );
    }
    else { //not literal (root node or not)
        QPushButton *btnAdd = new QPushButton("+");
        btnAdd->setBaseSize(20, 20);
        connect(btnAdd, SIGNAL(clicked()),this, SLOT(addRestriction()));
        d->subjectLayout->addWidget(btnAdd);
    }
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

    d->objectCB = new ComboBox();
    d->objectCB->resize( GlobalSize );
    d->objectCB->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(d->objectCB, SIGNAL(currentIndexChanged(int)),
                this, SLOT(updateQueryPart()));
    connect(d->objectCB, SIGNAL(editTextChanged(QString)),
                this, SLOT(updateQueryPart()));
    connect(d->objectCB, SIGNAL(addVarToOutput(QString)),
            this, SIGNAL(addVarToOutput(QString)));

    findObjects();

    d->subjectLayout->addWidget(d->objectCB);
}

QString QueryNode::queryPart()
{
    //FIXME: assign var names at creation
    //FIXME: the only changed query part returns its query string:
    //       use this, and don't recompute anything
    QString var = d->objectCB->varName();

    if(d->parentClass.isEmpty()) { //root node    
        QString classUri = d->objectCB->itemData(d->objectCB->currentIndex()).toString();
        QString childrenQueryParts;
        foreach(QueryNode *node, d->restrictions) {
            childrenQueryParts.append(var + " " + node->queryPart());
        }
        return QString(var + " a <" + classUri + "> . " + childrenQueryParts);
    }    
    if(d->objectCB->isEditable()) { //Literal node //FIXME: segmentation fault if objectCB doesn't exist?
        //FIXME: wrong output if no value is entered
        QString predUri = d->predicateCB->itemData(d->predicateCB->currentIndex()).toString();
        QString filterStr;
        QString object = d->objectCB->currentText();
        if (d->relationCB->currentText() == "equals") {
            filterStr = QString(". FILTER regex(" + var + ", '^" + object + "$', 'i') . ");
        } else if (d->relationCB->currentText() == "contains") {
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
