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
    void queryPartChanged(int index, QString constraint);
    void refreshQuery();
    void addVarToOutput(QString var);
    void tabChanged(int index);

signals:
    void refresh();

private:
    void addSubjectTree();
    void init();

    class Private;
    Private * const d;

private slots:
    void btnAdd_clicked();
};

#endif // VQBFORM_H
