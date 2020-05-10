#pragma once

#include "GXEZ/Graphic/ATexture.h"

namespace GXEZ
{
	class RendererSDL;

	class TextureSDL : public ATexture
	{
	public:
		TextureSDL(const Definition& definition, RendererSDL* rendererSDL);

	private:
		virtual void	SetRenderer(IRenderer* renderer) override;

		RendererSDL*	_rendererSDL;
	};
}