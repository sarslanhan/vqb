#ifndef VQBINSTANCESSELECT_H
#define VQBINSTANCESSELECT_H

#include "vqbform.h"
class VqbMainWindow;
class QueryThread;
class QModelIndex;

namespace Ui {
    class VqbInstancesSelect;
}

class VqbInstancesSelect : public VqbForm {
    Q_OBJECT
    Q_DISABLE_COPY(VqbInstancesSelect)
public:
    explicit VqbInstancesSelect(VqbMainWindow *parent = 0);
    virtual ~VqbInstancesSelect();

signals:
    void errorMessage(QString);

private slots:
    void on_listBoxConditions_removed(QString text);
    void on_listBoxConditions_added(QString text);
    void on_listVars_doubleClicked(QModelIndex index);
    void on_buttonDown_clicked();
    void on_buttonUp_clicked();
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
