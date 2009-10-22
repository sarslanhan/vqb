#include "completerlineedit.h"

#include <QCompleter>
#include <QComboBox>
#include <QKeyEvent>
#include <QStringListModel>

#include <KDebug>

CompleterLineEdit::CompleterLineEdit(QWidget *parent)
        : QLineEdit(parent), m_completer(0)
{
    setCompleter(new QCompleter(this));
    completer()->setWidget(this);
    completer()->popup()->setAlternatingRowColors( true );
    QObject::connect(completer(), SIGNAL(activated(const QString&)),
                  this, SLOT(setText(const QString&)));
}


void CompleterLineEdit::setCompleter(QCompleter *completer)
{
    kDebug() << "Setting completer to " << completer;
    m_completer = completer;
}


QCompleter *CompleterLineEdit::completer() const
{
    return m_completer;
}


void CompleterLineEdit::keyPressEvent(QKeyEvent *e)
{
   if( e->key() == Qt::Key_Backspace ) {
       backspace();
       e->accept();
   }
   else if (e->key() == Qt::Key_Delete) {
       del();
       e->accept();
   }
   else if ( e->key() == Qt::Key_Down ) {
       QLineEdit::keyPressEvent( e );
       e->accept();
   }
   else if( e->key() & 0x01000000 ) { //all other non-text keys
       QLineEdit::keyPressEvent( e );
       return; // let the completer do default behavior
   }
   else {
       QLineEdit::keyPressEvent( e );
       e->accept();
   }  

    QStringList completionList;
    foreach( QString item, m_items ) {
        if(item.contains(text(), Qt::CaseInsensitive)) {
            //FIXME: highlight autocompletion expression
            completionList << item;
        }
    }
    kDebug() << (int)completer() << " " << (int) m_completer;
    completer()->setModel(new QStringListModel(completionList));

    //create completer with empty prefix, and show popup
    completer()->setCompletionPrefix( "" );
    completer()->popup()->setCurrentIndex(completer()->completionModel()->index(0, 0));
    completer()->complete();
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

