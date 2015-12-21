
#include "DropBrush.h"

DropBrush::DropBrush(){
    birthTime = ofGetElapsedTimef();
    brushShader.load("brushShader");

    this->setup();
}

DropBrush::DropBrush(int histSteps, int sFrames, float mSize){
    birthTime = ofGetElapsedTimef();
    historySteps = histSteps;
    subFrames = sFrames;
    maxSize = mSize;
    brushShader.load("brushShader");

    this->setup();
}

DropBrush::~DropBrush(){}

//--------------------------------------------------------------
void DropBrush::setup(){
    
    brushMesh.setMode(OF_PRIMITIVE_POINTS);

    for (int i=0; i < historySteps; i++) {
        history.push_back(ofVec3f(0,0,0));
    }
}

//--------------------------------------------------------------
void DropBrush::update(ofVec3f newPos){
    position = newPos;
    if(ofGetElapsedTimef()-deathTime >= fadeTime) {
        beKilled = true;
    }
}

//--------------------------------------------------------------
void DropBrush::draw(){
    int vertexCount = 0;
    brushMesh.clear();
    glPointSize(maxSize);
    glEnable(GL_PROGRAM_POINT_SIZE);
    for (int i=historySteps-1; i >= 0; i--) {
        if (i == 0) {
            history[i] = position;
        } else {
            float prgrs, bprgrs,curSize;
            for (int j=0; j < subFrames; j++) {
                prgrs = j/float(subFrames);
                bprgrs = 1-ofMap(i-prgrs, 0, historySteps-1, 0, 1);
                
                curColor = ofColor(startColor.r * bprgrs + endColor.r * (1-bprgrs),
                                   startColor.g * bprgrs + endColor.g * (1-bprgrs),
                                   startColor.b * bprgrs + endColor.b * (1-bprgrs));
                brushMesh.addVertex(ofVec3f(
                         ofMap(prgrs,0,1,history[i].x,history[i-1].x),
                         ofMap(prgrs,0,1,history[i].y,history[i-1].y),
                         ofMap(prgrs,0,1,history[i].z,history[i-1].z)));
                brushMesh.addColor(curColor);
                brushMesh.addNormal(ofVec3f(bprgrs,0,0));
                vertexCount++;
            }
            history[i] = history[i-1];
        }
    }
    brushShader.begin();
    brushShader.setUniform1i( "vertexCount", vertexCount);
    brushMesh.draw();
    brushShader.end();

}

//--------------------------------------------------------------
void DropBrush::kill(){
    deathTime = ofGetElapsedTimef();
}

