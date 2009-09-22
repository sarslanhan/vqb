#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QComboBox>

//FIXME: add this to constraint.cpp ?

class ComboBox : public QComboBox
{
    Q_OBJECT

public:
    ComboBox( QWidget * parent = 0 );

    QString varName; /*I know this is a terrible way to do it. I'm sorry. It's almost 8 o'clock, and I'm tired, and I have to present this practically a day from now. */

public slots:
    void addToOutput();

signals:
    void addVarToOutput( QString var );

};

#endif // COMBOBOX_H
