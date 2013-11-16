#ifndef _FGLBIND_H_
#define _FGLBIND_H_
#include "common.h"
#include "pro_ext2.h"
#include "fgl.h"
#include "fglLog.h"

class fglEventParams
{
public:
	FoxParameter* pParameters;
	int nParams;
	Value defVal;
	
	fglEventParams(void): pParameters(NULL), nParams(0) { defVal.ev_type = 'L'; defVal.ev_length = 0; }
	fglEventParams(FoxParameter* ppParameters, int pnParams): pParameters(ppParameters), nParams(pnParams) { defVal.ev_type = 'L'; defVal.ev_length = 0; }
	Value& operator[] (int i) { return i>=nParams ? defVal : pParameters[i].val; }
};

class fglEvent
{
public:
	int name;
	fglEventParams& aParams;
	int nParams; //the number of parameters passed

	fglEvent(int pnName, fglEventParams& paParams, int pnParams=0): name(pnName), aParams(paParams), nParams(pnParams) { }
};


class fglScene;

class fglExtVal
{
private:
	char cType;
	int vI;
	float vF;
	fglString vC;
	bool vL;

public:
	fglExtVal(void) { clear(); }
	~fglExtVal(void) {}
	
	void clear(void) {cType=0; vI=0; vF=0.0f; vC=""; vL=false;}
	
	void setNull(void) { clear(); }
	void setVal(int pVal) { clear(); cType='I'; vI=pVal; }
	void setVal(float pVal) { clear(); cType='F'; vF=pVal; }
	void setVal(const fglString& pVal) { clear(); cType='C'; vC=pVal; }
	void setVal(bool pVal) { clear(); cType='L'; vL=pVal; }

	char getType(void) const { return cType; }
	bool getNull(void) const { return cType==0; }
	int getValI(void) const { return vI; }
	float getValF(void) const { return vF; }
	const fglString& getValC(void) const { return vC; }
	bool getValL(void) const { return vL; }
};

class fglExtProps
{
private:
	typedef fglMap<fglString, fglExtVal> APROPS;
	APROPS aProps;
public:
	fglExtProps(void) {}
	~fglExtProps(void) {}
	
	void set(const fglString& pcName, const fglExtVal& poVal) { aProps[pcName] = poVal; }
	const fglExtVal* get(const fglString& pcName) const { if (aProps.find(pcName) == aProps.end()) return NULL; return &aProps.at(pcName); }
};


class fglBind
{
public:
	typedef fglSEQUENCE_ARRAY<fglREF<fglBind>,2000000> FGLABINDS; 

private:
	
	HFGL hFGL;

	fglREF<fglBind> oRef;
	static FGLABINDS aBinds;
	
	void reg(void) { oRef.bind(this); hFGL = aBinds.add(oRef); }
	
	fglBind(const fglBind& poSrc) {}
	fglREF<fglScene> oScene;

	fglExtProps* aExtProps;

public:

	fglString name;

	static enum {fglDUPLICATE_NEVER=0, fglDUPLICATE_COPY=1, fglDUPLICATE_INSTANCE=2, fglDUPLICATE_ANY=3};
	unsigned duplicateMode;

	virtual fglString fgl_class(void) const { return "bind"; }
	
	fglBind(void);
	
	virtual ~fglBind(void);
	
	const fglBind& operator=(const fglBind& poSrc) {  copy(&poSrc); return *this; }

	HFGL getHFGL(void) const {return hFGL;}

	template<class ttType> 
	const fglREF<ttType>& getRef(void) const { return (fglREF<ttType>&)oRef; }
	const fglREF<fglBind>& getRef(void) const { return getRef<fglBind>(); }
	
	template<class ttType> 
	static const fglREF<ttType>* getBind(HFGL phFGL) 
	{ 
		const fglREF<fglBind>* loBind = aBinds.get(phFGL);
		if (!loBind) return NULL;
		if (dynamic_cast<ttType*>(loBind->map()) == NULL) return NULL;
		return (const fglREF<ttType>*)loBind;
	}
	
	static void unload(void);
	static void unloadSceneObjects(const fglScene* poScene);

	const fglREF<fglScene>& getScene(void) const;
	bool setScene(const fglREF<fglScene>& poScene);
	
	fglExtProps& getExtProps(void);
	void setExtProps(const fglExtProps* poExtProps);
	
	virtual void call(fglEvent& poEvent);

	void setSceneRenderFlag(void);

	virtual bool copy(const fglBind* poSrc, fglBITFLAG pnShare=0);

	template <class ttType>
	static const ttType* duplicate(const ttType* poSrc, unsigned pnDuplicateMode=1, fglBITFLAG pnShare=0)
	{
		if ( ! dynamic_cast<const fglBind*>(poSrc) ) return NULL;
		unsigned lnDuplicateMode = poSrc->duplicateMode;
		if (lnDuplicateMode == fglDUPLICATE_ANY) lnDuplicateMode = pnDuplicateMode;
		if (lnDuplicateMode == fglDUPLICATE_NEVER) return NULL;
	
		if (lnDuplicateMode == fglDUPLICATE_COPY)
		{
			fglBind* loBind = fglScene::createObject(poSrc->fgl_class());
			if ( ! loBind ) return NULL;
			ttType* loDuplicate = dynamic_cast<ttType*>(loBind);
			if ( ! loDuplicate ) { delete loBind; return NULL; }
			loDuplicate->copy(poSrc, pnShare);
			return loDuplicate;
		}
		else if (lnDuplicateMode == fglDUPLICATE_INSTANCE)
		{
			return poSrc;
		}
		return NULL;
	}

};



template<class ttType> class fglCollection : public fglBind
{
private:
	typedef fglArray<fglREF<ttType>> AITEMS;
	typedef fglUMap<const void*, unsigned> AITEMSHASH;
	fglREF<AITEMS> aItems;
	//fglREF<AITEMSHASH> aItemsHash;

public:
	
	fglCollection(void) { }
	virtual ~fglCollection(void) { clear(); }
	
	typedef typename AITEMS::const_iterator iterator;
	
	inline const fglREF<ttType>& operator[] (unsigned pIndex) const { return get(pIndex); }

	virtual bool add(const fglREF<ttType>& poItem) 
	{ 
		if (dynamic_cast<ttType*>(poItem.map())==NULL) return false;
		if (aItems.empty()) aItems.create();
		
		AITEMS::iterator iItem = std::find(aItems->begin(), aItems->end(), poItem);
		if (iItem != aItems->end()) return false;

		aItems->push_back(poItem);

		return true;
	}
	virtual bool del(const fglREF<ttType>& poItem) 
	{ 
		if (aItems.empty()) return false;
				
		AITEMS::iterator iItem = std::find(aItems->begin(), aItems->end(), poItem);
		if (iItem == aItems->end()) return false;
		aItems->erase(iItem);
		return true;
	}
	
	inline const fglREF<ttType>& get(unsigned pIndex) const { return aItems->at(pIndex); }
	const fglREF<ttType>& getsafe(unsigned pIndex) const
	{ 
		static fglREF<ttType> loEmpty;
		if (aItems.empty()) return loEmpty; 
		if (pIndex<0 || pIndex>=size()) return loEmpty; 
		return get(pIndex);
	}

	virtual int find (const fglREF<ttType>& poItem) const 
	{ 
		if (poItem.empty()) return -1;
		if (aItems.empty()) return -1;

		AITEMS::iterator iItem = std::find(aItems->begin(), aItems->end(), poItem);
		if (iItem == aItems->end()) return -1;
		return std::distance(aItems->begin(), iItem);
	}

	iterator begin(void) const { if (aItems.empty()) return iterator(); return aItems->cbegin(); }
	iterator end(void) const { if (aItems.empty()) return iterator(); return aItems->cend(); }

	inline unsigned size(void) const { if (aItems.empty()) return 0; return aItems->size(); }
	inline bool empty(void) const { return size()==0; }
	inline bool exist(void) const  { return size()!=0; }
	virtual void clear(void) { if (aItems.exist()) aItems->clear(); }
	void reserve(unsigned pnSize) 
	{ 
		if (pnSize==0) return; 
		if (aItems.empty()) aItems.create();
		aItems->reserve(pnSize); 
	}
	
	virtual bool copy(const fglBind* poSrc, fglBITFLAG pnShare=0) 
	{
		if ( !fglBind::copy(poSrc, pnShare) ) return false;
		const fglCollection* loSrc = dynamic_cast<const fglCollection*>(poSrc);
		if (!loSrc) return false;

		if (pnShare.get())
		{
			aItems.inst(&loSrc->aItems); 
		}
		else
		{
			aItems.copy(&loSrc->aItems);
		}
		return true;
	}

	virtual void call(fglEvent& poEvent)
	{
		if (poEvent.name == fglEVENT_add) 
		{ 
			_RetValue2l(0); 
			if (!poEvent.nParams) return;
			const fglREF<ttType>* loItem = getBind<ttType>(_GetValue2i(poEvent.aParams[0]));
			if (loItem) _RetValue2l(add(*loItem)); 
			return;
		}
		if (poEvent.name == fglEVENT_del) 
		{ 
			_RetValue2l(0); 
			if (!poEvent.nParams) return;
			const fglREF<ttType>* loItem = getBind<ttType>(_GetValue2i(poEvent.aParams[0]));
			if (loItem) _RetValue2l(del(*loItem)); 
			return;
		}
		if (poEvent.name == fglEVENT_reserve) { if (poEvent.nParams) reserve(_GetValue2i(poEvent.aParams[0])); return;}
		if (poEvent.name == fglEVENT_clear) { clear(); return;}
		if (poEvent.name == fglEVENT_size) { _RetValue2i(size()); return;}

		if (poEvent.name == fglEVENT_find) 
		{ 
			_RetValue2i(-1);
			if (!poEvent.nParams) return;
			const fglREF<ttType>* loItem = getBind<ttType>(_GetValue2i(poEvent.aParams[0]));
			if (loItem) _RetValue2i(find(*loItem)); 
			return;
		}

		if (poEvent.name == fglEVENT_get) 
		{ 
			fglBind* loItem(NULL); 
			if (poEvent.nParams) loItem = dynamic_cast<fglBind*>(getsafe(_GetValue2i(poEvent.aParams[0])).map()); 
			_RetValue2i(loItem ? loItem->getHFGL() : 0); 
			return;
		}

		fglBind::call(poEvent);
	}

};


#endif