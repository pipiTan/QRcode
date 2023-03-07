#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QZXing.h"
#include <QImage>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //编码
    connect(ui->encodeBtn, &QPushButton::clicked, this, &MainWindow::on_generator_qrcode_pbtn_clicked);

    //解码
    connect(ui->decodeBtn, &QPushButton::clicked, this,[this]() { decode(this->encodeImg); });

    connect(ui->fileDecodeBtn, &QPushButton::clicked, this, &MainWindow::on_qrcode_file_pbtn_clicked);

    connect(ui->saveCodeBtn, &QPushButton::clicked, this, &MainWindow::on_save_qrcode_pbtn_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_generator_qrcode_pbtn_clicked()
{
    //使用encodeData需要加上宏DEFINES += ENABLE_ENCODER_GENERIC
    encodeImg = QZXing::encodeData(ui->encodeText->toPlainText(),
                             QZXing::EncoderFormat_QR_CODE);
    ui->encodeLabel->setPixmap(QPixmap::fromImage(encodeImg));
}

void MainWindow::on_qrcode_file_pbtn_clicked()
{
    //选择图片文件
    QString filename = QFileDialog::getOpenFileName(this,tr("open file"),"","jpg file (*.jpg);;any files(*.*)");
    if(!(decodeImg.load(filename))){
            return;
    }
    decodeImg = decodeImg.scaled(ui->decodeLabel->width(),ui->decodeLabel->height(),Qt::KeepAspectRatio);
    //加载选择的图片显示到label标签
    ui->decodeLabel->setPixmap(QPixmap::fromImage(decodeImg));

    decode(decodeImg);
}

void MainWindow::on_save_qrcode_pbtn_clicked()
{
    QString filename =  QFileDialog::getSaveFileName(this,tr("save file"),"","jpg file (*.jpg);;all files(*.*)");
    encodeImg.save(filename);
}

void MainWindow::decode(QImage img)
{
    if(img.isNull())
        return;
    QZXing decoder;
    //QR Code二维码
    decoder.setDecoder(QZXing::DecoderFormat_QR_CODE);
    //可选设置，赋值自文档demo
    decoder.setSourceFilterType(QZXing::SourceFilter_ImageNormal);
    decoder.setTryHarderBehaviour(QZXing::TryHarderBehaviour_ThoroughScanning |
                                  QZXing::TryHarderBehaviour_Rotate);
    QString info = decoder.decodeImage(img);
    if(info.isEmpty()){
        QMessageBox::about(this,"warning","Wrong picture format!");
        return;
    }
    ui->decodeText->setPlainText(info);
}

