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
public:
    Constraint ( const QString & title, QWidget * parent = 0 );
private:
    void addConstraintLine();
    QList<ConstraintLine> constraintLines;
};

#endif // CONSTRAINT_H
