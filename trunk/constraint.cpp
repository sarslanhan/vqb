#include "constraint.h"
#include "querythread.h"


#include <QList>
#include <QPainter>
#include <QTimer>

#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>

#include <KDebug>

Constraint::Constraint( const QString & title, QWidget * parent )
        : QGroupBox(title, parent)
{
    //QPushButton *btnClose = new QPushButton( this );
    setLayout( new QVBoxLayout () );
    addConstraintLine();
    findSubjectsWithLabels( false );//finds and feeds it to the Subject ComboBox
    m_relations << "equals" << "contains";
}

void Constraint::paintEvent ( QPaintEvent * event )
{
    QGroupBox::paintEvent( event );
    QPainter painter( this );
}

void Constraint::addConstraintLine()
{
    QHBoxLayout * layout = new QHBoxLayout();

    QComboBox *cb1 = new QComboBox();
    QComboBox *cb2 = new QComboBox();
    QComboBox *cb3 = new QComboBox();
    QLineEdit *le = new QLineEdit();

    cb1->setEditable( true );
    cb2->setEditable( true );

    layout->addWidget( cb1, 1 );
    layout->addWidget( cb2, 1 );
    layout->addWidget( cb3, 1 );
    layout->addWidget( le, 1 );

    ((QBoxLayout*)(this->layout()))->addLayout( layout );

    ConstraintLine cl;
    cl.s = cb1;
    cl.p = cb2;
    cl.rel = cb3;
    cl.o = le;

    constraintLines.append(cl);

    connect( cl.s, SIGNAL(currentIndexChanged( int )),
             this, SLOT(subjectSelected( int )) );

    connect( cl.p, SIGNAL(currentIndexChanged( int )),
             this, SLOT(predicateSelected( int )) );
}


// ************* ACTION SLOTS ***************

void Constraint::subjectSelected( int index )
{
    //FIXME: remove all constraint lines above this one
    QString classS = constraintLines.last().s->itemData( index ).toString();
    findPredicatesForSubject( classS );
}

void Constraint::predicateSelected( int index )
{
    //FIXME: remove all constraint lines above this one
    QString predicate = constraintLines.last().p->itemData( index ).toString();
    findDomainForPredicate( predicate );
}

void Constraint::threadTerminated()
{
    kDebug() << "Thread is terminated.";
}

void Constraint::unblockPredicate()
{
    constraintLines.last().p->blockSignals( false );
}

// ************* QUERYING SLOTS ***************

void Constraint::findSubjectsWithLabels( bool filter )
{
    QueryThread *qt = new QueryThread( this );
    connect( qt, SIGNAL(queryDone( QList<StringPair> )),
             this, SLOT(addSubjects( QList<StringPair> )));

    connect( qt, SIGNAL(terminated()),
             this, SLOT(threadTerminated()));

    if( filter && constraintLines.count() >= 2 ) {
        //take previous subject and predicate
        QString p = constraintLines.at( constraintLines.count() - 2 ).p->currentText();
        QString s = constraintLines.at( constraintLines.count() - 2 ).s->currentText();

        QString q = QString("SELECT DISTINCT ?label ?classP WHERE {"
                            "  ?property <http://www.w3.org/2000/01/rdf-schema#label> \"%1\""
                            ". ?property <http://www.w3.org/2000/01/rdf-schema#domain> ?classS "
                            ". ?property <http://www.w3.org/2000/01/rdf-schema#range> ?classP "
                            ". ?classS <http://www.w3.org/2000/01/rdf-schema#label> \"%2\""
                            ". ?classP <http://www.w3.org/2000/01/rdf-schema#label> ?label }")
                    .arg( p ).arg( s );
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
                    " ?property <http://www.w3.org/2000/01/rdf-schema#domain> <%1>;"
                    "             <http://www.w3.org/2000/01/rdf-schema#label> ?label }" ).arg( subject );
    qt->setQuery( query );

    qt->start();
}

void Constraint::findDomainForPredicate( QString predicate )
{
    QueryThread *qt = new QueryThread( this );
    connect( qt, SIGNAL(queryDone( QList<StringPair> )),
             this, SLOT(addPredicateDomain( QList<StringPair> )));

    QString query =  QString("?resource a ?class"
    " . ?property <http://www.w3.org/2000/01/rdf-schema#label> \"%1\"  "
    " . ?property <http://www.w3.org/2000/01/rdf-schema#range> ?class ;").arg( predicate );
    //query.arg( predicate );

    qt->setQuery( query );

    qt->start();
}

// ************* INSERTING SLOTS ***************

//FIXME: get pairs of values: the label and the URI (??)
void Constraint::addSubjects( QList<StringPair> subjects )
{
    constraintLines.last().s->clear();
    constraintLines.last().p->clear();
    foreach( StringPair sp, subjects ) {
        constraintLines.last().s->addItem( sp.s1, sp.s2 );//add s2 as the data associated to the item
        kDebug() << "++**++ Added item" << sp.s1 << sp.s2;
    }
    //constraintLines.last().s->insertItem( 0, "?s" );
}

//FIXME: get pairs of values: the label and the URI (??)
void Constraint::addPredicates( QList<StringPair> predicates )
{
    constraintLines.last().p->blockSignals( true );//don't trigger predicate domain population
    constraintLines.last().p->clear();
    foreach( StringPair sp, predicates ) {
        constraintLines.last().p->addItem( sp.s1, sp.s2 );//add s2 as the data associated to the item
    }
    constraintLines.last().p->setCurrentIndex( -1 );//insertItem( 0, "?p" );
    constraintLines.last().p->blockSignals( false );
}

void Constraint::addPredicateDomain( QList<StringPair> subjects )
{
    if( !subjects.isEmpty() ) {
        if ( subjects.first().s2 == "<http://www.w3.org/2001/XMLSchema#string>" ) {
            constraintLines.last().rel->clear();
            constraintLines.last().rel->insertItems(0, m_relations);
        }
        else if ( !subjects.empty() ) {
        //disable editable part
        constraintLines.last().rel->setDisabled( true );
        constraintLines.last().p->setDisabled( true );

        //add new constraint line
        addConstraintLine();
        findSubjectsWithLabels( true );
        }
    }
}



#include "constraint.moc"


