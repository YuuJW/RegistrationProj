#include "detection.h"
#include <fstream>
#include <iostream>

using namespace cv;
using namespace cv::dnn;
using namespace std;

Detection::Detection()
{
    // 默认参数
    m_confThreshold = 0.5f;
    m_nmsThreshold  = 0.4f;
}

Detection::~Detection()
{
    // 析构函数，如有需要可以在这里做一些内存清理
}

bool Detection::ReadModel(const std::string& modelConfig,
                          const std::string& modelWeights,
                          const std::string& namesFile)
{
    // 读取类别名称
    m_classNames.clear();
    ifstream ifs(namesFile.c_str());
    if (!ifs.is_open()) {
        cerr << "Failed to open names file: " << namesFile << endl;
        return false;
    }
    string line;
    while (getline(ifs, line)) {
        m_classNames.push_back(line);
    }
    ifs.close();

    // 加载网络
    m_net = readNetFromDarknet(modelConfig, modelWeights);
    if (m_net.empty()) {
        cerr << "Failed to load network. Check cfg/weights path." << endl;
        return false;
    }

    // 设置为 CPU 或 CUDA
    // CPU:
    //m_net.setPreferableBackend(DNN_BACKEND_OPENCV);
    //m_net.setPreferableTarget(DNN_TARGET_CPU);

    // CUDA:
    //m_net.setPreferableBackend(DNN_BACKEND_CUDA);
    //m_net.setPreferableTarget(DNN_TARGET_CUDA);

    cout << "Model loaded successfully!" << endl;
    return true;
}

void Detection::SetParams(float confThreshold, float nmsThreshold)
{
    m_confThreshold = confThreshold;
    m_nmsThreshold  = nmsThreshold;
}

bool Detection::Detecting(const Mat& frame)
{
    if (frame.empty()) {
        cerr << "Input frame is empty!" << endl;
        return false;
    }

    // 1) 创建4D的blob，用于网络输入
    //    YOLOv4 的输入大小一般是 416x416 或 608x608 等，看你的 cfg 文件
    Mat blob;
    blobFromImage(frame, blob, 1/255.0, Size(416, 416), Scalar(), true, false);

    // 2) 前向传播
    m_net.setInput(blob);
    vector<Mat> outs;
    m_net.forward(outs, GetOutputsNames()); // 得到输出层

    // 3) 后处理：NMS + 画框
    PostProcess(frame, outs);

    return true;
}

Mat Detection::GetOutputImage() const
{
    return m_resultImage;
}

vector<cv::String> Detection::GetOutputsNames()
{
    static vector<String> names;
    if (names.empty()) {
        // 获取输出层的索引号
        vector<int> outLayers = m_net.getUnconnectedOutLayers();
        // 获取网络中所有层名称
        vector<String> layersNames = m_net.getLayerNames();

        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i) {
            names[i] = layersNames[outLayers[i] - 1];
        }
    }
    return names;
}

void Detection::PostProcess(const Mat& frame, const vector<Mat>& outs)
{
    // 用于保存检测结果
    vector<int> classIds;
    vector<float> confidences;
    vector<Rect> boxes;

    // 将 outs 中的数据解析出来
    for (size_t i = 0; i < outs.size(); i++)
    {
        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; j++, data += outs[i].cols)
        {
            // confidence 是 [5, outs[i].cols) 这部分
            // 前面5个表示 [center_x, center_y, w, h, confidence]
            float score = data[4];
            if (score > m_confThreshold)
            {
                // 找到最大的类别分数
                Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
                Point classIdPoint;
                double maxClassScore;
                minMaxLoc(scores, 0, &maxClassScore, 0, &classIdPoint);

                if (maxClassScore > m_confThreshold)
                {
                    // 获得框的位置和大小
                    int centerX = (int)(data[0] * frame.cols);
                    int centerY = (int)(data[1] * frame.rows);
                    int width   = (int)(data[2] * frame.cols);
                    int height  = (int)(data[3] * frame.rows);
                    int left    = centerX - width / 2;
                    int top     = centerY - height / 2;

                    classIds.push_back(classIdPoint.x);
                    confidences.push_back((float)maxClassScore);
                    boxes.push_back(Rect(left, top, width, height));
                }
            }
        }
    }

    // 执行 Non-maximum suppression
    vector<int> indices;
    NMSBoxes(boxes, confidences, m_confThreshold, m_nmsThreshold, indices);

    // 复制原图，准备在上面画框
    m_resultImage = frame.clone();

    // 画检测结果
    for (int idx : indices)
    {
        Rect box = boxes[idx];
        int classId = classIds[idx];
        float conf  = confidences[idx];

        // 随机定义框的颜色（或固定颜色）
        Scalar color(0, 255, 0);
        rectangle(m_resultImage, box, color, 2);

        // 文本信息
        string label;
        if ((int)classId < (int)m_classNames.size()) {
            label = m_classNames[classId];
        } else {
            label = "unknown";
        }

        label += cv::format(": %.2f", conf);

        int baseLine;
        Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
        int top = max(box.y, labelSize.height);

        // 画文字背景
        rectangle(m_resultImage,
                  Point(box.x, top - labelSize.height),
                  Point(box.x + labelSize.width, top + baseLine),
                  Scalar(255, 255, 255), FILLED);

        // 写文字
        putText(m_resultImage, label, Point(box.x, top),
                FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,0,0), 1);
    }
}
