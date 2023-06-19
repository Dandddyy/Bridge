#include "optionwindow.h"
#include "ui_optionwindow.h"

optionwindow::optionwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::optionwindow)
{
    ui->setupUi(this);

    ui->textEdit->setReadOnly(true);

}

optionwindow::~optionwindow()
{
    delete ui;
}

void optionwindow::on_comboBox_currentIndexChanged(int index)
{
    QString QSM = ui->comboBox->currentText();
    emit QSMsignal(QSM);
}

void optionwindow::QSMslot2(bool b, bool a)
{
    if(b)
        ui->comboBox->setCurrentIndex(1);
    else
        ui->comboBox->setCurrentIndex(0);
    if(a){
        ui->comboBox_2->setCurrentIndex(1);
        ui->label_4->setText("Game over if someone has more than 225 points. 125 and 225 nullified.");
    }
    else{
        ui->comboBox_2->setCurrentIndex(0);
        ui->label_4->setText("Game over if someone has more than 125 points. 125 nullified.");
    }
}


void optionwindow::on_comboBox_2_currentIndexChanged(int index)
{
    QString PointsMode = ui->comboBox_2->currentText();
    if(PointsMode == "225")
        ui->label_4->setText("Game over if someone has more than 225 points. 125 and 225 nullified.");
    else
        ui->label_4->setText("Game over if someone has more than 125 points. 125 nullified.");

    emit Pointssignal(PointsMode);
}

