#include "fglBind.h"
#include "fglScene.h"

fglBind::FGLABINDS fglBind::aBinds;

void fglBind::call(fglEvent& poEvent) 
{
	if (poEvent.name == fglEVENT_hScene) { _RetValue2i(0); if (oScene.exist()) _RetValue2i(oScene->getHFGL()); return; }
	if (poEvent.name == fglEVENT_name) { if (poEvent.nParams) name = _GetValue2c(poEvent.aParams[0]); _RetValue2c(name); return; }
	if (poEvent.name == fglEVENT_fgl_class) { _RetValue2c(fgl_class()); return; }

	if (poEvent.name == fglEVENT_copy) 
	{
		_RetValue2l(0);
		if (poEvent.nParams!=2) return;
		const fglREF<fglBind>* loSrc = getBind<fglBind>(_GetValue2i(poEvent.aParams[0]));
		if (!loSrc) return;

		if ( copy( loSrc->map(), _GetValue2i(poEvent.aParams[1]) ) ) _RetValue2l(1);
		return;
	}

	if (poEvent.name == fglEVENT_duplicateMode) { if (poEvent.nParams) duplicateMode = _GetValue2i(poEvent.aParams[0]); _RetValue2i(duplicateMode); return; }

	if (poEvent.name == fglEVENT_duplicate) 
	{
		_RetValue2i(0);
		if (poEvent.nParams!=3) return;
		const fglREF<fglBind>* loSrc = getBind<fglBind>(_GetValue2i(poEvent.aParams[0]));
		if (!loSrc) return;

		const fglBind* loDuplicate=NULL;
		loDuplicate = duplicate<fglBind>( loSrc->map(), _GetValue2i(poEvent.aParams[1]), _GetValue2i(poEvent.aParams[2]) );
		if (loDuplicate) _RetValue2i(loDuplicate->getHFGL());
		return;
	}

	if (poEvent.name == fglEVENT_setProp) 
	{
		if (poEvent.nParams!=2) return;
		fglString lcName = _GetValue2c(poEvent.aParams[0]);
		if (lcName.empty()) return;

		fglExtVal loVal;
		if (poEvent.aParams[1].ev_type == 'I') loVal.setVal(_GetValue2i(poEvent.aParams[1]));
		else if (poEvent.aParams[1].ev_type == 'N') loVal.setVal(_GetValue2f(poEvent.aParams[1]));
		else if (poEvent.aParams[1].ev_type == 'C') loVal.setVal(_GetValue2c(poEvent.aParams[1]));
		else if (poEvent.aParams[1].ev_type == 'L') loVal.setVal(_GetValue2l(poEvent.aParams[1]));
		
		getExtProps().set(lcName, loVal);
		return;
	}

	if (poEvent.name == fglEVENT_getProp) 
	{
		_RetValue2null();
		if (!poEvent.nParams) return;
		fglString lcName = _GetValue2c(poEvent.aParams[0]);
		if (lcName.empty()) return;
		
		const fglExtVal* loVal = getExtProps().get(lcName);
		
		if (!loVal) _RetValue2null();
		else if (loVal->getType() == 'I') _RetValue2i(loVal->getValI());
		else if (loVal->getType() == 'F') _RetValue2f(loVal->getValF());
		else if (loVal->getType() == 'C') _RetValue2c(loVal->getValC());
		else if (loVal->getType() == 'L') _RetValue2l(loVal->getValL());

		return;
	}

}

fglBind::fglBind(void) 
{
	reg(); 
	aExtProps=NULL; 
	duplicateMode=fglDUPLICATE_ANY; 
}

fglBind::~fglBind(void) 
{ 
	oRef.expired(); 
	setExtProps(NULL); 
}

const fglREF<fglScene>& fglBind::getScene(void) const 
{ 
	return oScene; 
}
bool fglBind::setScene(const fglREF<fglScene>& poScene) 
{ 
	if ( oScene.empty() ) oScene = poScene; 
	return (oScene == poScene); 
}
	
fglExtProps& fglBind::getExtProps(void) 
{ 
	if (aExtProps==NULL) aExtProps = new fglExtProps(); 
	return *aExtProps; 
}

void fglBind::setExtProps(const fglExtProps* poExtProps) 
{
	if (poExtProps==NULL) { if (aExtProps) delete aExtProps; aExtProps=NULL; return; } 
	if (aExtProps==NULL) aExtProps = new fglExtProps(); 
	*aExtProps = *poExtProps;
}


bool fglBind::copy(const fglBind* poSrc, fglBITFLAG pnShare)
{
	if (!poSrc) return false;
	if (poSrc==this) return false;
	if ( ! setScene(poSrc->getScene()) ) return false;

	name = poSrc->name;
	setExtProps(poSrc->aExtProps);
	duplicateMode = poSrc->duplicateMode;
	setSceneRenderFlag();
	return true;
}

void fglBind::unload(void)
{
	fglREF<fglBind> loBind;

	unsigned lnSize = aBinds.size();
	if ( lnSize == 0 ) return;

	while (lnSize)
	{
		loBind.inst(aBinds.get(lnSize));
		if ( loBind.exist() ) 
		{
			if ( loBind.map() == loBind->getScene().map() ) unloadSceneObjects( loBind->getScene().map() );
			loBind.releasedata();
		}
		lnSize--;
	}
}

void fglBind::unloadSceneObjects(const fglScene* poScene)
{
	if ( ! poScene ) return;

	fglScene::oCurrentScene = NULL;

	fglREF<fglBind> loBind;
	
	unsigned lnSize = aBinds.size();
	if ( lnSize == 0 ) return;

	while (lnSize)
	{
		loBind.inst(aBinds.get(lnSize));
		
		if ( loBind.exist() && loBind.map() != poScene ) if ( loBind->oScene.map() == poScene ) 
		{	
			loBind.releasedata();
		}
		lnSize--;
	}
}


void fglBind::setSceneRenderFlag(void)
{
	fglScene* loScene;
	if ( loScene = getScene().map() ) loScene->setRenderFlag(true);
}
