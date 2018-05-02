#pragma once
#include "ofMain.h"
#include "ofxLivingRoom.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    
private:
    ofxLivingRoom room;
};
