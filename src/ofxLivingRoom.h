#pragma once
#include "ofMain.h"
#include "ofxBonjour.h"
#include "ofxOsc.h"

class ofxLivingRoom : public ofxBonjourBrowserFoundNotificationReceiverInterface {
public:
    void setup();
    void update();
    void draw();
    
    std::string getLatest();
    
    void assertFact(const string &fact);
    void getFacts(const string &fact);
    
    bool found();
    void foundService(const string &type, const string &name, const string &ip, const string &domain, const int port);
    
private:
    ofxBonjourBrowser browser;
    int _port;
    std::string _type;
    std::string _name;
    std::string _host; // _ip
    ofxOscSender _sender;
    ofxOscReceiver _receiver;
};
