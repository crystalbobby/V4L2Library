#include "filterwindow.h"
#include "ui_filterwindow.h"

FilterWindow::FilterWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    timerSet = new QTimer(this);
    connect(timerSet,SIGNAL(timeout()),this,SLOT(refreshSets()));
    timerSet->start(200);
}

void FilterWindow::refreshSets(){
    if(ui->Brightness->isEnabled()){
        if(cam->setBrightness(ui->Brightness->value()) == -1)
            ui->Brightness->setEnabled(false);
    }
    if(ui->Contrast->isEnabled()){
        if(cam->setContrast(ui->Contrast->value()) == -1)
            ui->Contrast->setEnabled(false);
    }
    if(ui->HUE->isEnabled()){
        if(cam->setHUE(ui->HUE->value()) == -1)
            ui->HUE->setEnabled(false);
    }
    if(ui->Saturation->isEnabled()){
        if(cam->setSaturation(ui->Saturation->value()) == -1)
            ui->Saturation->setEnabled(false);
    }
    if(ui->GAMMA->isEnabled()){
        if(cam->setGAMMA(ui->GAMMA->value()) == -1)
            ui->GAMMA->setEnabled(false);
    }
    if(ui->AutoWhiteBalance->isEnabled()){
        if(cam->setAutoWhiteBalance(ui->AutoWhiteBalance->isChecked()) == -1)
            ui->AutoWhiteBalance->setEnabled(false);
    }
    if(ui->WhiteBalance->isEnabled()){
        if(cam->setWhiteBalance(ui->WhiteBalance->value()) == -1)
            ui->WhiteBalance->setEnabled(false);
    }
    if(ui->AutoGain->isEnabled()){
        if(cam->setAutoGain(ui->AutoGain->isChecked()) == -1)
            ui->AutoGain->setEnabled(false);
    }
    if(ui->Gain->isEnabled()){
        if(cam->setGain(ui->Gain->value()) == -1)
            ui->Gain->setEnabled(false);
    }
    if(ui->AutoExposure->isEnabled()){
        if(cam->setAutoExposure(ui->AutoExposure->isChecked()) == -1)
            ui->AutoExposure->setEnabled(false);
    }
    if(ui->Exposure->isEnabled()){
        if(cam->setExposure(ui->Exposure->value()) == -1)
            ui->Exposure->setEnabled(false);
    }
}

int FilterWindow::readOptions(){
    int valueBuff;

    valueBuff = cam->getBrightness();
    if(valueBuff!=-255){
        cam->setBrightness(-255);
        ui->Brightness->setMinimum(cam->getBrightness());
        cam->setBrightness(255);
        ui->Brightness->setMaximum(cam->getBrightness());
        cam->setBrightness(valueBuff);
        ui->Brightness->setValue(valueBuff);
    }
    else
        ui->Brightness->setEnabled(false);

    valueBuff = cam->getContrast();
    if(valueBuff!=-255){
        cam->setContrast(255);
        ui->Contrast->setMaximum(cam->getContrast());
        cam->setContrast(valueBuff);
        ui->Contrast->setValue(valueBuff);
    }
    else
        ui->Contrast->setEnabled(false);

    valueBuff = cam->getHUE();
    if(valueBuff!=-255){
        cam->setHUE(-255);
        ui->HUE->setMinimum(cam->getHUE());
        cam->setHUE(255);
        ui->HUE->setMaximum(cam->getHUE());
        cam->setHUE(valueBuff);
        ui->HUE->setValue(valueBuff);
    }
    else
        ui->HUE->setEnabled(false);

    valueBuff = cam->getSaturation();
    if(valueBuff!=-255){
        cam->setSaturation(255);
        ui->Saturation->setMaximum(cam->getSaturation());
        cam->setSaturation(valueBuff);
        ui->Saturation->setValue(valueBuff);
    }
    else
        ui->Saturation->setEnabled(false);

    valueBuff = cam->getGAMMA();
    if(valueBuff!=-255){
        cam->setGAMMA(255);
        ui->GAMMA->setMaximum(cam->getGAMMA());
        cam->setGAMMA(valueBuff);
        ui->GAMMA->setValue(valueBuff);
    }
    else
        ui->GAMMA->setEnabled(false);

    ui->AutoWhiteBalance->setChecked(cam->getAutoWhiteBalance());

    valueBuff = cam->getWhiteBalance();
    if(valueBuff!=-255){
        cam->setWhiteBalance(255);
        ui->WhiteBalance->setMaximum(cam->getWhiteBalance());
        cam->setWhiteBalance(valueBuff);
        ui->WhiteBalance->setValue(valueBuff);
    }
    else
        ui->WhiteBalance->setEnabled(false);

    ui->AutoGain->setChecked(cam->getAutoGain());

    valueBuff = cam->getGain();
    if(valueBuff!=-255){
        cam->setGain(255);
        ui->Gain->setMaximum(cam->getGain());
        cam->setGain(valueBuff);
        ui->Gain->setValue(valueBuff);
    }
    else
        ui->Gain->setEnabled(false);

    ui->AutoExposure->setChecked(cam->getAutoExposure());

    valueBuff = cam->getExposure();
    if(valueBuff!=-255){
        cam->setExposure(255);
        ui->Exposure->setMaximum(cam->getExposure());
        cam->setExposure(valueBuff);
        ui->Exposure->setValue(valueBuff);
    }
    else
        ui->Exposure->setEnabled(false);

    return 0;
}

FilterWindow::~FilterWindow()
{
    delete ui;
}

void FilterWindow::on_cancelButton_clicked()
{
    delete this;
}

void FilterWindow::on_Brightness_valueChanged(int value)
{
    ui->numBrightness->setText(QString::number(value));
}

void FilterWindow::on_Contrast_valueChanged(int value)
{
    ui->numContrast->setText(QString::number(value));
}

void FilterWindow::on_HUE_valueChanged(int value)
{
    ui->numHUE->setText(QString::number(value));
}

void FilterWindow::on_Saturation_valueChanged(int value)
{
    ui->numSaturation->setText(QString::number(value));
}

void FilterWindow::on_GAMMA_valueChanged(int value)
{
    ui->numGAMMA->setText(QString::number(value));
}

void FilterWindow::on_WhiteBalance_valueChanged(int value)
{
    ui->numWhiteBalance->setText(QString::number(value));
}

void FilterWindow::on_Gain_valueChanged(int value)
{
    ui->numGain->setText(QString::number(value));
}

void FilterWindow::on_Exposure_valueChanged(int value)
{
    ui->numExposure->setText(QString::number(value));
}

void FilterWindow::on_AutoWhiteBalance_toggled(bool checked)
{
    if(checked == false)
        ui->WhiteBalance->setEnabled(true);
}

void FilterWindow::on_AutoGain_toggled(bool checked)
{
    if(checked == false)
        ui->Gain->setEnabled(true);
}

void FilterWindow::on_AutoExposure_toggled(bool checked)
{
    if(checked == false)
        ui->Exposure->setEnabled(true);
}
