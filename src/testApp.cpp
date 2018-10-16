#include "testApp.h"

//--------------------------------------------------------------
void ofApp::setup(){


	ofSetFrameRate(55);
	ofSetVerticalSync(true);

	handle_shared_texture = NULL;
	gl_device = NULL;
	ofLogToConsole();
	ofSetLogLevel(OF_LOG_VERBOSE);


	//We run direct3d rendering in a separate thread to be closer to a real life situation
	dxThread = new myDxThread();
	dxThread->startThread(false);


	// With D3D11, we need to be sure nothing is being rendered on the device/texture while we open it up. 
	// So here's some simple synchronization mechanism to make sure of it
	dxThread->waitForDxReady();
	gl_device = wglDXOpenDeviceNV(dxThread->g_pd3dDevice);

	if (!gl_device) ofLogError("dxTest", "Error while opening shared device");


	ID3D11Texture2D* p_backBuffer;
	dxThread->g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *) &p_backBuffer);



	sharedTexture.allocate(640, 480, GL_RGBA); //put the size of your window
	handle_shared_texture = wglDXRegisterObjectNV(gl_device, p_backBuffer, sharedTexture.texData.textureID, GL_TEXTURE_RECTANGLE_ARB, WGL_ACCESS_READ_ONLY_NV);
	if (!handle_shared_texture) ofLogError("dxTest", "Error while opening shared texture");


	//Once we're done creating our thing we can kick in the rendering on the D3D side
	dxThread->notifyGlReady();

}

//--------------------------------------------------------------
void ofApp::update(){
	ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0, 0, 0);


	//Here I'm reusing the previous synchronization object to assure that nothing is being rendered onto the surface while it's being used in openGL.
	//It's probably not the best option as we're forcing D3D and openGL to have synced drawing cycles.
	//A better idea would be to use some sort of triple buffering on the D3D side, so we would alway have a surface ready to be drawn in oF and a surface ready to be written onto in D3D
	//But for the sake of simplicity, we stick for the current synced solution for this example.
	dxThread->waitForDxReady();

	wglDXLockObjectsNV(gl_device, 1, &handle_shared_texture); //you shouldnt use this if the opening of the device/texture failed.....
	sharedTexture.draw(0, 0);
	wglDXUnlockObjectsNV(gl_device, 1, &handle_shared_texture); //It appears that everything works fine if you don't unlock your object. But that's not what the specs says, so be careful.
	dxThread->notifyGlReady();
}

void ofApp::exit(ofEventArgs & args) {
	cleanUp();
}


void ofApp::cleanUp() {
	//We should probably have some lock here as well.
	if (gl_device) {
		if (handle_shared_texture) wglDXUnregisterObjectNV(gl_device, handle_shared_texture);
		wglDXCloseDeviceNV(gl_device);
	}
	gl_device = NULL;
	handle_shared_texture = NULL;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
