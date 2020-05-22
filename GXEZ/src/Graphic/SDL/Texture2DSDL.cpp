#include "GXEZ/Graphic/SDL/Texture2DSDL.h"
#include "GXEZ/Graphic/SDL/RendererSDL.h"

// STD
#include <iostream> // Error Debug

// SDL
#include <SDL_image.h>

namespace GXEZ
{
	Texture2DSDL::Texture2DSDL(const Definition& definition, RendererSDL* rendererSDL) : ATexture2D(definition, (IRenderer*)rendererSDL)
	{
		_rendererSDL = rendererSDL;
		if (!Init(definition))
		{
			std::cerr << "Texture2DSDL::Init : ERROR : Failed to Init" << std::endl;
			// throw ("Texture2DSDL::Texture2DSDL : ERROR : Failed to Init");
		}
	}

	Texture2DSDL::~Texture2DSDL()
	{
		if (_textureSDL)
		{
			SDL_DestroyTexture(_textureSDL);
			_textureSDL = NULL;
		}
	}

	SDL_Texture* Texture2DSDL::GetSDLTexture() const
	{
		return (_textureSDL);
	}

	bool Texture2DSDL::Init(const Definition& definition)
	{
		_textureAccess = SDL_TEXTUREACCESS_TARGET;
		if (definition.file.size())
		{ // Load File
			// Temporary Variables
			SDL_Surface* surface = NULL;
			SDL_Texture* texture;

			if (definition.file.find(".bmp") == (definition.file.size() - 5))
			{
				surface = SDL_LoadBMP(definition.file.c_str());
				if (surface == NULL)
				{
					std::cerr << "Texture2DSDL::Init : ERROR : SDL_LoadBMP :" << SDL_GetError() << std::endl;
					return (false);
				}
			}
			else
			{
				surface = IMG_Load(definition.file.c_str());
				if (surface == NULL)
				{
					std::cerr << "Texture2DSDL::Init : ERROR : SDL_IMG_Load :" << SDL_GetError() << std::endl;
					return (false);
				}
			}

			if (surface)
			{
				Uint32 colorkey = SDL_MapRGB(surface->format, 0, 0, 0);
				SDL_SetColorKey(surface, SDL_TRUE, colorkey);
				texture = SDL_CreateTextureFromSurface(_rendererSDL->GetRendererUsed(), surface);
				if (texture == NULL)
				{
					std::cerr << "Texture2DSDL::Init : ERROR : SDL_CreateTextureFromSurface :" << SDL_GetError() << std::endl;
					return (false);
				}
				
				_textureSDL = SDL_CreateTexture(_rendererSDL->GetRendererUsed(), SDL_PIXELFORMAT_RGBA8888, _textureAccess, surface->w, surface->h);
				if (_textureSDL == NULL)
				{
					std::cerr << "Texture2DSDL::Init : ERROR : SDL_CreateTexture :" << SDL_GetError() << std::endl;
					return (false);
				}
				SDL_SetRenderTarget(_rendererSDL->GetRendererUsed(), _textureSDL); /* Render Target is now textureSDL. */
				SDL_RenderCopy(_rendererSDL->GetRendererUsed(), texture, NULL, NULL); /* Copy texture on textureSDL */
				SDL_DestroyTexture(texture);
				_definition.size.x = surface->w;
				_definition.size.y = surface->h;
				SDL_FreeSurface(surface);
				SDL_SetRenderTarget(_rendererSDL->GetRendererUsed(), NULL); // Render Target is now the renderer again
				// std::cout << "Texture2DSDL::Init : OK (" << definition.file << ")" << std::endl;
			}
			_center.x = (_definition.size.x / 2);
			_center.y = (_definition.size.y / 2);
		}
		else if (definition.size.x > 0 && definition.size.y > 0)
		{ // Create an empty Texture 
			_textureAccess = SDL_TEXTUREACCESS_TARGET;
			_textureSDL = SDL_CreateTexture(_rendererSDL->GetRendererUsed(), SDL_PIXELFORMAT_RGBA8888, _textureAccess, definition.size.x, definition.size.y);
			if (_textureSDL == NULL)
			{
				std::cerr << "Texture2DSDL::Init : ERROR : SDL_CreateTexture :" << SDL_GetError() << std::endl;
				return (false);
			}
			_center.x = (_definition.size.x / 2);
			_center.y = (_definition.size.y / 2);
			// std::cout << "Texture2DSDL::Init : OK (" << definition.size.ToString() << ")" << std::endl;
		}
		else
		{
		}

		SDL_SetTextureBlendMode(_textureSDL, SDL_BLENDMODE_BLEND);

		return (true);
	}

	void Texture2DSDL::UseAsRenderTarget()
	{
		_rendererSDL->SetRenderTarget(this);
	}
}
