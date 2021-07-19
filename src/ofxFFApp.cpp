#include "ofxFFApp.h"

//--------------------------------------------------------------
void ofxFFApp::setup(string name_, int port_) {
    
    //setup openFrameworks
    ofSetFrameRate(24);
    ofSetVerticalSync(true);
    ofSetWindowPosition(ofGetScreenWidth() / 18., ofGetScreenHeight() / 18.);
    ofSetWindowShape(ofGetScreenWidth() / 18. * 16, ofGetScreenHeight() / 18. * 16);
    ofBackground(0);
    ofSetCircleResolution(128);
    //ofSetSphereResolution(128);
    
    //setup name
    name = name_;
    
    //setup frame
    xml.load("/Users/tskokmt/Documents/PROJECT/FRAME FREE/DATA/frameData.xml");
    frame = ofRectangle(0, 0, 1280, 800);
    
    //setup fbo
    fbo.allocate(frame.width, frame.height, GL_RGBA, 4);
    
    //setup osc
    receiver.setup(port_);
    sender.setup("localhost", 50100);
    message.clear();
    
    //setup syphon
    server.setName(name + "Server");
    
    //load windowRect
    tool.loadWindowRect();
}

//--------------------------------------------------------------
void ofxFFApp::update() {
    
    //update windowTitle
    ofSetWindowTitle(name + " | " + ofToString(round(ofGetFrameRate())));
    
    //update time
    previousTime = time;
    time = ofGetElapsedTimef();
    
    //update previousObject
    previousObject = objects;
    
    //send connect
    if (!bConnected) {
        //send connect
        message.clear();
        message.setAddress("connect");
        message.addIntArg(receiver.getPort());
        sender.sendMessage(message);
    }
    
    //send signal
    if (fmod(time, signalTerm) < fmod(previousTime, signalTerm)) {
        message.clear();
        message.setAddress("signal");
        message.addIntArg(receiver.getPort());
        sender.sendMessage(message);
    }
    
    //receive
    while (receiver.hasWaitingMessages()) {
        message.clear();
        receiver.getNextMessage(message);
        
        //receive signal
        if (message.getAddress() == "signal") {
            bConnected = true;
            lastSignalRecivedTime = time;
        }
        
        //receive
        while (receiver.hasWaitingMessages()) {
            message.clear();
            receiver.getNextMessage(message);
            
            //receive person
            if (message.getAddress() == "object") {
                objects.clear();
                for (int i = 1; i <= message.getArgAsInt(0) * 2; i += 2) {
                    objects.push_back(ofPoint(message.getArgAsFloat(i), message.getArgAsFloat(i + 1)));
                }
            }
        }
    }
    
    //update bConnected
    if (time - lastSignalRecivedTime > signalTerm * 4) bConnected = false;
    
    if (!bActive) return;
}

//--------------------------------------------------------------
void ofxFFApp::begin(bool bClear_) {
    
    if (!bActive) return;
    
    //begin fbo
    fbo.begin();
    if (bClear_) ofClear(0);
}

//--------------------------------------------------------------
void ofxFFApp::end() {
    
    if (!bActive) return;
    
    //end fbo
    fbo.end();
    
    //publish fbo
    server.publishTexture(&fbo.getTexture());
}

//--------------------------------------------------------------
void ofxFFApp::draw() {
    
    ofPushStyle();
    
    if (bActive) {
        ofPushMatrix();
        
        //smartFit
        translator.reset();
        translator.smartFit(frame, ofGetWindowRect());
        
        //draw fbo
        ofSetColor(255);
        fbo.draw(0, 0);
        
        //draw frame
        ofPushStyle();
        ofNoFill();
        ofSetColor(255);
        ofDrawRectangle(frame);
        ofPopStyle();
        
        ofPopMatrix();
    }
    
    //draw bConnected
    float radius = sqrt(ofGetWindowRect().getArea()) / 90.;
    if (bConnected) ofSetColor(0, 255, 0);
    else ofSetColor(255, 0, 0);
    ofDrawCircle(ofGetWidth() - radius, radius * 2, radius);
    
    ofPopStyle();
}

//--------------------------------------------------------------
void ofxFFApp::exit() {
 
    //disconnect
    message.clear();
    message.setAddress("disconnect");
    message.addIntArg(receiver.getPort());
    sender.sendMessage(message);
    
    //save windowRect
    tool.saveWindowRect();
    
}

//--------------------------------------------------------------
float ofxFFApp::getBaseSize() {
    
    return sqrt(frame.getArea());
}

//--------------------------------------------------------------
float ofxFFApp::getTimeAngle() {
    
    return time * 2 * PI;
}

