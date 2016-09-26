//
//  PongWorld.cpp
//  PaperBounce3
//
//  Created by Chaim Gingold on 9/20/16.
//
//

#include "PongWorld.h"
#include "geom.h"
#include "cinder/rand.h"

PongWorld::PongWorld()
{
	mPlayerColor[0] = Color(1,0,0);
	mPlayerColor[1] = Color(0,0,1);
	
	mPlayerScore[0] = mPlayerScore[1] = 0;
	
	mMaxScore=5;
}

vec2 PongWorld::fieldCorner( int i ) const
{
	int j = i + mFieldPlayer0LeftCornerIndex;
	
	return getWorldBoundsPoly().getPoints()[ j % getWorldBoundsPoly().getPoints().size() ];
}

void PongWorld::update()
{
	// update ball sim
	BallWorld::update();
	
	// make new ball?
	if ( getBalls().empty() )
	{
		serve();
	}
}

void PongWorld::onBallBallCollide   ( const Ball&, const Ball& )
{
	if (0) cout << "ball ball collide" << endl;
}

void PongWorld::onBallContourCollide( const Ball&, const Contour& )
{
	if (0) cout << "ball contour collide" << endl;
}

void PongWorld::onBallWorldBoundaryCollide	( const Ball& b )
{
	if (0) cout << "ball world collide" << endl;

	// which player side did it hit?
	for( int i=0; i<2; i++ )
	{
		vec2 c = closestPointOnLineSeg( b.mLoc, mPlayerSide[i][0], mPlayerSide[i][1] );
		
		// in goal
		if ( glm::distance(c,b.mLoc) <= b.mRadius * 2.f ) // a little hacky, but it works.
		{
			didScore( 1 - i );
		}
	}
}

void PongWorld::didScore( int player )
{
	cout << "didScore " << player << endl;

	mPlayerScore[player] = ( mPlayerScore[player] + 1 ) % mMaxScore;
}

void PongWorld::serve()
{
	Ball ball ;
	
	ball.mColor = Color(1,0,0);
	ball.setLoc( mCenter ) ;
	ball.mRadius = 2.f;
	ball.setMass( M_PI * powf(ball.mRadius,3.f) ) ;
	
	ball.mCollideWithContours = false;
	
	ball.setVel( Rand::randVec2() * ball.mRadius/2.f ) ;
	
	getBalls().push_back( ball ) ;
}

void PongWorld::draw( bool highQuality )
{
	//
	// Draw field layout markings
	//
	{
		gl::color(0,.8,0,1.f);
		
		PolyLine2 wb = getWorldBoundsPoly();
		wb.setClosed();
		gl::draw( wb );
		
		gl::drawLine( mCenterLine[0], mCenterLine[1] );
		
		gl::drawStrokedCircle(mCenter, mFieldCenterCircleRadius);
		
		gl::color(1,0,0);
		for( int i=0; i<2; ++i )
		{
			gl::drawLine(mPlayerSide[i][0] + mPlayerVec[1-i], mPlayerSide[i][1] + mPlayerVec[1-i]);
		}
	}
	
	//
	// Draw world
	//
	BallWorld::draw(highQuality);
	
	//
	// Draw Score/UI
	//
	for( int i=0; i<2; ++i )
	{
		drawScore( i, mScoreDotStart[i], mScoreDotStep[i], mScoreDotRadius, mPlayerScore[i], mMaxScore );
	}
}

void PongWorld::worldBoundsPolyDidChange()
{
	computeFieldLayout();
}

void PongWorld::computeFieldLayout()
{
	assert( getWorldBoundsPoly().size()==4 );
	
	mFieldPlayer0LeftCornerIndex=1;
		// hard code this for now, since it is just right
		// we should probably want to move this to xml :P
		// whatever, it's all hacky.
	
	mCenterLine[0] = lerp(fieldCorner(0), fieldCorner(1), .5f);
	mCenterLine[1] = lerp(fieldCorner(2), fieldCorner(3), .5f);
	mCenter = lerp( mCenterLine[0], mCenterLine[1], .5f );
	
	mCenterVec = normalize( mCenterLine[0] - mCenterLine[1] );
	
	mPlayerSide[0][0] = fieldCorner(0);
	mPlayerSide[0][1] = fieldCorner(3);
	mPlayerSide[1][0] = fieldCorner(1);
	mPlayerSide[1][1] = fieldCorner(2);
	
	mPlayerGoal[0] = lerp(fieldCorner(0), fieldCorner(3), .5f);
	mPlayerGoal[1] = lerp(fieldCorner(1), fieldCorner(2), .5f);
	
	mPlayerVec[1] = normalize( mPlayerGoal[1] - mPlayerGoal[0] );
	mPlayerVec[0] = -mPlayerVec[1];
	
	mFieldCenterCircleRadius = glm::distance( mCenterLine[0], mCenterLine[1] ) * .15f;


	mScoreDotRadius = 1.f; // 1mm
	float scoreDotGutter = mScoreDotRadius * 1.5f ;
	
	for ( int i=0; i<2; ++i )
	{
		mScoreDotStep[i] = mPlayerVec[i] * (mScoreDotRadius + scoreDotGutter);

		mScoreDotStart[i] = mCenterLine[0]
			+ mScoreDotStep[i]
			- mCenterVec    * (mScoreDotRadius + scoreDotGutter);
		
//		mScoreDotStart[0] = mCenterLine[1] + mPlayerVec[0] * mScoreDotStep[0].x + mCenterVec * mScoreDotStep[0].y;
//		mScoreDotStart[1] = mCenterLine[1] + mPlayerVec[1] * mScoreDotStep[1].x + mCenterVec * mScoreDotStep[1].y;
	}
}

void PongWorld::drawScore( int player, vec2 dotStart, vec2 dotStep, float dotRadius, int score, int maxScore ) const
{
	const int   kNumSegments = 12;
	const float kStrokeWidth = .2f;
	
	gl::color(mPlayerColor[player]);
	
	for( int i=0; i<maxScore; ++i )
	{
		vec2 c = dotStart + dotStep * (float)i;
		
		// full
		if ( i <= score-1 )
		{
			gl::drawSolidCircle(c,dotRadius,kNumSegments);
		}
		// empty
		else
		{
			gl::drawStrokedCircle(c,dotRadius,kStrokeWidth,kNumSegments);
		}
	}
}
