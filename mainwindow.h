#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QMediaPlayer>
#include <QtMultimediaWidgets/QVideoWidget>
#include <vector>
#include <QProgressBar>
#include <QLabel>


#include "detection.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/stitching.hpp>





QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //初始化指针
    void init();
    //是否禁用播放按钮
    void setEnableBtn(bool state);

protected:
    //RGB播放器
    QMediaPlayer* mw_rgb_player =  nullptr;
    //RGB的videoWidget
    QVideoWidget* mw_rgb_videoWidget = nullptr;
    //红外播放器
    QMediaPlayer* mw_infra_player =  nullptr;
    //红外的videoWidget
    QVideoWidget* mw_infra_videoWidget = nullptr;
    //配准播放器
    QMediaPlayer* mw_regis_player =  nullptr;
    //配准的videoWidget
    QVideoWidget* mw_regis_videoWidget = nullptr;

    //进度条
    QProgressBar* mw_ProgressBar;
    //进度条使用方法，首先设置mw_ProgressBar->setVisable(true);
    //在循环里加上mw_ProgressBar->setValue(当前值0～100)；
    //使用完在setVisable(false)

    //状态文字
    QLabel* mw_curStateLabel;
    //状态Label使用方法 首先设置mw_curStateLabel->setVisable(true);
    //在循环里加上mw_curStateLabel->setText("配准视频balabala")；
    //使用完在setVisable(false)


    //RGB图片/视频文件地址
    QString mw_rgbImgFilePath;//如果想转成std:String格式下->toStdString()
    //红外图片/视频地址
    QString mw_infraImgFilePath;
    //配准后图片/视频储存位置
    QString mw_regisImgFilePath;

    // 显示图片1
    QLabel *mw_rgb_imageLabel;
    // 存储图像1
    QPixmap mw_rgbImage;
    QLabel *mw_rgb_label;


    // 显示图片2
    QLabel *mw_infra_imageLabel;
    // 存储图像2
    QPixmap mw_infraImage;
    QLabel *mw_infra_label;

    // 显示配准图片
    QLabel *mw_regis_imageLabel;
    // 配准图像
    QPixmap mw_regisImage;
    QLabel *mw_regis_label;

    // 显示鱼眼图片
    QLabel *mw_fish_imageLabel;
    // 鱼眼图像
    QPixmap mw_fishImage;
    QLabel *mw_fish_label;



    QImage qPano;
    cv::Mat pano;
    //  鱼眼矫正
    QImage qfishCor;
    cv::Mat fishCor;


    //预留接口方便处理
    //储存RGB图像的向量
    std::vector<cv::Mat> mw_rgb_ImgVec;
    //储存红外图像向量
    std::vector<cv::Mat> mw_infra_ImgVec;
    //储存配准图像向量
    std::vector<cv::Mat> mw_regis_ImgVec;

    //RGB视频加载状态
    bool mw_rgb_LoadState;
    //红外视频加载状态
    bool mw_infra_LoadState;

//    Mat pano;

private slots:
    //槽函数
    void on_loadImgBtn_clicked();

    void on_startRegBtn_clicked();

    void on_saveImgBtn_clicked();

    void on_rgbImgFilePathTBtn_clicked();

    void on_infraImgFilePathTBtn_clicked();

    void on_regisImgFilePathTBtn_clicked();



    void on_FishBtn_clicked();

    void on_DetectionBtn_clicked();

private:
    Ui::MainWindow *ui;

    Detection m_detector;

    QLabel *mw_detection_imageLabel;


};
#endif // MAINWINDOW_H
