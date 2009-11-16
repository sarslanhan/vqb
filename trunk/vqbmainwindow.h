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
    void addVarToOutput(QString var);

private:
    void init();

    Ui::VqbMainWindow *m_ui;
    VqbForm *m_mainForm;
    QString m_queryPart;

    const QByteArray m_pastebinApiKey;
    QByteArray m_pastebinData;;

private slots:
    void on_actionSave_triggered();
    void on_actionExit_triggered();
    void on_actionPostToPastebin_triggered();
    void tabChanged(int index);

    void moveOutputUp();
    void moveOutputDown();
    void removeOutput();

    void refreshQuery();
    
    void finished(KJob* job);
    void readKIOData(KIO::Job *job, const QByteArray &data);

};

#endif // VQBMAINWINDOW_H
