#ifndef COMPLETERLINEEDIT_H
#define COMPLETERLINEEDIT_H

#include </home/kde-devel/kde/include/klineedit.h>

class CompleterLineEdit : public KLineEdit
{
    Q_OBJECT

public:
    CompleterLineEdit(QWidget *parent = 0);

public slots:
    void addItem(QString item);

protected slots:
    void makeCompletion (const QString &);

private:
    QString m_lastText;
};

#endif // COMPLETERLINEEDIT_H
