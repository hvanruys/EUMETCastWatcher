#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QMessageBox>

#include "dialogfiletemplate.h"
#include "ui_dialogfiletemplate.h"

extern Options opts;

DialogFileTemplate::DialogFileTemplate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFileTemplate)
{
    ui->setupUi(this);
}

DialogFileTemplate::~DialogFileTemplate()
{
    delete ui;
}

/*
void DialogFileTemplate::on_btnOK_clicked()
{
    if (ui->leFileTemplate->text().length() > 0)
    {
       opts.filetemplatelist.append(ui->leFileTemplate->text());
       opts.startdatetemplatelist.append(QString::number(ui->sbStartDate->value()));
    }

    this->close();
}
*/

QString DialogFileTemplate::getTemplate()
{
    return (ui->leFileTemplate->text());
}

quint16 DialogFileTemplate::getStartDateTemplate()
{
    return (ui->sbStartDate->value());
}


void DialogFileTemplate::on_btnTemp_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                 tr("Open File"), "/home", tr("Files (*)"));
    if (!fileName.isEmpty())
    {
        qDebug() << fileName;
        ui->leFileTemplate->setText( fileName );
    }

}

void DialogFileTemplate::accept()
{
    int j = ui->leFileTemplate->text().indexOf("*");
    if (j>0)
        QDialog::accept();
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::information(this, tr("QMessageBox::information()"), "There must be a '*' in the template");
     }

}
