#include "ofxLivingRoom.h"

void ofxLivingRoom::setup() {
    _name = "";
    _type = "";
    _host = "";
    _port = 0;

    browser.setup();
    browser.setFoundNotificationReceiver(this);
    browser.startBrowse("_livingroom._sub._osc._udp");
}

void ofxLivingRoom::foundService(const string &type, const string &name, const string &ip, const string &domain, const int port) {
    _name = name;
    _type = type;
    _host = ip;
    _port = port;
    ofLog() << "Found Device: " << type << ", " << name << "@" << ip << " in " << domain;
}

bool ofxLivingRoom::found() {
    return !(_name.empty() || _type.empty() || _port == 0);
}

void ofxLivingRoom::draw() {
    ofSetColor(255, 0, 0);
    ofFill();
    
    //std::string mdns = string_format("%s :// %s : %d", type, host, port);
    ofDrawBitmapStringHighlight(_host, width * 0.2, height * 0.4);
    ofDrawBitmapStringHighlight(_type, width * 0.2, height * 0.5);
    ofDrawBitmapStringHighlight(ofToString(_port), width * 0.2, height * 0.6);
}
