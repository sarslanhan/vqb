#include "combobox.h"
#include "vqbglobal.h"

#include <QAction>
#include <QPaintEvent>
#include <QPaintEngine>
#include <QPainter>

#include <krandom.h>
#include <kdebug.h>
#include <KAction>
#include <KStandardAction>

ComboBox::ComboBox(QWidget* parent)
        : QComboBox(parent), m_isOutput(false)
{
    m_varName = VqbGlobal::randomVarName();//QString("?v" +  QString::number(KRandom::random() % 80 + 20)) ;

    setToolTip(m_varName);

    QAction *add = KStandardAction::findNext(this, SLOT(addToOutput()), this);
    add->setText(QString("Add " + m_varName + " to output"));
    add->setStatusTip(tr("Adds the variable to the output list"));
    add->setShortcut(0);
    addAction(add);
    setContextMenuPolicy(Qt::ActionsContextMenu);

    //initialize reove action (but don't add it to the list)
    m_removeAction = KStandardAction::findNext(this, SLOT(removeFromOutput()), this);
    m_removeAction->setText(QString("Remove" + m_varName + " from to output"));
    m_removeAction->setStatusTip(tr("Removes the variable from the output list"));
    m_removeAction->setShortcut(0);
}

void ComboBox::addToOutput()
{
    if(m_isOutput) {
        return;//already an output
    }

    if(!actions().contains(m_removeAction)) {
        addAction(m_removeAction);
    }

    emit addVarToOutput(m_varName);
    m_isOutput = true;
    repaint();
}

QString ComboBox::varName()
{
    return m_varName;
}

void ComboBox::paintEvent ( QPaintEvent * event )
{
    QComboBox::paintEvent(event);
    if(m_isOutput) {
        QPainter p(this);
        p.fillRect(0, 0, width(), height(), QColor(255, 0, 0, 127));
        //p.drawText(this
        //FIXME: write variable name on it?
    }
}

void ComboBox::removeFromOutput()
{
    m_isOutput = false;
    repaint();
    emit(removeVarFromOutput(m_varName));
    kDebug() << "Emitting removeVarFromOutput";
    removeAction(m_removeAction);
}
