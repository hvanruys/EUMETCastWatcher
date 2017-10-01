#ifndef DIALOGFILETEMPLATE_H
#define DIALOGFILETEMPLATE_H

#include <QDialog>
#include "options.h"

namespace Ui {
class DialogFileTemplate;
}

class DialogFileTemplate : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogFileTemplate(QWidget *parent = 0);
    QString getTemplate();
    quint16 getStartDateTemplate();
    ~DialogFileTemplate();
    
private slots:

    void on_btnTemp_clicked();


private:
    Ui::DialogFileTemplate *ui;
    void accept();

};

#endif // DIALOGFILETEMPLATE_H
