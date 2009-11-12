#ifndef VQBINSTANCESFORM_H
#define VQBINSTANCESFORM_H

#include "vqbform.h"
class VqbMainWindow;
class QueryThread;

namespace Ui {
    class VqbInstancesForm;
}

class VqbInstancesForm : public VqbForm {
    Q_OBJECT
    Q_DISABLE_COPY(VqbInstancesForm)
public:
    explicit VqbInstancesForm(VqbMainWindow *parent = 0);
    virtual ~VqbInstancesForm();    

private slots:
    void on_buttonAddToOutput_clicked();
    void on_listBoxConditions_changed();
    void init();

    void updateCurrentTriple();
    void updateTypes();
    void updateVars();
    //void updateCompleters();

    void updateCompletersSubject(QString text);
    void updateCompletersPredicate(QString text);
    void updateCompletersObject(QString text);

    void colorLineEdits(bool hasResults);//isEmpty specifies whether the current triple has any results or not

private:  
    Ui::VqbInstancesForm *m_ui;
    QString m_currentTriple;
    QString m_lastTriple;
    QStringList m_varList;
    QList<QueryThread*> m_queryThreads;
    QueryThread *m_queryThread;
    QString m_queryPart;

    QString constructCompletionQuery(QString text, int slotNumber);






};

#endif // VQBINSTANCESFORM_H
