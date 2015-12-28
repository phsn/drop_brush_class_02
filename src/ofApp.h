#pragma once

#include "ofMain.h"
#include "DropEmitter.h"
#include "DropBrush.h"
#include "ofxMidi.h"
#include "Spiropath.h"

#define IVI_MIDI_SPEED 1
#define IVI_MIDI_NUMDROPS 2
#define IVI_MIDI_RSHD_COPIES 3
#define IVI_MIDI_RSHD_ROTATION 4
#define IVI_MIDI_RSHD_SCALE 5



class ofApp : public ofBaseApp, public ofxMidiListener {
    
public:
    void setup();
    void update();
    void draw();
    void drawRepeat();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofFbo baseFBO;
    ofFbo repeatFBO;

    ofShader repeatShader;
    ofShader fisheye;
    
    int rShd_Copies = 20;
    float rShd_Rotation = 60;
    float rShd_RotationInput = 60;
    float rShd_Scale = 0.1;
    float rShd_ScaleInput = 0.1;
    float rShd_Ease = 0.01;

    bool shaderEnabled = true;;
    
    DropEmitter myEmitter;
    
    ofxMidiIn	midiIn;
    ofxMidiMessage midiMessage;
    void newMidiMessage(ofxMidiMessage& eventArgs);
    
    stringstream text;
    void drawMidiUI();
    
    vector<int> midiNotes;
    
    ofEasyCam myCam;
    DropBrush testBrush;

};
