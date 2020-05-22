#pragma once

#include "GXEZ/Graphic/ATexture2D.h"

// SDL
#include <SDL.h>

namespace GXEZ
{
	class RendererSDL;

	class Texture2DSDL : public ATexture2D
	{
	public:
		Texture2DSDL(const Definition& definition, RendererSDL* rendererSDL);
		~Texture2DSDL();

		virtual void	UseAsRenderTarget() override;

		// SDL //
		SDL_Texture*	GetSDLTexture() const;

	private:

		bool			Init(const Definition& definition);

		RendererSDL*	_rendererSDL;
		SDL_Texture*	_textureSDL;
		uint8_t			_textureAccess;
	};
}