#include "vqbform.h"
#include "vqbmainwindow.h"

VqbForm::VqbForm(VqbMainWindow *parent)
        : QWidget(parent)
{
    connect(this, SIGNAL(addVarToOutput(QString)), parent, SLOT(addVarToOutput(QString)));
    connect(this, SIGNAL(queryChanged(QString)), parent, SLOT(queryChanged(QString)));
}



#include "vqbform.moc"
