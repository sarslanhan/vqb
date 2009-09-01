#include "constraint.h"

#include <QList>
#include <QComboBox>
#include <QLineEdit>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

Constraint::Constraint( const QString & title, QWidget * parent )
        : QGroupBox(title, parent)
{
    addConstraintLine();
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

    setLayout( layout );

    ConstraintLine cl;
    cl.s = cb1;
    cl.p = cb2;
    cl.rel = cb3;
    cl.o = le;

    constraintLines.append(cl);

}
