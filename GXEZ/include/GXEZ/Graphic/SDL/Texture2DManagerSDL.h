#pragma once


// STD
#include <list>

// GXEZ
#include "GXEZ/Graphic/ATexture2DManager.h"
#include "GXEZ/Graphic/SDL/Texture2DSDL.h""

namespace GXEZ
{
	class RendererSDL;

	class Texture2DManagerSDL : public ATexture2DManager
	{
	public:
		Texture2DManagerSDL(RendererSDL* rendererSDL);

		virtual ATexture2D* CreateTexture2D(const ATexture2D::Definition& definition) override;
		virtual void		RemoveTexture2D(ATexture2D* texture) override;
		// Renderer
		RendererSDL*		GetRendererSDL() const;

		void				RestoreTextureWithSDLTextureAccess(const uint8_t& textureAccess);



		// Register
		virtual void	RegisterTexture2D(ATexture2D* texture) override;
		void			RegisterTexture2D(Texture2DSDL* textureSDL);
		// Unregister
		virtual void	UnRegisterTexture2D(ATexture2D* texture) override;
		void			UnRegisterTexture2D(Texture2DSDL* textureSDL);

	private:

		// Register
		void			RegisterTexture2DInList(Texture2DSDL* textureSDL);
		// Unregister
		void			UnRegisterTexture2DFromList(Texture2DSDL* textureSDL);

		std::list<Texture2DSDL*>	_textures;
		RendererSDL*				_rendererSDL;
	};

}