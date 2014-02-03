#include "testApp.h"

void setupGlLight(){

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    GLfloat lmKa[] = {0.1,0.0, 0.2, 0.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    //GLfloat light_position[] = { 108, 20.0, 100.0, 0.0 }; //light directional if w=0 without shader works
    GLfloat light_position[] = { 220.0, 10.0, 0.0, 1.0 }; //light directional if w=0 without shader works

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);
}

void setupGlMaterial(){

    GLfloat mat_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat mat_diffuse[] = {0.03f, 0.02f, 0.7f, 0.6f};
    GLfloat mat_specular[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glMaterialfv(GL_FRONT,GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT,GL_SHININESS, 40.0);

}

//--------------------------------------------------------------
void testApp::setup(){
	
	//width = ofGetWindowWidth();
	width = ofGetViewportWidth();
	//height = ofGetWindowHeight();
	height = ofGetViewportHeight();
	/*
	light.setDirectional();
	light.setPosition(100,120,100);
	diffuseColor.set(1.02f,1.1f,1.04f,1.0f);
    ambientColor.set(0.2f,0.1f,0.0f,1.0f);
    specularColor.set(.01f,.02f,.04f,.0f);

	light.setAmbientColor(ambientColor);
	light.setDiffuseColor(diffuseColor);
	light.setSpecularColor(specularColor);
	*/
    //some model / light stuff
    
	
	vidGrabber.setDeviceID(1);
	vidGrabber.initGrabber(width, height);
	vidGrabber.listDevices();

	//shader.load("shaders/pointlight.vert", "shaders/pointlight.frag");
	
	colorImage.allocate(width, height);
	grayImage.allocate(width, height);
	grayThres.allocate(width, height);
	
	
	scultura_132.loadModel("sculpture_132_ID.3DS",1.0);
	scultura_134.loadModel("sculpture_134_ID.3DS",1.0);
	scultura_193.loadModel("sculpture_193_ID.3DS",1.0);
	scultura_194.loadModel("sculpture_194_ID.3DS",1.0);
	scultura_195.loadModel("sculpture_195_ID.3DS",1.0);
	scultura_196.loadModel("sculpture_196_ID.3DS",1.0);
	scultura_198.loadModel("sculpture_198_ID.3DS",1.0);

	//sculture_134.setScale(0.5,0.5,0.5);

	// This uses the default camera calibration and marker file
	artk.setup(width, height);
	

	// The camera calibration file can be created using GML:
	// http://graphics.cs.msu.ru/en/science/research/calibration/cpp
	// and these instructions:
	// http://studierstube.icg.tu-graz.ac.at/doc/pdf/Stb_CamCal.p
	// This only needs to be done once and will aid with detection
	// for the specific camera you are using
	// Put that file in the data folder and then call setup like so:
	// artk.setup(width, height, "myCamParamFile.cal", "markerboard_480-499.cfg");
	
	// Set the threshold
	// ARTK+ does the thresholding for us
	// We also do it in OpenCV so we can see what it looks like for debugging
	threshold = 85;
	artk.setThreshold(threshold);
	thresoldV = false;

	ofBackground(127,127,127);
	
}

//--------------------------------------------------------------
void testApp::update(){


	vidGrabber.grabFrame();
	bool bNewFrame = vidGrabber.isFrameNew();
	
	if(bNewFrame) {
		

		colorImage.setFromPixels(vidGrabber.getPixels(), width, height);
		
		
		// convert our camera image to grayscale
		grayImage = colorImage;
		// apply a threshold so we can see what is going on
		grayThres = grayImage;
		grayThres.threshold(threshold);
		
		// Pass in the new image pixels to artk
		artk.update(grayImage.getPixels());
		
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	// Main image
	ofSetHexColor(0xffffff);
	ofPushView();
	ofPushStyle(); 
	grayImage.draw(0, 0);
	ofPopStyle();
	ofPopView();
	
	ofSetHexColor(0x666666);	
	ofDrawBitmapString(ofToString(artk.getNumDetectedMarkers()) + " marker(s) found", 10, 20);

	// Threshold image
	if(thresoldV){
	ofSetHexColor(0xffffff);

	
	grayThres.draw(800, 0,400,300);
	

	ofSetHexColor(0x666666);	
	ofDrawBitmapString("Threshold: " + ofToString(threshold), 650, 20);
	ofDrawBitmapString("Use the Up/Down keys to adjust the threshold", 650, 40);
	// ARTK draw
	// An easy was to see what is going on
	// Draws the marker location and id number
	artk.draw(800, 0);

	}
	
	
	
	// ARTK 3D stuff
	// This is another way of drawing objects aligned with the marker
	// First apply the projection matrix once
	artk.applyProjectionMatrix();
	// Find out how many markers have been detected
	int numDetected = artk.getNumDetectedMarkers();
	
	
	// Draw for each marker discovered
	for(int i=0; i<numDetected; i++) {
		int IDnumber = artk.getMarkerID(i);
		string IDstring; 
		// Set the matrix to the perspective of this marker
		// The origin is in the middle of the marker	
		artk.applyModelMatrix(i);
		if(thresoldV){
		ofPushMatrix();
		ofRotateX(180);
		ofDrawBitmapString("ID number:" + ofToString(IDnumber),10,20,0);
		ofPopMatrix();
		}
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_SMOOTH);
		
		ofEnableAlphaBlending();
		 //setupGlLight();
		 //setupGlMaterial();

			//shader.begin();
		
		if(IDnumber==132)
		{	
			setupGlLight();
			setupGlMaterial();
			ofPushMatrix();
			glEnable(GL_DEPTH_TEST);
			
			scultura_132.draw();
			
			glDisable(GL_DEPTH_TEST);
			ofPopMatrix();
			//glDisable(GL_LIGHT0);


		}
		else if(IDnumber==134)
		{
			setupGlLight();
			setupGlMaterial();
			ofPushMatrix();
			glEnable(GL_DEPTH_TEST);
			scultura_134.draw();
			glDisable(GL_DEPTH_TEST);
			ofPopMatrix();
			//glDisable(GL_LIGHT0);
		}
		else if(IDnumber==193)
		{	
			setupGlLight();
			setupGlMaterial();
			glEnable(GL_DEPTH_TEST);
			scultura_193.draw();
			glDisable(GL_DEPTH_TEST);
			//glDisable(GL_LIGHT0);

		}
		else if(IDnumber==194)
		{
			setupGlLight();
			setupGlMaterial();
			glEnable(GL_DEPTH_TEST);
			scultura_194.draw();
			glDisable(GL_DEPTH_TEST);
			//glDisable(GL_LIGHT0);

		}
		else if(IDnumber==195)
		{
			setupGlLight();
			setupGlMaterial();
			glEnable(GL_DEPTH_TEST);
			scultura_195.draw();
			glDisable(GL_DEPTH_TEST);
			//glDisable(GL_LIGHT0);

		}
		else if(IDnumber==196)
		{
			setupGlLight();
			setupGlMaterial();
			ofPushMatrix();
			glEnable(GL_DEPTH_TEST);
			scultura_196.draw();
			glDisable(GL_DEPTH_TEST);
			ofPopMatrix();
			//glDisable(GL_LIGHT0);

		}
		else if(IDnumber==198)
		{
			setupGlLight();
			setupGlMaterial();
			glEnable(GL_DEPTH_TEST);
			scultura_198.draw();
			glDisable(GL_DEPTH_TEST);
			//glDisable(GL_LIGHT0);

		}
		
	}
	
	//shader.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == OF_KEY_UP) {
		artk.setThreshold(++threshold);
		
	} else if(key == OF_KEY_DOWN) {
		artk.setThreshold(--threshold);		
	}
	
	switch(key) {
        case 's':
           vidGrabber.videoSettings();
            break;
        case 't':
           thresoldV= true;
            break;
		case 'T':
			thresoldV= false;
			break;
		case 'f':
			//ofAppGlutWindow window;
			//ofWindow
			//ofSetupOpenGL(&window, 1280, 1024, OF_WINDOW);
			break;
    }
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

