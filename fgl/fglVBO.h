#ifndef _FGLVBO_H_
#define _FGLVBO_H_

#include "fgl.h"
#include "fglMem.h"

///////////////////////////////////////////////////////////////////////////
#define fglVBO_ID unsigned
#define fglVBO_VERTICES GL_ARRAY_BUFFER
#define fglVBO_INDICES GL_ELEMENT_ARRAY_BUFFER_ARB
#define fglVBO_READ_ONLY GL_READ_ONLY
#define fglVBO_WRITE_ONLY GL_WRITE_ONLY
#define fglVBO_READ_WRITE GL_READ_WRITE
#define fglVBO_STATIC GL_STATIC_DRAW
#define fglVBO_DYNAMIC GL_DYNAMIC_DRAW

class fglVBO;
///////////////////////////////////////////////////////////////////////////
class fglVBOPoolSubBlock
{
	friend class fglVBOPool;
	friend class fglVBOPoolBlock;
	friend class fglVBO;
private:
	fglVBO_ID id;
	unsigned target;
	unsigned usage;
	unsigned offset;
	unsigned size;
	void* pMap;

	fglVBOPoolSubBlock(const fglVBOPoolSubBlock&) {}

public:
	fglVBOPoolSubBlock(void);
	~fglVBOPoolSubBlock(void);

	void init(fglVBO_ID pnId, unsigned pnTarget, unsigned pnUsage, unsigned pnOffset, unsigned pnSize);

	bool bind(void) const;

	bool getData(void* pData) const;

	void* map(unsigned pnAccess);
	void unmap(void);
};

///////////////////////////////////////////////////////////////////////////
class fglVBOPoolBlock
{
	friend class fglVBOPool;
private:
	fglVBOPoolBlock(const fglVBOPoolBlock&) {}

	fglVBO_ID id;
	unsigned target;
	unsigned usage;
	unsigned size;
	unsigned freeOffset;
	unsigned freeSize;

public:
	fglArray<fglREF<fglVBOPoolSubBlock>> aSubBlocks;

	fglVBOPoolBlock(void);
	~fglVBOPoolBlock(void);

	void init(unsigned pnTarget, unsigned pnUsage, unsigned pnSize);
	void addSubBlock(fglVBO& poVBO, unsigned pnSize);
};

///////////////////////////////////////////////////////////////////////////
class fglVBOPool
{
private:
	fglArray<fglREF<fglVBOPoolBlock>> aBlocks;
	void reorganizeBlocks(void);

	fglVBOPool(const fglVBOPool&) {}

public:

	fglVBOPool(void);
	~fglVBOPool(void);

	void setData(fglVBO& poVBO, unsigned pnSize, const void* pData, unsigned pnUsage);
	void freeData(fglVBO& poVBO);

};

///////////////////////////////////////////////////////////////////////////
class fglScene;

class fglVBO 
{
	friend class fglVBOPool;
	friend class fglVBOPoolBlock;

private:
	fglVBO(const fglVBO&) { }
	fglREF<fglScene> oScene;
	fglREF<fglVBOPoolSubBlock> oSubBlock;

protected:
	unsigned target;
	
public:
	fglVBO(void);
	virtual ~fglVBO(void);

	void setScene(const fglREF<fglScene>& poScene);

	fglVBO_ID getId(void) const;
	unsigned getOffset(void) const;
	unsigned getSize(void) const;
	unsigned getUsage(void) const;

	bool bind(void) const;
	static void unbind(unsigned pnTarget);

	void setData(unsigned pnSize, const void* pData, unsigned pnUsage);
	bool getData(void* pData) const;

	void* map(unsigned pnAccess=fglVBO_READ_WRITE);
	void unmap(void);

	void clear(void);

};

///////////////////////////////////////////////////////////////////////////
class fglVBO_Array : public fglVBO
{
private:
	fglVBO_Array(const fglVBO_Array&) {}

public:
	fglVBO_Array(void);
};

class fglVBO_Element_Array : public fglVBO
{
private:
	fglVBO_Element_Array(const fglVBO_Element_Array&) {}
public:
	fglVBO_Element_Array(void);
};



#endif
