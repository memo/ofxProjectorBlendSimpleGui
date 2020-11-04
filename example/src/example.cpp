//                                      __
//     ____ ___  ___  ____ ___  ____   / /__   __
//    / __ `__ \/ _ \/ __ `__ \/ __ \ / __/ | / /
//   / / / / / /  __/ / / / / / /_/ // /_ | |/ /
//  /_/ /_/ /_/\___/_/ /_/ /_/\____(_)__/ |___/
//
//
//  Created by Memo Akten, www.memo.tv


#include "ofMain.h"
#include "ofxProjectorBlendSimpleGui.h"

class testApp : public ofBaseApp{
public:
    ofxProjectorBlendSimpleGui blenderGui;
    ofVideoGrabber grabber;

    void setup() {
        blenderGui.setup(gui);
        gui.show();
        gui.setAutoSave(false);
        grabber.setup(320, 240);
    }

    void update() {
        grabber.update();
        blenderGui.update();
    }

    void draw() {
        blenderGui.begin(); //call blenderGui.begin() to draw onto the blendable canvas
        {
            // DRAW CONTENTS
            ofSetColor(255);
            // draw cameras
            grabber.draw(0, 0, blenderGui.getWidth(), blenderGui.getHeight());

            // draw grid
            ofSetLineWidth(3);
            for(int i = 0; i <= blenderGui.getWidth(); i+=40) ofLine(i, 0, i, blenderGui.getHeight());
            for(int j = 0; j <= blenderGui.getHeight(); j+=40) ofLine(0, j, blenderGui.getWidth(), j);
        }
        blenderGui.end(); //call when you are finished drawing

        blenderGui.draw();

        gui.draw();
    }
};

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 3);
    settings.setSize(1280, 800);
//    settings.windowMode = OF_FULLSCREEN;
    settings.multiMonitorFullScreen = true;
    ofCreateWindow(settings);			// <-------- setup the GL context
    ofRunApp(new testApp());
}
