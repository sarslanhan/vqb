#include "combobox.h"

#include <krandom.h>
#include <kdebug.h>

ComboBox::ComboBox(QWidget* parent)
        : QComboBox(parent)
{
    this->varName = getRandomVarName();
}

void ComboBox::addToOutput()
{
    emit addVarToOutput(varName);
}

void ComboBox::attach()
{
    //FIXME: use or remove index
    emit attachConstraint(0, varName, varClass);
}


QString ComboBox::getRandomVarName()
{
    return QString("?v" +  QString::number(KRandom::random() % 80 + 20)) ;
}
