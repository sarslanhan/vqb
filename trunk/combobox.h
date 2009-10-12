#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QComboBox>

class ComboBox : public QComboBox
{
    Q_OBJECT

public:
    ComboBox(QWidget * parent = 0);
    QString varName();

public slots:
    void addToOutput();
    //void attach();

signals:
    void addVarToOutput(QString var);
    //void attachConstraint(int constraintNo, QString var, QString className);

private:
    QString m_varName;


};

#endif // COMBOBOX_H
