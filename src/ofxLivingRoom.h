#pragma once
#include "ofMain.h"
#include "ofxOsc.h"

class ofxLivingRoom {
public:
    void setup();
    void update();
    void draw();
    
    std::string getLatest();
    
    void assertFact(const string &fact);
    void retractFact(const string &fact);
    void getFacts(const string &fact);
    bool found();
    
private:
    int _port;
    std::string _type;
    std::string _name;
    std::string _host; // _ip
    ofxOscSender _sender;
    ofxOscReceiver _receiver;
};
