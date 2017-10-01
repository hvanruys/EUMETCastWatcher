#include "dialog.h"
#include "ui_dialog.h"

#include <QDebug>
#include <QFileDialog>
#include <QDir>

extern Options opts;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    showSegmentDirectoryList();
    showFileTemplateList();

}

void Dialog::showSegmentDirectoryList(void)
{

    ui->lwSegmentDirectories->clear();

    QStringList::Iterator itd = opts.segmentdirectorylist.begin();
    while( itd != opts.segmentdirectorylist.end() )
    {
        ui->lwSegmentDirectories->addItem(*itd);
        ++itd;
    }

}

void Dialog::showFileTemplateList(void)
{

    ui->lwFileTemplate->clear();


    QStringList::Iterator itfile = opts.filetemplatelist.begin();
    QStringList::Iterator itdate = opts.startdatetemplatelist.begin();

    while( itfile != opts.filetemplatelist.end() )
    {
        ui->lwFileTemplate->addItem(*itfile + "     Start date (YYYYMMDD) = " + *itdate);
        ++itfile;
        ++itdate;
    }

}

void Dialog::addSegmentDirectory()
{

    QString directory = QFileDialog::getExistingDirectory(this,
                               tr("Select directory"), QDir::currentPath());

    if (!directory.isEmpty())
    {
        qDebug() << QString("directory = %1").arg(directory);
        opts.segmentdirectorylist.append(directory);
        showSegmentDirectoryList();
    }
}

void Dialog::addFileTemplates()
{
    DialogFileTemplate prefdialog;
    if(prefdialog.exec() == QDialog::Accepted)
    {
        QString tmpl(prefdialog.getTemplate());
        qDebug() << "tmpl = " << tmpl;
        opts.filetemplatelist.append(tmpl);
        opts.startdatetemplatelist.append(QString::number(prefdialog.getStartDateTemplate()));

        showFileTemplateList();
    }

}

void Dialog::deleteSegmentDirectory()
{
    QString sel;

    QListWidgetItem *itm = ui->lwSegmentDirectories->currentItem();
    opts.deleteSegmentDirectory( itm->text());
    showSegmentDirectoryList();

}

void Dialog::deleteFileTemplate()
{
    QString sel;

    if (ui->lwFileTemplate->currentRow()>0)
    {
        QListWidgetItem *itm = ui->lwFileTemplate->currentItem();
        opts.deleteFileTemplate( itm->text());
        showFileTemplateList();
    }

}

Dialog::~Dialog()
{
    delete ui;
    opts.Save();
}


void Dialog::on_btnAddDirectory_clicked()
{
    addSegmentDirectory();
}

void Dialog::on_btnDeleteDirectory_clicked()
{
    deleteSegmentDirectory();
}

void Dialog::on_btnExit_clicked()
{
    this->close();
}

void Dialog::on_pushButton_3_clicked()
{
    addFileTemplates();
}

void Dialog::on_pushButton_4_clicked()
{
    deleteFileTemplate();
}
