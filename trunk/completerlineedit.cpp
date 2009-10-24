#include "completerlineedit.h"

#include <QCompleter>
#include <QComboBox>
#include <QKeyEvent>
#include <QStringListModel>

#include <KDebug>

CompleterLineEdit::CompleterLineEdit(QWidget *parent)
        : KLineEdit(parent), m_completer(0)
{
    setCompleter(new QCompleter(this));
}


void CompleterLineEdit::setCompleter(QCompleter *completer)
{
    if(completer == m_completer) {
        return;
    }

    if(m_completer) {
        disconnect(m_completer, 0, this, 0);
        if(m_completer->parent() == this) {
            delete m_completer;
        }
    }
    m_completer = completer;

    if(!m_completer) {
        return;
    }

    m_completer->setWidget(this);
    m_completer->popup()->setAlternatingRowColors( true );
    QObject::connect(m_completer, SIGNAL(activated(QString)),
                  this, SLOT(setText(QString)));
}


QCompleter *CompleterLineEdit::completer() const
{
    return m_completer;
}


void CompleterLineEdit::keyPressEvent(QKeyEvent *e)
{
    KLineEdit::keyPressEvent(e);
    return;

    if(e->key() == Qt::Key_Backspace) {
       backspace();
       e->accept();
    }
    else if (e->key() == Qt::Key_Delete) {
       del();
       e->accept();
    }
    else if (e->key() == Qt::Key_Down) {
       KLineEdit::keyPressEvent(e);
       e->accept();
    }
    else if(e->key() & 0x01000000) { //all other non-text keys
       KLineEdit::keyPressEvent(e);
       return; // let the completer do default behavior
    }
    else {
       KLineEdit::keyPressEvent(e);
       e->accept();
    }

    QStringList completionList;
    foreach(QString item, m_items) {
        if(item.contains(text(), Qt::CaseInsensitive)) {
            //FIXME: highlight autocompletion expression
            completionList << item;
        }
    }
    //m_completer->setModel(new QStringListModel(completionList));

    QStringListModel *model = new QStringListModel();
    model->setStringList(completionList);

    m_completer->setModel(model);
    //CompleterLineEdit::setCompleter(new QCompleter(model, this));

    //create completer with empty prefix, and show popup
    m_completer->setCompletionPrefix( "" );
    m_completer->popup()->setCurrentIndex(m_completer->completionModel()->index(0, 0));
    m_completer->complete();


}


void CompleterLineEdit::addItems(QStringList newItems)
{
    foreach(QString item, newItems) {
        m_items.insert(item);
    }
}


void CompleterLineEdit::clearItems()
{
    m_items.clear();
}

#include "completerlineedit.moc"

