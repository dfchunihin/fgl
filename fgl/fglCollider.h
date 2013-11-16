#ifndef _FGLCOLLIDER_H_
#define _FGLCOLLIDER_H_

#include "common.h"
#include "fglSceneGraph.h"

class fglCollideInfo : public fglBind
{
public:
	class fglCollidePair
	{
	public:
		bool operator==(const fglCollidePair& poOther) const { return (oNode1==poOther.oNode1 || oNode1==poOther.oNode2 || oNode2==poOther.oNode1 || oNode2==poOther.oNode2); }
		
		fglREF<fglNode> oNode1;
		fglREF<fglNode> oNode2;
		float nDist;

		fglCollidePair(void) { clear(); }
		void clear(void) { nDist = fglINFINITE; oNode1.clear(); oNode2.clear(); }
	};

	typedef fglArray<fglCollidePair> APAIRS;
	APAIRS aPairs;
	fglREF<fglNode> oNode1;
	fglREF<fglNode> oNode2;
	float nMinDist;
						
	fglCollideInfo(void) { clear(); }
	virtual ~fglCollideInfo(void) { }

	virtual void call(fglEvent& poEvent);
	virtual fglString fgl_class(void) const { return "collideinfo"; }

	void clear(void) { nMinDist = fglINFINITE; aPairs.clear(); oNode1.clear(); oNode2.clear(); }

	bool add(const fglCollidePair& poPair) 
	{ 
		APAIRS::iterator iPair = std::find(aPairs.begin(), aPairs.end(), poPair);
		if (iPair != aPairs.end()) return false;
		aPairs.push_back(poPair);
		return true;
	}
	bool del(const fglCollidePair& poPair) 
	{ 
		if (aPairs.empty()) return false;
		APAIRS::iterator iPair = std::find(aPairs.begin(), aPairs.end(), poPair);
		if (iPair == aPairs.end()) return false;
		aPairs.erase(iPair);
		return true;
	}
	int find(const fglNode& poNode) const
	{ 
		unsigned i=0;
		APAIRS::const_iterator iPair = aPairs.begin(), iPairEnd = aPairs.end();
		for (; iPair!=iPairEnd; ++iPair)
		{
			if (iPair->oNode1 == poNode.getRef()) return i;
			if (iPair->oNode2 == poNode.getRef()) return i;
			i++;
		}
		return -1;
	}

	const fglCollidePair* get(int pIndex) const
	{ 
		if (pIndex<0 || pIndex>=(int)size()) { throw std::exception("Index is out of array bounds"); return NULL; }
		return &aPairs[pIndex];
	}
		
	bool empty(void) const { return aPairs.empty(); }
	unsigned size(void) const { return aPairs.size(); }
};

class fglCollider : public fglBind
{
public:

	class ANODES : public fglCollection<fglNode>
	{
	public:
		
		ANODES(void) {}
		virtual ~ANODES(void) {}
		virtual void call(fglEvent& poEvent)
		{
			if (poEvent.name == fglEVENT_add) 
			{ 
				if ( poEvent.nParams )
				{
					const fglREF<fglCollection<fglNode>>* laNodes = getBind<fglCollection<fglNode>>(_GetValue2i(poEvent.aParams[0]));
					if (laNodes) 
					{
						for (unsigned i=0; i!=laNodes->map()->size(); i++) add(laNodes->map()->get(i));
						return;
					}
				}
			}
			fglCollection<fglNode>::call(poEvent);
		}
	};

	ANODES aNodes;

	fglCollider(void);
	virtual ~fglCollider(void);

	virtual fglString fgl_class(void) const { return "collider"; }
	virtual void call(fglEvent& poEvent);
	bool testPair(const fglNode& poNode1, const fglNode& poNode2, fglCollideInfo::fglCollidePair* poPair) const;
	bool testNode(const fglNode& poNode, fglCollideInfo* poInfo) const;

};



#endif