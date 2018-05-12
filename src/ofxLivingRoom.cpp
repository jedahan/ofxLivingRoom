#include "ofxLivingRoom.h"

void ofxLivingRoom::setup() {
    _name = "default";
    _type = "osc";
    _host = "localhost";
    _port = 41234;
    _receiver.setup(1234);
    _sender.setup(_host, _port);
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

void ofxLivingRoom::retractFact(const string &fact) {
    if (!found()) return;
    ofxOscMessage m;
    m.setAddress("/retract");
    m.addStringArg(fact);
    _sender.sendMessage(m, false);
}

void ofxLivingRoom::draw() {
    
}
