#include "ofApp.h"

void ofApp::setup() {
    ofBackground(100,100,100);

    roomdb_uri = ofGetEnv("ROOMDB_URI");

    if (roomdb_uri == "") {
      roomdb_uri = "http://localhost:3000";
    }

    width = 320;
    height = 240;

    movie.setup(width, height, true);

    //reserve memory for cv images
    rgb.allocate(width, height);
    hsb.allocate(width, height);
    hue.allocate(width, height);
    sat.allocate(width, height);
    bri.allocate(width, height);
    filtered.allocate(width, height);
}

void ofApp::update(){

    movie.update();

    if (movie.isFrameNew()) {

        //copy webcam pixels to rgb image
        rgb.setFromPixels(movie.getPixels());

        //mirror horizontal
        rgb.mirror(false, true);

        hsb = rgb; // copy rgb

        hsb.convertRgbToHsv();

        //store the three channels as grayscale images
        hsb.convertToGrayscalePlanarImages(hue, sat, bri);

        //filter image based on the hue value were looking for
        for (int i=0; i<width*height; i++) {
            filtered.getPixels()[i] = ofInRange(hue.getPixels()[i],findHue-5,findHue+5) ? 255 : 0;
        }

        filtered.flagImageChanged();
        //run the contour finder on the filtered image to find blobs with a certain hue
        contours.findContours(filtered, 50, width*height/2, 5, false);

        sendContours();
    }
}

void ofApp::sendContours() {
    ofHttpRequest request;
    request.headers["Accept"] = "application/json";
    request.contentType = "application/json";
    request.url = roomdb_uri + "/assert";
    request.method = ofHttpRequest::POST;

    for (int i = 0; i < contours.nBlobs; i++) {
        auto blob = contours.blobs[i];
        auto x = blob.centroid.x / width;
        auto y = blob.centroid.y / height;
        char fact [128];
        auto length = sprintf(fact, "#label%d is a label at (%03.2f, %03.2f)", i, x, y);
        //auto fact = "label" + ofToString(i) + " is a label at (" + ofToString(x) + ", " + ofToString(y) + ")";
        auto body = "{ \"fact\": \"" + ofToString(fact) + "\" }";

        request.body = body;
        /*
        auto boundingRect = contours.blobs[i].boundingRect;
        auto x = ofToString(boundingRect.x / width);
        auto y = ofToString(boundingRect.y / height);
        auto xx = ofToString((boundingRect.x + boundingRect.width) / width);
        auto yy = ofToString((boundingRect.y + boundingRect.height) / height);

        request.body = "{ " +
          json_var_value("label", "hand" + ofToString(i) + " is a boundingbox at (" +
          x + ", " +
          y + ", " +
          xx + ", " +
          yy + ")") +
        "}";
        */

        auto response = http.handleRequest(request);

        cout << request.body << std::endl;
        if (response.status != 200) {
          cout << ofToString(response.data) << std::endl;
        }
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

    ofSetColor(255, 0, 0);
    ofFill();

    //draw red circles for found blobs
    for (int i=0; i<contours.nBlobs; i++) {
        ofDrawCircle(contours.blobs[i].centroid.x, contours.blobs[i].centroid.y, 20);
    }
}

void ofApp::mousePressed(int x, int y, int button) {
    //calculate local mouse x,y in image
    int mx = x % width;
    int my = y % height;

    //get hue value on mouse position
    findHue = hue.getPixels()[my*width+mx];
}
