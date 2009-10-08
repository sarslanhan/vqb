#include "querynode.h"
#include "querythread.h"
#include "combobox.h"

#include <QHBoxLayout>
#include <QSize>
#include <QList>
#include <QPushButton>

#include <KDebug>

static const QSize GlobalSize( 80, 20 );
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
    else {
        d->predicateCB = new QComboBox();
        findPredicates();//FIXME: get predicates from parent

        d->subjectLayout->addWidget( d->predicateCB );
        connect(d->predicateCB, SIGNAL(currentIndexChanged(int)),
                this, SLOT(addObjectToLayout()));
    }    
    //Restriction Layout 
    d->restrictionLayout = new QVBoxLayout();
    this->addLayout( d->subjectLayout );
    this->addLayout( d->restrictionLayout );
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
        d->relationCB->resize( GlobalSize );
        d->relationCB->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
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
}

void QueryNode::addObjectToLayout()
{
    d->objectCB = new ComboBox();
    d->objectCB->resize( GlobalSize );
    d->objectCB->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    findObjects();

    d->subjectLayout->addWidget(d->objectCB);
}

#include "querynode.moc"
