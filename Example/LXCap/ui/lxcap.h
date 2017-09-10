#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <libv4l2.h>
#include <QMainWindow>
#include <QTimer>
#include <QComboBox>
#include <QPixmap>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <QUrl>
#include <QDesktopServices>
#include <QKeyEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>
#include <QInputDialog>
#include <QSound>

namespace Ui {
class MainWindow;
}

struct camDev{
    uint numDev;
    string nameDev;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    v4l2Cam *cam;
    QPixmap camimg;
    QTimer *timerShowImg;
    QTimer *timerShootImg;
    vector<camDev> camdevs;
    uchar CamNum;
    uint CamWidth, CamHeight, CamFps;
    QUrl imgSaveAddr;
    QAction *actRefresh;
    int getDevs();
    int turnOnCam();
    void setSavePath();
    void keyPressEvent(QKeyEvent *event);

private slots:
    void refreshDev();
    void refreshImg();
    void trigerMenu(QAction* act);
    void changePin(uint width, uint height, uint fps);
    void changeDev(uint num);
    void showStatus(QString txt);
    void on_actionExit_triggered();
    void on_actionVideo_Capture_Filter_triggered();
    void on_actionHelp_triggered();
    void on_actionGithub_triggered();
    void on_actionAuthor_s_Blog_triggered();
    void on_actionVideo_Capture_Pin_triggered();
    void on_actionAbout_triggered();
    void on_actionPause_toggled(bool arg1);
    void on_actionFull_Screen_triggered();
    void on_actionSnap_triggered();
    void on_actionSet_save_path_triggered();
    void on_actionTimed_shoot_triggered(bool checked);
};

#endif // MAINWINDOW_H
