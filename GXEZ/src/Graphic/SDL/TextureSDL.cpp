#include "GXEZ/Graphic/SDL/TextureSDL.h"
#include "GXEZ/Graphic/SDL/RendererSDL.h"

namespace GXEZ
{
	TextureSDL::TextureSDL(const Definition& definition, RendererSDL* rendererSDL) : ATexture(definition, (IRenderer*)rendererSDL)
	{
		_rendererSDL = rendererSDL;
	}

	void TextureSDL::SetRenderer(IRenderer* renderer)
	{
		
	}
}
