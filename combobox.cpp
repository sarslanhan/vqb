#include "combobox.h"

ComboBox::ComboBox( QWidget* parent )
        : QComboBox( parent )
{
}

void ComboBox::addToOutput()
{
    emit addVarToOutput( varName );
}


