//
// Created by LittleSheep on 17/04/2022.
//

#include "cvImage.h"

cv::Mat cvImage::showImage(cv::Mat image) {
    std::vector<unsigned char> data_encode;
    int res = imencode(".jpg", image, data_encode);
    std::string str_encode(data_encode.begin(), data_encode.end());
    const char* c = str_encode.c_str();
    std::string cvImage = base64_encode(c, str_encode.size());
    return cvImage;
}