#ifndef _FGLSKEL_H_
#define _FGLSKEL_H_

#include "fglSceneGraph.h"

class fglJoint : public fglNode
{
public:	
	fglJoint(void) {}
	virtual ~fglJoint(void) {}
	virtual fglString fgl_class(void) const { return "joint"; }

	virtual bool setParent(const fglREF<fglNode>* poParent);

	virtual bool copy(const fglBind* poSrc, fglBITFLAG pnShare=0);
};


struct fglBONE_INDEX_WEIGHT
{
public:
	fglINDEX index;
	float weight;
};

class fglBone : public fglNode
{
public:
	fglREF<fglBONE_INDEX_WEIGHT> aVerts;
		
	fglBone(void) {}
	virtual ~fglBone(void) {}
	virtual fglString fgl_class(void) const { return "bone"; }
	
	virtual bool setParent(const fglREF<fglNode>* poParent);
	virtual bool copy(const fglBind* poSrc, fglBITFLAG pnShare=0);
};

#endif