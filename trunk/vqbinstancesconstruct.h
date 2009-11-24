#ifndef VQBINSTANCESCONSTRUCT_H
#define VQBINSTANCESCONSTRUCT_H

#include "vqbform.h"

class VqbMainWindow;
class QueryThread;

namespace Ui {
    class VqbInstancesConstruct;
}

class VqbInstancesConstruct : public VqbForm {
    Q_OBJECT
    Q_DISABLE_COPY(VqbInstancesConstruct)
public:
    explicit VqbInstancesConstruct(VqbMainWindow *parent = 0);
    virtual ~VqbInstancesConstruct();

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

    void colorLineEdits();

private:  
    Ui::VqbInstancesConstruct *m_ui;
    QString m_currentTriple;
    QStringList m_varList;
    QList<QueryThread*> m_queryThreads;
    QueryThread *m_queryThread;
    QString m_queryPart;

    QString constructCompletionQuery(QString text, int slotNumber);
};

#endif // VQBINSTANCESCONSTRUCT_H
