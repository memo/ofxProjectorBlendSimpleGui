//                                      __
//     ____ ___  ___  ____ ___  ____   / /__   __
//    / __ `__ \/ _ \/ __ `__ \/ __ \ / __/ | / /
//   / / / / / /  __/ / / / / / /_/ // /_ | |/ /
//  /_/ /_/ /_/\___/_/ /_/ /_/\____(_)__/ |___/
//
//
//  Created by Memo Akten, www.memo.tv


#pragma once

#include "ofMain.h"
#include "ofxProjectorBlend.h"
#include "ofxSimpleGuiToo.h"

class ofxProjectorBlendSimpleGui {

public:
    int numProjectors = 2;
    int projectorWidth = 1920;
    int projectorHeight = 1080;
    int pixelOverlap = 0;

    // wrap ofxProjectorBlend common
    void begin() { blender.begin(); }
    void end() { blender.end(); }
    void draw() { blender.draw(); }
    int getDisplayWidth() const { return blender.getDisplayWidth(); }
    int getDisplayHeight() const { return blender.getDisplayHeight(); }
    int getCanvasWidth() const { return blender.getCanvasWidth(); }
    int getCanvasHeight() const { return blender.getCanvasHeight(); }
    int getWidth() const { return getCanvasWidth(); }
    int getHeight() const { return getCanvasHeight(); }
    ofVec2f getSize() const { return ofVec2f(getWidth(), getHeight()); }
    ofVec2f getInvSize() const { return ofVec2f(1.0f/getWidth(), 1.0f/getHeight()); }

    void setup(ofxSimpleGuiToo& gui, string xmlPath="settings/projectorblend.xml") {
        // create page and save pointer to it for later saving
        blenderGuiPage = &gui.addPage("PROJECTOR BLEND").setXMLName(xmlPath);

        // NOTE: having to setup gui twice
        // FIRST to load the number of projectors
        setupGui();
        gui.loadFromXML();

        // AGAIN to reconstruct the gui with correct number of projectors
        blenderGuiPage->clear(); // clear page first
        setupGui();
        gui.loadFromXML();

    }

    void setupGui() {
        ofLogNotice("ofxProjectorBlendSimpleGui") << __func__;
        blenderGuiPage->clear(); // clear page first
        blenderGuiPage->addToggle("enabled", blender.enabled);
        blenderGuiPage->addToggle("Show Blend", blender.showBlend);
        blenderGuiPage->addTitle("Init");
        blenderGuiPage->addSlider("numProjectors", numProjectors, 2, 10);
        blenderGuiPage->addSlider("projectorWidth", projectorWidth, 0, 1920);
        blenderGuiPage->addSlider("projectorHeight", projectorHeight, 0, 1080);
        blenderGuiPage->addSlider("pixelOverlap", pixelOverlap, 0, 1000);

        for (int i=0; i<numProjectors-1; i++)
        {
            string si = ofToString(i+1);
            blenderGuiPage->addTitle("Blend " + si);
            blenderGuiPage->addSlider("Blend Power "+si, blender.blendPower[i], 0.0, 4.0);
            blenderGuiPage->addSlider("Gamma "+si, blender.gamma[i], 0.1, 4.0);
            blenderGuiPage->addSlider("Luminance "+si, blender.luminance[i], 0.0, 1.0);
        }
    }


    void update() {
        // if any of the initialization parameters have changed, setup the blender
        if(blender.getNumProjectors() != numProjectors ||
                blender.getProjectorWidth() != projectorWidth ||
                blender.getProjectorHeight() != projectorHeight||
                blender.getPixelOverlap() != pixelOverlap) {

            ofLogWarning("ofxProjectorBlendSimpleGui") << __func__ << " Reinitializing ofxProjectorBlend";
            // if number of projectors have changed, reset the gui as well
            bool bInitGui = blender.getNumProjectors() != numProjectors;

            blender.setup(projectorWidth, projectorHeight, numProjectors, pixelOverlap);

            // if number of projectors have changed, reset the gui as well
            if(bInitGui) setupGui();
        }
    }

    //protected:
    ofxProjectorBlend blender;
    ofxSimpleGuiPage* blenderGuiPage = NULL;
};
