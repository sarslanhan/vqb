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
    findSubjectsWithLabels();//finds and feeds it to the Subject ComboBox
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

    connect( cl.s, SIGNAL(currentIndexChanged( QString )),
             this, SLOT(subjectSelected( QString )) );

    connect( cl.p, SIGNAL(currentIndexChanged( QString )),
             this, SLOT(predicateSelected( QString )) );
}


// ************* ACTION SLOTS ***************

void Constraint::subjectSelected( QString subject )
{
    //FIXME: remove all constraint lines above this one
    findPredicatesForSubject( subject );
}

void Constraint::predicateSelected( QString predicate )
{
    //FIXME: remove all constraint lines above this one
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

void Constraint::findSubjectsWithLabels()
{
    QueryThread *qt = new QueryThread( this );
    connect( qt, SIGNAL(queryDone( QStringList )),
             this, SLOT(addSubjects(QStringList)));

    connect( qt, SIGNAL(terminated()),
             this, SLOT(threadTerminated()));

    qt->setQuery( "?resource a ?class .  ?class <http://www.w3.org/2000/01/rdf-schema#label> ?label", "label" );
    qt->start();
}

void Constraint::findPredicatesForSubject( QString subject )
{
    QueryThread *qt = new QueryThread( this );
    connect( qt, SIGNAL(queryDone( QStringList )),
             this, SLOT(addPredicates(QStringList)));

    QString query =  "?resource a ?class";
    query += " . ?class <http://www.w3.org/2000/01/rdf-schema#label> \"" + subject + "\"";
    query += " . ?property <http://www.w3.org/2000/01/rdf-schema#domain> ?class ;";
    query += "             <http://www.w3.org/2000/01/rdf-schema#label> ?label ";
    qt->setQuery( query, "label" );

    qt->start();
}

void Constraint::findDomainForPredicate( QString predicate )
{
    QueryThread *qt = new QueryThread( this );
    connect( qt, SIGNAL(queryDone( QStringList )),
             this, SLOT(addPredicateDomain(QStringList)));

    QString query =  QString("?resource a ?class"
    " . ?property <http://www.w3.org/2000/01/rdf-schema#label> \"%1\"  "
    " . ?property <http://www.w3.org/2000/01/rdf-schema#range> ?class ;"
    " . ?class    <http://www.w3.org/2000/01/rdf-schema#label> ?label  ").arg( predicate );
    //query.arg( predicate );

    qt->setQuery( query, "label" );

    qt->start();
}

// ************* INSERTING SLOTS ***************

//FIXME: get pairs of values: the label and the URI (??)
void Constraint::addSubjects( QStringList subjects )
{
    this->constraintLines.last().s->clear();
    this->constraintLines.last().s->insertItems( 0, subjects );
}

//FIXME: get pairs of values: the label and the URI (??)
void Constraint::addPredicates( QStringList predicates )
{
    this->constraintLines.last().p->clear();
    this->constraintLines.last().p->insertItems( 0, predicates );
}

/*void Constraint::addRelations( QStringList relations )
{
    this->constraintLines.last().rel->clear();
    this->constraintLines.last().rel->insertItems( 0, relations );
}
*/

void Constraint::addPredicateDomain( QStringList subjects )
{
    if( !subjects.isEmpty() ) {
        //disable editable part
        constraintLines.last().rel->setDisabled( true );
        constraintLines.last().p->setDisabled( true );

        //add new constraint line
        addConstraintLine();

        constraintLines.last().p->blockSignals( true );
        addSubjects( subjects );
        QTimer::singleShot( 1000, this, SLOT(unblockPredicate()));
    }
}



#include "constraint.moc"


