#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QPicture>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&timer, &QTimer::timeout, this, &MainWindow::updateCameraImage);
    timer.start(30);

    camera.reset(new cv::VideoCapture(0));
}

MainWindow::~MainWindow()
{
    camera->release();

    delete ui;
}

void MainWindow::on_pushButton_identify_clicked()
{
}

QImage cvMat2QImage(const cv::Mat &image)
{
    // 8-bits unsigned, NO. OF CHANNELS=1
    if (image.type() == CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i = 0; i < 256; ++i)
        {
            colorTable.push_back(qRgb(i, i, i));
        }
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*) image.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, image.cols, image.rows, image.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if (image.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*) image.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, image.cols, image.rows, image.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

void MainWindow::updateCameraImage()
{
    cv::Mat image;
    *camera >> image;

    QImage img = cvMat2QImage(image);

    QPixmap pixmap;
    pixmap.convertFromImage(img);
    ui->label->setPixmap(pixmap);
}

void MainWindow::on_actionCreate_database_triggered()
{
    databasecreator_gui.show();
}
