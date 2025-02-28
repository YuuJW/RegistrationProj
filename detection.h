#pragma once

#ifndef __DETECTION_H__
#define __DETECTION_H__

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <string>
#include <vector>

class Detection
{
public:
    Detection();
    ~Detection();

    // 1) 读取模型
    //    modelConfig  = "yolov4.cfg"
    //    modelWeights = "yolov4.weights"
    //    namesFile    = "coco.names"
    bool ReadModel(const std::string& modelConfig,
                   const std::string& modelWeights,
                   const std::string& namesFile);

    // 2) 设置阈值等参数，可根据需要自行调整
    void SetParams(float confThreshold, float nmsThreshold);

    // 3) 对输入图像进行检测，返回是否检测成功
    bool Detecting(const cv::Mat& frame);

    // 4) 获取检测完成后的图像
    cv::Mat GetOutputImage() const;

private:
    // 后处理：对网络输出进行 NMS，并在原图上画出结果
    void PostProcess(const cv::Mat& frame,
                     const std::vector<cv::Mat>& outs);

    // 获取网络输出层名称
    std::vector<cv::String> GetOutputsNames();

private:
    // YOLO 模型
    cv::dnn::Net m_net;
    // 类别名称
    std::vector<std::string> m_classNames;

    // 超参数
    float m_confThreshold;   // 置信度阈值
    float m_nmsThreshold;    // NMS 阈值

    // 最终输出的带框图像
    cv::Mat m_resultImage;
};

#endif // __DETECTION_H__
