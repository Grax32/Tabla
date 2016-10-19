//
//  MusicWorld.hpp
//  PaperBounce3
//
//  Created by Chaim Gingold on 9/20/16.
//
//

#ifndef MusicWorld_hpp
#define MusicWorld_hpp

#include "GameWorld.h"
#include "PureDataNode.h"
#include "RtMidi.h"

typedef std::shared_ptr<RtMidiOut> RtMidiOutRef;

class MusicWorld : public GameWorld
{
public:

	MusicWorld();
	~MusicWorld();

	void setParams( XmlTree ) override;
	
	string getSystemName() const override { return "MusicWorld"; }

	void update() override;
	void updateContours( const ContourVector &c ) override;
	void updateCustomVision( Pipeline& ) override; // extract bitmaps we need

	void draw( bool highQuality ) override;

private:
	
	// params
	float mStartTime;	// when MusicWorld created
	vec2  mTimeVec;		// in world space, which way does time flow forward?
	float mTempo;		// how fast to playback?
	int	  mNoteCount=8;
	int	  mBeatCount=32;
	
	// instrument info
	class Instrument
	{
	public:

		~Instrument();
		
		void setParams( XmlTree );
		void setup();

		int channelForNote(int note);
		
		// colors!
		ColorA mPlayheadColor;
		ColorA mScoreColor;
		ColorA mNoteOffColor, mNoteOnColor;
		
		//
		string mName;
		vector<int> mScale; // maps score rows -> notes

		//
		enum class SynthType
		{
			Additive = 1,
			MIDI	 = 2
		};
		
		SynthType mSynthType;
		
		// midi
		int  mPort=0;
		int  mChannel=0;
		int  mMapNotesToChannels=0; // for KORG Volca Sample
		RtMidiOutRef mMidiOut;
		
	};
	typedef std::shared_ptr<Instrument> InstrumentRef;
	
	map<string,InstrumentRef> mInstruments;
	
	// scores
	class Score
	{
	public:
		
		// shape
		vec2  mQuad[4];
		/*  Vertices are played back like so:
		
			0---1
			|   |
			3---2
			
			 --> time		*/


		// synth parameters
		cv::Mat		mImage;
		cv::Mat		mQuantizedImage;
//		SynthType	mSynthType;
		string		mInstrumentName;
		
		float		mStartTime;
		float		mDuration;
		
		float		mNoteRoot;
		int			mNoteCount;
		int         mBeatCount;
		int			mNoteInstrument;
		
		float		mPan;
		//float		mVolume;
		
		// constructing shape
		bool		setQuadFromPolyLine( PolyLine2, vec2 timeVec );
		
		// getting stuff
		PolyLine2	getPolyLine() const;
		float		getPlayheadFrac() const;
		void		getPlayheadLine( vec2 line[2] ) const;
		vec2		fracToQuad( vec2 frac ) const; // frac.x = time[0,1], frac.y = note_space[0,1]
	};
	vector<Score> mScores;
	
	InstrumentRef getInstrumentForScore( const Score& );
	
	// midi note playing and management
	bool  isScoreValueHigh( uchar ) const;
	float getNoteLengthAsScoreFrac( cv::Mat image, int x, int y ) const;
	int   getNoteLengthAsImageCols( cv::Mat image, int x, int y ) const;
	
	struct tOnNoteKey
	{
		tOnNoteKey();
		tOnNoteKey( InstrumentRef i, int n ) : mInstrument(i), mNote(n){}
		
		InstrumentRef mInstrument;
		int mNote;
		
	};

	struct cmpOnNoteKey {
		bool operator()(const tOnNoteKey& a, const tOnNoteKey& b) const {
			return pair<InstrumentRef,int>(a.mInstrument,a.mNote) > pair<InstrumentRef,int>(b.mInstrument,b.mNote);
		}
	};

	struct tOnNoteInfo
	{
		float mStartTime;
		float mDuration;
	};
	
	map< tOnNoteKey, tOnNoteInfo, cmpOnNoteKey > mOnNotes ;
		// (instrument,note) -> (start time, duration)
	
	bool isNoteInFlight( InstrumentRef instr, int note ) const;
	void updateNoteOffs();
	void doNoteOn( InstrumentRef instr, int note, float duration ); // start time is now



	// midi convenience methods
	void sendMidi( RtMidiOutRef, uchar, uchar, uchar );
	void sendNoteOn ( RtMidiOutRef midiOut, uchar channel, uchar note, uchar velocity );
	void sendNoteOff ( RtMidiOutRef midiOut, uchar channel, uchar note );

	void killAllNotes(); // sends a note off for all MIDI notes (0-127)
	
	// synthesis
	cipd::PureDataNodeRef	mPureDataNode;	// synth engine
	cipd::PatchRef			mPatch;			// music patch
//	vector<RtMidiOutRef>	mMidiOuts;

	void setupSynthesis();
	void updateAdditiveScoreSynthesis();
};

class MusicWorldCartridge : public GameCartridge
{
public:
	virtual std::shared_ptr<GameWorld> load() const override
	{
		return std::make_shared<MusicWorld>();
	}
};

#endif /* MusicWorld_hpp */
