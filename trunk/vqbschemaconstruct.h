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
    void populateOutputLists();
    void updateTriple();

private:
    void init();
    void emitQueryChanged();

    class Private;
    Private * const d;

};

#endif // VQBSCHEMACONSTRUCT_H
