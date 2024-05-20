#include "log_system/LoggingSystem.h"
#include "yolo/yolo.hpp"
#include "ImageMatcher/ImageMatcher.hpp"
#include <opencv2/opencv.hpp>

int main(int argc, char const *argv[])
{
    //*======================   INIT SYSTEM   ======================
    LoggingSystem::Initialize(LoggingSystem::Grade::INFO);

    cv::VideoCapture camera("./test.mp4");
    if (camera.isOpened() == false)
    {
        Log_ERROR << "./test.mp4 couldn't open" << Log_END;
        return -1;
    }

    struct YOLO::Net5_config yolov5_net;
    YOLO::yolov5_read("./config/yolov5.json", yolov5_net);
    YOLO::Recognizerv5 recognizer(yolov5_net);
    //*=============================================================

    //*====================   SET FIRST FRAME   ====================
    cv::Mat first = cv::imread("./first.jpg");

    cv::Rect first_rect;
    first_rect.x = 416;
    first_rect.y = 557;
    first_rect.width = 619;
    first_rect.height = 145;

    cv::Mat first_image = first(first_rect);
    //*=============================================================

    //*====================   SYSTEM MAIN LOOP   ===================
    YOLO::Result yolo_result;
    YOLO::Result result;
    while (true)
    {
        cv::Mat image;
        camera >> image;
        if (image.empty() == true)
        {
            break;
        }

        YOLO::Result_clear(yolo_result);
        YOLO::Result_clear(result);
        recognizer.IdentifyTarget(image, yolo_result);

        for (int i = 0; i < yolo_result.name.size(); i++)
        {
            if (yolo_result.name[i] == "keyboard")
            {
                result.name.emplace_back(yolo_result.name[i]);
                result.height.emplace_back(yolo_result.height[i]);
                result.width.emplace_back(yolo_result.width[i]);
                result.x.emplace_back(yolo_result.x[i]);
                result.y.emplace_back(yolo_result.y[i]);
            }
        }

        double key_index = -1;
        double similarity_max = -100;
        double similarity_process;
        double similarity_image;
        double similarity_rect;
        cv::Rect process_rect;

        for (int i = 0; i < result.name.size(); i++)
        {
            process_rect.x = result.x[i];
            process_rect.y = result.y[i];
            process_rect.width = result.width[i];
            process_rect.height = result.height[i];
            cv::Mat process_image = image(process_rect);

            similarity_image = ImageMatcher::compare(process_image, first_image) / 100;
            similarity_rect = ImageMatcher::ObtainDistanceParameters(first_rect,process_rect,image.cols,image.rows) / 100;
            similarity_process = (similarity_image * 0.4) + ((1 - similarity_rect) * 0.7);

            if (similarity_process > similarity_max)
            {
                similarity_max = similarity_process;
                key_index = i;
            }
        }

        recognizer.drawRect(image, yolo_result);

        process_rect.x = result.x[key_index];
        process_rect.y = result.y[key_index];
        process_rect.width = result.width[key_index];
        process_rect.height = result.height[key_index];
        cv::rectangle(image, process_rect, cv::Scalar(0, 0, 255), 2, 8, 0);

        cv::imshow("test", image);

        first_image = image(process_rect);
        first_rect = process_rect;
        Log_INFO << "sun = " << similarity_process << Log_END;

        if (cv::waitKey(1) > 0)
        {
            break;
        }
    }
    //*=============================================================

    return 0;
}