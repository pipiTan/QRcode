#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_generator_qrcode_pbtn_clicked();
    void on_qrcode_scan_pbtn_clicked();

private:
    Ui::MainWindow *ui;
    QImage encodeImg;
    QImage decodeImg;
    void decode(QImage);
};
#endif // MAINWINDOW_H
