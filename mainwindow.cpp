#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QBoxLayout>
#include "detection.h"


using namespace std;
using namespace cv;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //初始化
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//指针初始化&&连接操作
// void MainWindow::init()
// {
//     //主窗口设置
//     this->setWindowTitle("配准算法展示软件");
//     //基础图标设置
// //    ui->rgbVedioPlayBtn->setIcon(QPixmap(":/img/resource/play.png"));
// //    ui->rgbVedioPauseBtn->setIcon(QPixmap(":/img/resource/pause.png"));
// //    ui->rgbVedioStopBtn->setIcon(QPixmap(":/img/resource/stop.png"));
// //    ui->infraVedioPlayBtn->setIcon(QPixmap(":/img/resource/play.png"));
// //    ui->infraVedioPauseBtn->setIcon(QPixmap(":/img/resource/pause.png"));
// //    ui->infraVedioStopBtn->setIcon(QPixmap(":/img/resource/stop.png"));
// //    ui->regisVedioPlayBtn->setIcon(QPixmap(":/img/resource/play.png"));
// //    ui->regisVedioPauseBtn->setIcon(QPixmap(":/img/resource/pause.png"));
// //    ui->regisVedioStopBtn->setIcon(QPixmap(":/img/resource/stop.png"));

//     //没加载视频前播放按键禁止操作
// //    setEnableBtn(false);

//     //状态栏设置
//     //标签
//     mw_curStateLabel = new QLabel();
//     mw_curStateLabel->setText("");
//     mw_curStateLabel->setVisible(false);
//     ui->statusBar->addWidget(mw_curStateLabel);
//     //进度条
//     mw_ProgressBar = new QProgressBar(this);
//     mw_ProgressBar->setRange(0,100);
//     mw_ProgressBar->setMinimum(0);
//     mw_ProgressBar->setMaximum(100);
//     mw_ProgressBar->setTextVisible(false);
//     mw_ProgressBar->setVisible(false);
//     ui->statusBar->addPermanentWidget(mw_ProgressBar);

//     //加载视频状态操作
//     mw_rgb_LoadState = false;
//     mw_infra_LoadState = false;

//     //RGB播放器设置
// //    mw_rgb_player = new QMediaPlayer(this);
// //    mw_rgb_videoWidget = new QVideoWidget(this);
// //    mw_rgb_player->setVideoOutput(mw_rgb_videoWidget);
// //    mw_rgb_videoWidget->resize(320,240);
// //    ui->rgbVideoLayOut->addWidget(mw_rgb_videoWidget);

//     //    mw_rgb_imageLabel->setPixmap(QPixmap::fromImage(yourImage)); // 设置图片
//     //    mw_rgb_imageLabel->setScaledContents(true); // 图片自适应控件大小

//     // RGB图像显示初始化
//     mw_rgb_imageLabel = new QLabel(this); // 创建一个QLabel控件
//     ui->rgbVideoLayOut->addWidget(mw_rgb_imageLabel); // 将QLabel添加到布局


//     //红外播放器设置
// //    mw_infra_player = new QMediaPlayer(this);
// //    mw_infra_videoWidget = new QVideoWidget(this);
// //    mw_infra_player->setVideoOutput(mw_infra_videoWidget);
// //    mw_infra_videoWidget->resize(320,240);
// //    ui->infraVideoLayOut->addWidget(mw_infra_videoWidget);


//     // infra图像显示初始化
//     mw_infra_imageLabel = new QLabel(this); // 创建一个QLabel控件
//     ui->infraVideoLayOut->addWidget(mw_infra_imageLabel); // 将QLabel添加到布局


//     // 配准图像显示初始化
//     mw_regis_imageLabel = new QLabel(this); // 创建一个QLabel控件
//     ui->infraVideoLayOut_8->addWidget(mw_regis_imageLabel); // 将QLabel添加到布局

//     //  鱼眼矫正初始化
//     mw_fish_imageLabel = new QLabel(this); // 创建一个QLabel控件
//     ui->infraVideoLayOut_3->addWidget(mw_fish_imageLabel); // 将QLabel添加到布局

//     // 在这里加载 YOLO 模型
//     bool ok = m_detector.ReadModel(
//         "D:/QTPro/RegistrationProj/yolov4/yolov4.cfg",      // cfg 文件路径
//         "D:/QTPro/RegistrationProj/yolov4/yolov4.weights",  // weights 文件路径
//         "D:/QTPro/RegistrationProj/yolov4/coco.names"       // 类别名称文件
//         );
//     if(!ok) {
//         // 如果加载失败，可以弹出错误提示或打印日志
//         QMessageBox::critical(this, "错误", "加载 YOLO 模型失败，请检查路径！");
//     }

//     // 设置置信度和 NMS 阈值，可自行修改
//     m_detector.SetParams(0.5f, 0.4f);


//     //配准视频播放器设置
// //    mw_regis_player = new QMediaPlayer(this);
// //    mw_regis_videoWidget = new QVideoWidget(this);
// //    mw_regis_player->setVideoOutput(mw_regis_videoWidget);
// //    mw_regis_videoWidget->resize(320,240);
// //    ui->regisVideoLayOut->addWidget(mw_regis_videoWidget);

// }

void MainWindow::init()
{
    // 主窗口设置
    this->setWindowTitle("配准算法展示软件");

    // 状态栏设置
    mw_curStateLabel = new QLabel();
    mw_curStateLabel->setText("");
    mw_curStateLabel->setVisible(false);
    ui->statusBar->addWidget(mw_curStateLabel);

    mw_ProgressBar = new QProgressBar(this);
    mw_ProgressBar->setRange(0,100);
    mw_ProgressBar->setTextVisible(false);
    mw_ProgressBar->setVisible(false);
    ui->statusBar->addPermanentWidget(mw_ProgressBar);

    // 进度标志
    mw_rgb_LoadState   = false;
    mw_infra_LoadState = false;

    // =========== 左图显示 =============
    mw_rgb_imageLabel = new QLabel(this);
    mw_rgb_imageLabel->setAlignment(Qt::AlignCenter);
    mw_rgb_imageLabel->setText("左图像");
    ui->rgbVideoLayOut->addWidget(mw_rgb_imageLabel);

    // =========== 右图显示 =============
    mw_infra_imageLabel = new QLabel(this);
    mw_infra_imageLabel->setAlignment(Qt::AlignCenter);
    mw_infra_imageLabel->setText("右图像");
    ui->infraVideoLayOut->addWidget(mw_infra_imageLabel);

    // =========== 配准结果显示 =============
    mw_regis_imageLabel = new QLabel(this);
    mw_regis_imageLabel->setAlignment(Qt::AlignCenter);
    mw_regis_imageLabel->setText("图像配准结果");
    ui->infraVideoLayOut_8->addWidget(mw_regis_imageLabel);

    // =========== 鱼眼矫正结果显示 =============
    mw_fish_imageLabel = new QLabel(this);
    mw_fish_imageLabel->setAlignment(Qt::AlignCenter);
    mw_fish_imageLabel->setText("鱼眼矫正结果");
    ui->infraVideoLayOut_3->addWidget(mw_fish_imageLabel);

    // =========== 目标检测结果显示 =============
    mw_detection_imageLabel = new QLabel(this);
    mw_detection_imageLabel->setAlignment(Qt::AlignCenter);
    mw_detection_imageLabel->setText("目标检测结果");
    // infraVideoLayOut_5 就是你在 .ui 里对应“目标检测结果”区域的布局
    ui->infraVideoLayOut_5->addWidget(mw_detection_imageLabel);

    // =========== 加载 YOLO 模型 =============
    bool ok = m_detector.ReadModel(
        "D:/QTPro/RegistrationProj/yolov4/yolov4.cfg",      // cfg 文件路径
        "D:/QTPro/RegistrationProj/yolov4/yolov4.weights",  // weights 文件路径
        "D:/QTPro/RegistrationProj/yolov4/coco.names"       // 类别名称文件
        );
    if(!ok) {
        QMessageBox::critical(this, "错误", "加载 YOLO 模型失败，请检查路径！");
    }

    // 置信度和 NMS 阈值，可按需修改
    m_detector.SetParams(0.5f, 0.4f);
}

//设置按钮可用状态
//void MainWindow::setEnableBtn(bool state)
//{
//    ui->rgbVedioPlayBtn->setEnabled(state);
//    ui->rgbVedioPauseBtn->setEnabled(state);
//    ui->rgbVedioStopBtn->setEnabled(state);
//    ui->infraVedioPlayBtn->setEnabled(state);
//    ui->infraVedioPauseBtn->setEnabled(state);
//    ui->infraVedioStopBtn->setEnabled(state);
//    ui->regisVedioPlayBtn->setEnabled(state);
//    ui->regisVedioPauseBtn->setEnabled(state);
//    ui->regisVedioStopBtn->setEnabled(state);
//}


//加载图片按钮
void MainWindow::on_loadImgBtn_clicked()
{
    mw_curStateLabel->setVisible(true);
    mw_curStateLabel->setText("加载图像");
    mw_ProgressBar->setVisible(true);
    mw_ProgressBar->setValue(0);
    //首先对路径非空进行检验
    if(mw_rgbImgFilePath != "")
    {
        //TODO加载到你需要的位置
//        mw_rgb_player->setMedia(QUrl::fromLocalFile(mw_rgbImgFilePath));
//        mw_rgb_videoWidget->show();



        mw_rgbImage = QPixmap(mw_rgbImgFilePath);
        QSize labelSize = mw_rgb_imageLabel->size();
        mw_rgbImage = mw_rgbImage.scaled(labelSize, Qt::KeepAspectRatio);
        mw_rgb_imageLabel->setPixmap(mw_rgbImage);
//        mw_rgb_label->setScaledContents(true); // 可选，使图像按比例显示
        mw_rgb_LoadState = true;
        mw_ProgressBar->setValue(50);



    }
    else
    {
        QMessageBox::critical(this,"错误","RGB视频地址为空，请重新加载!");
    }
    //首先对路径非空进行检验
    if(mw_infraImgFilePath != "")
    {
        //TODO加载到你需要的位置
//        mw_infra_player->setMedia(QUrl::fromLocalFile(mw_infraImgFilePath));
//        mw_infra_videoWidget->show();


        mw_infraImage = QPixmap(mw_infraImgFilePath);
        QSize labelSize = mw_infra_imageLabel->size();
        mw_infraImage = mw_infraImage.scaled(labelSize, Qt::KeepAspectRatio);
        mw_infra_imageLabel->setPixmap(mw_infraImage);
//        mw_infra_label->setScaledContents(true); // 可选，使图像按比例显示
        mw_infra_LoadState = true;
        mw_ProgressBar->setValue(100);

    }
    else
    {
        QMessageBox::critical(this,"错误","红外视频地址为空，请重新加载!");
    }
    if(mw_rgb_LoadState&&mw_infra_LoadState)
    {
//        setEnableBtn(true);
        QMessageBox::information(this,"提示","载入图片成功！");
        mw_curStateLabel->setText("加载成功");
    }
    mw_ProgressBar->setVisible(false);
}


/*
 * @brief 进行图像配准和拼接，并显示进度
 * @param img1 第一张输入图像
 * @param img2 第二张输入图像
 * @return 返回拼接后的图像，如果失败则返回空 Mat
 */
Mat Joint(Mat img1, Mat img2)
{

    // 检查输入图像是否为空
    if (img1.empty() || img2.empty())
    {
        cerr << "读取文件失败！请检查文件路径或格式。" << endl;
        return Mat(); // 返回空 Mat 以避免未定义行为
    }

    cout << "[1/3] 读取图像完成，开始拼接..." << endl;

    vector<Mat> Img;
    Img.push_back(img1);
    Img.push_back(img2);

    Mat pano_ini;
    Stitcher::Mode mode = Stitcher::PANORAMA;
    Ptr<Stitcher> stitcher = Stitcher::create(mode); // 创建拼接器

    cout << "[2/3] 拼接处理中..." << endl;
    Stitcher::Status status = stitcher->stitch(Img, pano_ini); // 执行拼接

    // 检查拼接是否成功
    if (status != Stitcher::OK)
    {
        cerr << "拼接失败，错误代码: " << status << endl;
        return Mat(); // 返回空 Mat 以指示失败
    }

    cout << "[3/3] 拼接成功！" << endl;

    return pano_ini; // 返回拼接后的图像
}


//开始配准
void MainWindow::on_startRegBtn_clicked()
{
    string mw_rgbImgPath = mw_rgbImgFilePath.toStdString();
    string mw_infraImgPath = mw_infraImgFilePath.toStdString();
    Mat rgbImg = imread(mw_rgbImgPath);
    Mat infraImg = imread(mw_infraImgPath);
    pano = Joint(rgbImg, infraImg);

    // 将 cv::Mat 转换为 QImage
    qPano = QImage(pano.data, pano.cols, pano.rows, pano.step, QImage::Format_RGB888);
    mw_infraImage = QPixmap::fromImage(qPano);
    // 获取 QLabel 的大小
    QSize labelSize = mw_regis_imageLabel->size();
    // 缩放图像以适应 QLabel 大小
    mw_infraImage = mw_infraImage.scaled(labelSize, Qt::KeepAspectRatio);

    mw_regis_imageLabel->setPixmap(mw_infraImage);


}


Mat FisheyeCorrection(Mat fisheyeImage)
{
    if (fisheyeImage.empty()) {
        cerr << "输入图像为空，无法进行鱼眼校正！" << endl;
        return Mat();
    }

    // 获取输入图像尺寸
    Size imageSize = fisheyeImage.size();

    // 内参矩阵 K
    Mat K = Mat::eye(3, 3, CV_64F);
    K.at<double>(0, 0) = 500;  // fx：X轴上的焦距
    K.at<double>(1, 1) = 500;  // fy：Y轴上的焦距
    K.at<double>(0, 2) = 294;  // cx：X轴上的光学中心坐标
    K.at<double>(1, 2) = 244;  // cy：Y轴上的光学中心坐标

    // 畸变系数 D，确保有 4 个参数
    Mat D = Mat::zeros(4, 1, CV_64F);
    D.at<double>(0) = 0.1;  // k1
    D.at<double>(1) = 0.1;  // k2
    D.at<double>(2) = 0.01; // k3
    D.at<double>(3) = 0.1;  // p1

    // 计算新的内参矩阵（避免黑边）
    Mat newK = getOptimalNewCameraMatrix(K, D, imageSize, 1, imageSize);

    // 计算去畸变映射
    Mat mapX, mapY;
    fisheye::initUndistortRectifyMap(K, D, Mat(), newK, imageSize, CV_32F, mapX, mapY);

    // 输出矫正后的图像
    Mat undistortedImage;
    remap(fisheyeImage, undistortedImage, mapX, mapY, INTER_LINEAR);

    if (undistortedImage.empty()) {
        cout << "remap() 失败，未生成有效的矫正图像！" << endl;
        return Mat();
    }

    return undistortedImage;
}



// 开始鱼眼矫正
void MainWindow::on_FishBtn_clicked()
{
    fishCor = FisheyeCorrection(pano);


    // 将 cv::Mat 转换为 QImage
    qfishCor = QImage(fishCor.data, fishCor.cols, fishCor.rows, fishCor.step, QImage::Format_RGB888);
    mw_fishImage = QPixmap::fromImage(qfishCor);
    // 获取 QLabel 的大小
    QSize labelSize = mw_fish_imageLabel->size();
    // 缩放图像以适应 QLabel 大小
    mw_fishImage = mw_fishImage.scaled(labelSize, Qt::KeepAspectRatio);

    mw_fish_imageLabel->setPixmap(mw_fishImage);
}


//保存视频
void MainWindow::on_saveImgBtn_clicked()
{
    //TODO
    //保存视频，也可以加进度条
}

//获取图片1
void MainWindow::on_rgbImgFilePathTBtn_clicked()
{
    mw_rgbImgFilePath = QFileDialog::getOpenFileName(this,"选择RGB视频","../","图像文件(*.jpg *.png *.bmp *.jpeg)");
    ui->rgbImgFilePathEdit->setText(mw_rgbImgFilePath);
}

//获取图片2
void MainWindow::on_infraImgFilePathTBtn_clicked()
{
    mw_infraImgFilePath = QFileDialog::getOpenFileName(this,"选择RGB视频","../","图像文件(*.jpg *.png *.bmp *.jpeg)");
    ui->infraImgFilePathEdit->setText(mw_infraImgFilePath);
}

//获取配准视频保存路径
void MainWindow::on_regisImgFilePathTBtn_clicked()
{
//    mw_regisImgFilePath = QFileDialog::getSaveFileName(this,"选择配准后视频保存路径","../","视频格式(*.avi *.mp4 *.flv *.mkv)");
//    ui->regisImgFilePathEdit->setText(mw_regisImgFilePath);
}







void MainWindow::on_DetectionBtn_clicked()
{
    // 1. 检查拼接结果是否为空
    if (pano.empty()) {
        QMessageBox::critical(this, "错误", "配准图像为空，无法进行目标检测！");
        return;
    }

    // 2. 执行检测
    bool success = m_detector.Detecting(pano);
    if (!success) {
        QMessageBox::critical(this, "错误", "目标检测失败！");
        return;
    }

    // 3. 获取检测后带框的图像
    Mat detectedImg = m_detector.GetOutputImage();
    if (detectedImg.empty()) {
        QMessageBox::critical(this, "错误", "目标检测输出图像为空！");
        return;
    }

    // 4. BGR -> RGB  (OpenCV默认BGR，Qt需要RGB)
    cvtColor(detectedImg, detectedImg, cv::COLOR_BGR2RGB);

    // 5. 转换为 QPixmap
    QImage qimgDetected((uchar*)detectedImg.data,
                        detectedImg.cols,
                        detectedImg.rows,
                        detectedImg.step,
                        QImage::Format_RGB888);
    QPixmap pixmapDetected = QPixmap::fromImage(qimgDetected);

    // 6. 缩放以适应 “目标检测结果” 的 QLabel
    QSize labelSize = mw_detection_imageLabel->size();
    pixmapDetected = pixmapDetected.scaled(labelSize, Qt::KeepAspectRatio);

    // 7. 在 “目标检测结果” 区域中显示
    mw_detection_imageLabel->setPixmap(pixmapDetected);

    // 可选：提示检测完成
    QMessageBox::information(this, "提示", "目标检测完成！");
}


