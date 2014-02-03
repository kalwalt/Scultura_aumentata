#ifndef _TEST_APP
#define _TEST_APP

#include "ofxOpenCv.h"
#include "ofxARToolkitPlus.h"
#include "ofx3DModelLoader.h"
#include "ofVectorMath.h"

#include "ofMain.h"

#include "glut.h"



class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		/* Size of the image */
		int width, height;
	
		/* Use a camera  */
		
		ofVideoGrabber vidGrabber;
		ofShader shader;
		

		/* ARToolKitPlus class */	
		ofxARToolkitPlus artk;	
		int threshold;
		bool thresoldV;
	
		/* OpenCV images */
		ofxCvColorImage colorImage;
		ofxCvGrayscaleImage grayImage;
		ofxCvGrayscaleImage	grayThres;

		ofx3DModelLoader scultura_132;
		ofx3DModelLoader scultura_134;
		ofx3DModelLoader scultura_193;
		ofx3DModelLoader scultura_194;
		ofx3DModelLoader scultura_195;
		ofx3DModelLoader scultura_196;
		ofx3DModelLoader scultura_198;

		ofLight light;
		ofFloatColor ambientColor;
		ofFloatColor diffuseColor;
		ofFloatColor specularColor;

		ofMaterial material;

		ofVboMesh mesh;
		ofTexture tex;
	
};

#endif
