#include "querynode.h"
//#include "querythread.h"
#include "combobox.h"
#include "vqbglobal.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSize>
#include <QList>
#include <QPushButton>
#include <QFrame>

#include <KPushButton>
#include <KDebug>
#include <KRandom>

static const QSize GlobalSize(120, 20);//global size for some screen elements
static const int IndentSize = 40;
static const QStringList RelationList( QStringList() << "contains" << "equals" );

class QueryNode::Private
{
public:
    Private(QString _parentClass)
            : predicateCB(0), relationCB(0), objectCB(0), addBtn(0), removeBtn(0), subjectLayout(0), restrictionContainer(0), restrictionLayout(0)
    {
        parentClass  = _parentClass;
    }
    ~Private()
    {
        QList<QObject*> objects;
        //FIXME? does restrictionContainer delete restrictionLayout?
        objects << predicateCB << relationCB << objectCB << subjectLayout << addBtn << removeBtn;
        foreach(QObject *o, objects) {
            if(o) {
                delete o;
            }
        }
    }

    QComboBox *predicateCB;
    QComboBox *relationCB;
    ComboBox *objectCB;
    KPushButton *addBtn;
    KPushButton *removeBtn;
    QHBoxLayout *subjectLayout;//layout of the current subject

    QList<QueryNode*> restrictions;
    QWidget *restrictionContainer;
    QVBoxLayout *restrictionLayout;//layout of the restrictions

    QString parentClass;//URI of parent predicate
    int level;//level in the query tree
};

QueryNode::QueryNode(QString parentClass, int level)
        : QVBoxLayout(), d(new Private(parentClass))
{
    d->level = level;
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
        d->predicateCB->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        d->predicateCB->resize( GlobalSize );
        d->predicateCB->setMinimumSize(GlobalSize);
        d->predicateCB->setStatusTip("Predicate of the RDF triple");
        findPredicates();

        d->subjectLayout->addWidget( d->predicateCB );
        connect(d->predicateCB, SIGNAL(currentIndexChanged(int)), this, SLOT(addObjectToLayout()));
        connect(d->predicateCB, SIGNAL(currentIndexChanged(int)), this, SLOT(updateQueryPart()));
    }    

    //Restriction Layout 
    d->restrictionLayout = new QVBoxLayout();
    d->restrictionLayout->setAlignment(Qt::AlignLeft);
    d->restrictionLayout->setContentsMargins(0, 0, 0, 0);

    d->restrictionContainer = new QFrame();
    d->restrictionContainer->setAttribute(Qt::WA_PaintOnScreen, true);
    d->restrictionContainer->setMinimumSize(0, 0);
    d->restrictionContainer->resize(0, 0);
    d->restrictionContainer->setContentsMargins(2, 2, 2, 2);
    //d->restrictionContainer->setAutoFillBackground(true);
    //((QFrame*)d->restrictionContainer)->setFrameStyle(QFrame::Box);

    QPalette palette( d->restrictionContainer->palette() );
    //FIXME: use theme's default Window color
    palette.setColor( QPalette::Window, d->level % 2 ? QColor(232, 231, 230) : Qt::lightGray );
    d->restrictionContainer->setPalette(palette);
    d->restrictionContainer->setLayout(d->restrictionLayout);

    QHBoxLayout *container = new QHBoxLayout();//to indent restrictionLayout
    container->addSpacing(IndentSize);    
    container->addWidget(d->restrictionContainer);

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
        //kDebug() << (int) d->predicateCB;
        QString predicate = d->predicateCB->itemData( d->predicateCB->currentIndex() ).toString();
        query = QString("SELECT DISTINCT ?label ?class WHERE {"
                            "%1 rdfs:range ?class ."
                            "OPTIONAL { ?class rdfs:label ?label } } ORDER BY (?label)")
                    .arg(predicate);
    }
    else { //root node
    query = "SELECT DISTINCT ?label ?class WHERE"
                    "{ ?resource a ?class .  ?class rdfs:label ?label } ORDER BY (?label)";
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
                            " ?property rdfs:domain %1 "
                            " OPTIONAL { ?property  rdfs:label ?label }"
                            " } UNION {"
                            " ?instance a %1 . "
                            " ?instance ?property ?object "
                            " OPTIONAL { ?property rdfs:label ?label } "
                            " } } ORDER BY (?label)"
                            ).arg(d->parentClass);
    qt->setQuery( query );
    qt->start();
}

/**** INTERFACE AND TREE MANIPULATIONS ******/


void QueryNode::addObjectToLayout()
{
    //add the objectCB to the layout
    //FIXME: sometimes one of the CBs is longer
    if(d->objectCB == 0) {
        d->objectCB = new ComboBox();
        d->objectCB->setStatusTip("Object field of the RDF triple");
        d->objectCB->resize( GlobalSize );
        d->objectCB->setMinimumSize(GlobalSize);
        d->objectCB->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        //d->objectCB->setEditable(true);

        connect(d->objectCB, SIGNAL(currentIndexChanged(int)), this, SLOT(updateQueryPart()));
        connect(d->objectCB, SIGNAL(editTextChanged(QString)), this, SLOT(updateQueryPart()));
        connect(d->objectCB, SIGNAL(addVarToOutput(QString)), this, SIGNAL(addVarToOutput(QString)));
        connect(d->objectCB, SIGNAL(removeVarFromOutput(QString)), this, SIGNAL(removeVarFromOutput(QString)));
        connect(d->objectCB, SIGNAL(currentIndexChanged(int)), this, SLOT(removeAllRestrictions()));

        findObjects();

        d->subjectLayout->addWidget(d->objectCB);
    }
    else {
        findObjects();
    }
}


void QueryNode::addSubjects(QList<QStringPair> subjects)
{
    //cleanup
    removeAllRestrictions();

    d->objectCB->clear();


    //add subjects
    //foreach(QStringPairsp, subjects) {
    QStringPair sp;
    for(int i=0; i<subjects.count(); i++) {
        sp = subjects[i];
        d->objectCB->addItem(sp.first, sp.second);  //add s2 as the data associated to the item
        d->objectCB->setItemData(d->objectCB->count()-1, sp.second, Qt::StatusTipRole);
        d->objectCB->setItemData(d->objectCB->count()-1, sp.second, Qt::ToolTipRole);
        //kDebug() << "++**++ Added item" << sp.first << sp.second;
    }

    if(d->objectCB->count() == 1) {
        d->objectCB->setCurrentIndex(0);
    }

    //change interface
    QString object = d->objectCB->itemData( d->objectCB->currentIndex() ).toString();
    if (object == "xsd:string" ||
        object == "rdfs:Literal") { //Literal

        if(d->relationCB == 0) {
            d->relationCB = new QComboBox();
            d->relationCB->setStatusTip("Constraint relation");
            connect(d->relationCB, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(updateQueryPart()));
            d->relationCB->resize( GlobalSize );
            d->relationCB->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            d->relationCB->insertItems(0, RelationList);
            d->subjectLayout->insertWidget(1, d->relationCB );
        }

        d->restrictionLayout->removeWidget(d->addBtn);
        d->addBtn->deleteLater();
        d->addBtn = 0;

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
        d->predicateCB->addItem(sp.first.isEmpty() ? sp.second : sp.first, sp.second);  //add s2 as the data associated to the item
        d->predicateCB->setItemData(d->predicateCB->count()-1, sp.second, Qt::StatusTipRole);
        d->predicateCB->setItemData(d->predicateCB->count()-1, sp.second, Qt::ToolTipRole);
        //kDebug() << sp;
    }
    d->predicateCB->setCurrentIndex(-1);  //insertItem( 0, "?p" );
    d->predicateCB->blockSignals(false);
}

void QueryNode::addRestriction()
{
    QueryNode *qn = new QueryNode(d->objectCB->itemData(d->objectCB->currentIndex()).toString(), d->level+1);
    d->restrictions.append(qn);
    d->restrictionLayout->insertLayout(d->restrictionLayout->count()-1, qn);
    connect(qn, SIGNAL(queryPartChanged(QString)), this, SLOT(updateQueryPart()));
    connect(qn, SIGNAL(addVarToOutput(QString)), this, SIGNAL(addVarToOutput(QString)));
    connect(qn, SIGNAL(removeVarFromOutput(QString)), this, SIGNAL(removeVarFromOutput(QString)));
    connect(qn, SIGNAL(removeClicked(QueryNode*)), this, SLOT(removeRestriction(QueryNode*)));
}

void QueryNode::removeAllRestrictions()
{
    foreach(QueryNode *qn, d->restrictions) {
        d->restrictionLayout->removeItem(qn);
        qn->deleteLater();
    }
    d->restrictions.clear();
}

void QueryNode::removeRestriction(QueryNode *qn)
{
    //kDebug() << "Removing Restriction...";
    d->restrictionLayout->removeItem(qn);
    d->restrictions.removeAt(d->restrictions.indexOf(qn));
    qn->deleteLater();

    updateQueryPart();//refresh
}

void QueryNode::emitRemove()
{
    //kDebug() << "Emitting remove";
    emit removeClicked(this);
}

/****** QUERY COMPUTATION *******/

QString QueryNode::queryPart()
{
    //WISH: only the changed query part returns its query string:
    //       use that, and don't recompute anything

    //FIXME (future): use vars - based on the relation combobox (have a "variable" option).

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
        return QString(var + " a " + classUri + " . \n" + childrenQueryParts);
    }    

    if(d->objectCB->isEditable()) { //Literal node
        QString relStr = QString("contains");
        if(d->relationCB) {
            relStr = d->relationCB->currentText();
        }
        QString predUri = d->predicateCB->itemData(d->predicateCB->currentIndex()).toString();
        QString filterStr;
        QString object = d->objectCB->currentText();
        if (relStr == "equals") {
            filterStr = QString(". FILTER regex(" + var + ", '^" + object + "$', 'i') . ");
        } else if (relStr == "contains") {
            filterStr = QString(". FILTER regex(" + var + ", '" + object + "', 'i') . ") ;
        }
        return QString(predUri + " " + var + filterStr + "\n");
    }

    else { //Resource node
        QString predUri = d->predicateCB->itemData(d->predicateCB->currentIndex()).toString();
        QString classUri = d->objectCB->itemData(d->objectCB->currentIndex()).toString();
        QString childrenQueryParts;
        foreach(QueryNode *node, d->restrictions) {
            childrenQueryParts.append(var + " " + node->queryPart());
        }
        return QString(predUri + " " + var + " . "
                       + var + " a " + classUri +
                       + " . \n" + childrenQueryParts);
    }
}

void QueryNode::updateQueryPart()
{
    emit queryPartChanged(queryPart());
}


#include "querynode.moc"
