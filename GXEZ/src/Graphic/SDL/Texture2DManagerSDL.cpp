#include "GXEZ/Graphic/SDL/Texture2DManagerSDL.h"

namespace GXEZ
{
	Texture2DManagerSDL::Texture2DManagerSDL(RendererSDL* rendererSDL) : ATexture2DManager((IRenderer*)rendererSDL)
	{
		// If needed
		_rendererSDL = rendererSDL;
	}

	ATexture2D* Texture2DManagerSDL::CreateTexture2D(const ATexture2D::Definition& definition)
	{
		return (new Texture2DSDL(definition, this));
	}

	void Texture2DManagerSDL::RemoveTexture2D(ATexture2D* texture)
	{
		if (texture)
		{
			// Simply delete texture -> it will unregister by itself
			delete (texture);
		}
	}

	RendererSDL* Texture2DManagerSDL::GetRendererSDL() const
	{
		return (_rendererSDL);
	}

	void Texture2DManagerSDL::RestoreTextureWithSDLTextureAccess(const uint8_t& textureAccess)
	{
		// std::cout << "Texture2DManagerSDL::RestoreTextureWithSDLTextureAccess (" << int(textureAccess) << ")" << std::endl;
		std::list<Texture2DSDL*>::iterator it = _textures.begin();
		std::list<Texture2DSDL*>::iterator itEnd = _textures.end();

		for (; it != itEnd; it++)
		{
			// std::cout << "SDL TEXTURE ACCESS (" << (*it)->GetSDLTextureAccess() << ")" << std::endl;
			if ((*it)->GetSDLTextureAccess() == textureAccess)
				(*it)->Restore(); 
		}
	}

	void Texture2DManagerSDL::RegisterTexture2D(ATexture2D* texture)
	{
		// std::cout << "Texture2DManagerSDL::RegisterTexture2D()" << std::endl;
		Texture2DSDL* textureSDL = dynamic_cast<Texture2DSDL*>(texture);
		if (textureSDL) {
			RegisterTexture2D(textureSDL);
		}
	}

	void Texture2DManagerSDL::RegisterTexture2D(Texture2DSDL* textureSDL)
	{
		// std::cout << "Texture2DManagerSDL::RegisterTexture2D(SDL)" << std::endl;
		if (textureSDL->_managerSDL == this) // If the texture is managed by this manager
		{
			if (!textureSDL->__itList.has_value()) // If not registered
				RegisterTexture2DInList(textureSDL);
		}
	}

	void Texture2DManagerSDL::RegisterTexture2DInList(Texture2DSDL* textureSDL)
	{
		// std::cout << "Texture2DManagerSDL::RegisterTexture2DInList(SDL)" << std::endl;
		textureSDL->__itList.emplace(_textures.insert(_textures.end(), textureSDL));
	}

	void Texture2DManagerSDL::UnRegisterTexture2D(ATexture2D* texture)
	{
		// std::cout << "Texture2DManagerSDL::UnRegisterTexture2D()" << std::endl;
		Texture2DSDL* textureSDL = dynamic_cast<Texture2DSDL*>(texture);
		if (textureSDL) {
			UnRegisterTexture2D(textureSDL);
		}
	}

	void Texture2DManagerSDL::UnRegisterTexture2D(Texture2DSDL* textureSDL)
	{
		// std::cout << "Texture2DManagerSDL::UnRegisterTexture2D(SDL)" << std::endl;
		if (textureSDL->_managerSDL == this) // If the texture is managed by this manager
		{
			UnRegisterTexture2DFromList(textureSDL);
		}
	}

	void Texture2DManagerSDL::UnRegisterTexture2DFromList(Texture2DSDL* textureSDL)
	{
		_textures.erase(textureSDL->__itList.value());
		textureSDL->__itList.reset();
	}
}