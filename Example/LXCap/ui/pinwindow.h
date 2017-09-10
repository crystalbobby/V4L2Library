#ifndef PINWINDOW_H
#define PINWINDOW_H

#include <libv4l2.h>
#include <QWidget>
#include <QMessageBox>

namespace Ui {
class PinWindow;
}

class PinWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PinWindow(QWidget *parent = 0);
    ~PinWindow();
    vector<formatType> FormatType;
    int readOptions();

private slots:
    void on_cancelButton_clicked();
    void on_commitButton_clicked();
    void on_matSelect_currentIndexChanged(int index);

private:
    Ui::PinWindow *ui;

signals:
    void commitSets(uint width,uint height,uint fps);
};

#endif // PINWINDOW_H
