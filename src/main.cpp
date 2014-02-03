#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;
	//window.setGlutDisplayString("rgb double depth alpha samples>=4");
	//window.setGlutDisplayString("rgba double samples>=4 depth");
	ofSetupOpenGL(&window, 1280, 1024, OF_FULLSCREEN);			// <-------- setup the GL context
	//ofSetupOpenGL(&window, 1280, 1024, OF_WINDOW);	
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());
	
}
