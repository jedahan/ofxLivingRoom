#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxSocketIO.h"
#include "ofxSocketIOData.h"

class ofApp : public ofBaseApp {
    public:
        void setup();
        void update();
        void draw();
        void sendContours();

        void mousePressed(int x, int y, int button);

        int width, height;
        int findHue;
        ofVideoGrabber movie;
        ofxCvColorImage rgb,hsb;
        ofxCvGrayscaleImage hue,sat,bri,filtered;
        ofxCvContourFinder contours;

        ofxSocketIO socketIO;
        bool isConnected;
        std::string roomdb_uri;
        std::string status;

        void onConnection();
        void bindEvents();
        void gotEvent(std::string& name);

        ofEvent<ofxSocketIOData&> assertEvent;
        void onAssertEvent(ofxSocketIOData& data);
};
