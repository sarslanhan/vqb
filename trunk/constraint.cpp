#include "constraint.h"
#include "querythread.h"
#include "vqbform.h"
#include "combobox.h"

#include <QList>
#include <QPainter>
#include <QTimer>
#include <QPoint>
#include <QAction>

#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>

#include <KStandardAction>
#include <KAction>
#include <KRandom>
#include <KDebug>

Constraint::Constraint( int constraintNo, QWidget * parent, bool isAttached, QString parentVarName )
        : QGroupBox( QString("Constraint %1").arg(constraintNo + 1), parent)
{
    m_constraintNo = constraintNo;
    m_parent = (VqbForm *) parent;
    m_parentVarName = parentVarName;
    m_isAttached = isAttached;
    init();
}



// ************* INIT AND EVENTS ***************

void Constraint::init()
{
    setLayout( new QVBoxLayout () );
    m_relations << "contains" << "equals" ;

    addConstraintLine( true );
    findSubjectsWithLabels( false );//finds and feeds it to the Subject ComboBox

    setAttribute( Qt::WA_DeleteOnClose  );//delete when closed

    QAction *removeAction = KStandardAction::close( this, SLOT( close() ), this );
    removeAction->setText( tr("Remove &constraint") );
    removeAction->setStatusTip(tr("Removes this constraint from the query and GUI"));
    removeAction->setShortcut( 0 );

    QAction *refresh = KStandardAction::findNext( this, SLOT( returnConstraint() ), this );
    refresh->setText( tr("&Refresh") );
    refresh->setShortcut( 0 );

    QAction *attach = KStandardAction::forward( this, SLOT( attach() ), this );
    attach->setText( tr("&Attach constraint") );
    attach->setShortcut( 0 );

    this->addAction( removeAction );
    this->addAction( refresh );
    this->addAction( attach );
    this->setContextMenuPolicy( Qt::ActionsContextMenu );
}

void Constraint::paintEvent ( QPaintEvent * event )
{
    QGroupBox::paintEvent( event );
    QPainter painter( this );
}

// ************* UTILITIES ***************

QString Constraint::getQueryConstraint()
{
    QString s = constraintLines.first().s->itemData( constraintLines.first().s->currentIndex() ).toString();
    if( s.isEmpty() ) {
        return QString();
    }

    //Defining the instance
    QString varS = constraintLines.first().s->varName;
    QString q;
    QString o;
    QString p;


    if( constraintLines.count() > 1 ) {//if there's only one line, it will be generated below
        q.append( varS + " a <" + s + "> . \n" );
    }

    //adding the constraint lines
    int i = 0;
    while( i < (constraintLines.count() - 1) ) {
        p = constraintLines[i].p->itemData( constraintLines[i].p->currentIndex()  ).toString();
        q.append( varS + " <" + p + "> ");
        varS = constraintLines[i+1].s->varName; //the subject variable of the next constraint line
        q.append( varS + ". \n" );
        //q.append( varS + " a <" + s + "> .\n" ); //maybe needed, maybe not

        i++;
    }

    //final (Literal) matching
    p = constraintLines[i].p->itemData( constraintLines[i].p->currentIndex()  ).toString();
    s = constraintLines[i].s->itemData( constraintLines[i].s->currentIndex()  ).toString();//FIXME: might result in a NULL pointer
    QString rel = constraintLines[i].rel->currentText();
    o = constraintLines[i].o->currentText();
    if( !o.isEmpty() ) {
        q.append( "" + varS + " <" + p + "> ");
        varS = constraintLines[i].o->varName;
        q.append( varS + " . ");

        QString filterStr;
        if ( rel == "equals" ) {
            filterStr = QString( " FILTER regex(" + varS + ", '^" + o + "$', 'i')" );
        }
        else if( rel == "contains" ) {
            filterStr = QString( " FILTER regex(" + varS + ", '" + o + "', 'i') .\n" ) ;
        }
        kDebug() << " *** filter string: " << filterStr;
        q.append( filterStr );


    }
    else {
        q.append( varS + " a <" + s + "> .\n" );
    }


    return q;
}

QString Constraint::getRandomVarName()
{
    return QString( "?v" +  QString::number(KRandom::random()%80 + 20) ) ;
}

// ************* ACTION SLOTS ***************

void Constraint::subjectSelected( int index )
{
    //FIXME: remove all constraint lines above this one
    QString classS = constraintLines.last().s->itemData( index ).toString();
    findPredicatesForSubject( classS );
    addVariableToCB( constraintLines.last().s);
}

void Constraint::predicateSelected( int index )
{
    //FIXME: remove all constraint lines above this one
    QString predicate = constraintLines.last().p->itemData( index ).toString();
    findDomainForPredicate( predicate );
    addVariableToCB( constraintLines.last().p);
}

void Constraint::threadTerminated()
{
    kDebug() << "Thread is terminated.";
}

void Constraint::unblockPredicate()
{
    constraintLines.last().p->blockSignals( false );
}

void Constraint::returnConstraint()
{
    emit constraintChanged( m_constraintNo, getQueryConstraint() );
}

void Constraint::attach()
{
    emit attachConstraint( m_constraintNo, constraintLines.first().s->itemData( constraintLines.first().s->currentIndex() ).toString() );
}

void Constraint::close()
{
    emit constraintChanged( m_constraintNo, QString() );
    //FIXME: remove variables from the output list
    QGroupBox::close();
}

// ************* QUERYING SLOTS ***************

void Constraint::findSubjectsWithLabels( bool filter )
{
    QueryThread *qt = new QueryThread( this );
    connect( qt, SIGNAL(queryDone( QList<StringPair> )),
             this, SLOT(addSubjects( QList<StringPair> )));

    connect( qt, SIGNAL(terminated()),
             this, SLOT(threadTerminated()));

    if( filter && constraintLines.count() >= 2 ) {        //filters on previous subject and predicate
        //FIXME: replace with query below (don't use labels)
        QString p = constraintLines.at( constraintLines.count() - 2 ).p->currentText();
        QString s = constraintLines.at( constraintLines.count() - 2 ).s->currentText();
        QString q = QString("SELECT DISTINCT ?label ?classP WHERE {"
                            "  ?property <http://www.w3.org/2000/01/rdf-schema#label> \"%1\""
                            ". ?property <http://www.w3.org/2000/01/rdf-schema#domain> ?classS "
                            ". ?property <http://www.w3.org/2000/01/rdf-schema#range> ?classP "
                            ". ?classS <http://www.w3.org/2000/01/rdf-schema#label> \"%2\""
                            ". ?classP <http://www.w3.org/2000/01/rdf-schema#label> ?label }")
                    .arg( p ).arg( s );

        /*int indx = constraintLines.count() - 2;
        p = constraintLines[indx].p->itemData( constraintLines[indx].p->currentIndex()  ).toString();
        s = constraintLines[indx].s->itemData( constraintLines[indx].s->currentIndex()  ).toString();//FIXME: might result in a NULL pointer

    QString q = QString("SELECT DISTINCT ?label ?classP WHERE {"
                            "  ?property <http://www.w3.org/2000/01/rdf-schema#label> \"%1\""
                            ". ?property <http://www.w3.org/2000/01/rdf-schema#domain> ?classS "
                            ". ?property <http://www.w3.org/2000/01/rdf-schema#range> ?classP "
                            ". ?classS <http://www.w3.org/2000/01/rdf-schema#label> \"%2\""
                            ". ?classP <http://www.w3.org/2000/01/rdf-schema#label> ?label }")
                    .arg( p ).arg( s );*/
        qt->setQuery( q );
    }
    else {
        qt->setQuery( "SELECT DISTINCT ?label ?class WHERE"
                      "{ ?resource a ?class .  ?class <http://www.w3.org/2000/01/rdf-schema#label> ?label }" );
    }
    qt->start();
}

void Constraint::findPredicatesForSubject( QString subject )
{
    QueryThread *qt = new QueryThread( this );
    connect( qt, SIGNAL(queryDone( QList<StringPair> )),
             this, SLOT(addPredicates( QList<StringPair> )));

    QString query = QString( "SELECT DISTINCT ?label ?property WHERE {"
                    " ?property <http://www.w3.org/2000/01/rdf-schema#domain> <%1> "
                    " OPTIONAL { ?property  <http://www.w3.org/2000/01/rdf-schema#label> ?label } }" ).arg( subject );
    qt->setQuery( query );

    qt->start();
}

void Constraint::findDomainForPredicate( QString predicate )
{
    QueryThread *qt = new QueryThread( this );
    connect( qt, SIGNAL(queryDone( QList<StringPair> )),
             this, SLOT(addPredicateDomain( QList<StringPair> )));

    //FIXME: check parent properties too? Or the foaf:Person is the subclass of owl:Thing, just Sorano doesn't know it?
    QString query =  QString("SELECT DISTINCT ?class WHERE {"
    " <%1> <http://www.w3.org/2000/01/rdf-schema#range> ?class }").arg( predicate );
    //query.arg( predicate );

    qt->setQuery( query );

    qt->start();
}

// ************* INSERTING SLOTS ***************


void Constraint::addConstraintLine( bool isFirst )
{
    QHBoxLayout * layout = new QHBoxLayout();

    ComboBox *cb1 = new ComboBox();
    if (isFirst && m_isAttached) { //if this is the first line in an attached constraint
        cb1->setEditText( m_parentVarName );
        cb1->setEditable( false );
    }
    else {
        cb1->setEditable( true );
    }

    ComboBox *cb2 = new ComboBox();
    ComboBox *cb3 = new ComboBox();
    ComboBox *cb4 = new ComboBox();

    cb2->setEditable( true );
    cb4->setEditable( true );

    layout->addWidget( cb1, 1 );
    layout->addWidget( cb2, 1 );
    layout->addWidget( cb3, 1 );
    layout->addWidget( cb4, 1 );

    ((QBoxLayout*)(this->layout()))->addLayout( layout );

    ConstraintLine cl;
    cl.s = cb1;
    cl.p = cb2;
    cl.rel = cb3;
    cl.o = cb4;

    constraintLines.append(cl);

    connect( cl.s, SIGNAL(currentIndexChanged( int )),
             this, SLOT(subjectSelected( int )) );

    connect( cl.p, SIGNAL(currentIndexChanged( int )),
             this, SLOT(predicateSelected( int )) );
}

//FIXME: get pairs of values: the label and the URI (??)
void Constraint::addSubjects( QList<StringPair> subjects )
{
    if( m_isAttached && constraintLines.count() == 1 ) {//if we are dealing with the varName of the previous constraint
        constraintLines.last().s->addItem( m_parentVarName, m_parentClass );
        return;
    }
    constraintLines.last().s->clear();
    constraintLines.last().p->clear();
    foreach( StringPair sp, subjects ) {
        constraintLines.last().s->addItem( sp.s1, sp.s2 );//add s2 as the data associated to the item
        //kDebug() << "++**++ Added item" << sp.s1 << sp.s2;
    }
    //constraintLines.last().s->setCurrentIndex( -1 );
}

//FIXME: get pairs of values: the label and the URI (??)
void Constraint::addPredicates( QList<StringPair> predicates )
{
    constraintLines.last().p->blockSignals( true );//don't trigger predicate domain population
    constraintLines.last().p->clear();
    foreach( StringPair sp, predicates ) {
        //FIXME: if label is empty, parse
        constraintLines.last().p->addItem( sp.s1.isEmpty() ? sp.s2 : sp.s1, sp.s2 );//add s2 as the data associated to the item
    }
    constraintLines.last().p->setCurrentIndex( -1 );//insertItem( 0, "?p" );
    constraintLines.last().p->blockSignals( false );
}

void Constraint::addPredicateDomain( QList<StringPair> subjects )
{
    if( !subjects.isEmpty() ) {
        kDebug() << "Comparing " << subjects.first().s1 << " to " << "http://www.w3.org/2000/01/rdf-schema#Literal";
        if ( subjects.first().s1 == "http://www.w3.org/2001/XMLSchema#string" ||
             subjects.first().s1 == "http://www.w3.org/2000/01/rdf-schema#Literal" ) {
            constraintLines.last().rel->clear();
            constraintLines.last().rel->insertItems(0, m_relations);
            addVariableToCB( constraintLines.last().o);
        }
        else if ( !subjects.empty() ) {
        //disable editable part
        constraintLines.last().rel->setDisabled( true );
        constraintLines.last().o->setDisabled( true );

        //add new constraint line
        addConstraintLine();
        findSubjectsWithLabels( true );
        }
    }
}

void Constraint::addVariableToCB( ComboBox *cb )
{
    QString v = getRandomVarName();
    cb->setToolTip( v );
    cb->varName = v;

    QAction *add = KStandardAction::forward( cb, SLOT(addToOutput()), this );
    add->setText( QString("Add *" + v + "* to output") );
    add->setStatusTip(tr("Adds the variable to the output list"));
    add->setShortcut( 0 );
    cb->addAction( add );
    //FIXME: check if DefaultContextMenu doesn't contain important options
    cb->setContextMenuPolicy(Qt::ActionsContextMenu);

    connect( cb, SIGNAL(addVarToOutput(QString)),
            m_parent, SLOT(addVarToOutput(QString)) );


    kDebug() << "added variable to CD";
}


#include "constraint.moc"


