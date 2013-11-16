#ifndef _FGLSHADER_H_
#define _FGLSHADER_H_

#include "common.h"
#include "fgl3DGen.h"
#include "fglBind.h"

class fglShader : public fglBind
{
private:
	unsigned int id;
	unsigned int nType;
	bool isCompiled;
public:
	static enum {VERTEX = 1, FRAGMENT, GEOMETRY};
	fglShader(void);
	virtual ~fglShader(void);
	
	virtual fglString fgl_class(void) const { return "shader"; }
	void create(unsigned int pnType);
	void release(void);
	bool isReady(void) { return (id!=0 && isCompiled); }

	unsigned int getId(void) {return id;}

	void build(unsigned pnType, const fglString& pcPathOrData);

	virtual void call(fglEvent& poEvent);
};

class fglShaderProg : public fglBind
{
private:
	int id;
	bool isLinked;

public:
	fglShaderProg(void);
	virtual ~fglShaderProg(void);
	virtual fglString fgl_class(void) const { return "shaderprog"; }

	void release(void);
	bool isReady(void) { return isLinked; }
	unsigned int getId(void) {return id;}

	void attach(fglShader& poShader);
	void detach(fglShader& poShader);
	void build(void);
	void bind(void);
	static void unbind(void);

	virtual void call(fglEvent& poEvent);
};

#endif