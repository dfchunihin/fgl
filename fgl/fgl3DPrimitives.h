#ifndef _FGL3DPRIMITIVES_H_
#define _FGL3DPRIMITIVES_H_

#include "fglMesh.h"

class fgl3DPrimitives 
{
private:
	bool isBuilded;
	fglREF<fglScene> oScene;
	fglMAT4 mSaveTrans;

	fglMesh oTriangle;
	fglMesh oQuadXZLines;
	fglMesh oQuadXZSolid;
	fglMesh oQuadXYLines;
	fglMesh oQuadXYSolid;
	fglMesh oQuadYZLines;
	fglMesh oQuadYZSolid;
	fglMesh oCubeLines;
	fglMesh oCubeSolid;
	fglMesh oSphere0;
	fglMesh oSphere1;
	fglMesh oSphere2;
	fglMesh oSphere3;
	fglMesh oSphere4;

	void renderBegin(const fglMaterial* poMaterial) const;
	void renderEnd(const fglMaterial* poMaterial) const;

public:
	
	fglMaterial oMaterial;

	fgl3DPrimitives(void);
	virtual ~fgl3DPrimitives(void);

	void renderTriangle(const fglVECTOR& poVert1, const fglVECTOR& poVert2, const fglVECTOR& poVert3, const fglMaterial* poMaterial=NULL);
	void renderQuad(const fglVECTOR& poCenter, const fglVECTOR& poSize, unsigned pnPlane, unsigned pnMode=1, const fglMaterial* poMaterial=NULL);
	void renderCube(const fglVECTOR& poCenter, const fglVECTOR& poSize, unsigned pnMode=1, const fglMaterial* poMaterial=NULL);
	void renderSphere(const fglVECTOR& poCenter, const fglVECTOR& poSize, unsigned pnTess=2, const fglMaterial* poMaterial=NULL);

	void build(const fglScene& poScene);

};

#endif
