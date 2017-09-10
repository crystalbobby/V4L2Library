#include "pinwindow.h"
#include "ui_pinwindow.h"

PinWindow::PinWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PinWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
}

int PinWindow::readOptions(){
    for(uint i=0;i<FormatType.size();i++){
        formatType type = (FormatType)[i];
        ui->matSelect->addItem(QString::fromStdString(type.description));
    }
    ui->fpsSelect->addItem("30");
    ui->fpsSelect->addItem("60");
    ui->fpsSelect->addItem("120");
    ui->fpsSelect->setCurrentIndex(1);
    return 0;
}

void PinWindow::on_commitButton_clicked()
{
    QString curText = ui->dpiSelect->currentText();
    int indexX = curText.indexOf("x");
    if(ui->matSelect->currentText().indexOf("JP") == -1)
        QMessageBox::information(this,"Unsupport format",
                                 "Sorry, the format you choose is not supported by LXCap v1.0,\n"
                                 "Pleace check update or do a second-hand development.\n"
                                 "PS: using OpenCV is easy to implement this :)");
    emit commitSets(curText.left(indexX).toUInt(),curText.mid(indexX+1).toUInt(),ui->fpsSelect->currentText().toUInt());
    delete this;
}

void PinWindow::on_matSelect_currentIndexChanged(int index)
{
    ui->dpiSelect->clear();
    formatType type = (FormatType)[index];
    for(uint i=0;i<type.framesize.size();i++){
        ui->dpiSelect->addItem(QString::fromStdString(type.framesize[i]));
    }
}

void PinWindow::on_cancelButton_clicked()
{
    delete this;
}

PinWindow::~PinWindow()
{
    delete ui;
}
