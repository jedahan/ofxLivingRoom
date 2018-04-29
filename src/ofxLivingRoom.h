#pragma once
#include "ofxBonjour.h"

class ofxLivingRoom : public ofxBonjourBrowserFoundNotificationReceiverInterface {
public:
    void setup();
    void update();
    void draw();
    
    bool found();
    void foundService(const string &type, const string &name, const string &ip, const string &domain, const int port);
    
private:
    ofxBonjourBrowser browser;
    int _port;
    std::string _type;
    std::string _name;
    std::string _host;
};
