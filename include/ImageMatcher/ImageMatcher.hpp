#pragma once

/**
 * @file ImageMatcher.hpp
 * @author jiyilin 2474803745@qq.com
 * @brief this program is an image matching program
 * @version 0.1
 * @date 2023-08-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "log_system/LoggingSystem.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <thread>
#include <utility>
#include <cmath>

/**
 * @brief image matching calculation using euclidean distance
 *
 */
#define EuclideanDistance

namespace ImageMatcher
{
#ifdef EuclideanDistance
    //! ===============================================================================
    //! ==================   Euclidean Distance Calculation Module   ==================

    /**
     * @brief image matching core calculation function
     *
     * @param image1
     * @param image2
     * @return double
     */
    double calculateImageSimilarity(const cv::Mat &image1, const cv::Mat &image2)
    {
        // Calculate the Euclidean distance between the two images
        double distance = cv::norm(image1, image2, cv::NORM_L2);
        // Calculate the similarity ratio
        return (1.0 - (distance / cv::norm(image1, cv::NORM_L2))) * 100;
    }
    //! ===============================================================================
#endif // EuclideanDistance

    /**
     * @brief numerical calculation function for image matching degree
     *
     * @param image1
     * @param image2
     * @return double
     */
    double compare(cv::Mat &image1, cv::Mat &image2)
    {
        cv::Mat input1 = image1.clone();
        cv::Mat input2 = image2.clone();

        if (input1.type() != CV_8UC1)
        {
            cv::cvtColor(input1, input1, cv::COLOR_BGR2GRAY);
        }
        int width = input1.cols;
        int height = input1.rows;

        if (input2.type() != CV_8UC1)
        {
            cv::cvtColor(input2, input2, cv::COLOR_BGR2GRAY);
        }
        cv::Size size_key = cv::Size(width, height);
        cv::resize(input2, input2, size_key);

        return calculateImageSimilarity(input1, input2);
    }

    std::pair<double, double> GetFrameCenter(const cv::Rect &rect_input)
    {
        std::pair<double, double> key;

        key.first = rect_input.x + (rect_input.width / 2.0);
        key.second = rect_input.y + (rect_input.height / 2.0);

        return key;
    }

    double GetRectDistance(const cv::Rect &rect_input1, const cv::Rect &rect_input2)
    {
        auto rect_input_one = GetFrameCenter(rect_input1);
        auto rect_input_two = GetFrameCenter(rect_input2);

        double width = rect_input_one.first - rect_input_two.first;
        double height = rect_input_one.second - rect_input_two.second;
        return std::sqrt(std::pow(width, 2) + std::pow(height, 2));
    }

    double ObtainDistanceParameters(const cv::Rect &rect_input1, const cv::Rect &rect_input2, double width , double height)
    {
        double max_distance = std::sqrt(std::pow(width, 2) + std::pow(height, 2));
        return (GetRectDistance(rect_input1, rect_input2) / max_distance) * 100;
    }

} // namespace ImageMatcher
