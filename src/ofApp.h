#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxOsc.h"
#include "ofxBonjour.h"

#define DEFAULT_HOST "http://localhost"
#define DEFAULT_PORT "3000"

class ofApp : public ofBaseApp, public ofxBonjourBrowserFoundNotificationReceiverInterface {
    public:
        void setup();
        void update();
        void draw();
        void sendContours();
    
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
    
        int width, height;
        int findHue;
        int range;
        ofVideoGrabber movie;
        ofxCvColorImage rgb,hsb;
        ofxCvGrayscaleImage hue,sat,bri,filtered;
        ofxCvContourFinder contours;
    
        std::string roomdb_host;
        int roomdb_port;
        int largest;
        int smallest;
    
        ofFbo fbo;
        ofPolyline mask;
        ofxOscSender sender;
    
        int _port;
        std::string _type;
        std::string _name;
        std::string _host;
    
        bool found();
        void foundService(const string &type, const string &name, const string &ip, const string &domain);
    
    private:
        ofxBonjourBrowser browser;
};
