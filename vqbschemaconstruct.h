#ifndef VQBSCHEMACONSTRUCT_H
#define VQBSCHEMACONSTRUCT_H


#include "vqbform.h"

class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class QMainWindow;

class VqbMainWindow;

namespace Ui
{
    class VqbSchemaConstruct;
}

class VqbSchemaConstruct : public VqbForm
{
    Q_OBJECT
public:
    VqbSchemaConstruct(VqbMainWindow *parent = 0);
    ~VqbSchemaConstruct();

public slots:
    void queryTreeChanged(int index, QString constraint);
    void queryTreeDeleted(int treeNumber);
    void addQueryTree();
    void populateOutputLists();//populates the lists for constructing the output
    void updateTriple();
    void addVarToOutput(QString var);//not implemented - for compatibility only
    void removeVarFromOutput(QString var);//not implemented - for compatibility only

private slots:
    void emitQueryChanged();

private:
    void init();


    class Private;
    Private * const d;

};

#endif // VQBSCHEMACONSTRUCT_H
