#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <QGroupBox>
#include <QList>

class ComboBox;
class QLineEdit;
class StringPair;
class VqbForm;

struct ConstraintLine {
    ComboBox *s, *p, *rel, *o;
    //QString sVar, pVar, oVar;//variables names corresponding to the elements
};

class Constraint : public QGroupBox
{
    Q_OBJECT

public:
    Constraint(int constraintNo, QWidget * parent = 0, bool isAttached = false, QString parentVarName = QString(), QString parentClassName = QString());

public slots:
    void addSubjects(QList<StringPair> subjects);
    void addPredicates(QList<StringPair> predicates);
    void addPredicateDomain(QList<StringPair> subjects);

    void threadTerminated();
    void returnConstraint();
    void close();

signals:
    /** Notifies a query changed, with the corresponding constraint number
     */
    void constraintChanged(int constraintNo, QString queryConstraint);


private slots:
    /** Finds subjects with labels.
        filter specifies whether to filter based on the previous constraint line, or not
      */
    void findSubjectsWithLabels(bool filter);
    void findPredicatesForSubject(QString subject);
    void findDomainForPredicate(QString predicate);


    void subjectSelected(int index);
    void predicateSelected(int index);
    void unblockPredicate();/*allows to delays unblocking with a singleShot*/

private:
    void paintEvent(QPaintEvent * event);
    void addConstraintLine(bool isFirst = false);
    void init();
    QString getRandomVarName();

    /** Returns the query formed by the current constraint
      */
    QString getQueryConstraint();

    /** Adds a variable name to the ComboBox,
        and the action of adding it to the output
      */
    void addVariableToCB(ComboBox *cb);

    /** Adds two actions:
                - adding the variable to the output
                - attaching a constraint
      */
    void addActionsToCB(ComboBox *cb);

    //FIXME: add a Private class, and/or a different static class for constants
    QList<ConstraintLine> constraintLines;
    QStringList m_relations;
    int m_constraintNo;
    VqbForm* m_parent;
    bool m_isAttached;
    QString m_parentVarName;
    QString m_parentClass;

};

#endif // CONSTRAINT_H
