#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QComboBox>

//FIXME: add this to constraint.cpp ?

class ComboBox : public QComboBox
{
    Q_OBJECT

public:
    ComboBox(QWidget * parent = 0);
    static QString getRandomVarName();

    QString varName; /*I know this is a terrible way to do it. I'm sorry. It's almost 8 o'clock, and I'm tired, and I have to present this practically a day from now. */
    QString varClass;

public slots:
    void addToOutput();
    void attach();

signals:
    void addVarToOutput(QString var);
    void attachConstraint(int constraintNo, QString var, QString className);

};

#endif // COMBOBOX_H
