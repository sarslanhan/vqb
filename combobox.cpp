#include "combobox.h"

#include <kdebug.h>

ComboBox::ComboBox( QWidget* parent )
        : QComboBox( parent )
{
}

void ComboBox::addToOutput()
{
    emit addVarToOutput( varName );
}

void ComboBox::attach()
{
    //FIXME: use or remove index
    emit attachConstraint( 0, varName, varClass );
}


