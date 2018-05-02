#include "ofxLivingRoom.h"

void ofxLivingRoom::setup() {
    _name = "";
    _type = "";
    _host = "";
    _port = 0;

    browser.setup();
    browser.setFoundNotificationReceiver(this);
    _receiver.setup(1234);
    browser.startBrowse("_livingroom._osc._udp");
}

void ofxLivingRoom::foundService(const std::string &type, const std::string &name, const std::string &ip, const std::string &domain, const int port) {
    _name = name;
    _type = type;
    _host = ip;
    _port = port;
    _sender.setup(_host, _port);
    ofLog() << "Found Device: " << type << ", " << name << "@" << ip << " in " << domain;
}

bool ofxLivingRoom::found() {
    return !(_name.empty() || _type.empty() || _port == 0);
}

std::string ofxLivingRoom::getLatest() {
    return _name + "://" + _host + ":" + ofToString(_port) + " (" + _type + ")";
}

void ofxLivingRoom::getFacts(const string &fact) {
    if (!found()) return;
    ofxOscMessage m;
    m.setAddress("/facts");
    _sender.sendMessage(m, false);
}

void ofxLivingRoom::assertFact(const string &fact) {
    if (!found()) return;
    ofxOscMessage m;
    m.setAddress("/assert");
    m.addStringArg(fact);
    _sender.sendMessage(m, false);
}

void ofxLivingRoom::draw() {
    
}
