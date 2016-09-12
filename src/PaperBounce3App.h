//
//  PaperBounce3App.h
//  PaperBounce3
//
//  Created by Chaim Gingold on 9/12/16.
//
//

#ifndef PaperBounce3App_h
#define PaperBounce3App_h

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "cinder/Text.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Xml.h"

#include "cinder/Capture.h"
#include "CinderOpenCV.h"

#include "LightLink.h"
#include "Vision.h"
#include "Contour.h"
#include "BallWorld.h"
#include "XmlFileWatch.h"
#include "Pipeline.h"

#include "PipelineStageView.h"
#include "WindowData.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class PaperBounce3App : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void mouseUp( MouseEvent event ) override;
	void mouseMove( MouseEvent event ) override;
	void mouseDrag( MouseEvent event ) override;
	void update() override;
	void draw() override;
	void resize() override;
	void keyDown( KeyEvent event ) override;
	
	void drawWorld();
	
	LightLink			mLightLink; // calibration for camera <> world <> projector
	CaptureRef			mCapture;	// input device		->
	Vision				mVision ;	// edge detection	->
	ContourVector		mContours;	// edges output		->
	BallWorld			mBallWorld ;// world simulation
	
	Pipeline			mPipeline; // traces processing
	
	// world info
	vec2 getWorldSize() const ;
		// world rect might be more appropriate...
		// this is kind of a deprecated concept; it is used to:
		// - draw a frame
		// - spawn random balls
		// What are we talking about? Replace with these concepts:
		// - contour boundaries?
		// - camera world bounds?
		// - projector world bounds?
		// - union of camera + projector bounds?
		// - some other arbitrary thing?
	
	
	// ui
	Font				mFont;
	gl::TextureFontRef	mTextureFont;
	
	WindowRef			mMainWindow;// projector
	WindowRef			mUIWindow; // for other debug info, on computer screen
	
	double				mLastFrameTime = 0. ;
	
	// to help us visualize
	void addProjectorPipelineStages();
	void updatePipelineViews( bool areViewsVisible );
	
	void updateMainImageTransform( WindowRef );
	
	/* Coordinates spaces, there are a few:
		
		UI (window coordinates, in points, not pixels)
			- pixels
			- points
			
		Image
			- Camera
				e.g. pixel location in capture image
			- Projector
				e.g. pixel location on a screen
			- Arbitrary
				e.g. supersampled camera image subset
				e.g. location of a pixel on a shown image
		
		World
			(sim size, unbounded)	eg. location of a bouncing ball
	 
	 
	Transforms
		UI    <> Image -- handled by View objects
		Image <> World -- currently handled by Pipeline::Stage transforms

	*/
	
	// settings
	bool mAutoFullScreenProjector = false ;
	bool mDrawCameraImage = false ;
	bool mDrawContours = false ;
	bool mDrawContoursFilled = false ;
	bool mDrawMouseDebugInfo = false ;
	bool mDrawPolyBoundingRect = false ;
	bool mDrawContourTree = false ;
	bool mDrawPipeline = false;
	bool mDrawContourMousePick = false;

	fs::path myGetAssetPath( fs::path ) const ; // prepends the appropriate thing...
	string mOverloadedAssetPath;
	
	XmlFileWatch mXmlFileWatch;
};

#endif /* PaperBounce3App_h */
