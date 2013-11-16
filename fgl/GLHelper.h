#ifndef _GLHELPER_H_
#define _GLHELPER_H_

#include "common.h"
#include "fgl.h"

class fglScene;

class GLHelper 
{
	friend fglScene;
private:
	unsigned nMaxLights;
	unsigned nMaxElementsVertices;
	unsigned nMaxElementsIndices;
	unsigned nGLVersionMajor;
	unsigned nGLVersionMinor;

public:
	GLHelper (void);
	~GLHelper(void);
	
	bool isSupported(const fglString& pcName) { return glewIsSupported(pcName.c_str()) ? true : false; }

	void takeCaps(void);

	inline unsigned getMaxLights(void) { return nMaxLights; }
	inline unsigned getMaxElementsVertices(void) { return nMaxElementsVertices; }
	inline unsigned getMaxElementsIndices(void) { return nMaxElementsIndices; }
	inline unsigned getGLVersionMajor(void) { return nGLVersionMajor; }
	inline unsigned getGLVersionMinor(void) { return nGLVersionMinor; }

	void resetLights(void);

	void setVSync(bool plEnable);

	// Returns a hardware accelerated anti-aliasing (MSAA or CSAA) pixel format for current device and rendering context
	// that supports the specified samples per pixel. Functions selects minimum available pixel format and returns it in pnSamples
	// If a matching MSAA or CSAA pixel format was found it will be returned, otherwise '0' will be returned.
	int getAntiAliasingPixelFormat(unsigned& pnSamples, fglString& pcResultDescr);
};


class fglRenderStats
{
private:
	double nPerfTimerFrequency;
	double nTicksPerSecond;

	double nBeginTicks;
	double nEndTicks;
	double nTimeAccum;
	double nFramesAccum;
	
	double nTime;
	double nFPS;
	double nFrameTime;
public:

	fglRenderStats(void);
	~fglRenderStats(void);

	const double& getTime(void) const;
	const double& getFPS(void) const;
	const double& getFrameTime(void) const;
	double getIdleTime(void) const;
	
	void reset(void);
	void beginFrame(void);
	void endFrame(void);

};

#endif