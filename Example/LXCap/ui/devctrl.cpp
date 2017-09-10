#include "lxcap.h"
#include "ui_lxcap.h"

int MainWindow::getDevs(){
    ui->menuDevices->addAction(actRefresh);
    ui->menuDevices->addSeparator();
    vector<uint> devs;//存放文件名
    DIR *dir;
    struct dirent *ptr;

    if ((dir=opendir("/dev")) == NULL){
        QMessageBox::critical(this, "ERROR", "The software support Linux only."); exit(1);
    }

    while ((ptr=readdir(dir)) != NULL){
        if(ptr->d_type == 2){
            string filename(ptr->d_name);
            if(filename.find("video") == 0){
                devs.push_back((uint)atoi(filename.substr(5).c_str()));
            }
        }
    }
    closedir(dir);
    if(devs.size() == 0){
        QMessageBox::critical(this, "ERROR", "No video device found."); return -1;
    }

    camdevs.clear();
    for (uint i=0; i<devs.size(); i++){
        camDev camdev;
        camdev.numDev = devs[devs.size()-i-1];
        cam = new v4l2Cam(camdev.numDev);
        camdev.nameDev = cam->devName;
        if(camdev.nameDev == "") continue;
        ui->menuDevices->addAction(QString::fromStdString(camdev.nameDev)+" - video"+QString::number(camdev.numDev));
        camdevs.push_back(camdev);
    }
    CamNum = camdevs[0].numDev;
    return 0;
}

void MainWindow::refreshDev(){
    ui->menuDevices->clear();
    timerShowImg->stop();
    cam->releaseMen();
    getDevs();
    turnOnCam();
}

void MainWindow::changeDev(uint num){
    CamNum = num;
    turnOnCam();
}

void MainWindow::changePin(uint width,uint height,uint fps){
    CamWidth = width; CamHeight = height; CamFps = fps;
    cam->releaseMen();
    turnOnCam();
}

int MainWindow::turnOnCam(){
    timerShowImg->stop();
    this->statusBar()->showMessage("Turning on camera, please wait...");
    cam = new v4l2Cam(CamNum);
    if(cam->devName == ""){
        QMessageBox::critical(this, "ERROR", "The camera open failed. Please refresh the "
                                             "device list or retry."); return -1;
    }
    cam->setPixelformat(CamWidth,CamHeight);
    cam->setFps(CamFps);
    if(cam->memsetCam() == -1){
        QMessageBox::critical(this, "ERROR", "The camera open failed. Please refresh the "
                                             "device list or retry."); return -1;
    }
    this->setWindowTitle("LXCap v1.0 - "+ QString::fromStdString(cam->devName)+" (video"+QString::number(CamNum)+")");
    this->statusBar()->showMessage(QString::fromStdString(cam->devName)+" (video"+QString::number(CamNum)+")    |   "+\
                                   QString::number(cam->Width)+" x "+QString::number(cam->Height)+\
                                   " "+QString::number(cam->Fps)+"Hz",0);
    timerShowImg->start(50);
    return 0;
}

void MainWindow::refreshImg(){
    v4l2pic *rece = cam->Camera_Read();
    camimg.loadFromData(rece->addr,rece->imgheight*rece->imgwidth);

    ui->imgshow->setPixmap(camimg);
    ui->imgshow->show();
}
