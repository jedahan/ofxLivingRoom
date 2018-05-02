#include "ofApp.h"

void ofApp::setup() {
    room.setup();
}

void ofApp::update() {
    
}

void ofApp::draw(){
    ofSetColor(255, 0, 0);
    ofFill();
    
    ofDrawBitmapStringHighlight(room.getLatest(), 10, 100);
}
