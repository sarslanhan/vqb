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

private:
    QHBoxLayout *createConstraintLine();
    void addConstraintLine();
    void init();

    Ui::VqbFormClass *ui;
    QPushButton *m_btnAdd;
    QVBoxLayout *m_topLayout; //layout holding the constraint lines    

private slots:
    void on_btnAdd_clicked();
};

#endif // VQBFORM_H
