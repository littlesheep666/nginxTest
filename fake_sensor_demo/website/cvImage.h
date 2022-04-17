//
// Created by LittleSheep on 17/04/2022.
//

#ifndef JSON_FASTCGI_WEB_API_MAIN_CVIMAGE_H
#define JSON_FASTCGI_WEB_API_MAIN_CVIMAGE_H


class cvImage {
public:
    void showImage(cv::Mat image);

private:
    cv::Mat image = cv::imread("test1_result.jpg" );;

};


#endif //JSON_FASTCGI_WEB_API_MAIN_CVIMAGE_H
