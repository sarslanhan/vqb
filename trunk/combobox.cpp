#include "combobox.h"
#include "vqbglobal.h"

#include <QAction>

#include <krandom.h>
#include <kdebug.h>
#include <KAction>
#include <KStandardAction>

ComboBox::ComboBox(QWidget* parent)
        : QComboBox(parent)
{
    m_varName = VqbGlobal::randomVarName();//QString("?v" +  QString::number(KRandom::random() % 80 + 20)) ;

    setToolTip(m_varName);

    QAction *add = KStandardAction::findNext(this, SLOT(addToOutput()), this);
    add->setText(QString("Add " + m_varName + " to output"));
    add->setStatusTip(tr("Adds the variable to the output list"));
    add->setShortcut(0);
    addAction(add);
    setContextMenuPolicy(Qt::ActionsContextMenu);
}

void ComboBox::addToOutput()
{
    //SUGGESTION: color combobox in red, or something
    emit addVarToOutput(m_varName);
}

/*
void ComboBox::attach()
{
    //FIXME: use or remove index
    emit attachConstraint(0, varName, varClass);
}
*/

QString ComboBox::varName()
{
    return m_varName;
}
