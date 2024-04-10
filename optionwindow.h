#ifndef OPTIONWINDOW_H
#define OPTIONWINDOW_H

#include <QDialog>

namespace Ui {
class optionwindow;
}

class optionwindow : public QDialog
{
    Q_OBJECT

public:
    explicit optionwindow(QWidget *parent = nullptr);
    ~optionwindow();

public slots:
    void QSMslot2(bool b, bool a, std::string diff, int mvol, int svol);

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_comboBox_3_currentIndexChanged(int index);

private:
    Ui::optionwindow *ui;

signals:
    void QSMsignal(QString);
    void Pointssignal(QString);
    void Difficultysignal(QString);
    void Musicsignal(int);
    void Soundsignal(int);
};

#endif // OPTIONWINDOW_H
