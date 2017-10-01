#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemWatcher>
#include <QStringList>
#include <QUdpSocket>
#include <QFutureWatcher>

#include "options.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void inspectDirectories(bool sendUDP);
    bool inspectDirectory( const QString &thedir, bool sendUDP);
    void setupNotifier();
    bool removeDir(const QString & dirName);
    void writeTolistwidget(QString txt);

    ~MainWindow();

private slots:
    void on_actionSetup_watcher_triggered();
    void on_btnExit_clicked();
    void inspectfinished();

public slots:
  void directoryChanged(const QString& path);


private:
    Ui::MainWindow *ui;
    QFileSystemWatcher m_fs;
    void showDialog();
    void SetNotifier(const QString& path);
    QStringList filetemplates;
    QUdpSocket *udpSocket;
    QFutureWatcher<void> *watcher;

};

#endif // MAINWINDOW_H
