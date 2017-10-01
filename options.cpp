#include "options.h"
#include <QSettings>
#include <QDebug>

Options::Options()
{

}

void Options::Initialize()
{
    //QSettings settings(QSettings::IniFormat, QSettings::UserScope, "EumetcastTools", "EumetcastWatcher");
    QSettings settings( "EumetcastWatcher.ini", QSettings::IniFormat);
    segmentdirectorylist = settings.value( "/segments/segmentdirectories" ).value<QStringList>();
    filetemplatelist = settings.value( "/segments/filetemplatelist" ).value<QStringList>();
    startdatetemplatelist = settings.value( "/segments/startdatetemplatelist" ).value<QStringList>();

}


void Options::Save()
{
    qDebug() << "writting ini file ";
    //QSettings settings(QSettings::IniFormat, QSettings::UserScope, "EumetcastTools", "EumetcastWatcher");
    QSettings settings( "EumetcastWatcher.ini", QSettings::IniFormat);
    settings.setValue("/segments/segmentdirectories", segmentdirectorylist );
    settings.setValue("/segments/filetemplatelist", filetemplatelist );
    settings.setValue("/segments/startdatetemplatelist", startdatetemplatelist );
}


void Options::deleteSegmentDirectory( QString sel )
{
    QStringList::Iterator its = segmentdirectorylist.begin();
    QStringList strlistout;

    while( its != segmentdirectorylist.end() )
    {
        if (*its != sel)
            strlistout << *its;
        ++its;
    }
    segmentdirectorylist = strlistout;

}

void Options::deleteFileTemplate( QString sel )
{
    QStringList::Iterator itfile = filetemplatelist.begin();
    QStringList::Iterator itdate = startdatetemplatelist.begin();
    QStringList filelistout;
    QStringList datelistout;

    QString select;
    int j = sel.indexOf(" ", 0);

    select = sel.mid(0, j);

    qDebug() << "select = " << select;

    while( itfile != filetemplatelist.end() )
    {
        if (*itfile != select)
        {
            filelistout << *itfile;
            datelistout << *itdate;
        }
        ++itfile;
        ++itdate;

    }
    filetemplatelist = filelistout;
    startdatetemplatelist = datelistout;

}


