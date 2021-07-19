#pragma once

#include "ofMain.h"

#include "ofxOsc.h"
#include "ofxSyphon.h"
#include "ofxCv.h"
#include "ofxFFTouch.h"
#include "ofxFFSilhouette.h"
#include "ofxGui.h"
#include "ofxTskokmtTranslator.h"
#include "ofxTskokmtTool.h"
#include "ofxtskokmtMath.h"

class ofxFFApp {
    
public:
    void setup(string name_, int port_);
    void update();
    void begin(bool bClear_ = true);
    void end();
    void draw();
    void exit();
    
    float getBaseSize();
    
    float getTimeAngle();
    
    //name
    string name;
    
    //time
    float time;
    float previousTime;
    
    //frame
    ofRectangle frame;
    
    //bActive
    bool bActive = true;
    
    //fbo
    ofFbo fbo;
    
    //osc
    ofxOscSender sender;
    bool bConnected = false;
    ofxOscReceiver receiver;
    ofxOscMessage message;
    float signalTerm = .2;
    float lastSignalRecivedTime = 0;
    
    //syphon
    ofxSyphonServer server;
    
    //object
    vector<ofPoint> objects;
    vector<ofPoint> previousObject;
    
    //sound
    float volume;
    float previousVolume;
    
    //xml
    ofxXmlSettings xml;
    
    //translator
    ofxTskokmtTranslator translator;
    
    //tool
    ofxTskokmtTool tool;
    
    //math
    ofxTskokmtMath math;
};
