#ifndef FILTERWINDOW_H
#define FILTERWINDOW_H

#include <libv4l2.h>
#include <QWidget>
#include <QTimer>

namespace Ui {
class FilterWindow;
}

class FilterWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FilterWindow(QWidget *parent = 0);
    ~FilterWindow();
    v4l2Cam *cam;
    int readOptions();

private:
    QTimer *timerSet;

private slots:
    void refreshSets();
    void on_cancelButton_clicked();
    void on_Brightness_valueChanged(int value);
    void on_Contrast_valueChanged(int value);
    void on_HUE_valueChanged(int value);
    void on_Saturation_valueChanged(int value);
    void on_GAMMA_valueChanged(int value);
    void on_WhiteBalance_valueChanged(int value);
    void on_Gain_valueChanged(int value);
    void on_Exposure_valueChanged(int value);
    void on_AutoWhiteBalance_toggled(bool checked);
    void on_AutoGain_toggled(bool checked);
    void on_AutoExposure_toggled(bool checked);

private:
    Ui::FilterWindow *ui;
};

#endif // FILTERWINDOW_H
