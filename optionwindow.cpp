#include "optionwindow.h"
#include "ui_optionwindow.h"

optionwindow::optionwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::optionwindow)
{
    ui->setupUi(this);

    ui->textEdit->setReadOnly(true);
    ui->horizontalSlider->setMaximum(100);
    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider_2->setMaximum(100);
    ui->horizontalSlider_2->setMinimum(0);
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

void optionwindow::QSMslot2(bool b, bool a, std::string diff, int mvol, int svol)
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
    if(diff == "Middle"){
        ui->comboBox_3->setCurrentIndex(0);
    }
    else if(diff == "Hard"){
        ui->comboBox_3->setCurrentIndex(1);
    }
    if(mvol >= 0 && mvol <= 100){
        ui->horizontalSlider->setValue(mvol);
        ui->horizontalSlider_2->setValue(svol);
    }
    else{
        ui->horizontalSlider->setValue(50);
        ui->horizontalSlider_2->setValue(50);
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

void optionwindow::on_horizontalSlider_valueChanged(int value)
{
    ui->label_6->setText(QString::number(value));
    emit Musicsignal(value);
}


void optionwindow::on_horizontalSlider_2_valueChanged(int value)
{
    ui->label_8->setText(QString::number(value));
    emit Soundsignal(value);
}


void optionwindow::on_comboBox_3_currentIndexChanged(int index)
{
    QString difficulty = ui->comboBox_3->currentText();
    emit Difficultysignal(difficulty);
}

