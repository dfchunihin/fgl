#ifndef _FGLSCENEGRAPH_H_
#define _FGLSCENEGRAPH_H_

#include "common.h"
#include "fgl3DGen.h"
#include "fglBind.h"
#include "fglMaterial.h"
#include "fglAnim.h"

class fglViewport;


class fglVisual : public fglBind
{
public:

	fglBBox bbox;
	fgl3DRect bboxInt;
	fgl3DRect bboxExt;
	
	
	float viewRangeMin;
	float viewRangeMax;
	fgl3DRect viewRangeRot;

	static enum { nStretchNone = 0, nStretchClip = 1, nStretchRepeat = 2, nStretchFit = 4, nStretchProper = 8 };
	fglBITFLAG nStretch; 
	fglREF<fglCLIP> oStretchClip;

	fglREF<fglMaterial> oMaterial;

	fglCollection<fglVisual> aLODs;
	
	fglVisual(void);
	virtual ~fglVisual(void);

	virtual fglString fgl_class(void) const { return "visual"; }
	virtual bool copy(const fglBind* poSrc, fglBITFLAG pnShare=0);
	void copyLODs(const fglVisual* poVisual, fglBITFLAG pnShare=0);
	
	virtual void calcBounds(void);

	//virtual void renderMesh(void) const;
	static void renderVisual(const fglVisual* poVisual);
	void (*renderImpl)(const fglVisual*);

	void render(void) const;
	const fglVisual* renderGetLOD(void) const;
	void renderStretch(void) const;
	
	virtual bool testIntersection(fglRAY& poRay, fglVECTOR* poPointIn=NULL) const;
	bool testInFrustum(const fglViewport& poViewport, const fglTRANSFORM& poTRG) const;
	bool testIsVisible(const fglViewport& poViewport, const fglTRANSFORM& poTRG) const;

	virtual void call(fglEvent& poEvent);
};


class fglNode : public fglBind
{
public:
	template <class ttType> class ACHILDS : public fglCollection<ttType>
	{
	public:
		fglNode* oParent;

		ACHILDS(void) { oParent=NULL; }
		virtual ~ACHILDS(void) {}
		virtual bool add(const fglREF<ttType>& poItem)
		{
			if ( !fglCollection<ttType>::add(poItem) ) return false;
			if (oParent) if ( ! poItem->setParentValue(&oParent->getRef<ttType>()) ) { del(poItem); return false; }
			return true;
		}
		virtual bool del(const fglREF<ttType>& poItem)
		{
			if ( !fglCollection<ttType>::del(poItem) ) return false;
			if (poItem.exist()) poItem->setParent(NULL);
			return true;
		}
		virtual bool copy(const fglBind* poSrc, fglBITFLAG pnShare=0) { return false; }
		virtual void clear(void) { while (size()) del(get(0)); fglCollection<ttType>::clear(); }
	};


private:
	fglREF<fglNode> oParent;

protected:
	bool setParentValue(const fglREF<fglNode>* poParent);

public:
	ACHILDS<fglNode> aChilds;
	fglCollection<fglAnimChanEval> aAnimChans; 
	fglCollection<fglVisual> aVisuals; 

	const fglREF<fglNode>& getParent(void) const { return oParent; }
	virtual bool setParent(const fglREF<fglNode>* poParent);
	const ACHILDS<fglNode>& getChilds(void) const { return aChilds; }
	
	template <class ttType> 
	const fglREF<ttType>* find(const fglString& pcName) const;
	const fglREF<fglNode>* find(const fglString& pcName) const { return find<fglNode>(pcName); }

	fglTRANSFORM tr;
	fglTRANSFORM trg;
	fglMAT4 trgMatrix;
	fglTRANSFORMConstraints trgConstraints;
	fglVECTOR oLookVector;
	fglVECTOR oLookVectorLocal;
	fglVECTOR oLookVectorGlobal;
	fglBBox bbox;
	fgl3DRect bboxExt;
	bool isEnabled;
	bool isVisible;
	
	fglNode(void);
	virtual ~fglNode(void);
	
	virtual fglString fgl_class(void) const { return "node"; }
	virtual bool copy(const fglBind* poSrc, fglBITFLAG pnShare=0);

	virtual void buildTransform(void);
	void calcBounds(void); 
	
	bool testInFrustum(const fglViewport& poViewport) const;

	void getVisuals(const fglString& pcVisualName, const fglString& pcMaterialName, fglArray<fglVisual*>& paResult);
	void getMaterials(const fglString& pcVisualName, const fglString& pcMaterialName, fglArray<fglMaterial*>& paResult);

	virtual void call(fglEvent& poEvent);

	void rotateBy(fglQUAT poRot, const fglVECTOR* poCenter=NULL) { tr.rotateBy(poRot, poCenter); buildTransform(); }
	
	void rotateTo(const fglQUAT& poRot, const fglVECTOR* poCenter=NULL) { tr.rotateTo(poRot, poCenter); buildTransform(); }
	
	void moveBy(const fglVECTOR& poMove) { tr.moveBy( poMove ); buildTransform(); }

	void moveDir(const fglVECTOR& poMove) { tr.moveDir( poMove ); buildTransform(); }

	void moveTo(const fglVECTOR& poMove) { tr.moveTo(poMove); buildTransform(); }

	void scaleBy(const fglVECTOR& poScale) { tr.scaleBy(poScale); buildTransform(); }
	
	void scaleAt(const fglVECTOR& poScale) { tr.scaleAt(poScale); buildTransform(); }
	
	void scaleTo(const fglVECTOR& poScale) { tr.scaleTo(poScale); buildTransform(); }

	void lookAt(const fglVECTOR& poLookAt, const float& pnDist=0.0f);

	bool getLookAtPlane(const fglROW4& poPlane, fglVECTOR& poLookAt) const;

};

template <class ttType> 
const fglREF<ttType>* fglNode::find(const fglString& pcName) const
{
	if (name == pcName) return &getRef<ttType>();
	ttType* loNode = NULL;
	const fglREF<ttType>* loFind;
	for (unsigned i=0; i!=aChilds.size(); i++) 
	{
		if ( (loNode = dynamic_cast<ttType*>(aChilds[i].map()))==NULL ) continue;
		if ( loFind = loNode->find<ttType>(pcName) ) return loFind;
	}
	return NULL;
};
/////////////////////////////////////////////////////////////////////////////////////////////


class fglWalkNodes
{
private:
	unsigned aStack[10000];
	unsigned nStack;

public:
	fglTRANSFORM currentView;
	fglMAT4 currentViewMatrix;


	bool (*onNode)(fglWalkNodes*, fglNode*);
	void (*onVisual)(fglWalkNodes*, fglNode*, fglVisual*);
	fglWalkNodes(void) { onNode=NULL; onVisual=NULL; nStack=0; }
	
	static void walkNodes(fglNode* poNode, fglWalkNodes* poHandler);
	
};

#endif