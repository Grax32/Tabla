//
//  QuadTestWorld.h
//  PaperBounce3
//
//  Created by Chaim Gingold on 12/22/16.
//
//

#ifndef QuadTestWorld_h
#define QuadTestWorld_h


#include <vector>
#include "cinder/gl/gl.h"
#include "cinder/Xml.h"
#include "cinder/Color.h"

#include "GameWorld.h"
#include "Contour.h"
#include "geom.h"
#include "RectFinder.h"

using namespace ci;
using namespace ci::app;
using namespace std;


class QuadTestWorld : public GameWorld
{
public:
	QuadTestWorld();

	string getSystemName() const override { return "QuadTestWorld"; }

	void setParams( XmlTree ) override;
	void updateVision( const Vision::Output&, Pipeline& ) override;

	void update() override;
	void draw( DrawType ) override;

	void drawMouseDebugInfo( vec2 ) override;

private:
	vec2 mTimeVec=vec2(1,0); // for orienting quads; probably doesn't matter at all.
	
	class Frame
	{
	public:
		bool mIsValid=false;
		
		PolyLine2 mContourPoly;
		PolyLine2 mContourPolyReduced;
		PolyLine2 mConvexHull;
		std::vector<PolyLine2> mReducedDiff;
		vec2 mQuad[4];
		
		float mOverlapScore;
		PolyLine2 getQuadAsPoly() const;
	};
	typedef vector<Frame> FrameVec; 
	FrameVec mFrames;
	
	FrameVec getFrames(
		const Pipeline::StageRef world,
		const ContourVector &contours,
		Pipeline& pipeline ) const;
	
	RectFinder mRectFinder;
};

class QuadTestWorldCartridge : public GameCartridge
{
public:
	virtual string getSystemName() const override { return "QuadTestWorld"; }

	virtual std::shared_ptr<GameWorld> load() const override
	{
		return std::make_shared<QuadTestWorld>();
	}
};


#endif /* QuadTestWorld_h */
