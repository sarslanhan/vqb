#ifndef VQBMAINWINDOW_H
#define VQBMAINWINDOW_H

#include <QtGui/QMainWindow>

class VqbForm;

namespace Ui {
    class VqbMainWindow;
}

class VqbMainWindow : public QMainWindow {
    Q_OBJECT
    Q_DISABLE_COPY(VqbMainWindow)
public:
    explicit VqbMainWindow(QWidget *parent = 0);
    virtual ~VqbMainWindow();

public slots:
    void queryChanged(QString queryPart);
    void addVarToOutput(QString var);

private:
    void init();

    Ui::VqbMainWindow *m_ui;
    VqbForm *m_mainForm;
    QString m_queryPart;

private slots:
    void tabChanged(int index);

    void moveOutputUp();
    void moveOutputDown();
    void removeOutput();

    void refreshQuery();
};

#endif // VQBMAINWINDOW_H
