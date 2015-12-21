#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    testBrush = DropBrush();
    
    for(int i=0; i<numBrushes; i++) {
        float noise=ofNoise(i*100.0);
        /*if(ofRandom(1.0f) > 0.9) {
         noise *= 8.0f;
         };*/
        brushes.push_back(DropBrush(noise*20.0f, 8, 20.0));
    }
    
    myCam = ofEasyCam();
    myCam.setDistance(2000);
    myCam.setupPerspective(true,80,100,50000, ofVec2f(0.0f, 0.0f));
    myCam.setTarget(ofVec3f(0,0,0));
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    std::stringstream strm;
    strm << "fps: " << ofGetFrameRate();
    ofSetWindowTitle(strm.str());
    
    testBrush.update(ofVec3f(ofGetMouseX(),ofGetMouseY(),0));
    
    int bigCircle = 300;
    float smallCircle = 1000;
    float innerRadius = 300;
    float scale = 1.0;
    
    float speed = 0.4;
    
    for(int i=0; i<numBrushes; i++) {
        float bigMoveX = (bigCircle-smallCircle)*sin(speed*ofGetElapsedTimef()*TWO_PI+i*(TWO_PI/float(numBrushes)));
        float bigMoveY = (bigCircle-smallCircle)*cos(speed*ofGetElapsedTimef()*TWO_PI+i*(TWO_PI/float(numBrushes)));
        float bigMoveZ = (bigCircle-smallCircle)*sin(speed*ofGetElapsedTimef()*TWO_PI+i*(TWO_PI/float(numBrushes)));
        
        float smallMoveX = innerRadius*sin(speed*ofGetElapsedTimef()*TWO_PI*-(1.0f/5.0f)*(bigCircle/(smallCircle+0.0001f))+i*(TWO_PI/float(numBrushes)));
        float smallMoveY = innerRadius*cos(speed*ofGetElapsedTimef()*TWO_PI*-(1.0f/5.0f)*(bigCircle/(smallCircle+0.0001f))+i*(TWO_PI/float(numBrushes)));
        float smallMoveZ = innerRadius*cos(speed*ofGetElapsedTimef()*TWO_PI*-(1.0f/5.0f)*(bigCircle/(smallCircle+0.0001f))+i*(TWO_PI/float(numBrushes)));
        
        float combinedX = scale*(bigMoveX + smallMoveX);
        float combinedY = scale*(bigMoveY + smallMoveY);
        float combinedZ = scale*(bigMoveZ + smallMoveZ);
        
        brushes[i].update(ofVec3f(combinedX,combinedY,combinedZ));
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    myCam.begin();
    
    for(int i=0; i<numBrushes; i++) {
        brushes[i].draw();
    }
    
    //testBrush.draw();
    
    myCam.end();
    
}


//--------------------------------------------------------------
bool ofApp::killBrush(DropBrush &brsh) {
    return brsh.beKilled;
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
