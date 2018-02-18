#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxSocketIO.h"

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
        void onConnection();
        void bindEvents();
        ofEvent<ofxSocketIOData&> serverEvent;
        ofEvent<ofxSocketIOData&> pingEvent;
        ofEvent<ofxSocketIOData&> nspingEvent;
        std::string address;std::string status;

        std::string roomdb_uri;
};
