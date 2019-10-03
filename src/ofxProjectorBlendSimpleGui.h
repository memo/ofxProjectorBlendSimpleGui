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
    int projectorWidth = 1280;
    int projectorHeight = 720;
    int pixelOverlap = 100;
    bool doDrawGrid = false;
    bool doDrawGridInvert = false;

    // wrap ofxProjectorBlend common
    void begin() { blender.begin(); }
    void end() { 
        if (blender.enabled && doDrawGrid) {
            ofPushStyle();
            float col = doDrawGridInvert ? 0 : 255;
            ofClear(255 - col);
            ofEnableAlphaBlending();
            int gridSize = 20;
            for (int i = 0; i < blender.getCanvasWidth()/gridSize; i ++) {
                bool major = i % 5 == 0;
                ofSetColor(col, major ? 255 : 128);
                int c = i * gridSize;
                for (int x=c-major; x<=c+ major; x++) // diy line thickness
                    ofDrawLine(x, 0, x, blender.getCanvasHeight());
            }
            for (int i = 0; i < blender.getCanvasHeight()/gridSize; i++) {
                bool major = i % 5 == 0;
                ofSetColor(col, major ? 255 : 128);
                int c = i * gridSize;
                for (int y = c - major; y <= c + major; y++) // diy line thickness
                    ofDrawLine(0, y, blender.getCanvasWidth(), y);
            }
			ofSetLineWidth(2);
			ofSetColor(col, 255);
			for (int i = -1;  i <= 1; i++) {
				ofDrawLine(0, i, blender.getCanvasWidth(), blender.getCanvasHeight()+i);
				ofDrawLine(0, blender.getCanvasHeight()+i, blender.getCanvasWidth(), i);
			}

			ofPushMatrix();
			ofNoFill();
			ofSetCircleResolution(64);
			ofSetLineWidth(2);
			vector<int> colors = { 0xFF0000, 0x00FF00, 0x0000FF };
			for (int i = 0; i < blender.getNumProjectors(); i++) {
				ofSetHexColor(colors[i%colors.size()]);
				ofDrawLine(0, 0, blender.getProjectorWidth(), blender.getProjectorHeight());
				ofDrawLine(0, blender.getProjectorHeight(), blender.getProjectorWidth(), 0);
				ofDrawCircle(blender.getProjectorWidth() / 2, blender.getProjectorHeight() / 2, blender.getProjectorHeight()*0.3);
				ofTranslate(blender.getProjectorWidth(), 0);
				ofDrawRectangle(0, 0, blender.getProjectorWidth(), blender.getProjectorHeight());
			}
			ofPopMatrix();
            ofPopStyle();

        }
        blender.end();
    }

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
        // FIRST to load the number of projectors and width/height etc.
        numProjectors = 0;
        setupGui();

        // now setup the blender properly, and setup GUI again all projector params
        blender.setup(projectorWidth, projectorHeight, numProjectors, pixelOverlap);
        setupGui();
    }

    void setupGui() {
        ofLogNotice("ofxProjectorBlendSimpleGui") << __func__;
        blenderGuiPage->clear(); // clear page first
        blenderGuiPage->addToggle("enabled", blender.enabled);
        blenderGuiPage->addToggle("showBlend", blender.showBlend);
		blenderGuiPage->addToggle("doBlend", blender.doBlend);	
        blenderGuiPage->addToggle("doDrawGrid", doDrawGrid);
        blenderGuiPage->addToggle("doDrawGridInvert", doDrawGridInvert);
        blenderGuiPage->addTitle("Init");
        blenderGuiPage->addSlider("numProjectors", numProjectors, 2, 10);
        blenderGuiPage->addSlider("projectorWidth", projectorWidth, 0, 1920);
        blenderGuiPage->addSlider("projectorHeight", projectorHeight, 0, 1200);
        blenderGuiPage->addSlider("pixelOverlap", pixelOverlap, 0, 1000);

        for (int i=0; i<numProjectors-1; i++) { // loop to n-1 because we're only interested in crossovers
            string si = ofToString(i+1);
            blenderGuiPage->addTitle("Blend " + si);
            blenderGuiPage->addSlider("Blend Power "+si, blender.blendPower[i], 0.0, 4.0);
            blenderGuiPage->addSlider("Gamma "+si, blender.gamma[i], 0.1, 4.0);
            blenderGuiPage->addSlider("Luminance "+si, blender.luminance[i], 0.0, 1.0);
        }
        gui.loadFromXML();
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
