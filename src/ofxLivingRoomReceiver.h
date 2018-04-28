#pragma once

#include "ofxBonjour.h"

class ofxLivingRoomReceiver : public ofxBonjourBrowserFoundNotificationReceiverInterface {
    virtual void foundService(const string &type, const string &name, const string &ip, const string &domain);
    
public:
    int _port;
    std::string _type;
    std::string _name;
    std::string _host;
    
    bool found();
    void setup();
};
