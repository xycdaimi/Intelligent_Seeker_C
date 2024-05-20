#include "log_system/LoggingSystem.h"
#include "nlohmann/JSON.hpp"
#include <opencv2/opencv.hpp>
#include "yolo/yolo.hpp"

int main(int argc, char **argv)
{
    YOLO::Data config;
    YOLO::data_r("./yolov5.json", config);
    struct YOLO::Net5_config yolov5_nets = config.yolov5_Net;
    YOLO::Recognizerv5 yolov5_model(yolov5_nets);

    cv::VideoCapture camera(0);
    if (!camera.isOpened())
    {
        std::cerr << "Error opening video file\n";
        return -1;
    }

    YOLO::Result result;
    cv::Mat image;
    while (true)
    {
        camera >> image;

        YOLO::Result_clear(result);
        yolov5_model.IdentifyTarget(image, result);

        yolov5_model.drawRect(image, result);
        YOLO::result_write("./result.json", result);
        cv::imshow("test", image);

        if (cv::waitKey(1) > 0)
        {
            std::cout << "finished by user\n";
            break;
        }
    }

    camera.release();
    cv::destroyAllWindows();
    return 0;
}
