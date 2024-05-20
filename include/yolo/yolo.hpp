#pragma once
/**
 * @file yolo.h
 * @author xyc 2332607961@qq.com
 * @brief this program is used for yolo
 * @version 0.1
 * @date 2023-08-01
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <vector>
#include "nlohmann/JSON.hpp"
#include <cstring>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "log_system/LoggingSystem.h"
#include <string>
#include <cstdlib>

namespace YOLO
{
    /**
     * @brief Item Data
     */

    struct Result
    {
        /**
         * @param position Target positioning information
         * @param name name
         */

        std::vector<int> x;
        std::vector<int> y;
        std::vector<int> width;
        std::vector<int> height;
        std::vector<std::string> name;
    };

    void Result_clear(struct Result &p)
    {
        p.name.clear();
        p.x.clear();
        p.y.clear();
        p.height.clear();
        p.width.clear();
    }
    /**
     * @brief Save item positioning information
     */

    struct Detection
    {
        /**
         * @param class_id id
         * @param confidence Confidence level
         * @param box rectange information
         */
        int class_id;
        float confidence;
        cv::Rect box;
    };

    /**
     * @brief for yolov5 to use file
     */

    struct Net5_config
    {
        /**
         *@param CONFIDENCE_THRESHOLD Confidence threshold
         *@param NMS_THRESHOLD Non-maximum suppression threshold
         *@param SCORE_THRESHOLD score
         *@param INPUT_WIDTH Width of network's input image
         *@param INPUT_HEIGHT Height of network's input image
         *@param classesFile name file
         *@param modelWeights weights
         *@param netname name
         *@param id id number
         */
        std::vector<float> CONFIDENCE_THRESHOLD; // Confidence threshold
        std::vector<float> NMS_THRESHOLD;        // Non-maximum suppression threshold
        std::vector<float> SCORE_THRESHOLD;
        std::vector<int> INPUT_WIDTH;  // Width of network's input image
        std::vector<int> INPUT_HEIGHT; // Height of network's input image
        std::vector<std::string> classesFile;
        std::vector<std::string> modelWeights;
        std::vector<std::string> netname;
        int id = 0;
    };

    /**
     * @brief yolov5
     */

    class Recognizerv5
    {
    public:
        /**
         * @brief this function is used to create yolov5
         *
         * @tparam Function   must be of type bool fun(nlohmann::json&, Data&)
         * @tparam Data       structure types that require serialization
         * @param path        path to the json file to be saved
         * @param Fun         pass in a function pointer to convert the structure type to be serialized into nlohmann::json
         * @param config      data to be serialized
         * @return true       function execution successful
         * @return false      function execution failed
         */

        Recognizerv5(Net5_config config);

        /**
         * @brief for image to rect
         */

        bool IdentifyTarget(cv::Mat &frame, Result &output);

        /**
         * @brief draw rectange
         */

        bool drawRect(cv::Mat &frame, Result &output);

    private:
        /**
         *@param confThreshold Confidence threshold
         *@param nmsThreshold Non-maximum suppression threshold
         *@param SCORE_THRESHOLD score
         *@param inpWidth width
         *@param inpHeight height
         *@param netname name
         *@param classes coco
         *@param net
         */

        float confThreshold;
        float nmsThreshold;
        float SCORE_THRESHOLD;
        int inpWidth;
        int inpHeight;
        char netname[20];
        std::vector<std::string> classes;
        cv::dnn::Net net;
    };

    /**
     * @brief this function is used to yolo copy
     *
     * @tparam Function   must be of type bool fun(nlohmann::json&, Data&)
     * @tparam Data       structure types that require serialization
     * @param path        path to the json file to be saved
     * @param Fun         pass in a function pointer to convert the structure type to be serialized into nlohmann::json
     * @param config      data to be serialized
     * @return true       function execution successful
     * @return false      function execution failed
     */

    cv::Mat format_yolov5(const cv::Mat &source)
    {
        int col = source.cols;
        int row = source.rows;
        int _max = MAX(col, row);
        cv::Mat result = cv::Mat::zeros(_max, _max, CV_8UC3);
        source.copyTo(result(cv::Rect(0, 0, col, row)));
        return result;
    }

    /**
     * @brief         序列化函数 for Net5_config
     *
     * @param key     json实例对象
     * @param lock    待序列化数据
     * @return true   序列化成功
     * @return false  序列化失败
     */

    bool write_5(nlohmann::json &key, struct Net5_config &lock)
    {
        try
        {
            key["CONFIDENCE_THRESHOLD"] = lock.CONFIDENCE_THRESHOLD;
            key["NMS_THRESHOLD"] = lock.NMS_THRESHOLD;
            key["INPUT_WIDTH"] = lock.INPUT_WIDTH;
            key["INPUT_HEIGHT"] = lock.INPUT_HEIGHT;
            key["classesFile"] = lock.classesFile;
            key["SCORE_THRESHOLD"] = lock.SCORE_THRESHOLD;
            key["modelWeights"] = lock.modelWeights;
            key["netname"] = lock.netname;
        }
        catch (const std::exception &e)
        {
            Log_ERROR << e.what() << Log_END;
            return false;
        }

        return true;
    }

    /**
     * @brief    write for yolov5
     *
     */

    bool yolov5_write(std::string Configure, Net5_config yolov5_nets)
    {
        return JSON::Write_Json(Configure, write_5, yolov5_nets);
    }

    /**
     * @brief         反序列化函数 for Net5_config
     *
     * @param key     json实例对象
     * @param lock    待获取的数据
     * @return true   反序列化成功
     * @return false  反序列化失败
     */

    bool read_5(nlohmann::json &key, struct Net5_config &lock)
    {
        try
        {
            key.at("CONFIDENCE_THRESHOLD").get_to(lock.CONFIDENCE_THRESHOLD);
            key.at("NMS_THRESHOLD").get_to(lock.NMS_THRESHOLD);
            key.at("INPUT_WIDTH").get_to(lock.INPUT_WIDTH);
            key.at("INPUT_HEIGHT").get_to(lock.INPUT_HEIGHT);
            key.at("classesFile").get_to(lock.classesFile);
            key.at("SCORE_THRESHOLD").get_to(lock.SCORE_THRESHOLD);
            key.at("modelWeights").get_to(lock.modelWeights);
            key.at("netname").get_to(lock.netname);
        }
        catch (const std::exception &e)
        {
            Log_ERROR << e.what() << Log_END;
            return false;
        }

        return true;
    }

    /**
     * @brief net yolov5
     *
     *
     */

    bool yolov5_read(std::string Configure, Net5_config &yolov5_nets)
    {
        return JSON::Read_Json(Configure, read_5, yolov5_nets);
    }

    /**
     * @brief         序列化函数 for Result
     *
     * @param key     json实例对象
     * @param lock    待序列化数据
     * @return true   序列化成功
     * @return false  序列化失败
     */

    bool write_r(nlohmann::json &key, struct Result &lock)
    {
        try
        {
            key["x"] = lock.x;
            key["y"] = lock.y;
            key["width"] = lock.width;
            key["height"] = lock.height;
            key["name"] = lock.name;
        }
        catch (const std::exception &e)
        {
            Log_ERROR << e.what() << Log_END;
            return false;
        }

        return true;
    }

    /**
     * @brief    writer for Result
     *
     */

    bool result_write(std::string Configure, struct Result result)
    {

        return JSON::Write_Json(Configure, write_r, result);
    }

    /**
     * @brief         反序列化函数 for Result
     *
     * @param key     json实例对象
     * @param lock    待获取的数据
     * @return true   反序列化成功
     * @return false  反序列化失败
     */

    bool read_r(nlohmann::json &key, struct Result &lock)
    {
        try
        {
            key.at("name").get_to(lock.name);
            key.at("x").get_to(lock.x);
            key.at("y").get_to(lock.y);
            key.at("width").get_to(lock.width);
            key.at("height").get_to(lock.height);
        }
        catch (const std::exception &e)
        {
            Log_ERROR << e.what() << Log_END;
            return false;
        }

        return true;
    }

    /**
     * @brief net result
     *
     *
     */

    bool result_read(std::string Configure, Result &rtr)
    {
        return JSON::Read_Json(Configure, read_r, rtr);
    }

    /**
     *  @brief 待序列化的结构体类型
     *
     */

    struct Data
    {
        Net5_config yolov5_Net;
        Result result;
    };

    /**
     * @brief      write for data
     *
     *
     */

    bool data_w(std::string Configure, struct Data &data)
    {
        yolov5_write(Configure, data.yolov5_Net);
        result_write(Configure, data.result);
        return true;
    }

    /**
     * @brief      write for data
     *
     *
     */

    bool data_w(std::string Configure1, std::string Configure2, struct Data &data)
    {
        yolov5_write(Configure1, data.yolov5_Net);
        result_write(Configure2, data.result);
        return true;
    }

    /**
     * @brief    read for Data
     *
     *
     */

    bool data_r(std::string Configure, struct Data &data)
    {
        yolov5_read(Configure, data.yolov5_Net);
        result_read(Configure, data.result);
        return true;
    }

    /**
     * @brief    read for Data
     *
     *
     */

    bool data_r(std::string Configure1, std::string Configure2, struct Data &data)
    {
        yolov5_read(Configure1, data.yolov5_Net);
        result_read(Configure2, data.result);
        return true;
    }

}

/**
 * @brief this function is used to create yolov5
 *
 * @tparam Function   must be of type bool fun(nlohmann::json&, Data&)
 * @tparam Data       structure types that require serialization
 * @param path        path to the json file to be saved
 * @param Fun         pass in a function pointer to convert the structure type to be serialized into nlohmann::json
 * @param config      data to be serialized
 * @return true       function execution successful
 * @return false      function execution failed
 */

YOLO::Recognizerv5::Recognizerv5(Net5_config config)
{
    std::cout << "Net use " << config.netname[config.id] << std::endl;
    this->confThreshold = config.CONFIDENCE_THRESHOLD[config.id];
    this->nmsThreshold = config.NMS_THRESHOLD[config.id];
    this->inpWidth = config.INPUT_WIDTH[config.id];
    this->inpHeight = config.INPUT_HEIGHT[config.id];
    this->SCORE_THRESHOLD = config.SCORE_THRESHOLD[config.id];
    strcpy(this->netname, config.netname[config.id].c_str());

    std::ifstream ifs(config.classesFile[config.id].c_str());
    std::string line;
    while (getline(ifs, line))
        this->classes.push_back(line);
    auto result = cv::dnn::readNet(config.modelWeights[config.id]);
    std::cout << "Running on CPU\n";
    result.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    result.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    this->net = result;
}

/**
 * @brief for image to rect
 */

bool YOLO::Recognizerv5::IdentifyTarget(cv::Mat &frame, Result &output)
{
    std::vector<YOLO::Detection> p;
    cv::Mat blob;
    auto input_image = YOLO::format_yolov5(frame);

    cv::dnn::blobFromImage(input_image, blob, 1. / 255., cv::Size(this->inpWidth, this->inpHeight), cv::Scalar(), true, false);
    this->net.setInput(blob);
    std::vector<cv::Mat> outputs;
    this->net.forward(outputs, net.getUnconnectedOutLayersNames());

    float x_factor = input_image.cols / this->inpWidth;
    float y_factor = input_image.rows / this->inpHeight;

    float *data = (float *)outputs[0].data;

    const int dimensions = 85;
    const int rows = 25200;

    std::vector<int> class_ids;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    for (int i = 0; i < rows; ++i)
    {

        float confidence = data[4];
        if (confidence >= this->confThreshold)
        {

            float *classes_scores = data + 5;
            cv::Mat scores(1, this->classes.size(), CV_32FC1, classes_scores);
            cv::Point class_id;
            double max_class_score;
            minMaxLoc(scores, 0, &max_class_score, 0, &class_id);
            if (max_class_score > SCORE_THRESHOLD)
            {

                confidences.push_back(confidence);

                class_ids.push_back(class_id.x);

                float x = data[0];
                float y = data[1];
                float w = data[2];
                float h = data[3];
                int left = int((x - 0.5 * w) * x_factor);
                int top = int((y - 0.5 * h) * y_factor);
                int width = int(w * x_factor);
                int height = int(h * y_factor);
                boxes.push_back(cv::Rect(left, top, width, height));
            }
        }

        data += 85;
    }

    std::vector<int> nms_result;
    cv::dnn::NMSBoxes(boxes, confidences, SCORE_THRESHOLD, this->nmsThreshold, nms_result);
    for (int i = 0; i < nms_result.size(); i++)
    {
        int idx = nms_result[i];
        Detection result;
        result.class_id = class_ids[idx];
        result.confidence = confidences[idx];
        result.box = boxes[idx];
        p.push_back(result);
    }

    int detections = p.size();

    for (int i = 0; i < detections; ++i)
    {

        auto detection = p[i];
        auto box = detection.box;
        auto classId = detection.class_id;

        output.name.push_back(this->classes[classId]);
        output.x.push_back(box.x);
        output.y.push_back(box.y);
        output.width.push_back(box.width);
        output.height.push_back(box.height);
    }

    return true;
}

/**
 * @brief draw rectange
 */

bool YOLO::Recognizerv5::drawRect(cv::Mat &frame, Result &output)
{
    for (int i = 0; i < output.name.size(); i++)
    {
        cv::Rect box(output.x[i], output.y[i], output.width[i], output.height[i]);
        cv::rectangle(frame, box, cv::Scalar(0, 255, 0), 3);
        cv::rectangle(frame, cv::Point(box.x, box.y - 20), cv::Point(box.x + box.width, box.y), cv::Scalar(0, 255, 0), cv::FILLED);
        cv::putText(frame, output.name[i].c_str(), cv::Point(box.x, box.y - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
    }
    return true;
}
