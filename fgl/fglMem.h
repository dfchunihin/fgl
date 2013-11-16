#ifndef _FGLMEM_H_
#define _FGLMEM_H_

#include "common.h"
#include "fgl.h"
#include "fglMutex.h"
#include "fglLog.h"


class fglBITFLAG 
{
private:
	unsigned val;
public:
	fglBITFLAG(unsigned pnVal=0) {val=pnVal;}
	virtual ~fglBITFLAG(void) {}
	fglBITFLAG& operator =(unsigned pnVal) {val=pnVal; return *this;}
	
	unsigned get(void) const { return val; }
	bool check(unsigned pnVal) const { return (val & pnVal)!=0; }
	fglBITFLAG& set(unsigned pnVal) { val |= pnVal; return *this; }
	fglBITFLAG& reset(unsigned pnVal) { val = val & ~pnVal; return *this; }
};


template <class ttType> class fglREF
{
private:

	class fglREFData
	{
	public:
		ttType* p;
		unsigned int n;
		unsigned int nRef;
		unsigned isLock;
		
		fglREFData(void): p(NULL), n(0), nRef(0), isLock(0) { }
		~fglREFData(void) {}
	};

	fglREFData* data;

	inline void addRef(void) 
	{
		if (!data) data = new fglREFData();
		data->nRef++;
	}
	
	inline void err(void) const { throw fglException("fglREF is empty"); }
	inline void null(void) { data=NULL; }
	
public:
	fglREF(void) { null(); }
	
	fglREF(const fglREF<ttType>& poSrc) { null(); inst(&poSrc); }
	fglREF (const ttType* poSrc, unsigned pnCount=1) { bind(poSrc, pnCount); }
	
	template<class Other>fglREF(const fglREF<Other>& poSrc) { null(); inst((fglREF<ttType>*)(&poSrc)); }
	
	inline const fglREF<ttType>& operator=(const fglREF<ttType>& poSrc) { inst(&poSrc); return *this;}
	
	inline bool operator==(const fglREF<ttType>& poOther) const { if (data==poOther.data) return true; if (data && poOther.data) if (data->p == poOther.data->p) return true; return false; }
	inline bool operator!=(const fglREF<ttType>& poOther) const { if (data==poOther.data) return false; if (data && poOther.data) if (data->p == poOther.data->p) return false; return true; }
	
	virtual ~fglREF(void) { release();  }
	
	inline ttType* map(void) const { if (!data) return NULL; return data->p; }
	inline ttType* map(unsigned pIndex) const { if (!data) return NULL; return &data->p[pIndex]; }
	inline ttType* operator->(void) const { return data->p; }
	inline ttType& operator[] (unsigned pIndex) const { return get(pIndex); }
	inline ttType& get(unsigned pIndex=0) const { return *map(pIndex); }
	inline const void* getRefData(void) const { return data; }

	inline const unsigned int size(void) const { if (!data) return 0; return data->n; }
	inline const bool empty(void) const {return map()==NULL;}
	inline const bool exist(void) const {return map()!=NULL;}
	
	void lock(void) { if (data) data->isLock=1; }
	void unlock(void) { if (data) data->isLock=0; }
	bool isLock(void) const { return data ? data->isLock : false;  }

	void expired(void) { if (data) {data->p = NULL; data->n=0;} }
	void releasedata(void)
	{
		if (!data) return;
		if (data->nRef==0) unlock();
		if (data->isLock) return;
		ttType* lp = data->p;
		unsigned ln = data->n;
		expired();
		if (lp) {if (ln == 1) delete lp; else delete [] lp;}
		release();
	}

	void release(void) 
	{
		if (!data) return;
		if (data->nRef==0) return;
		data->nRef--;
		if (data->nRef==0) { releasedata(); delete data; }
		null();
	}

	void inst(const fglREF<ttType>* poSrc) 
	{
		if (poSrc==this) return;
		release(); 
		if (!poSrc) return; 
		if (!poSrc->data) return;

		data = poSrc->data;
		addRef();
	}

	void bind(const ttType* poSrc, unsigned pnCount=1) 
	{
		if (exist()) return;
		if ((void*)poSrc==(void*)this) return;
		release(); 
		addRef();
		data->p = (ttType*)( poSrc );
		data->n = pnCount;
		lock();
	}

	void create(unsigned int pnCount=1)
	{
		release();
		if (pnCount==0) return;
		addRef();		
		data->n = pnCount;
		if (pnCount==1) data->p = new ttType(); else data->p = new ttType[pnCount];
		lock();
	}

	void set_size(unsigned int pnCount) { create(pnCount); }
	void clear(void) { set_size(0); }

	void copy(const fglREF<ttType>* poSrc) 
	{
		if (!poSrc) return;
		if (map()==poSrc->map()) return;
		if (poSrc->empty()) return;
		if (empty()) create(poSrc->size());
		for (unsigned int i=0; i<poSrc->size(); i++) map()[i] = poSrc->map()[i];
	}
};


class fglSEQUENCE
{
private:
	HFGL n;
	fglSEQUENCE(const fglSEQUENCE&); //nocopyable
	const fglSEQUENCE& operator=(const fglSEQUENCE&); //nocopyable
protected:
	mutable fglMutex oMutex;
public:
	fglSEQUENCE(void) { n = 0; }
	virtual ~fglSEQUENCE(void) {}

	HFGL next(void)
	{
		HFGL lnNext;
		oMutex.lock();
		lnNext = ++n;
		oMutex.unlock();
		return lnNext;
	}

	HFGL last(void) const
	{
		HFGL lnLast;
		oMutex.lock();
		lnLast = n;
		oMutex.unlock();
		return lnLast;
	}
};

template <class ttType, int tnSize=100> 
class fglSEQUENCE_ARRAY : private fglSEQUENCE
{
private:
	fglArray<ttType> aSeq;

public:
	fglSEQUENCE_ARRAY(void) {aSeq.reserve(tnSize);}
	virtual ~fglSEQUENCE_ARRAY(void) { }
	
	HFGL add(const ttType& pVal) 
	{
		HFGL lhFGL = next();
		oMutex.lock();
		aSeq.push_back(pVal);
		oMutex.unlock();
		return lhFGL;
	}

	const ttType* get(HFGL phFGL) const
	{
		if (phFGL==0) return NULL;
		if (phFGL > aSeq.size()) return NULL;
		return &aSeq[phFGL-1];
	}
	unsigned size(void) const { return aSeq.size(); }
};

template <class ttType> 
class fglSEQUENCE_UMAP : private fglSEQUENCE
{
private:
	fglUMap<HFGL,ttType> aSeq;

public:
	fglSEQUENCE_UMAP(void) {}
	virtual ~fglSEQUENCE_UMAP(void) { }
	
	HFGL add(const ttType& pVal) 
	{
		HFGL lhFGL = next();
		oMutex.Lock();
		aSeq[lhFGL] = pVal;
		oMutex.Unlock();
		return lhFGL;
	}

	const ttType* get(HFGL phFGL) const
	{
		if (phFGL==0) return NULL;
		return &aSeq[phFGL];
	}
	unsigned size(void) const { return aSeq.size(); }
};


#endif
