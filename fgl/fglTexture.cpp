#include "fglTexture.h"
#include "fglScene.h"

#include <IL/devil_cpp_wrapper.hpp>


fglTexture::fglTexture(void)
{
	id=0;
}

fglTexture::~fglTexture(void)
{
	if (id) glDeleteTextures(1, &id);
}

void fglTexture::build(void)
{
	if (path.empty()) return;

	if (id) { glDeleteTextures(1, &id); id=0; }
	ilImage loImage(path.c_str());
	if ( ! loImage.GetId() ) { throw fglException(ilError::String()); return; }
	
	id = ilOgl::BindTex(loImage);

	if ( ! id ) { throw fglException(ilError::String()); return; }

	setSceneRenderFlag();

	//ilOgl::Mipmap(loImage);
}

void fglTexture::bind(void)
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void fglTexture::unbind(void)
{
	glBindTexture(GL_TEXTURE_2D, NULL);
}

