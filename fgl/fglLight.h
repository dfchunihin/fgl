#ifndef _FGLLIGHT_H_
#define _FGLLIGHT_H_

#include "fglSceneGraph.h"
#include "fglCam.h"

class fglLight : public fglNode
{
public:
	fglCOLOR ambient;
	fglCOLOR diffuse;
	fglCOLOR specular;
	
	bool isSpot;
	float spotCutoff;
	float spotExp;
	
	float atten;
	float radius;
	float squareRadius;


	fglLight(void);
	virtual ~fglLight(void);

	virtual fglString fgl_class(void) const { return "light"; }
	virtual void call(fglEvent& poEvent);
	void setLight(unsigned piLight, const fglTRANSFORM& poTrgInv);
	
	bool testInFrustum(const fglFRUSTUM& poFrustum) const;
};

#endif
