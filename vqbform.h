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
    void constraintChanged( int index, QString constraint );
    void slotRefresh();
    /** Attaches a constraint line to an existing one
      */
    void attachConstraint(int index, QString varName, QString varClass );
    /** Adds a variable to the output list
      */
    void addVarToOutput( QString var );
    void refreshQuery();

signals:
    void refresh();

private:
    void addConstraint(bool isAttached, QString parentVarName, QString parentVarClass );
    void init();

    class Private;
    Private * const d;

private slots:
    void btnAdd_clicked();
};

#endif // VQBFORM_H
