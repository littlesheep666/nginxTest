/*
 * Copyright (c) 2013-2021  Bernd Porr <mail@berndporr.me.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 */

#include <string.h>
#include <unistd.h>
#include <math.h>
#include "CppTimer.h"
#include <opencv2/opencv.hpp>
#include "json_fastcgi_web_api.h"
//#include "showCVImage.h"
#include "base64.h"

/**
 * Flag to indicate that we are running.
 * Needed later to quit the idle loop.
 **/
int mainRunning = 1;

/**
 * Handler when the user has pressed ctrl-C
 * send HUP via the kill command.
 **/
//void sigHandler(int sig) {
//    if((sig == SIGHUP) || (sig == SIGINT)) {
//        mainRunning = 0;
//    }
//}


/**
 * Sets a signal handler so that you can kill
 * the background process gracefully with:
 * kill -HUP <PID>
 **/
//void setHUPHandler() {
//    struct sigaction act;
//    memset (&act, 0, sizeof (act));
//    act.sa_handler = sigHandler;
//    if (sigaction (SIGHUP, &act, NULL) < 0) {
//        perror ("sigaction");
//        exit (-1);
//    }
//    if (sigaction (SIGINT, &act, NULL) < 0) {
//        perror ("sigaction");
//        exit (-1);
//    }
//}

/**
 * Callback handler which returns data to the
 * nginx server. Here, simply the current temperature
 * and the timestamp is transmitted to nginx and the
 * javascript application.
 **/
class JSONCGIADCCallback : public JSONCGIHandler::GETCallback {
private:
    /**
     * Pointer to the ADC event handler because it keeps
     * the data in this case. In a proper application
     * that would be probably a database class or a
     * controller keeping it all together.
     **/
    cv::Mat cvCgiImage;

public:
    /**
     * Constructor: argument is the cvImage callback handler
     * which keeps the data as a simple example.
     **/
    JSONCGIADCCallback(cv::Mat argImage) {
        cvCgiImage = argImage;
    }

    /**
     * Gets the data sends it to the webserver.
     * The callback creates two json entries.
     **/
    virtual std::string getJSONString() {
        JSONCGIHandler::JSONGenerator jsonGenerator;
        std::vector<unsigned char> data_encode;
        cv::Mat image = cvCgiImage;
        int res = imencode(".jpg", image, data_encode);
        std::string str_encode(data_encode.begin(), data_encode.end());
        const char* c = str_encode.c_str();
        jsonGenerator.add("mat",base64_encode(c, str_encode.size()));
        return jsonGenerator.getJSON();
    }
};


/**
 * Callback handler which receives the JSON from jQuery
 **/
class CVPOSTCallback : public JSONCGIHandler::POSTCallback {
public:
    CVPOSTCallback() {}

    /**
     * receives the JSON from jQuery.
     **/
    virtual void postString(std::string postArg) {

    }

};


// Main program
int main(int argc, char *argv[]) {
    cv::Mat CVImage = cv::imread("test1_result.jpg" );

    // Setting up the JSONCGI communication

    // The callback which is called when fastCGI needs data
    // gets a pointer to the cvImage callback class.
    JSONCGIADCCallback fastCGIADCCallback(CVImage);

    CVPOSTCallback postCallback;

    // starting the fastCGI handler with the callback and the
    // socket for nginx.
    JSONCGIHandler* fastCGIHandler = new JSONCGIHandler(&fastCGIADCCallback,
                                                        &postCallback,
                                                        "/tmp/sensorsocket");

    // catching Ctrl-C or kill -HUP so that we can terminate properly
//    setHUPHandler();

    fprintf(stderr,"'%s' up and running.\n",argv[0]);

    // Just do nothing here and sleep. It's all dealt with in threads!
    // At this point for example a GUI could be started such as QT
    // Here, we just wait till the user presses ctrl-c which then
    // sets mainRunning to zero.
    while (mainRunning) sleep(1);

    fprintf(stderr,"'%s' shutting down.\n",argv[0]);

    // stops the fast CGI handlder
    delete fastCGIHandler;

    return 0;
}