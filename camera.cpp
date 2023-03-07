#include "qapplication.h"
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QVBoxLayout>
#include <QZXing>

QZXing *decoder;

void captureImage()
{
    QImage capturedImage = cameraImageCapture->capture();
    if (!capturedImage.isNull()) {
        // 将捕获的图像发送给解码器
        decoder->decodeImage(capturedImage);
    }
}

void onDecoded(QString result)
{
    qDebug() << "解码结果：" << result;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 创建Camera组件
    QCamera *camera = new QCamera;
    camera->setCaptureMode(QCamera::CaptureStillImage);

    // 创建Viewfinder组件
    QCameraViewfinder *viewfinder = new QCameraViewfinder;

    // 创建ImageCapture组件
    QCameraImageCapture *imageCapture = new QCameraImageCapture(camera);

    // 将Viewfinder和ImageCapture组件附加到Camera上
    camera->setViewfinder(viewfinder);
    camera->setImageCapture(imageCapture);

    // 创建解码器组件
    decoder = new QZXing;
    QObject::connect(decoder, &QZXing::decodingStarted, []() {
        qDebug() << "开始解码...";
    });
    QObject::connect(decoder, &QZXing::decodingFinished, onDecoded);

    // 创建主窗口并添加Viewfinder组件
    QWidget *window = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(viewfinder);
    window->setLayout(layout);

    // 连接Camera组件的信号和槽
    QObject::connect(imageCapture, &QCameraImageCapture::imageCaptured, captureImage);

    // 启动Camera组件
    camera->start();

    window->show();

    return a.exec();
}
