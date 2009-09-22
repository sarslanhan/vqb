#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <QGroupBox>
#include <QList>

class ComboBox;
class QLineEdit;
class StringPair;
class VqbForm;

struct ConstraintLine {
    ComboBox *s,*p,*rel, *o;
    //QString sVar, pVar, oVar;//variables names corresponding to the elements
};

class Constraint : public QGroupBox
{
    Q_OBJECT

public:
    Constraint ( int constraintNo, QWidget * parent = 0, bool isAttached = false, QString parentVarName = QString() );

public slots:
    void addSubjects( QList<StringPair> subjects );
    void addPredicates( QList<StringPair> predicates );
    void addPredicateDomain( QList<StringPair> subjects );

    void threadTerminated();
    void returnConstraint();
    void close();

signals:
    /** Notifies a query changed, with the corresponding constraint number
     */
    void constraintChanged( int constraintNo, QString queryConstraint );
    /** Signals the parrent to attach a constraint line`
      */
    void attachConstraint( int constraintNo, QString varName );

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
    /** Signals the parrent to attach a constraint line`
      */
    void attach();

private:
    void paintEvent ( QPaintEvent * event );
    void addConstraintLine( bool isFirst = false);
    void init();
    QString getRandomVarName();
    /** Returns the query formed by the current constraint
      */
    QString getQueryConstraint();
    void addVariableToCB( ComboBox *cb );

    //FIXME: add a Private class, or a different static class for constants
    QList<ConstraintLine> constraintLines;
    QStringList m_relations;
    int m_constraintNo;
    VqbForm* m_parent;
    bool m_isAttached;
    QString m_parentVarName;
    QString m_parentClass;

};

#endif // CONSTRAINT_H
