#ifndef VQBSCHEMAFORM_H
#define VQBSCHEMAFORM_H


#include "vqbform.h"

class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class QMainWindow;

class VqbMainWindow;

namespace Ui
{
class VqbSchemaFormClass;
}

class VqbSchemaForm : public VqbForm
{
    Q_OBJECT
public:
    VqbSchemaForm(VqbMainWindow *parent = 0);
    ~VqbSchemaForm();

public slots:
    void queryTreeChanged(int index, QString constraint);
    void queryTreeDeleted(int treeNumber);
    void addQueryTree();

private:
    void init();
    void emitQueryChanged();

    class Private;
    Private * const d;

};

#endif // VQBSCHEMAFORM_H
