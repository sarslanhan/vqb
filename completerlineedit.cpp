#include "completerlineedit.h"

#include <QCompleter>
#include <QComboBox>
#include <QKeyEvent>
#include <QStringListModel>

#include <KDebug>
#include <KCompletion>
#include <KCompletionBox>

CompleterLineEdit::CompleterLineEdit(QWidget *parent)
        : KLineEdit(parent), m_items(new QSet<QString>)
{
    completionBox()->setActivateOnSelect(false);
}

void CompleterLineEdit::makeCompletion(const QString& text)
{
    setCompletedItems( completionObject()->substringCompletion( text ) );
    m_lastText = text;
}

void CompleterLineEdit::addItems(QStringList newItems)
{
    completionObject()->insertItems(newItems);
}

void CompleterLineEdit::addItem(QString newItem)
{
    //FIXME: some problems when pressing arrow-down for the first item
    kDebug() << "New item: " << newItem;
    completionObject()->addItem(newItem);
    if(completionBox()->isVisible()) {
        setText(m_lastText);
        QListWidgetItem *item = completionBox()->currentItem();
        doCompletion(text());
        completionBox()->setCurrentItem(item);
    }
}

void CompleterLineEdit::clearItems()
{
    m_items->clear();
}

#include "completerlineedit.moc"

