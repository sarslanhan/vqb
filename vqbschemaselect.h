#ifndef VQBSCHEMASELECT_H
#define VQBSCHEMASELECT_H


#include "vqbform.h"

class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class QMainWindow;

class VqbMainWindow;

namespace Ui
{
class VqbSchemaSelectClass;
}

class VqbSchemaSelect : public VqbForm
{
    Q_OBJECT
public:
    VqbSchemaSelect(VqbMainWindow *parent = 0);
    ~VqbSchemaSelect();

public slots:
    void queryTreeChanged(int index, QString constraint);
    void queryTreeDeleted(int treeNumber);
    void addQueryTree();
    void addVarToOutput(QString var);
    void removeVarFromOutput(QString var);

private:
    void init();
    void emitQueryChanged();

    class Private;
    Private * const d;

};

#endif // VQBSCHEMAFORM_H
