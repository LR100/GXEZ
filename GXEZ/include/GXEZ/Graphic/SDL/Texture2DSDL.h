#pragma once

// GXEZ
#include "GXEZ/Graphic/ATexture2D.h"

// STD
#include <list> // for TextureManager list iterator
#include <optional>

// SDL
#include <SDL.h>

namespace GXEZ
{
	
	class Texture2DManagerSDL;

	class Texture2DSDL : public ATexture2D
	{
	public:
		friend class Texture2DManagerSDL;

		Texture2DSDL(const Definition& definition, Texture2DManagerSDL* managerSDL);
		~Texture2DSDL();

		void			UseAsRenderTarget(const bool& state) override;
		virtual void	Save() override;
		virtual void	Restore() override;

		// SDL //
		SDL_Texture*	GetSDLTexture() const;
		const uint8_t	GetSDLTextureAccess() const;

	private:

		bool			Init(const Definition& definition);

		char*												_data; // Data Used for Save

		SDL_Texture*										_textureSDL;
		uint8_t												_textureAccess;

		Texture2DManagerSDL*								_managerSDL;
		// Texture Manager Attributes
		std::optional<std::list<Texture2DSDL*>::iterator>	__itList;
	};
}