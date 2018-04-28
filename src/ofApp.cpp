#include "ofApp.h"

void ofApp::setup() {
    
    ofSetFrameRate(30);
    
    ofBackground(100,100,100);
    
    std::string env_host = ofGetEnv("LIVING_ROOM_HOST");
    std::string env_port = ofGetEnv("LIVING_ROOM_PORT");
    
    largest = width * height / 8;
    smallest = width * height / 16;
    range = 10;
    
    if (env_host == "") env_host = DEFAULT_HOST;
    if (env_port == "") env_port = DEFAULT_PORT;
    
    roomdb_host = env_host;
    roomdb_port = ofToInt(env_port);
    
    sender.setup(roomdb_host, roomdb_port);
    
    ofLog() << "roomdb_host is " << roomdb_host << std::endl;
    ofLog() << "roomdb_port is " << roomdb_port << std::endl;
    
    width = 320;
    height = 240;
    movie.listDevices();
    //movie.setDeviceID(1);
    
    movie.setup(width, height, true);
    
    //reserve memory for cv images
    rgb.allocate(width, height);
    fbo.allocate(width, height, GL_LUMINANCE);
    hsb.allocate(width, height);
    hue.allocate(width, height);
    sat.allocate(width, height);
    bri.allocate(width, height);
    filtered.allocate(width, height);
    
    _name = "";
    _type = "";
    _host = "";
    _port = 0;
    
    browser.setup();
    browser.setFoundNotificationReceiver(this);
    browser.startBrowse("_osc._tcp,livingroom");
}

template<typename ... Args>
string string_format( const std::string& format, Args ... args )
{
    size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    unique_ptr<char[]> buf( new char[ size ] );
    snprintf( buf.get(), size, format.c_str(), args ... );
    return string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

void ofApp::foundService(const string &type, const string &name, const string &ip, const string &domain, const int port) {
    _name = name;
    _type = type;
    _host = ip;
    _port = 1234;
    ofLog() << "Found Device: " << type << ", " << name << "@" << ip << " in " << domain;
}

bool ofApp::found() {
    return !(_name.empty() || _type.empty() || _port == 0);
}


void ofApp::update(){
    movie.update();
    
    if (movie.isFrameNew()) {
        
        //copy webcam pixels to rgb image
        rgb.setFromPixels(movie.getPixels());
        
        if (mask.size() == 4) {
            for (int i = 0; i < width * height; i++) {
                int x = i % width;
                int y = i / width;
                if (!mask.inside(x, y)) {
                    rgb.getPixels()[3*i] = 0;
                    rgb.getPixels()[3*i+1] = 0;
                    rgb.getPixels()[3*i+2] = 0;
                }
            }
        }
        
        
        hsb = rgb; // copy rgb
        filtered.erode();
        
        hsb.convertRgbToHsv();
        
        //store the three channels as grayscale images
        hsb.convertToGrayscalePlanarImages(hue, sat, bri);
        
        //filter image based on the hue value were looking for
        for (int i=0; i<width*height; i++) {
            //filtered.getPixels()[i] = ofInRange(hue.getPixels()[i],findHue-range,findHue+range) ? 255 : 0;
            filtered.getPixels()[i] = sat.getPixels()[i] > 100 ? 255 : 0;
        }
        
        filtered.dilate();
        filtered.dilate();
        
        filtered.flagImageChanged();
        //run the contour finder on the filtered image to find blobs with a certain hue
        contours.findContours(filtered, smallest, largest, 10, false);
        
        sendContours();
    }
}

void ofApp::sendContours() {
    for (int i = 0; i < contours.nBlobs; i++) {
        auto blob = contours.blobs[i];
        auto x = blob.centroid.x / width;
        auto y = blob.centroid.y / height;
        
        ofxOscMessage m;
        m.setAddress("/assert");
        std::string facts = string_format("there is a color at (%03.2f, %03.2f)", x, y);
        
        ofLog() << facts;
        
        m.addStringArg(facts);
        sender.sendMessage(m, false);
    }
}

void ofApp::draw(){
    ofSetColor(255,255,255);
    
    //draw all cv images
    rgb.draw(0,0);
    hsb.draw(640,0);
    hue.draw(0,240);
    sat.draw(320,240);
    bri.draw(640,240);
    filtered.draw(0,480);
    contours.draw(0,480);
    mask.draw();
    
    ofSetColor(255, 0, 0);
    ofFill();
    
    //draw red circles for found blobs
    for (int i=0; i<contours.nBlobs; i++) {
        ofDrawCircle(contours.blobs[i].centroid.x, contours.blobs[i].centroid.y, 20);
    }
    
    std::string debug = string_format("%d -> %d, %d", smallest, largest, findHue);
    ofDrawBitmapStringHighlight(debug, width * 0.8, height * 0.8);
    if (found()) {
        //std::string mdns = string_format("%s :// %s : %d", type, host, port);
        ofDrawBitmapStringHighlight(_host, width * 0.8, height * 0.9);
        ofDrawBitmapStringHighlight(ofToString(_port), width * 0.9, height * 0.9);
    }
}

void ofApp::mousePressed (int x, int y, int button) {
    int mx = x % width;
    int my = y % height;
    if (button == 0) {
        //get hue value on mouse position
        findHue = hue.getPixels()[my*width+mx];
    } else if (button == 1) {
        ofPoint pt;
        pt.set(x,y);
        mask.insertVertex(pt, 0);
        
        if (mask.size() > 4) {
            mask.resize(4);
            mask.close();
        }
    }
    
}

void ofApp::mouseDragged (int x, int y, int button) {
    int mx = x % width;
    int my = y % height;
    
    if (button == 1) {
        range = 100 * mx / width;
    } else if (button == 2) {
        smallest = mx;
        largest = my;
    }
    
    std::string debug = string_format("%d -> %d, %d", smallest, largest, findHue);
    ofLog() << debug << std::endl;
}
