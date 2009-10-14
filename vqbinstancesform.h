#ifndef VQBINSTANCESFORM_H
#define VQBINSTANCESFORM_H

#include <QtGui/QWidget>

namespace Ui {
    class VqbInstancesForm;
}

class VqbInstancesForm : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(VqbInstancesForm)
public:
    explicit VqbInstancesForm(QWidget *parent = 0);
    virtual ~VqbInstancesForm();

private:
    Ui::VqbInstancesForm *m_ui;
};

#endif // VQBINSTANCESFORM_H
