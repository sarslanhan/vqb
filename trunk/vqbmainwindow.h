#ifndef VQBMAINWINDOW_H
#define VQBMAINWINDOW_H

#include <QtGui/QMainWindow>

#include <kio/global.h>
#include <kio/job.h>
#include <kjob.h>

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
    void postErrorMessage(QString message);//writes an error message to the status bar

private:
    void initMainForm();

    VqbForm *m_mainForm;
    Ui::VqbMainWindow *m_ui;
    QString m_queryPart;
    bool m_quitAfterInit;//if cancel is pressed at startup

    const QByteArray m_pastebinApiKey;
    QByteArray m_pastebinData;;

private slots:
    void on_dockWidget_topLevelChanged(bool topLevel);
    void on_action_Return_to_Startup_triggered();
    void on_actionSave_triggered();
    void on_actionExit_triggered();
    void on_actionPostToPastebin_triggered();
    void tabChanged(int index);

    void initStartupMenu();
    void showStartupMenu(bool exitOnCancel);

    //void refreshQuery();

    void finished(KJob* job);
    void readKIOData(KIO::Job *job, const QByteArray &data);

};

#endif // VQBMAINWINDOW_H
