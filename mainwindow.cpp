#include <QtCore>
#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialog.h"

#include <QDir>
#include <QtConcurrent>

extern Options opts;

void doInspectDirectories(MainWindow *t, bool sendUDP)
{
    t->inspectDirectories(sendUDP);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QStorageInfo storage = QStorageInfo::root();

//    qDebug() << storage.rootPath();
//    if (storage.isReadOnly())
//        qDebug() << "isReadOnly:" << storage.isReadOnly();

//    qDebug() << "name:" << storage.name();
//    qDebug() << "fileSystemType:" << storage.fileSystemType();
//    qDebug() << "size:" << storage.bytesTotal()/1000/1000 << "MB";
//    qDebug() << "availableSize:" << storage.bytesAvailable()/1000/1000 << "MB";


    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
        if (storage.isValid() && storage.isReady()) {
            if (!(storage.isReadOnly() || storage.fileSystemType() == "tmpfs") ) {
                qDebug() << "name:" << storage.name();
                qDebug() << "fileSystemType:" << storage.fileSystemType();
                qDebug() << "size:" << storage.bytesTotal()/1000/1000 << "MB";
                qDebug() << "availableSize:" << storage.bytesAvailable()/1000/1000 << "MB";
            }
        }
    }



    udpSocket = new QUdpSocket(this);

    qApp->processEvents();

    writeTolistwidget(" Checking directories.......");
//    inspectDirectories();
    watcher = new QFutureWatcher<void>(this);
    connect(this->watcher, SIGNAL(finished()), this, SLOT(inspectfinished()));

    QFuture<void> future;
    future = QtConcurrent::run(doInspectDirectories, this, false);
    watcher->setFuture(future);

}

void MainWindow::inspectfinished()
{
    delete watcher;
    writeTolistwidget("Setting up notifier .....");
    setupNotifier();
}

void MainWindow::writeTolistwidget(QString txt)
{
    QDateTime now = QDateTime::currentDateTimeUtc();
    ui->lwTraffic->insertItem(0, QString(now.toString(Qt::ISODate)) + " " + txt);
    if (ui->lwTraffic->count() > 100)
    {
        delete ui->lwTraffic->item(ui->lwTraffic->count() - 1);
    }
}

void MainWindow::inspectDirectories(bool sendUDP)
{
    qDebug() << "start inspectDirectories()";

    if (opts.segmentdirectorylist.count() > 0)
    {
        QStringList::Iterator its = opts.segmentdirectorylist.begin();

        while( its != opts.segmentdirectorylist.end() )
        {
            QString stits = *its;

            qApp->processEvents();
            inspectDirectory(stits, sendUDP);
            if(!sendUDP) // when in thread ...
                writeTolistwidget(" Inspecting " + stits);
            ++its;
        }
    }

}


bool MainWindow::inspectDirectory(const QString &thedir, bool sendUDP)
{
    QString filename;
    QString restfile;
    long count = 0;
    bool gelukt = false;
    bool movefile = false;
    QDir dir( thedir );
    QString fileyear;
    QString filemonth;
    QString fileday;

    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setSorting(QDir::Name); //::Time);
    QStringList list = dir.entryList();

    for (int i = 0; i < list.size(); ++i)
    {
        filename = list.at(i);
        if (opts.filetemplatelist.count() > 0)
        {

            QStringList::Iterator itfile = opts.filetemplatelist.begin();
            QStringList::Iterator itdate = opts.startdatetemplatelist.begin();

            while( itfile != opts.filetemplatelist.end() )
            {
                //qDebug() << *itfile << "  " << *itdate;
                QDir filterdir(thedir);

                QString stritdate = *itdate;
                QStringList filters;
                filters << *itfile;

                filterdir.setFilter(QDir::Files | QDir::NoSymLinks);
                filterdir.setSorting(QDir::Name); //::Time);
                filterdir.setNameFilters(filters);
                QStringList filterlist = filterdir.entryList();

                movefile = false;
                for (int j = 0; j < filterlist.size(); ++j)
                {
                    if(list.at(i) == filterlist.at(j))
                    {
                        movefile = true;
                        break;
                    }
                }
                if (movefile)
                {
                    if(filename.mid(0, 6) == "OR_ABI")
                    {
                        int nyear = filename.mid( (*itdate).toInt(), 4).toInt();
                        int ndayofyear = filename.mid( (*itdate).toInt() + 4, 3).toInt();
                        QDate filedate = QDate(nyear, 1, 1).addDays(ndayofyear - 1);
                        fileyear = filedate.toString("yyyy");
                        filemonth = filedate.toString("MM");
                        fileday = filedate.toString("dd");
                    }
                    else
                    {
                        fileyear = filename.mid( (*itdate).toInt(), 4);
                        filemonth = filename.mid( (*itdate).toInt() + 4, 2);
                        fileday = filename.mid( (*itdate).toInt() + 6, 2);
                    }

                    QDir dirdateyear(dir.absolutePath() + "/" + fileyear);
                    QDir dirdatemonth(dir.absolutePath() + "/" + fileyear + "/" + filemonth);
                    QDir dirdateday(dir.absolutePath() + "/" + fileyear + "/" + filemonth + "/" + fileday);

                    gelukt = false;
                    if (!dirdateyear.exists())
                    {
                        gelukt = dir.mkdir(dir.absolutePath() + "/" + fileyear);
                        qDebug() << dir.absolutePath() + "/" + fileyear;
                    }
                    if (!dirdatemonth.exists())
                    {
                        gelukt = dir.mkdir(dir.absolutePath() + "/" + fileyear + "/" + filemonth);
                        qDebug() << dir.absolutePath() + "/" + fileyear + "/" + filemonth;
                    }
                    if (!dirdateday.exists())
                    {
                        gelukt = dir.mkdir(dir.absolutePath() + "/" + fileyear + "/" + filemonth + "/" + fileday);
                        qDebug() << dir.absolutePath() + "/" + fileyear + "/" + filemonth + "/" + fileday;
                    }
                    if (!dirdateday.exists(filename))
                    {
                        gelukt = dir.rename(dir.absolutePath() + "/" + filename,
                                            dir.absolutePath() + "/" +  fileyear + "/" + filemonth + "/" + fileday + "/" + filename);
                        QByteArray datagram = (dir.absolutePath() + "/" +  fileyear + "/" + filemonth + "/" + fileday + "/" + filename).toLatin1();
                        if(sendUDP)
                            udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 45454);

                    }


                    break;
                }
                else
                {
                }

                ++itfile;
                ++itdate;
            }

            gelukt = dir.remove(filename);

        }

        if(sendUDP) // not in thread ....
        {
            if(movefile)
                writeTolistwidget(" File " + dir.absolutePath() + "/" + filename + " moved");
            else
                writeTolistwidget(" File " + dir.absolutePath() + "/" + filename + " is deleted");
        }

        qApp->processEvents();

        ////////////////////////////////////////////////////////////////////////////////////////
        //    this->thread()->msleep(500);
        //////////////////////////////////////////////////////////////////////////////////////////


    }

    return movefile;
}

void MainWindow::setupNotifier()
{
    if (opts.segmentdirectorylist.count() > 0)
    {
        QStringList::Iterator its = opts.segmentdirectorylist.begin();

        while( its != opts.segmentdirectorylist.end() )
        {
            SetNotifier(*its);
            ++its;
        }
    }

}

void MainWindow::showDialog()
{

    Dialog *pref=new Dialog(this);
    pref->setAttribute(Qt::WA_DeleteOnClose);
    pref->show();

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSetup_watcher_triggered()
{
    showDialog();

}

void MainWindow::on_btnExit_clicked()
{
    qApp->quit();
}

void MainWindow::SetNotifier(const QString& path)
{
    connect(&m_fs, SIGNAL(directoryChanged(const QString&)), this, SLOT(directoryChanged(const QString&)));
    QDir dir(path);

    m_fs.addPath(path);
}

//void MainWindow::directoryChanged(const QString& path)
//{
//    QDir dir(path);
//    QStringList entries = dir.entryList(QDir::Files, QDir::Name);
//    if (entries.size() > 0)
//    {
//        QDateTime now = QDateTime::currentDateTimeUtc();

//        if (inspectDirectory(path))
//        {
//            ui->lwTraffic->insertItem(0, QString(now.toString(Qt::ISODate)) + " File " + dir.absolutePath() + "/" + entries.at(0) + " moved");

//        }
//        else
//            ui->lwTraffic->insertItem(0, QString(now.toString(Qt::ISODate)) + " File " + dir.absolutePath() + "/" + entries.at(0) + " is deleted");


//        if (ui->lwTraffic->count() > 100)
//        {
//            delete ui->lwTraffic->item(ui->lwTraffic->count() - 1);
//        }

//    }

//}

void MainWindow::directoryChanged(const QString& path)
{
    QDir dir(path);
    QStringList entries = dir.entryList(QDir::Files, QDir::Name);
    if (entries.size() > 0)
    {
        QDateTime now = QDateTime::currentDateTimeUtc();

        inspectDirectory(path, true);

    }

}
bool MainWindow::removeDir(const QString & dirName)
{
    bool result;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
        {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }
    return result;
}
