#ifndef _FGLTEXT_H_
#define _FGLTEXT_H_
#include "fglSceneGraph.h"
#include "fglMesh.h"

#define FREETYPE2_STATIC
#define USE_FREETYPE2_STATIC
#include <ft2build.h>
#include FT_FREETYPE_H
#pragma comment (lib,"freetype.lib")

class fglFontGlyph : public fglMesh
{

public:
	fglFontGlyph(void);
	~fglFontGlyph(void);

	virtual fglString fgl_class(void) const { return "fontglyph"; }

	float getWidth(void) const;
	float getHeight(void) const;
	float getAdvanceHor(void) const;
	float getAdvanceVert(void) const;
	
	void load(const fglWChar& pChar, const FT_Face poFace);


private:
	float width;
	float height;
	float advanceHor;
	float advanceVert;
	
	class contour
	{
	public:
		typedef fglArray<fglVECTOR> APOINTS;
		APOINTS aPoints;
		APOINTS aPointsOutset;
		bool isClockwise;

		contour(const FT_Vector* paPoints, const char* paTags, unsigned pnPoints);
		void addPoint(const fglVECTOR& poPoint);
		void addQuadraticBezierCurve(const fglVECTOR& a, const fglVECTOR& b, const fglVECTOR& c);
		void addCubicBezierCurve(const fglVECTOR& a, const fglVECTOR& b, const fglVECTOR& c, const fglVECTOR& d);
		void setParity(unsigned pnParity);
		fglVECTOR calcOutsetPoint(fglVECTOR a, fglVECTOR b, fglVECTOR c);
	};

	class tessVert
	{ 
	public: 
		double x; 
		double y;  
		double z; 
		tessVert(const double& px, const double& py, const double& pz) { x=px; y=py; z=pz; } 
		tessVert(void) { x=y=z=0; } 
	};

	static void CALLBACK tessError(unsigned pnErrCode, fglFontGlyph* mesh);
	static void CALLBACK tessVertex(void* pData, fglFontGlyph* poMesh);
	static void CALLBACK tessCombine(double poNewVert[3], void* paVerts[4], float paVertsWeight[4], void** pOutData, fglFontGlyph* poMesh);
	static void CALLBACK tessBegin(unsigned pnType, fglFontGlyph* poMesh);
	static void CALLBACK tessEnd(fglFontGlyph* poMesh);
	
	fglArray<tessVert> aTessVerts;
	unsigned nTessStep;
	unsigned nTessSubMeshVerts;
	unsigned nTessSubMeshType;
	unsigned nTessVerts;
	unsigned nTessFaces;

};


class fglFont
{
public:

	fglFont(void);

	virtual ~fglFont(void);

	void load(const fglString& pcFontPath, unsigned pnType, unsigned pnSize, float pnOutset=0.0f, float pnDepth=0.0f);
	const fglFontGlyph* getGlyph(fglWChar pChar);

	void setScene(const fglREF<fglScene>& poScene);

	float getAscender(void) const;
	float getDescender(void) const;
	float getHeight(void) const;
	float getMaxAdvance(void) const;

private:
	fglFont(const fglFont&);
	const fglFont& operator = (const fglFont&);
	
	FT_Face oFace;
	fglREF<fglScene> oScene;

	typedef fglUMap<fglWChar, fglFontGlyph> AGLYPHS;
	AGLYPHS aGlyphs;

	float ascender;
	float descender;
	float height;
	float maxAdvance;
};


class fglFontManager
{
private:
	typedef fglMap<fglString, fglFont*> AFONTS;

	fglFontManager(const fglFontManager&);
	const fglFontManager& operator = (const fglFontManager&);

	AFONTS aFonts;

	fglREF<fglScene> oScene;

public:

	fglString fontspath;

	fglFontManager(void);

	virtual ~fglFontManager(void);

	fglFont* getFont(const fglString& pcFontName, unsigned pnType, unsigned pnSize, float pnOutset=0.0f, float pnDepth=0.0f);

	void setScene(const fglREF<fglScene>& poScene);
};


class fglText : public fglVisual
{
private:
	fglFont* oFont;

	unsigned fontType;
	fglString fontName;
	unsigned fontSize;
	unsigned alignment;
	float lineLength;
	float lineSpacing;
	float outset;
	float depth;

	fglString textA;
	fglWString textW;
		
	class fglTextLine
	{
	public:
		float left;
		float top;
		float width;
		float height;
		float justifySpacing;
		
		typedef fglArray<const fglFontGlyph*> AGLYPHS;
		AGLYPHS aGlyphs;
		
		fglTextLine(void) { left = top = width = height = justifySpacing = 0.0f; }
	};

	typedef fglArray<fglTextLine> ALINES;
	ALINES aLines;
	

public:

	static enum {TEXTURE=1, POLYGON, OUTLINE, EXTRUDED};
	static enum {ALIGN_LEFT=1, ALIGN_CENTER, ALIGN_RIGHT, ALIGN_JUSTIFY};
	
	virtual fglString fgl_class(void) const { return "text"; }
	virtual bool copy(const fglBind* poSrc, fglBITFLAG pnShare=0);

	fglText(void);
	virtual ~fglText(void);

	virtual void calcBounds(void);

	void build(void);
	
	static void renderText(const fglText* poText);
			
	virtual void call(fglEvent& poEvent);

	fglString toString(void) 
	{ 
		std::ostringstream lcRes; 
		lcRes << "fontName: " << fontName << std::endl; 
		lcRes << "fontType: " << fontType << std::endl; 
		lcRes << "fontSize: " << fontSize << std::endl; 
		lcRes << "alignment: " << alignment << std::endl; 
		lcRes << "lineLength: " << lineLength << std::endl; 
		lcRes << "lineSpacing: " << lineSpacing << std::endl; 
		return lcRes.str(); 
	}
};


#endif