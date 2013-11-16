#ifndef _FGLANIM_H_
#define _FGLANIM_H_

#include "common.h"
#include "fgl3DGen.h"
#include "fglMem.h"

class fglAnimVectKey
{
public:
	float nFrame;
	fglVECTOR v;
};

class fglAnimQuatKey
{
public:
	float nFrame;
	fglQUAT q;
};

class fglAnimChan
{
public:
	fglString jointname;

	fglArray<fglAnimVectKey> aPosKeys;
	fglArray<fglAnimQuatKey> aRotKeys;
	fglArray<fglAnimVectKey> aScaleKeys;
	
	fglAnimChan(void) {};
	virtual ~fglAnimChan(void) {};

};


class fglAnim 
{
public:
	fglString name;

	typedef fglUMap<fglString, fglREF<fglAnimChan>> ACHANS;
	ACHANS aChans;

	float nFrames;
	unsigned nFPS;

	fglAnim(void) {nFPS=0; nFrames=0.0f;};
	virtual ~fglAnim(void) {};

};


class fglAnimChanEval
{
public:
	fglREF<fglAnimChan> oChan;
	
	float nFrames;

	fglTRANSFORM tr;
	int nCurrPos;
	int nCurrRot;
	int nCurrScale;
	
	//fglAnimChanEval(void) {;}
	fglAnimChanEval(fglREF<fglAnimChan>& poChan) { oChan = poChan; nFrames=0.0f;}
	virtual ~fglAnimChanEval(void);

	bool getNextPos(float pnFrame, fglVECTOR& pNextPos);
	bool getNextScale(float pnFrame, fglVECTOR& pNextScale);
	bool getNextRot(float pnFrame, fglQUAT& pNextRot);

	void build(float pnFrame);

};


class fglAnimEval
{
public:
	
	fglREF<fglAnim> oAnim;
	//typedef std::vector<fglAnimChanEval> ACHANS;
	//ACHANS aChans;

	float nStartTime;
	float nCurrFrame;

	int nRepeat; //repeat nRepeat times, value -1 means infinite loop
	float nSpeed; //animation speed factor x1.0, x1.5, x2.0 etc

	fglAnimEval (fglREF<fglAnim>& poAnim=fglREF<fglAnim>(), int pnRepeat=1, float pnSpeed=1.0f) 
	{
		oAnim = poAnim; 
		nStartTime = 0.0f;
		nCurrFrame = 0.0f;
		nRepeat = pnRepeat; 
		nSpeed = pnSpeed; 
		//fglAnim::ACHANS::iterator iChan = oAnim->aChans.begin(), iChanEnd = oAnim->aChans.end();
		//for (; iChan != iChanEnd; ++iChan) { aChans.push_back(fglAnimChanEval(iChan->second)); }

	};

	virtual ~fglAnimEval (void) {};

	void build(float pnTime);

};


#endif