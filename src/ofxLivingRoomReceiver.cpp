#include "ofxLivingRoomReceiver.h"

void ofxLivingRoomReceiver::setup() {
    _name = "";
    _type = "";
    _host = "";
    _port = 0;
}

void ofxLivingRoomReceiver::foundService(const string &type, const string &name, const string &ip, const string &domain) {
    _name = name;
    _type = type;
    _host = ip;
    _port = 1234;
    ofLog() << "Found Device: " << type << ", " << name << "@" << ip << " in " << domain;
}

bool ofxLivingRoomReceiver::found() {
    return !(_name.empty() || _type.empty() || _port == 0);
}
