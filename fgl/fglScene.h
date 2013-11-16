#ifndef _FGLSCENE_H_
#define _FGLSCENE_H_

#include "common.h"
#include "fglMutex.h"
#include "fglBind.h"
#include "fglSceneGraph.h"
#include "fglViewport.h"
#include "fglCam.h"
#include "fglModel.h"
#include "fglText.h"
#include "GLHelper.h"
#include "fgl3DPrimitives.h"


class fglScene : public fglViewport
{
private:
	HWND hWnd;
	HWND hWndParent;

	HDC hDC;
	HGLRC hRC;

	unsigned MSAAMode; //multisample anti-aliasing mode (0 - disabled, 2,4,8,16 - valid values)
	fglString MSAAModeInfo; //MSAA mode description
	
	bool FLAG_RENDER;

	bool vsync;

	static LRESULT CALLBACK wndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	static fglUMap<HWND, fglREF<fglScene>> aSceneWindows;

	fglShaderProg oDefaultShaderProg;
	fglShader oDefaultShaderVert;
	fglShader oDefaultShaderFrag;

public:
	
	static fglBind* createObject(fglString pcClassname);
	
	fglLog oLog;
	fglRenderStats oRenderStats;

	static GLHelper oGLHelper;
	fgl3DPrimitives o3DPrimitives;
	fglVBOPool oVBOPool;

	static const fglTRANSFORM identTrans;
	static const fglMAT4 identTransMatrix;
		
	fglRECT oWindowRect;
	fglFontManager oFontManager;
	
	fglCollection<fglViewport> aViewports;

	////////////////////////////////////////////////////
	static fglScene* oCurrentScene;
	static HGLRC hCurrentContext;
	void seCurrentContext(void);

	static fglViewport* oRenderViewport;
	static fglCam* oRenderCam;
	typedef fglArray<fglLight*> ARENDERLIGHTS;
	static ARENDERLIGHTS aRenderLights;
	static unsigned nRenderLights;
	static fglMaterial* oRenderMaterial;
	////////////////////////////////////////////////////

	virtual fglString fgl_class(void) const { return "scene"; }
	fglScene(void);
	virtual ~fglScene(void);

	virtual void call(fglEvent& poEvent);
	
	void loadGL(HWND phWndParent);
	void unloadGL(void);
	void loadDefaultShaders(void);

	void setWindow(void);

	bool getVSync(void) const;
	void setVSync(const bool& plVSync);
	void setMSAAMode(const unsigned& pnMSAAMode);
	unsigned getMSAAMode(fglString* pcMSAAModeInfo=NULL);
	void setRenderFlag(const bool& plValue);
	
	void render(void);
		
};

#endif