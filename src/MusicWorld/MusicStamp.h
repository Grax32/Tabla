//
//  MusicStamp.hpp
//  PaperBounce3
//
//  Created by Chaim Gingold on 11/21/16.
//
//

#ifndef MusicStamp_hpp
#define MusicStamp_hpp

#include "Instrument.h"
#include "cinder/gl/gl.h"

using namespace ci;

// animation pose
struct tIconAnimState
{
	tIconAnimState(){}
	tIconAnimState( vec4 v )
	{
		mTranslate = vec2(v.x,v.y); // normalized to icon size
		mScale  = vec2(v.z,v.z);
		mRotate = toRadians(v.w);
	}
	
	vec2	mTranslate;
	vec2	mScale = vec2(1.f,1.f);
	float	mRotate=0.f; // radians
	ColorA  mColor = ColorA(1,1,1,1);

	// some math so we can lerp, blend, etc...
	tIconAnimState operator+( tIconAnimState rhs ) const
	{
		tIconAnimState s;
		s.mTranslate = mTranslate + rhs.mTranslate;
		s.mScale     = mScale     + rhs.mScale;
		s.mRotate    = mRotate    + rhs.mRotate;
		s.mColor     = mColor     + rhs.mColor;
		return s;
	}
	tIconAnimState& operator+=( tIconAnimState rhs ) { *this = *this + rhs; return *this; }
	tIconAnimState  operator- ( tIconAnimState rhs ) const { return *this + (rhs*-1.f); }
	tIconAnimState  operator* ( tIconAnimState rhs ) const
	{
		tIconAnimState s;
		s.mTranslate = mTranslate * rhs.mTranslate;
		s.mScale     = mScale     * rhs.mScale;
		s.mRotate    = mRotate    * rhs.mRotate;
		s.mColor     = mColor     * rhs.mColor;
		return s;
	}
	tIconAnimState operator*( float rhs ) const
	{
		tIconAnimState s;
		s.mTranslate = mTranslate * rhs;
		s.mScale     = mScale     * rhs;
		s.mRotate    = mRotate    * rhs;
		s.mColor     = mColor     * rhs;
		return s;
	}

	// intended as additive blend-ins
	//   scale is 0,0; color is 0,0,0,0
	static tIconAnimState getSwayForScore( float playheadFrac );
	static tIconAnimState getIdleSway( float phaseInBeats, float beatDuration );
	
};
tIconAnimState operator*( float lhs, tIconAnimState rhs ) { return rhs*lhs; }

inline std::ostream &operator<<(std::ostream &os, tIconAnimState const &m) {
	os << "tIconAnimState" << endl;
	os << "\tmTranslate: " << m.mTranslate << endl;
	os << "\tmScale: " << m.mScale << endl;
	os << "\tmRotate: " << m.mRotate << endl;
	os << "\tmColor: " << m.mColor << endl;
	
	return os;
}


// stamp
class MusicStamp
{
public:

	void draw() const;
	void tick();
	
	vec2 mXAxis = vec2(1,0);
	vec2 mLoc; // drawn loc
	vec2 mSearchForPaperLoc; // the secret location that tracks paper; we pop to here if we lose our score
	float mIconWidth = 10.f;

	InstrumentRef mInstrument;
	tIconAnimState mIconPose, mIconPoseTarget;

	bool mHasScore=false;
	bool mLastHasScore=false;
	
private:
	void drawInstrumentIcon( vec2 worldx, tIconAnimState pose ) const;
	
};

#endif /* MusicStamp_hpp */
