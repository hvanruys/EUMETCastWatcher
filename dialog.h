#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "options.h"
#include "dialogfiletemplate.h"


namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void showSegmentDirectoryList(void);
    void showFileTemplateList(void);

    void addFileTemplates();

private:
    Ui::Dialog *ui;


private slots:
    void addSegmentDirectory();
    void deleteSegmentDirectory();
    void deleteFileTemplate();

    void on_btnAddDirectory_clicked();
    void on_btnDeleteDirectory_clicked();
    void on_btnExit_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
};

#endif // DIALOG_H
