#ifndef COMPLETERLINEEDIT_H
#define COMPLETERLINEEDIT_H

#include <KLineEdit>
#include <QSet>

class CompleterLineEdit : public KLineEdit
{
    Q_OBJECT

public:
    CompleterLineEdit(QWidget *parent = 0);

public slots:
    void addItems(QStringList items);
    void clearItems();

protected slots:
    void makeCompletion (const QString &);

private:
    QSet<QString> *m_items;
};

#endif // COMPLETERLINEEDIT_H
