//
//  ContourVision.hpp
//  PaperBounce3
//
//  Created by Chaim Gingold on 12/13/16.
//
//

#ifndef ContourVision_hpp
#define ContourVision_hpp

#include <stdio.h>
#include "xml.h"
#include "Contour.h"
#include "Pipeline.h"

class ContourVision
{
public:
	
	class Params
	{
	public:
		void set( XmlTree );
		
		float mThreshold=-1.f; // -1 is otsu, otherwise 0..255
		
		float mContourMinRadius	=	3;
		float mContourMinArea	=	100;
		float mContourDPEpsilon	=	5;
		float mContourMinWidth	=	5;
		bool  mContourGetExactMinCircle = false; // otherwise, we'll approximate with min bounding box
		
	};

	void setParams( Params p ) { mParams=p; }
	
	ContourVec findContours( const Pipeline::StageRef input, Pipeline& pipeline, float contourPixelToWorld );
	// contourPixelToWorld is redundant to transform data that is already present in the pipeline,
	// and so should be refactored so it isn't explicitly passed. however, i'm not doing such a deep refactor just yet.
	
private:
	Params mParams;
	
};

#endif /* ContourVision_hpp */
