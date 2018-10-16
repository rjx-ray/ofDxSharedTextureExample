#pragma once

#include "ofMain.h"
#include "dxThread.h"

class ofApp : public ofBaseApp{

	public:
		ofTexture sharedTexture;
		HANDLE gl_device;
		HANDLE handle_shared_texture;

		myDxThread *dxThread;

		void cleanUp();
		void setup();
		void update();
		void draw();
		void exit(ofEventArgs & args);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
