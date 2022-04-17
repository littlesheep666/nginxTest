#ifndef __FAKESENSOR_H
#define __FAKESENSOR_H

/**
 * Copyright (c) 2021  Bernd Porr <mail@berndporr.me.uk>
 **/

#include <math.h>
#include "CppTimer.h"
#include <opencv2/opencv.hpp>


/**
 * Callback for new samples which needs to be implemented by the main program.
 * The function hasSample needs to be overloaded in the main program.
 **/
class SensorCallback {
public:
    /**
     * Called after a sample has arrived.
     **/
    virtual void hasSample(cv::Mat sample) = 0;
};


/**
 * This class reads data from a fake sensor in the background
 * and calls a callback function whenever data is available.
 **/
class FakeSensor : public CppTimer {

public:
    FakeSensor() {
    }

    ~FakeSensor() {
        stop();
    }

    /**
     * Sets the callback which is called whenever there is a sample
     **/
    void setCallback(SensorCallback* cb) {
        sensorCallback = cb;
    }

    /**
     * Starts the data acquisition in the background and the
     * callback is called with new samples
     **/
    void startSensor() {
        start(250000000);
    }

    /**
     * Stops the data acquistion
     **/
    void stopSensor() {
        stop();
    }

    /**
     * Fake the arrival of data
     **/
    void timerEvent() {
        float value = sin(t) * 5 + 20;
        t += 0.1;
//        setCVImage(cv::imread("test1_result.jpg" ));
        if (nullptr != sensorCallback) {
            sensorCallback->hasSample(getCVImage());
        }
    }

    cv::Mat getCVImage(){
        return cvimage;
    }

    void setCVImage(cv::Mat image){
        cvimage = image;
    }

private:
    SensorCallback* sensorCallback = nullptr;
    float t = 0;
    cv::Mat cvimage;
};


#endif
