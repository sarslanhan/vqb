#ifndef VQBFORM_H
#define VQBFORM_H

#include <QtGui/QWidget>

class QHBoxLayout;
class QVBoxLayout;
class QPushButton;

namespace Ui
{
class VqbFormClass;
}

class VqbForm : public QWidget
{
    Q_OBJECT

public:
    VqbForm(QWidget *parent = 0);
    ~VqbForm();

public slots:
    //void attachConstraint(int index, QString varName, QString varClass); //DEPRECATED
    /** Adds a variable to the output list
      */
    void queryTreeChanged(int index, QString constraint);
    void refreshQuery();
    void addVarToOutput(QString var);
    void tabChanged(int index);
    void queryTreeDeleted(int treeNumber);

signals:
    void refresh();

private:
    void init();

    class Private;
    Private * const d;

private slots:
    void addQueryTree();
    void moveOutputUp();
    void moveOutputDown();
    void removeOutput();
};

#endif // VQBFORM_H
