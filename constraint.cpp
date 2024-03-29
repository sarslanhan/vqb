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

#include <kdebug.h>
#include <kstandardaction.h>
#include <kaction.h>
#include <krandom.h>

Constraint::Constraint(int constraintNo, QWidget * parent, bool isAttached, QString parentVarName, QString parentClassName)
        : QGroupBox(QString("Constraint %1").arg(constraintNo + 1), parent)
{
    m_constraintNo = constraintNo;
    m_parent = (VqbForm *) parent;
    m_parentVarName = parentVarName;
    m_parentClass = parentClassName;
    m_isAttached = isAttached;
    init();
}



// ************* INIT AND EVENTS ***************

void Constraint::init()
{
    if (m_isAttached) {
        setTitle(QString("Constraint %1 (attached to %2)").arg(m_constraintNo + 1).arg(m_parentVarName));
    }
    setLayout(new QVBoxLayout());
    m_relations << "contains" << "equals" ;

    addConstraintLine(true);  //isFirst = true
    if (!m_isAttached) {
        findSubjectsWithLabels(false);  //filter = false
    } else { //if it is attached
        addSubjects(QList<StringPair>());
    }

    setAttribute(Qt::WA_DeleteOnClose);   //delete when closed

    QAction *removeAction = KStandardAction::close(this, SLOT(close()), this);
    removeAction->setText(tr("Remove &constraint"));
    removeAction->setStatusTip(tr("Removes this constraint from the query and GUI"));
    removeAction->setShortcut(0);

    QAction *refresh = KStandardAction::redisplay(this, SLOT(returnConstraint()), this);
    refresh->setText(tr("&Refresh constraint"));
    refresh->setShortcut(0);

    /*QAction *attach = KStandardAction::forward( this, SLOT( attach() ), this );
    attach->setText( tr("&Attach constraint") );
    attach->setShortcut( 0 );
    */
    this->addAction(removeAction);
    this->addAction(refresh);
    //this->addAction( attach );
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void Constraint::paintEvent(QPaintEvent * event)
{
    QGroupBox::paintEvent(event);
    QPainter painter(this);
}

// ************* UTILITIES ***************

QString Constraint::getQueryConstraint()
{
    QString s = constraintLines.first().s->itemData(constraintLines.first().s->currentIndex()).toString();
    if (s.isEmpty()) {
        return QString();
    }

    //Defining the instance
    QString varS = constraintLines.first().s->varName;
    QString q;
    QString o;
    QString p;


    //if( constraintLines.count() > 1 ) {//if there's only one line, it will be generated below
    q.append(varS + " a <" + s + "> . \n");
    //}

    //adding the constraint lines
    int i = 0;
    while (i < (constraintLines.count() - 1)) {
        p = constraintLines[i].p->itemData(constraintLines[i].p->currentIndex()).toString();
        q.append(varS + " <" + p + "> ");
        varS = constraintLines[i+1].s->varName; //the subject variable of the next constraint line
        q.append(varS + ". \n");
        //q.append( varS + " a <" + s + "> .\n" ); //maybe needed, maybe not

        i++;
    }

    //final (Literal) matching
    p = constraintLines[i].p->itemData(constraintLines[i].p->currentIndex()).toString();
    s = constraintLines[i].s->itemData(constraintLines[i].s->currentIndex()).toString();   //FIXME: might result in a NULL pointer
    QString rel = constraintLines[i].rel->currentText();
    o = constraintLines[i].o->currentText();
    //if( !o.isEmpty() ) {
    if (!p.isEmpty()) {
        q.append("" + varS + " <" + p + "> ");
        varS = constraintLines[i].o->varName;
        q.append(varS + " . ");

        QString filterStr;
        if (rel == "equals") {
            filterStr = QString(" FILTER regex(" + varS + ", '^" + o + "$', 'i') .\n");
        } else if (rel == "contains") {
            filterStr = QString(" FILTER regex(" + varS + ", '" + o + "', 'i') .\n") ;
        }
        //kDebug() << " *** filter string: " << filterStr;
        q.append(filterStr);

    }
    /*}
    else {
        q.append( varS + " a <" + s + "> .\n" );
    }
    */


    return q;
}

QString Constraint::getRandomVarName()
{
    return QString("?v" +  QString::number(KRandom::random() % 80 + 20)) ;
}

// ************* ACTION SLOTS ***************

void Constraint::subjectSelected(int index)
{
    //FIXME: remove all constraint lines above this one
    QString classS = constraintLines.last().s->itemData(index).toString();
    constraintLines.last().s->varClass = classS;//the combobox's class is the class of the selected item
    findPredicatesForSubject(classS);
    returnConstraint();
}

void Constraint::predicateSelected(int index)
{
    //FIXME: remove all constraint lines above this one
    QString predicate = constraintLines.last().p->itemData(index).toString();
    constraintLines.last().p->varClass = predicate;//the combobox's class is the class of the selected item
    findDomainForPredicate(predicate);
    addVariableToCB(constraintLines.last().p);
    //returnConstraint();
}

void Constraint::threadTerminated()
{
    //kDebug() << "Thread is terminated.";
}

void Constraint::unblockPredicate()
{
    constraintLines.last().p->blockSignals(false);
}

void Constraint::returnConstraint()
{
    emit constraintChanged(m_constraintNo, getQueryConstraint());
}

void Constraint::close()
{
    emit constraintChanged(m_constraintNo, QString());
    //FIXME: remove variables from the output list
    QGroupBox::close();
}

// ************* QUERYING SLOTS ***************

void Constraint::findSubjectsWithLabels(bool filter)
{
    QueryThread *qt = new QueryThread(this);
    connect(qt, SIGNAL(queryDone(QList<StringPair>)),
            this, SLOT(addSubjects(QList<StringPair>)));

    connect(qt, SIGNAL(terminated()),
            this, SLOT(threadTerminated()));

    if (filter && constraintLines.count() >= 2) {  //if NOT the first line: filters on previous subject and predicate
        //FIXME: replace with query below (don't use labels)
        QString p = constraintLines.at(constraintLines.count() - 2).p->currentText();
        QString s = constraintLines.at(constraintLines.count() - 2).s->currentText();
        QString q = QString("SELECT DISTINCT ?label ?classP WHERE {"
                            "  ?property <http://www.w3.org/2000/01/rdf-schema#label> \"%1\""
                            ". ?property <http://www.w3.org/2000/01/rdf-schema#domain> ?classS "
                            ". ?property <http://www.w3.org/2000/01/rdf-schema#range> ?classP "
                            ". ?classS <http://www.w3.org/2000/01/rdf-schema#label> \"%2\""
                            ". ?classP <http://www.w3.org/2000/01/rdf-schema#label> ?label }")
                    .arg(p).arg(s);

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
        qt->setQuery(q);
    } else {
        qt->setQuery("SELECT DISTINCT ?label ?class WHERE"
                     "{ ?resource a ?class .  ?class <http://www.w3.org/2000/01/rdf-schema#label> ?label }");
    }
    qt->start();
}

void Constraint::findPredicatesForSubject(QString subject)
{
    QueryThread *qt = new QueryThread(this);
    connect(qt, SIGNAL(queryDone(QList<StringPair>)),
            this, SLOT(addPredicates(QList<StringPair>)));

    QString query = QString("SELECT DISTINCT ?label ?property WHERE {"
                            " ?property <http://www.w3.org/2000/01/rdf-schema#domain> <%1> ."
                            " OPTIONAL { ?property  <http://www.w3.org/2000/01/rdf-schema#label> ?label } }").arg(subject);
    qt->setQuery(query);

    qt->start();
}

void Constraint::findDomainForPredicate(QString predicate)
{
    QueryThread *qt = new QueryThread(this);
    connect(qt, SIGNAL(queryDone(QList<StringPair>)),
            this, SLOT(addPredicateDomain(QList<StringPair>)));

    //FIXME: check parent properties too? Or the foaf:Person is the subclass of owl:Thing, just Sorano doesn't know it?
    QString query =  QString("SELECT DISTINCT ?class WHERE {"
                             " <%1> <http://www.w3.org/2000/01/rdf-schema#range> ?class }").arg(predicate);
    //query.arg( predicate );

    qt->setQuery(query);

    qt->start();
}

// ************* INSERTING SLOTS ***************


void Constraint::addConstraintLine(bool isFirst)
{
    QHBoxLayout * layout = new QHBoxLayout();

    ComboBox *cb1 = new ComboBox();
    if (isFirst && m_isAttached) { //if this is the first line in an attached constraint
        cb1->setEditText(m_parentVarName);
        cb1->setEditable(false);
        cb1->varName = m_parentVarName;
        cb1->varClass = m_parentClass;
    } else {
        cb1->setEditable(true);
    }

    ComboBox *cb2 = new ComboBox();
    ComboBox *cb3 = new ComboBox();
    ComboBox *cb4 = new ComboBox();

    cb2->setEditable(true);
    cb4->setEditable(true);

    layout->addWidget(cb1, 1);
    layout->addWidget(cb2, 1);
    layout->addWidget(cb3, 1);
    layout->addWidget(cb4, 1);

    ((QBoxLayout*)(this->layout()))->addLayout(layout);

    ConstraintLine cl;
    cl.s = cb1;
    cl.p = cb2;
    cl.rel = cb3;
    cl.o = cb4;

    constraintLines.append(cl);

    connect(cl.s, SIGNAL(currentIndexChanged(int)),
            this, SLOT(subjectSelected(int)));

    connect(cl.p, SIGNAL(currentIndexChanged(int)),
            this, SLOT(predicateSelected(int)));

    connect(cl.rel, SIGNAL(currentIndexChanged(int)),
            this, SLOT(returnConstraint()));

    connect(cl.o, SIGNAL(editTextChanged(QString)),
            this, SLOT(returnConstraint()));
}

void Constraint::addSubjects(QList<StringPair> subjects)
{
    constraintLines.last().s->clear();
    constraintLines.last().p->clear();

    //FIXME: might add variable action several times?
    if (!m_isAttached) {
        addVariableToCB(constraintLines.last().s);
    }

    addActionsToCB(constraintLines.last().s);

    if (m_isAttached && constraintLines.count() == 1) { //if we are dealing with the varName of the previous constraint
        constraintLines.last().s->addItem(m_parentVarName, m_parentClass);
        return;
    }

    foreach(StringPair sp, subjects) {
        constraintLines.last().s->addItem(sp.s1, sp.s2);  //add s2 as the data associated to the item
        //kDebug() << "++**++ Added item" << sp.s1 << sp.s2;
    }
    //constraintLines.last().s->setCurrentIndex( -1 );
}

void Constraint::addPredicates(QList<StringPair> predicates)
{
    constraintLines.last().p->blockSignals(true);  //don't trigger predicate domain population
    constraintLines.last().p->clear();
    foreach(StringPair sp, predicates) {
        //FIXME: if label is empty, parse (I don't know what I meant by this) - ?????
        constraintLines.last().p->addItem(sp.s1.isEmpty() ? sp.s2 : sp.s1, sp.s2);  //add s2 as the data associated to the item
    }
    constraintLines.last().p->setCurrentIndex(-1);  //insertItem( 0, "?p" );
    constraintLines.last().p->blockSignals(false);
}

void Constraint::addPredicateDomain(QList<StringPair> subjects)
{
    if (!subjects.isEmpty()) {
        //kDebug() << "Comparing " << subjects.first().s1 << " to " << "http://www.w3.org/2000/01/rdf-schema#Literal";
        if (subjects.first().s1 == "http://www.w3.org/2001/XMLSchema#string" ||
                subjects.first().s1 == "http://www.w3.org/2000/01/rdf-schema#Literal") {
            constraintLines.last().rel->clear();
            constraintLines.last().rel->insertItems(0, m_relations);
            addVariableToCB(constraintLines.last().o);
        } else if (!subjects.empty()) {
            //disable editable part
            constraintLines.last().rel->setDisabled(true);
            constraintLines.last().o->setDisabled(true);

            //add new constraint line
            addConstraintLine();
            findSubjectsWithLabels(true);
        }
    }
}

void Constraint::addVariableToCB(ComboBox *cb)
{
    QString v = getRandomVarName();
    cb->setToolTip(v);
    cb->varName = v;

    QAction *add = KStandardAction::findNext(cb, SLOT(addToOutput()), this);
    add->setText(QString("Add " + cb->varName + " to output"));
    add->setStatusTip(tr("Adds the variable to the output list"));
    add->setShortcut(0);

    connect(cb, SIGNAL(addVarToOutput(QString)),
            m_parent, SLOT(addVarToOutput(QString)));

    foreach(QAction *a, cb->actions()) {
        cb->removeAction(a);
    }
    cb->addAction(add);
    cb->setContextMenuPolicy(Qt::ActionsContextMenu);
}


void Constraint::addActionsToCB(ComboBox *cb)
{
    QAction *add = KStandardAction::findNext(cb, SLOT(addToOutput()), this);
    add->setText(QString("Add " + cb->varName + " to output"));
    add->setStatusTip(tr("Adds the variable to the output list"));
    add->setShortcut(0);

    QAction *attach = KStandardAction::redo(cb, SLOT(attach()), this);
    attach->setText(QString("&Attach constraint to " + cb->varName));
    attach->setShortcut(0);

    cb->actions().clear();//clear previous actions
    foreach(QAction *a, cb->actions()) {
        cb->removeAction(a);
    }
    cb->addAction(add);
    cb->addAction(attach);
    cb->setContextMenuPolicy(Qt::ActionsContextMenu);

    disconnect(cb, SIGNAL(addVarToOutput(QString)), //disconnect slots from that signal
               0, 0);

    connect(cb, SIGNAL(addVarToOutput(QString)),
            m_parent, SLOT(addVarToOutput(QString)));

    connect(cb, SIGNAL(attachConstraint(int, QString, QString)),
            m_parent, SLOT(attachConstraint(int, QString, QString)));
}


#include "constraint.moc"


