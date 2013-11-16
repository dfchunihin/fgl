#include "fgl3DPrimitives.h"
#include "fglScene.h"
#include "fglImport.h"

fgl3DPrimitives::fgl3DPrimitives(void)
{
	isBuilded = false;
}


fgl3DPrimitives::~fgl3DPrimitives(void)
{

}

void fgl3DPrimitives::renderBegin(const fglMaterial* poMaterial) const
{
	if (poMaterial) const_cast<fglMaterial*>(poMaterial)->bind();
}
void fgl3DPrimitives::renderEnd(const fglMaterial* poMaterial) const
{
	if (poMaterial) const_cast<fglMaterial*>(poMaterial)->unbind();
}

void fgl3DPrimitives::renderTriangle(const fglVECTOR& poVert1, const fglVECTOR& poVert2, const fglVECTOR& poVert3, const fglMaterial* poMaterial) 
{
	if ( ! isBuilded ) return;
	renderBegin(poMaterial);
	oTriangle.aVerts[0].p = poVert1;
	oTriangle.aVerts[1].p = poVert2;
	oTriangle.aVerts[2].p = poVert3;
	oTriangle.fillVBO();
	fglMesh::renderMesh(&oTriangle);
	renderEnd(poMaterial);
}

void fgl3DPrimitives::renderQuad(const fglVECTOR& poCenter, const fglVECTOR& poSize, unsigned pnPlane, unsigned pnMode, const fglMaterial* poMaterial)
{
	if ( ! isBuilded ) return;
	
	mSaveTrans = fglScene::oRenderNodes.currentViewMatrix;
	if ( ! poCenter.isNull() ) fglScene::oRenderNodes.currentViewMatrix.moveTo( fglScene::oRenderNodes.currentView * poCenter );
	fglScene::oRenderNodes.currentViewMatrix.scaleAt( poSize );
	fglScene::oRenderNodes.currentViewMatrix.setCurrentView();
	renderBegin(poMaterial);
	if (pnPlane==1)
	{
		if (pnMode==0) fglMesh::renderMesh(&oQuadXZLines);
		else if (pnMode==1) fglMesh::renderMesh(&oQuadXZSolid); 
	} 
	else if (pnPlane==2)
	{
		if (pnMode==0) fglMesh::renderMesh(&oQuadXYLines);
		else if (pnMode==1) fglMesh::renderMesh(&oQuadXYSolid); 
	}
	else if (pnPlane==3)
	{
		if (pnMode==0) fglMesh::renderMesh(&oQuadYZLines);
		else if (pnMode==1) fglMesh::renderMesh(&oQuadYZSolid); 
	}
	renderEnd(poMaterial);
	fglScene::oRenderNodes.currentViewMatrix = mSaveTrans;
	fglScene::oRenderNodes.currentViewMatrix.setCurrentView();
}

void fgl3DPrimitives::renderCube(const fglVECTOR& poCenter, const fglVECTOR& poSize, unsigned pnMode, const fglMaterial* poMaterial)
{
	if ( ! isBuilded ) return;

	mSaveTrans = fglScene::oRenderNodes.currentViewMatrix;
	if ( ! poCenter.isNull() ) fglScene::oRenderNodes.currentViewMatrix.moveTo( fglScene::oRenderNodes.currentView * poCenter );
	fglScene::oRenderNodes.currentViewMatrix.scaleAt( poSize );
	fglScene::oRenderNodes.currentViewMatrix.setCurrentView();
	renderBegin(poMaterial);
	if ( ! poSize.x ) 
	{
		if (pnMode==0) fglMesh::renderMesh(&oQuadYZLines);
		else if (pnMode==1) fglMesh::renderMesh(&oQuadYZSolid); 
	}
	else if ( ! poSize.y ) 
	{
		if (pnMode==0) fglMesh::renderMesh(&oQuadXZLines);
		else if (pnMode==1) fglMesh::renderMesh(&oQuadXZSolid); 
	}
	else if ( ! poSize.z ) 
	{
		if (pnMode==0) fglMesh::renderMesh(&oQuadXYLines);
		else if (pnMode==1) fglMesh::renderMesh(&oQuadXYSolid); 
	}
	else 
	{
		if (pnMode==0) fglMesh::renderMesh(&oCubeLines);
		else if (pnMode==1) fglMesh::renderMesh(&oCubeSolid); 
	}

	renderEnd(poMaterial);
	fglScene::oRenderNodes.currentViewMatrix = mSaveTrans;
	fglScene::oRenderNodes.currentViewMatrix.setCurrentView();
}

void fgl3DPrimitives::renderSphere(const fglVECTOR& poCenter, const fglVECTOR& poSize, unsigned pnTess, const fglMaterial* poMaterial)
{
	if ( ! isBuilded ) return;
	
	mSaveTrans = fglScene::oRenderNodes.currentViewMatrix;
	if ( ! poCenter.isNull() ) fglScene::oRenderNodes.currentViewMatrix.moveTo( fglScene::oRenderNodes.currentView * poCenter );
	fglScene::oRenderNodes.currentViewMatrix.scaleAt( poSize );
	fglScene::oRenderNodes.currentViewMatrix.setCurrentView();
	renderBegin(poMaterial);
	if (pnTess==0) fglMesh::renderMesh(&oSphere0);
	else if (pnTess==1) fglMesh::renderMesh(&oSphere1);
	else if (pnTess==2) fglMesh::renderMesh(&oSphere2);
	else if (pnTess==3) fglMesh::renderMesh(&oSphere3);
	else if (pnTess==4) fglMesh::renderMesh(&oSphere4);
	renderEnd(poMaterial);
	fglScene::oRenderNodes.currentViewMatrix = mSaveTrans;
	fglScene::oRenderNodes.currentViewMatrix.setCurrentView();
}


void fgl3DPrimitives::build(const fglScene& poScene)
{
	if (isBuilded) return;
	if ( ! fglScene::hCurrentContext ) return;

	isBuilded = true;
	oScene = poScene.getRef();
	
	oTriangle.setScene(oScene);
	oTriangle.aVerts.set_size(3);
	oTriangle.aVerts[0].n.z = 1.0f;
	oTriangle.aVerts[1].n.z = 1.0f;
	oTriangle.aVerts[2].n.z = 1.0f;
	oTriangle.setFaces(1,3);
	oTriangle.aInds[0] = 0;
	oTriangle.aInds[1] = 1;
	oTriangle.aInds[2] = 2;
	oTriangle.buildVBO();
	/////////////////////////////////////////////////////


	oQuadXZLines.setScene(oScene);
	fgl_import_mesh_from_string(oQuadXZLines, "p 2\n-0.5 0 -0.5\n0.5 0 -0.5\np 2\n-0.5 0 0.5\n0.5 0 0.5\np 2\n-0.5 0 -0.5\n-0.5 0 0.5\np 2\n0.5 0 -0.5\n0.5 0 0.5", "nff");
	oQuadXZLines.buildVBO();
	
	oQuadXZSolid.setScene(oScene);
	fgl_import_mesh_from_string(oQuadXZSolid, "p 4\n-0.5 0 -0.5\n-0.5 0 0.5\n0.5 0 0.5\n0.5 0 -0.5", "nff");
	oQuadXZSolid.buildVBO();

	oQuadXYLines.setScene(oScene);
	fgl_import_mesh_from_string(oQuadXYLines, "p 2\n-0.5 -0.5 0\n0.5 -0.5 0\np 2\n-0.5 0.5 0\n0.5 0.5 0\np 2\n-0.5 -0.5 0\n-0.5 0.5 0\np 2\n0.5 -0.5 0\n0.5 0.5 0", "nff");
	oQuadXYLines.buildVBO();
	
	oQuadXYSolid.setScene(oScene);
	fgl_import_mesh_from_string(oQuadXYSolid, "p 4\n-0.5 -0.5 0\n0.5 -0.5 0\n0.5 0.5 0\n-0.5 0.5 0", "nff");
	oQuadXYSolid.buildVBO();

	oQuadYZLines.setScene(oScene);
	fgl_import_mesh_from_string(oQuadYZLines, "p 2\n0 -0.5 -0.5\n0 -0.5 0.5\np 2\n0 0.5 -0.5\n0 0.5 0.5\np 2\n0 -0.5 -0.5\n0 0.5 -0.5\np 2\n0 -0.5 0.5\n0 0.5 0.5", "nff");
	oQuadYZLines.buildVBO();
	
	oQuadYZSolid.setScene(oScene);
	fgl_import_mesh_from_string(oQuadYZSolid, "p 4\n0 -0.5 -0.5\n0 -0.5 0.5\n0 0.5 0.5\n0 0.5 -0.5", "nff");
	oQuadYZSolid.buildVBO();


	/////////////////////////////////////////////////////
	oCubeLines.setScene(oScene);
	fgl_import_mesh_from_string(oCubeLines, "p 2\n-0.5 -0.5 -0.5\n0.5 -0.5 -0.5\np 2\n-0.5 0.5 -0.5\n0.5 0.5 -0.5\np 2\n-0.5 -0.5 -0.5\n-0.5 0.5 -0.5\np 2\n0.5 -0.5 -0.5\n0.5 0.5 -0.5\np 2\n-0.5 -0.5 0.5\n0.5 -0.5 0.5\np 2\n-0.5 0.5 0.5\n0.5 0.5 0.5\np 2\n-0.5 -0.5 0.5\n-0.5 0.5 0.5\np 2\n0.5 -0.5 0.5\n0.5 0.5 0.5\np 2\n-0.5 0.5 -0.5\n-0.5 0.5 0.5\np 2\n0.5 0.5 -0.5\n0.5 0.5 0.5\np 2\n-0.5 -0.5 -0.5\n-0.5 -0.5 0.5\np 2\n0.5 -0.5 -0.5\n0.5 -0.5 0.5", "nff");
	oCubeLines.buildVBO();

	oCubeSolid.setScene(oScene);
	fgl_import_mesh_from_string(oCubeSolid, "hex 0 0 0 1", "nff");
	oCubeSolid.buildVBO();

	oSphere0.setScene(oScene);
	fgl_import_mesh_from_string(oSphere0, "tess 0\ns 0 0 0 0.5", "nff");
	oSphere0.buildVBO();

	oSphere1.setScene(oScene);
	fgl_import_mesh_from_string(oSphere1, "tess 1\ns 0 0 0 0.5", "nff");
	oSphere1.buildVBO();

	oSphere2.setScene(oScene);
	fgl_import_mesh_from_string(oSphere2, "tess 2\ns 0 0 0 0.5", "nff");
	oSphere2.buildVBO();

	oSphere3.setScene(oScene);
	fgl_import_mesh_from_string(oSphere3, "tess 3\ns 0 0 0 0.5", "nff");
	oSphere3.buildVBO();

	oSphere4.setScene(oScene);
	fgl_import_mesh_from_string(oSphere4, "tess 4\ns 0 0 0 0.5", "nff");
	oSphere4.buildVBO();

	/////////////////////////////////////////////////////
}
