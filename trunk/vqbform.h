#ifndef VQBFORM_H
#define VQBFORM_H

#include <QtGui/QWidget>
#include <QtGui/QMainWindow>

class VqbMainWindow;

class VqbForm : public QWidget
{
    Q_OBJECT

public:
    VqbForm(VqbMainWindow *parent = 0);

signals:
    void queryChanged(QString query);
    
};

#endif // VQBFORM_H
