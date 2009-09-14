#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <QGroupBox>
#include <QList>

class QComboBox;
class QLineEdit;

struct ConstraintLine {
    QComboBox *s,*p,*rel;
    QLineEdit *o;
};

class Constraint : public QGroupBox
{
    Q_OBJECT

public:
    Constraint ( const QString & title, QWidget * parent = 0 );

public slots:
    void addSubjects( QStringList subjects );
    void addPredicates( QStringList predicates );
    void addPredicateDomain( QStringList subjects );

    void threadTerminated();

private slots:
    void findSubjectsWithLabels();
    void findPredicatesForSubject( QString subject );
    void findDomainForPredicate( QString predicate );


    void subjectSelected( QString subject );
    void predicateSelected( QString predicate );
    void unblockPredicate();/*allows to delays unblocking with a singleShot*/

private:
    void paintEvent ( QPaintEvent * event );

    void addConstraintLine();
    QList<ConstraintLine> constraintLines;

};

#endif // CONSTRAINT_H
