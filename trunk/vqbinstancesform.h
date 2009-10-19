#ifndef VQBINSTANCESFORM_H
#define VQBINSTANCESFORM_H

#include "vqbform.h"
class VqbMainWindow;

namespace Ui {
    class VqbInstancesForm;
}

class VqbInstancesForm : public VqbForm {
    Q_OBJECT
    Q_DISABLE_COPY(VqbInstancesForm)
public:
    explicit VqbInstancesForm(VqbMainWindow *parent = 0);
    virtual ~VqbInstancesForm();

private:
    Ui::VqbInstancesForm *m_ui;
    QString m_currentTriple;
    QString m_lastQuery;    

private slots:
    void init();
    void updateCurrentTriple();
    void updateTypes();
    void updateCompleters();
    void colorLineEdits(bool hasResults);//isEmpty specifies whether the current triple has any results or not



};

#endif // VQBINSTANCESFORM_H
