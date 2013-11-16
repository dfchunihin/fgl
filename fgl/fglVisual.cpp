#include "fglSceneGraph.h"
#include "fglScene.h"

void fglVisual::call(fglEvent& poEvent)
{

	if (poEvent.name == fglEVENT_calcBounds) {calcBounds(); return;}

	if (poEvent.name == fglEVENT_boundsVisible) {if (poEvent.nParams) { bbox.isVisible = _GetValue2l(poEvent.aParams[0]); setSceneRenderFlag(); } _RetValue2l(bbox.isVisible); return;}
	if (poEvent.name == fglEVENT_boundsMeshType) {if (poEvent.nParams) { bbox.meshType = _GetValue2i(poEvent.aParams[0]); setSceneRenderFlag(); } _RetValue2i(bbox.meshType); return;}
	if (poEvent.name == fglEVENT_boundsColor) {if (poEvent.nParams) { bbox.meshColor.set(_GetValue2ui(poEvent.aParams[0])); setSceneRenderFlag(); } _RetValue2i(bbox.meshColor.get()); return;}
	if (poEvent.name == fglEVENT_boundsFill) {if (poEvent.nParams) { bbox.meshFill = _GetValue2l(poEvent.aParams[0]); setSceneRenderFlag(); } _RetValue2l(bbox.meshFill); return;}
	if (poEvent.name == fglEVENT_boundsLineWidth) {if (poEvent.nParams) { bbox.meshLineWidth = _GetValue2f(poEvent.aParams[0]); setSceneRenderFlag(); } _RetValue2f(bbox.meshLineWidth); return;}
	if (poEvent.name == fglEVENT_boundsLighting) {if (poEvent.nParams) { bbox.meshLighting = _GetValue2i(poEvent.aParams[0]); setSceneRenderFlag(); } _RetValue2i(bbox.meshLighting); return;}

	if (poEvent.name == fglEVENT_getBounds)
	{
		if ( poEvent.nParams!=3 ) return;
		_SetObjectProperty2vec( poEvent.aParams[0], bbox.getMinLocal() );
		_SetObjectProperty2vec( poEvent.aParams[1], bbox.getMaxLocal() );
		_SetObjectProperty2vec( poEvent.aParams[2], bbox.getSizeLocal() );
		return;
	}

	if (poEvent.name == fglEVENT_getBoundsExt)
	{
		if ( poEvent.nParams!=3 ) return;
		_SetObjectProperty2vec( poEvent.aParams[0], bboxExt.min );
		_SetObjectProperty2vec( poEvent.aParams[1], bboxExt.max );
		_SetObjectProperty2vec( poEvent.aParams[2], bboxExt.getSize() );
		return;
	}
	if (poEvent.name == fglEVENT_setBoundsExt) 
	{
		if ( ! poEvent.nParams ) return;
		if ( poEvent.nParams!=6 ) return;

		if ( poEvent.aParams[0].ev_type == 'O' && poEvent.aParams[1].ev_type == 'O' ) 
		{
			bboxExt.min = _GetObjectProperty2vec(poEvent.aParams[0]);
			bboxExt.max = _GetObjectProperty2vec(poEvent.aParams[1]);
		}
		else if (poEvent.aParams[0].ev_type != 'O' && poEvent.aParams[1].ev_type != 'O' && poEvent.aParams[2].ev_type != 'L' ) 
		{
			bboxExt.min.build( _GetValue2f(poEvent.aParams[0]), _GetValue2f(poEvent.aParams[1]), _GetValue2f(poEvent.aParams[2]) );
			bboxExt.max.build( _GetValue2f(poEvent.aParams[3]), _GetValue2f(poEvent.aParams[4]), _GetValue2f(poEvent.aParams[5]) );
		}
		else 
		{
			const fglREF<fglNode>* loNode = getBind<fglNode>(_GetValue2i(poEvent.aParams[0]));
			const fglREF<fglVisual>* loVisual = getBind<fglVisual>(_GetValue2i(poEvent.aParams[0]));
			if ( loNode ) 
			{
				loNode->map()->calcBounds();
				bboxExt.min = loNode->map()->bbox.min;
				bboxExt.max = loNode->map()->bbox.max;
			}
			else if ( loVisual ) 
			{
				loVisual->map()->calcBounds();
				bboxExt.min = loVisual->map()->bbox.min;
				bboxExt.max = loVisual->map()->bbox.max;
			}
		}
		calcBounds();
		return;
	}
	if (poEvent.name == fglEVENT_correctBoundsExt) 
	{
		if ( ! poEvent.nParams ) return;
		if ( poEvent.nParams!=6 ) return;
		if ( poEvent.aParams[0].ev_type == 'O' && poEvent.aParams[1].ev_type == 'O' ) 
		{
			bboxExt.min = bboxExt.min + _GetObjectProperty2vec(poEvent.aParams[0]);
			bboxExt.max = bboxExt.max + _GetObjectProperty2vec(poEvent.aParams[1]);
		}
		else if (poEvent.aParams[0].ev_type != 'O' && poEvent.aParams[1].ev_type != 'O' && poEvent.aParams[2].ev_type != 'L' ) 
		{
			bboxExt.min = bboxExt.min + fglVECTOR( _GetValue2f(poEvent.aParams[0]), _GetValue2f(poEvent.aParams[1]), _GetValue2f(poEvent.aParams[2]) );
			bboxExt.max = bboxExt.max + fglVECTOR( _GetValue2f(poEvent.aParams[3]), _GetValue2f(poEvent.aParams[4]), _GetValue2f(poEvent.aParams[5]) );
		}
		calcBounds();
		return;
	}


	if (poEvent.name == fglEVENT_viewRangeMin) {if (poEvent.nParams) { viewRangeMin = _GetValue2f(poEvent.aParams[0]); setSceneRenderFlag(); } _RetValue2f(viewRangeMin); return;}
	if (poEvent.name == fglEVENT_viewRangeMax) {if (poEvent.nParams) { viewRangeMax = _GetValue2Infinite(_GetValue2f(poEvent.aParams[0])); setSceneRenderFlag(); } _RetValue2f(_RetValue2Infinite(viewRangeMax)); return;}

	if (poEvent.name == fglEVENT_getViewRangeRot)
	{
		if ( poEvent.nParams!=2 ) return;
		_SetObjectProperty2vec( poEvent.aParams[0], viewRangeRot.min );
		_SetObjectProperty2vec( poEvent.aParams[1], viewRangeRot.max );
		return;
	}
	if (poEvent.name == fglEVENT_setViewRangeRot)
	{
		if ( poEvent.nParams!=6 ) return;
		if ( poEvent.aParams[0].ev_type == 'O' || poEvent.aParams[1].ev_type == 'O' )
		{	
			viewRangeRot.min = _GetObjectProperty2vec( poEvent.aParams[0] );
			viewRangeRot.max = _GetObjectProperty2vec( poEvent.aParams[1] );
		}
		else
		{
			viewRangeRot.min.build( _GetValue2f(poEvent.aParams[0]), _GetValue2f(poEvent.aParams[1]), _GetValue2f(poEvent.aParams[2]) );
			viewRangeRot.max.build( _GetValue2f(poEvent.aParams[3]), _GetValue2f(poEvent.aParams[4]), _GetValue2f(poEvent.aParams[5]) );
		}
		setSceneRenderFlag();
		return;
	}


	if (poEvent.name == fglEVENT_nStretch) { if (poEvent.nParams) { nStretch = _GetValue2i(poEvent.aParams[0]); setSceneRenderFlag(); } _RetValue2i(nStretch.get()); return; }

	if (poEvent.name == fglEVENT_hMaterial) 
	{
		if (poEvent.nParams) { oMaterial.inst( getBind<fglMaterial>(_GetValue2i(poEvent.aParams[0])) ); setSceneRenderFlag(); }
		if (oMaterial.empty()) _RetValue2i(0); else _RetValue2i(oMaterial->getHFGL());
		return;
	}

	if (poEvent.name == fglEVENT_aLODs) { _RetValue2i(aLODs.getHFGL()); return; }

	fglBind::call(poEvent);
}

fglVisual::fglVisual(void) : bbox(fglScene::identTrans,fglScene::identTrans)
{
	viewRangeMin = 0.0f; 
	viewRangeMax = fglINFINITE; 
	oStretchClip.create();
	renderImpl = fglVisual::renderVisual;
}

fglVisual::~fglVisual(void) 
{

}

bool fglVisual::copy(const fglBind* poSrc, fglBITFLAG pnShare)
{
	const fglVisual* loVisual = dynamic_cast<const fglVisual*>(poSrc);
	if (!loVisual) return false;

	if ( !fglBind::copy(poSrc, pnShare) ) return false;
	
	bbox = loVisual->bbox;
	bboxInt = loVisual->bboxInt;
	bboxExt = loVisual->bboxExt;
	viewRangeMin = loVisual->viewRangeMin;
	viewRangeMax = loVisual->viewRangeMax;
	viewRangeRot = loVisual->viewRangeRot;

	nStretch = loVisual->nStretch;
	oStretchClip->build(bbox);

	oMaterial.clear();
	const fglMaterial* loMaterial;
	if ( loMaterial = duplicate<fglMaterial>( loVisual->oMaterial.map(), (pnShare.check(fglSHARE_MATERIAL) ? fglDUPLICATE_INSTANCE : fglDUPLICATE_COPY), pnShare ) )
	{
		oMaterial = loMaterial->getRef();
	}
	
	copyLODs(loVisual, pnShare);
	
	return true;
}

void fglVisual::copyLODs(const fglVisual* poVisual, fglBITFLAG pnShare)
{
	if (!poVisual || poVisual==this) return;
	
	aLODs.clear();
	if ( ! poVisual->aLODs.size() ) return;
	
	const fglVisual* loLOD;
	aLODs.reserve(poVisual->aLODs.size());
	for (unsigned i=0; i!=poVisual->aLODs.size(); i++)
	{
		if (poVisual->aLODs[i].empty()) continue;
		loLOD = duplicate<fglVisual>( poVisual->aLODs[i].map(), (pnShare.check(fglSHARE_LOD) ? fglDUPLICATE_INSTANCE : fglDUPLICATE_COPY), pnShare );
		if ( ! loLOD ) continue;
		aLODs.add( loLOD->getRef() );
	}
}


void fglVisual::calcBounds(void)
{
	if ( ! bboxExt.isNull() ) 
	{
		bbox.min = bboxExt.min;
		bbox.max = bboxExt.max;
	} 
	else
	{ 
		bbox.min = bboxInt.min; 
		bbox.max = bboxInt.max; 
	}

	if ( aLODs.exist() )
	{
		fglVisual* loLOD(NULL);
		unsigned lnLOD(aLODs.size());
		while (lnLOD--)
		{
			if ( loLOD = aLODs[lnLOD].map() ) loLOD->calcBounds();
		}
	}
	oStretchClip->build(bbox);
	setSceneRenderFlag();
}


void fglVisual::renderVisual(const fglVisual* poVisual)
{

}

void fglVisual::render(void) const
{
	if (bbox.isVisible) bbox.render();

	if (oMaterial.exist()) oMaterial->bind();

	if ( nStretch.get() && ! bboxExt.isNull() ) 
	{
		renderStretch();
	}
	else
	{
		const fglVisual* loLOD = renderGetLOD();
		if ( loLOD ) loLOD->render(); else renderImpl(this);
	}

	if (oMaterial.exist()) oMaterial->unbind();
}

const fglVisual* fglVisual::renderGetLOD(void) const
{
	const fglVisual* loLOD(NULL);
	unsigned lnLOD( aLODs.size() );
	
	while (lnLOD)
	{
		lnLOD--;
		if ( ! (loLOD = aLODs[lnLOD].map()) ) {loLOD = NULL; continue;}
		if ( loLOD->testIsVisible(*fglScene::oRenderViewport, fglScene::oRenderNodes.currentView) ) break; 
		loLOD = NULL;
	}

	return loLOD;
}


void fglVisual::renderStretch(void) const
{
	const fglVisual *loMainLOD, *loRepeatLOD;
		
	fglTRANSFORM loSaveTrans(fglScene::oRenderNodes.currentView);
	fglMAT4 loSaveTransMatrix(fglScene::oRenderNodes.currentViewMatrix);

	//////////////////////////////////////////////////////////////////
	fglVECTOR loSavePos = fglScene::oRenderNodes.currentView.pos;
	if ( fglScene::oRenderViewport->oFrustum.testPoint(fglScene::oRenderNodes.currentView.pos)==fglFRUSTUM::OUTSIDE ) 
	{
		fglROW4 loCamPlane;
		fglVECTOR loCamLook=fglScene::oRenderCam->oLookVectorGlobal;
		loCamLook.x = fabs(loCamLook.x); loCamLook.y = fabs(loCamLook.y); loCamLook.z = fabs(loCamLook.z);
		if (loCamLook.x > loCamLook.y && loCamLook.x > loCamLook.z ) loCamLook.build(1.0f, 0.0f, 0.0f);
		else if (loCamLook.y > loCamLook.x && loCamLook.y > loCamLook.z ) loCamLook.build(0.0f, 1.0f, 0.0f);
		else loCamLook.build(0.0f, 0.0f, 1.0f);
		loCamPlane.build(loCamLook, (loCamLook^fglScene::oRenderNodes.currentView.pos));
		if ( fglScene::oRenderCam->getLookAtPlane(loCamPlane, loCamLook) )
		{
			fglScene::oRenderNodes.currentView.pos = loCamLook;
		}
	}
	
	fglBITFLAG lnStretch(nStretch);
	
	if ( loMainLOD = renderGetLOD() ) 
	if ( loMainLOD->nStretch.get() ) lnStretch=loMainLOD->nStretch;

	fglScene::oRenderNodes.currentView.pos = loSavePos;
	//////////////////////////////////////////////////////////////////

	if ( lnStretch.check(nStretchClip) ) oStretchClip->bind();
	
	//////////////////////////////////////////////////////////////////
	const fglVECTOR& loBMin(bboxExt.min);
	const fglVECTOR& loBMax(bboxExt.max);
	fglVECTOR loSize(bboxExt.getSize());
	fglVECTOR loBMinInt, loBMaxInt, loSizeInt, loCenterInt;
	if ( loMainLOD )
	{
		loBMinInt = loMainLOD->bbox.min;
		loBMaxInt = loMainLOD->bbox.max;
		loSizeInt = loMainLOD->bbox.getSize();
		loCenterInt = loMainLOD->bbox.getCenter();
	}
	else
	{
		loBMinInt = bboxInt.min;
		loBMaxInt = bboxInt.max;
		loSizeInt = bboxInt.getSize();
		loCenterInt = bboxInt.getCenter();
	}
	//////////////////////////////////////////////////////////////////

	float lnBMin, lnBMax, lnSize, lnModelBMin, lnModelBMax, lnModelSize, lnRepeatSize, lnScale;
	int lnSide, lnRepeatCount[2], lnDir, i;
	fglVECTOR loDir, loScale(1.0f,1.0f,1.0f), loCenter;
	
	if ( lnStretch.check(nStretchFit) ) 
	{
		lnSide = 3;
		while (lnSide--)
		{
			lnSize = *(&loSize.x + lnSide);
			lnModelSize = *(&loSizeInt.x+lnSide);
			if ( ! lnModelSize ) continue;
		
			lnScale = (lnSize / lnModelSize);
			if ( lnStretch.check(nStretchRepeat) ) lnScale =  lnScale / ( floor(lnScale*0.5f)*2.0f + 1.0f );
			*(&loScale.x+lnSide) = lnScale;
		}
	
		if ( lnStretch.check(nStretchProper) ) 
		{
			loScale.x=loScale.y=loScale.z = std::min<float>(loScale.x,std::min<float>(loScale.y,loScale.z));
		}
		loCenter = bboxExt.getCenter() - (loCenterInt * loScale);

		fglScene::oRenderNodes.currentView.moveDir(loCenter);
		fglScene::oRenderNodes.currentView.scaleAt(loScale);
		fglScene::oRenderNodes.currentViewMatrix.scaleAt(loScale);
		
	}
	
	//////////////////////////////////////////////////////////////////
	loCenter = fglScene::oRenderNodes.currentView.pos;
	//////////////////////////////////////////////////////////////////

	if ( lnStretch.check(nStretchRepeat) &&  (lnStretch.check(nStretchFit) || ! lnStretch.check(nStretchProper)) ) 
	{
		lnSide = 3;
		while (lnSide--)
		{
			lnBMin = *(&loBMin.x+lnSide);
			lnBMax = *(&loBMax.x+lnSide);
			lnSize = *(&loSize.x+lnSide);

			lnModelBMin = *(&loBMinInt.x+lnSide);
			lnModelBMax = *(&loBMaxInt.x+lnSide);
			lnModelSize = *(&loSizeInt.x+lnSide);
			if ( ! lnModelSize ) continue;

			if ( lnStretch.check(nStretchFit) ) 
			{
				lnRepeatCount[0] = lnRepeatCount[1] = int (floor( (lnSize / lnModelSize) * 0.5f ));
			}
			else
			{
				lnRepeatSize = (lnModelBMin - lnBMin) / lnModelSize;
				lnRepeatCount[0] = int (ceil( lnRepeatSize > 0.001f ? lnRepeatSize : 0.0f ));
				lnRepeatSize = (lnBMax - lnModelBMax) / lnModelSize;
				lnRepeatCount[1] = int (ceil( lnRepeatSize > 0.001f ? lnRepeatSize : 0.0f ));
			}

			lnScale = (*(&loScale.x+lnSide));
			loDir.x=loDir.y=loDir.z=0.0f;
			*(&loDir.x + lnSide) = lnModelSize*lnScale;
			lnDir = 2;
			while (lnDir--)
			{
				fglScene::oRenderNodes.currentView.moveTo(loCenter);
				for (i=0; i < lnRepeatCount[lnDir]; i++)
				{
					fglScene::oRenderNodes.currentView.moveDir(loDir);
					fglScene::oRenderNodes.currentViewMatrix.moveTo(fglScene::oRenderNodes.currentView.pos);
					fglScene::oRenderNodes.currentViewMatrix.setCurrentView();
					//////////////////////////////////////////////////////////////////
					//render mesh
					if ( loRepeatLOD = renderGetLOD() ) loRepeatLOD->render(); else renderImpl(this);
					//////////////////////////////////////////////////////////////////
				}
				*(&loDir.x + lnSide) = -(*(&loDir.x + lnSide));
			}
		}
	}

	if ( lnStretch.check(nStretchFit) ) 
	{
		fglScene::oRenderNodes.currentView.moveTo(loCenter);
		fglScene::oRenderNodes.currentViewMatrix.moveTo(loCenter);
		fglScene::oRenderNodes.currentViewMatrix.setCurrentView();
	}
	else
	{
		fglScene::oRenderNodes.currentView = loSaveTrans;
		fglScene::oRenderNodes.currentViewMatrix = loSaveTransMatrix;
		fglScene::oRenderNodes.currentViewMatrix.setCurrentView();
	}

	//////////////////////////////////////////////////////////////////
	//render mesh
	if ( loMainLOD ) loMainLOD->render(); else renderImpl(this);
	//////////////////////////////////////////////////////////////////

	if ( lnStretch.check(nStretchFit) ) 
	{
		fglScene::oRenderNodes.currentView = loSaveTrans;
		fglScene::oRenderNodes.currentViewMatrix = loSaveTransMatrix;
		fglScene::oRenderNodes.currentViewMatrix.setCurrentView();
	}

	if ( lnStretch.check(nStretchClip) ) oStretchClip->unbind();
}



bool fglVisual::testIntersection(fglRAY& poRay, fglVECTOR* poPointIn) const
{
	if ( ! poRay.testInBoundBox(bbox.min, bbox.max, poPointIn) ) return false;
	return true;
}

bool fglVisual::testInFrustum(const fglViewport& poViewport, const fglTRANSFORM& poTRG) const
{
	if (poViewport.oCam.empty()) return false;

	fglBBox loBBox(poTRG, poTRG); 
	loBBox = bbox;
	
	if ( poViewport.oFrustum.testBoundBox(loBBox)==fglFRUSTUM::OUTSIDE ) return false;

	return testIsVisible(poViewport, poTRG);
}

bool fglVisual::testIsVisible(const fglViewport& poViewport, const fglTRANSFORM& poTRG) const
{
	if (poViewport.oCam.empty()) return false;

	if ( viewRangeMin!=0.0f || viewRangeMax!=fglINFINITE )
	{
		fglBBox loBBox(poTRG, poTRG); 
		loBBox = bbox;

		float lnViewportArea = float(poViewport.oRect.width * poViewport.oRect.height);
		float lnBBoxArea = loBBox.getSquareRadiusLocal(); 
		lnBBoxArea *= fglPI;
		
		fglVECTOR loDist = poViewport.oCam->trg.pos - loBBox.getCenterGlobal();
		loDist = poViewport.oCam->trgInv.rot * loDist; 
		float lnDist = std::max<float>(loDist.z*loDist.z, fglEPSILON);

		float lnSize = (lnBBoxArea * lnViewportArea * poViewport.mProjPerspective.a1 * poViewport.mProjPerspective.b2) / lnDist;

		if ( lnSize < (viewRangeMin*viewRangeMin) ) return false; 
		if ( viewRangeMax!=fglINFINITE ) if ( lnSize > (viewRangeMax*viewRangeMax) ) return false; 
	}

	if ( ! viewRangeRot.isNull() )
	{
		fglVECTOR loAngles = poTRG.rot.toEuler() - poViewport.oCam->trg.rot.toEuler();
		loAngles = loAngles * fglRAD2ANG;
		if ( fabs(loAngles.x) > 180.0f ) loAngles.x = loAngles.x - (loAngles.x < 0.0f ? -1.0f : 1.0f) * ceil(fabs(loAngles.x)/180.0f)*180.0f;
		if ( fabs(loAngles.y) > 180.0f ) loAngles.y = loAngles.y - (loAngles.y < 0.0f ? -1.0f : 1.0f) * ceil(fabs(loAngles.y)/180.0f)*180.0f;
		if ( fabs(loAngles.z) > 180.0f ) loAngles.z = loAngles.z - (loAngles.z < 0.0f ? -1.0f : 1.0f) * ceil(fabs(loAngles.z)/180.0f)*180.0f;
				
		if ( viewRangeRot.min.x < viewRangeRot.max.x ) 
		{
			if ( ! (viewRangeRot.min.x <= loAngles.x  && loAngles.x <= viewRangeRot.max.x) ) return false;
		}
		if ( viewRangeRot.min.y < viewRangeRot.max.y ) 
		{
			if ( ! (viewRangeRot.min.y <= loAngles.y  && loAngles.y <= viewRangeRot.max.y) ) return false;
		}
		if ( viewRangeRot.min.z < viewRangeRot.max.z ) 
		{
			if ( ! (viewRangeRot.min.z <= loAngles.z  && loAngles.z <= viewRangeRot.max.z) ) return false;
		}
				
	}

	return true;
}
