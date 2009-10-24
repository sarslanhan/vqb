#include "completerlineedit.h"

#include <QCompleter>
#include <QComboBox>
#include <QKeyEvent>
#include <QStringListModel>

#include <KDebug>

CompleterLineEdit::CompleterLineEdit(QWidget *parent)
        : KLineEdit(parent), m_items(new QSet<QString>)
{
}

void CompleterLineEdit::makeCompletion(const QString& text)
{
    setCompletedItems( completionObject()->substringCompletion( text ) );
}


void CompleterLineEdit::addItems(QStringList newItems)
{
    foreach(QString item, newItems) {
        m_items->insert(item);
    }
}


void CompleterLineEdit::clearItems()
{
    m_items->clear();
}

#include "completerlineedit.moc"

