#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QComboBox>
class QPaintEvent;

//FIXME: rename to something more relevant

/**
  Custom QComboBox: having a variable name, and a signal returning it
  */

class ComboBox : public QComboBox
{
    Q_OBJECT

public:
    ComboBox(QWidget * parent = 0);
    QString varName();

public slots:
    void addToOutput();
    void removeFromOutput();
    void paintEvent ( QPaintEvent * event );

signals:
    void addVarToOutput(QString var);
    void removeVarFromOutput(QString var);

private:
    QString m_varName;
    bool m_isOutput;
    QAction *m_removeAction;
};

#endif // COMBOBOX_H

