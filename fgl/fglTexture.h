#ifndef _FGLTEXTURE_H_
#define _FGLTEXTURE_H_

#include "common.h"
#include "fglBind.h"

#define fglTEXTURE_ID unsigned

class fglTexture : public fglBind
{
private:
	fglTEXTURE_ID id;
public:
	fglString path;
	float blend; //Defines the strength the n'th texture on the stack 't'. All color components (rgb) are multipled with this factor *before* any further processing is done. 

	fglTexture(void);
	virtual ~fglTexture(void);
	virtual fglString fgl_class(void) const { return "texture"; }

	inline bool isReady(void) const { return id!=0; }

	void build(void);
	void bind(void);
	static void unbind(void);
};

#endif