#include "GXEZ/Graphic/SDL/RendererSDL.h"
#include "GXEZ/Graphic/SDL/Texture2DSDL.h"

namespace GXEZ
{
	RendererSDL* RendererSDL::_singleton = NULL;

	RendererSDL::RendererSDL(WindowSDL* windowSDL) : IRenderer(windowSDL)
	{
		_rendererUsed = NULL;
		LinkWindow(windowSDL);
	}

	void RendererSDL::LinkWindow(IWindow* window)
	{
		WindowSDL* windowSDL = dynamic_cast<WindowSDL*>(window);
		if (windowSDL)
		{
			_rendererUsed = SDL_CreateRenderer(windowSDL->GetSDLWindow(), -1, SDL_RENDERER_ACCELERATED);
			
			
			windowSDL->SetSDLRenderer(_rendererUsed);
			_windowLinked.push_back(windowSDL);
		}
	}

	void RendererSDL::SetRenderTarget(IWindow* window)
	{
		WindowSDL* windowSDL = dynamic_cast<WindowSDL*>(window);
		if (windowSDL) {
			SetRenderTarget(windowSDL);
		}
	}

	void RendererSDL::SetRenderTarget(WindowSDL* windowSDL)
	{
		// std::cout << "SetRenderTarget(Window) SDL" << std::endl;
		_rendererUsed = windowSDL->GetSDLRenderer();
		SDL_SetRenderDrawBlendMode(_rendererUsed, SDL_BLENDMODE_NONE);
		SDL_SetRenderTarget(_rendererUsed, NULL);
	}

	void RendererSDL::SetRenderTarget(ATexture2D* texture)
	{
		Texture2DSDL* textureSDL = dynamic_cast<Texture2DSDL*>(texture);
		if (textureSDL) {
			SetRenderTarget(textureSDL);
		}
	}

	void RendererSDL::SetRenderTarget(Texture2DSDL* texture)
	{
		// std::cout << "SetRenderTarget(Texture) SDL" << std::endl;
		//// If no renderer currently used - use one from any linked window (SDL have one renderer per window)
		if (FindRendererSDL()) {
			// SDL_SetRenderDrawBlendMode(_rendererUsed, SDL_BLENDMODE_NONE);
			SDL_SetRenderTarget(_rendererUsed, texture->GetSDLTexture());
		}
	}

	ATexture2D* RendererSDL::CreateTexture2D(const ATexture2D::Definition& definition, ATexture2D* texture)
	{
		ATexture2D* ntexture;

		if (texture == NULL)
		{
			std::cout << "Create Texture SDL" << std::endl;
			ntexture = new Texture2DSDL(definition, this);
		}
		else {
			ntexture = texture;
		}
		return (ntexture);
	}

	void RendererSDL::PrepareScene()
	{
		SDL_SetRenderDrawColor(_rendererUsed, 10, 0, 0, 255);
		// SDL_SetRenderDrawBlendMode(_rendererUsed, SDL_BLENDMODE_BLEND);
		SDL_RenderClear(_rendererUsed);
	}

	void RendererSDL::PresentScene()
	{
		SDL_RenderPresent(_rendererUsed);
	}

	void RendererSDL::DrawPoint(const Vec2i& pos, const Color& color)
	{

	}

	void RendererSDL::DrawLine(const Vec2i& a, const Vec2i& b, const Color& color)
	{
		SDL_SetRenderDrawColor(_rendererUsed, color.r(), color.g(), color.b(), color.a());
		SDL_RenderDrawLine(_rendererUsed, a.x, a.x, b.x, b.y);
	}

	void RendererSDL::DrawRectBorder(const Vec2i& pos, const RectBorder& borderdef)
	{

	}

	void RendererSDL::DrawRect(const Vec2i& pos, const Rect& rect)
	{
		SDL_SetRenderDrawColor(_rendererUsed, rect.color.r(), rect.color.g(), rect.color.b(), rect.color.a());
		_rectSDL.x = pos.x;
		_rectSDL.y = pos.y;
		_rectSDL.w = rect.width;
		_rectSDL.h = rect.height;
		SDL_RenderDrawRect(_rendererUsed, &_rectSDL);
	}

	void RendererSDL::DrawCircle(const Vec2i& pos, const Circle& circle)
	{
	}

	void RendererSDL::DrawText(const Vec2i& pos, const Text& text)
	{
	}

	void RendererSDL::DrawTexture(const Vec2i& pos, ATexture2D* texture, float rotation)
	{
		Texture2DSDL* textureSDL = dynamic_cast<Texture2DSDL*>(texture);
		if (textureSDL) {
			DrawTexture(pos, textureSDL, rotation);
		}
	}

	void RendererSDL::DrawTexture(const Vec2i& pos, Texture2DSDL* textureSDL, float rotation)
	{
		if (_rendererUsed) {
			_rectSDL.x = pos.x;
			_rectSDL.y = pos.y;
			_rectSDL.w = textureSDL->GetDefinition().size.x;
			_rectSDL.h = textureSDL->GetDefinition().size.y;
			if (rotation == 0)
			{
				SDL_RenderCopy(_rendererUsed, textureSDL->GetSDLTexture(), NULL, &_rectSDL);
			}
			else 
			{
				SDL_RenderCopyEx(_rendererUsed, textureSDL->GetSDLTexture(), NULL, &_rectSDL, rotation, NULL, SDL_FLIP_NONE);
			}
		}
	}

	SDL_Renderer* RendererSDL::GetRendererUsed()
	{
		if (FindRendererSDL()) {
			return (_rendererUsed);
		}
		return (NULL);
	}


	/////////////////////////////
	/// PRIVATE METHODS /////////
	/////////////////////////////


	bool RendererSDL::FindRendererSDL()
	{
		if (!_rendererUsed)
		{
			std::list<WindowSDL*>::iterator iter = _windowLinked.begin();
			std::list<WindowSDL*>::iterator iterEnd = _windowLinked.end();
			for (; iter != iterEnd; iter++) {
				if ((*iter)->GetSDLRenderer()) {
					_rendererUsed = (*iter)->GetSDLRenderer();
					iter = iterEnd;
				}
			}
			if (_rendererUsed != NULL) {
				return (true);
			}
			return (false);
		}
		else
		{
			return (true);
		}
	}
}
