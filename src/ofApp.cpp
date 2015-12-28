#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //  ---------------------------------------------------
    //  _  _ _ ___  _    ____ ____ ___ _  _ ___
    //  |\/| | |  \ |    [__  |___  |  |  | |__]
    //  |  | | |__/ |    ___] |___  |  |__| |
    //
    //  ---------------------------------------------------
    //midiIn.listPorts(); //  List and get correct Midi-Port
    midiIn.openPort(0);
    
    // add testApp as a listener
    midiIn.addListener(this);
    
    testBrush = DropBrush();
    
    
    // Mit dieser Formel kann ich meine Brennweite der Kamera oben bequem
    // in Millimetern angeben (Quelle: http://kmp.bdimitrov.de/technology/fov.html)
    
    float focalLength = 12;
    
    float factor = 43.266615300557;
    float fov = -1*(2*atan(factor/(2*focalLength))*180/PI);
    
    myCam = ofEasyCam();
    myCam.setupPerspective(true,fov,100,50000, ofVec2f(0.0f, 0.0f));
    myCam.setTarget(ofVec3f(0,0,0));
    myCam.setPosition(ofVec3f(0,0,0));
    myCam.setDistance(1000);
    
    myEmitter.setup(300, 1000, 300);
    
    baseFBO.allocate( ofGetWidth(), ofGetHeight(), GL_RGB );
    //repeatFBO.allocate( ofGetWidth(), ofGetHeight(), GL_RGB );
    repeatShader.load("repeatShader");
    fisheye.load("fisheye");

    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    std::stringstream strm;
    strm << "fps: " << ofGetFrameRate();
    ofSetWindowTitle(strm.str());
    
    testBrush.update(ofVec3f(ofGetMouseX(),ofGetMouseY(),0));
 
    myEmitter.update();
    
    rShd_Rotation += (rShd_RotationInput-rShd_Rotation)*rShd_Ease;
    rShd_Scale += (rShd_ScaleInput-rShd_Scale)*rShd_Ease;

    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    repeatFBO.begin();
    drawRepeat();
    repeatFBO.end();
    
    if(shaderEnabled) {
        fisheye.begin();
        fisheye.setUniform2f( "iResolution", ofGetWidth(), ofGetHeight()*1.77);
        fisheye.setUniform2f( "iMouse", ofGetMouseX(), ofGetMouseY());
        fisheye.setUniform1f( "chromAbb", ofGetMouseY());
    }
    
    repeatFBO.draw(0,0);
    
    if(shaderEnabled) {
        fisheye.end();
    }
    
    drawMidiUI();
}


//--------------------------------------------------------------
void ofApp::drawRepeat(){
    
    float speed = 0.4;
    
    baseFBO.begin();
    
    ofPushMatrix();
    
    ofBackground(0);
    ofTranslate(ofGetWidth()/2,ofGetHeight()/2,0);
    ofScale(0.2,0.2,0.2);
    myEmitter.draw();
    
    ofPopMatrix();
    
    baseFBO.end();
    
    if(shaderEnabled) {
        repeatShader.begin();
        repeatShader.setUniform1i( "copies", rShd_Copies);
        repeatShader.setUniform1f( "scale", rShd_Scale);
        repeatShader.setUniform1f( "rotation", rShd_Rotation);
        repeatShader.setUniform2f( "screenSize", ofGetWidth(), ofGetHeight());
    }
    
    baseFBO.draw(0,0);
    
    if(shaderEnabled) {
        repeatShader.end();
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 's') {
        shaderEnabled = !shaderEnabled;
    }
    
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


//--------------------------------------------------------------
void ofApp::drawMidiUI(){
    // draw the last recieved message contents to the screen
    text << "Received: " << ofxMidiMessage::getStatusString(midiMessage.status);
    ofDrawBitmapString(text.str(), 20, 20);
    text.str(""); // clear
    
    text << "channel: " << midiMessage.channel;
    ofDrawBitmapString(text.str(), 20, 34);
    text.str(""); // clear
    
    text << "pitch: " << midiMessage.pitch;
    ofDrawBitmapString(text.str(), 20, 48);
    text.str(""); // clear
    ofRect(20, 58, ofMap(midiMessage.pitch, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "velocity: " << midiMessage.velocity;
    ofDrawBitmapString(text.str(), 20, 96);
    text.str(""); // clear
    ofRect(20, 105, ofMap(midiMessage.velocity, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "control: " << midiMessage.control;
    ofDrawBitmapString(text.str(), 20, 144);
    text.str(""); // clear
    ofRect(20, 154, ofMap(midiMessage.control, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "value: " << midiMessage.value;
    ofDrawBitmapString(text.str(), 20, 192);
    text.str(""); // clear
    
    if(midiMessage.status == MIDI_PITCH_BEND) {
        ofRect(20, 202, ofMap(midiMessage.value, 0, MIDI_MAX_BEND, 0, ofGetWidth()-40), 20);
    }
    else {
        ofRect(20, 202, ofMap(midiMessage.value, 0, 127, 0, ofGetWidth()-40), 20);
    }
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
    midiMessage = msg;
    // make a copy of the latest message
    if(msg.status == MIDI_NOTE_ON) {

        int height = ofMap(msg.pitch,21,108,0.0f,1.0f)*87.0f;

        if(midiMessage.velocity > 0) {
            
            myEmitter.curTone = height;
            myEmitter.emit(myEmitter.numDrops);
            if(!ofContains(midiNotes,height)) {
                midiNotes.push_back(height);
            }
       
        } else {
            //cout << "OFF " << msg.pitch << endl;
            if(midiNotes.size() > 0) {
                int idx = ofFind(midiNotes,height);
                midiNotes[idx] = midiNotes.back();
                midiNotes.pop_back();
                myEmitter.delete_tone(height);
            }
        }
        /*for (auto c : midiNotes)
            cout << c << ' ';        //cout << height << endl;
        cout << endl;*/
    } else if(msg.status == MIDI_CONTROL_CHANGE) {
        if(msg.control == IVI_MIDI_SPEED) {
            myEmitter.speed = ofMap(msg.value,0,127,0.0,0.8);
        } else if(msg.control == IVI_MIDI_NUMDROPS) {
            myEmitter.numDrops = (int) ofMap(msg.value,0,127,1,10);
        } else if(msg.control == IVI_MIDI_RSHD_COPIES) {
            rShd_Copies = (int) ofMap(msg.value,0,127,1,40);
        } else if(msg.control == IVI_MIDI_RSHD_ROTATION) {
            rShd_RotationInput = ofMap(msg.value,0,127,-90,90);
        } else if(msg.control == IVI_MIDI_RSHD_SCALE) {
            rShd_ScaleInput =  ofMap(msg.value,0,127,0.0,1.0);
        }
    }
}