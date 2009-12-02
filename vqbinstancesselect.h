#ifndef VQBINSTANCESSELECT_H
#define VQBINSTANCESSELECT_H

#include "vqbform.h"
class VqbMainWindow;
class QueryThread;

namespace Ui {
    class VqbInstancesSelect;
}

class VqbInstancesSelect : public VqbForm {
    Q_OBJECT
    Q_DISABLE_COPY(VqbInstancesSelect)
public:
    explicit VqbInstancesSelect(VqbMainWindow *parent = 0);
    virtual ~VqbInstancesSelect();

private slots:
    void on_buttonRemoveFromOutput_clicked();
    void on_buttonAddToOutput_clicked();
    void on_listBoxConditions_changed();
    void init();

    void updateCurrentTriple();
    void updateTypes();
    void updateVars();
    void updateQuery();

    void updateCompletersSubject(QString text);
    void updateCompletersPredicate(QString text);
    void updateCompletersObject(QString text);

    void colorLineEdits();

private:  
    Ui::VqbInstancesSelect *m_ui;
    QString m_currentTriple;
    QStringList m_varList;
    QList<QueryThread*> m_queryThreads;
    QueryThread *m_queryThread;
    QString m_queryPart;

    QString constructCompletionQuery(QString text, int slotNumber);


};

#endif // VQBINSTANCESSELECT_H
