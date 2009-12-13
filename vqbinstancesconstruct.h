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

signals:
    void errorMessage(QString);

private slots:
    void on_listBoxOutputs_changed();
    void on_listBoxConditions_changed();
    void init();

    void updateCurrentTripleC();
    void updateCurrentTripleO();
    void updateTypesC();
    void updateTypesO();
    void updateVars();
    void emitQueryChanged();

    void updateCompletersSubjectC(QString text);
    void updateCompletersPredicateC(QString text);
    void updateCompletersObjectC(QString text);
    void updateCompletersSubjectO(QString text);
    void updateCompletersPredicateO(QString text);
    void updateCompletersObjectO(QString text);

    void colorLineEditsC();

private:  
    Ui::VqbInstancesConstruct *m_ui;
    QString m_currentTriple;
    QStringList m_varList;
    QList<QueryThread*> m_queryThreads;
    QueryThread *m_queryThread;
    QString m_queryPart;

    QString constructCompletionQuery(QString text, int slotNumber, bool isWherePart);//creates completion query based on slot number and query part
};

#endif // VQBINSTANCESCONSTRUCT_H
