#ifndef COMPLETERLINEEDIT_H
#define COMPLETERLINEEDIT_H

#include <QLineEdit>
#include <QSet>

class QCompleter;

class CompleterLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    CompleterLineEdit(QWidget *parent = 0);
    void setCompleter(QCompleter *c);
    QCompleter *completer() const;

public slots:
    void addItems(QStringList items);
    void clearItems();

protected:
    void keyPressEvent(QKeyEvent *e);

private:
    QCompleter *m_completer;
    QSet<QString> m_items;
};

#endif // COMPLETERLINEEDIT_H
