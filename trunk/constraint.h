#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <QGroupBox>
#include <QList>

class QComboBox;
class QLineEdit;
class StringPair;

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
    void addSubjects( QList<StringPair> subjects );
    void addPredicates( QList<StringPair> predicates );
    void addPredicateDomain( QList<StringPair> subjects );

    void threadTerminated();

signals:
    void constraintChanged( QString queryConstraint );

private slots:
    /** Finds subjects with labels.
        filter specifies whether to filter based on the previous constraint line, or not
      */
    void findSubjectsWithLabels( bool filter);
    void findPredicatesForSubject( QString subject );
    void findDomainForPredicate( QString predicate );


    void subjectSelected( int index );
    void predicateSelected( int index );
    void unblockPredicate();/*allows to delays unblocking with a singleShot*/

private:
    void paintEvent ( QPaintEvent * event );
    void addConstraintLine();
    void init();
    /** Returns the query formed by the current constraint
      */
    QString getQueryConstraint();

    //FIXME: add a Private class, or a different static class for constants
    QList<ConstraintLine> constraintLines;
    QStringList m_relations;



};

#endif // CONSTRAINT_H
