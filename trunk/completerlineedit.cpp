#include "completerlineedit.h"

#include <QCompleter>
#include <QComboBox>
#include <QKeyEvent>
#include <QStringListModel>

#include <KDebug>
#include <KCompletion>
#include <KCompletionBox>

CompleterLineEdit::CompleterLineEdit(QWidget *parent)
        : KLineEdit(parent)
{
    completionBox()->setActivateOnSelect(false);
}

void CompleterLineEdit::makeCompletion(const QString& text)
{
    setCompletedItems( completionObject()->substringCompletion( text ) );
    m_lastText = text;
}

void CompleterLineEdit::addItem(QString newItem)
{
    //FIXME: some problems when pressing arrow-down for the first item
    completionObject()->addItem(newItem);
    if(hasFocus()) { //completionBox()->isVisible() || completionObject()->items().size() == 1) {//if it is visible, or if it had no objects before
        //kDebug() << "Completing with: " << m_lastText;
        setText(m_lastText);
        QListWidgetItem *item = completionBox()->currentItem();
        doCompletion(text());
        completionBox()->setVisible(true);
        completionBox()->setCurrentItem(item);
    }
}

#include "completerlineedit.moc"

