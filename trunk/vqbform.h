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
    void addSubjects( QStringList subjects );

private:
    QHBoxLayout *createConstraintLine();
    void addConstraintLine();
    void init();

    class Private;
    Private * const d;

private slots:
    void on_btnAdd_clicked();
};

#endif // VQBFORM_H
