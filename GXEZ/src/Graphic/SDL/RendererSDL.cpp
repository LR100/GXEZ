#include "GXEZ/Graphic/SDL/RendererSDL.h"
#include "GXEZ/Graphic/SDL/TextureSDL.h"

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
			_rendererUsed = windowSDL->GetSDLRenderer();	
		}
	}

	void RendererSDL::SetRenderTarget(ATexture* texture)
	{
		// If no renderer currently used - use one from any linked window (SDL have one renderer per window)
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
		}
	}

	ATexture* RendererSDL::CreateTexture(const ATexture::Definition& definition, ATexture* texture)
	{
		ATexture* ntexture;

		if (texture == NULL)
		{
			ntexture = new TextureSDL(definition, this);
		}
		else {
			ntexture = texture;
		}
		return (ntexture);
	}



	void RendererSDL::PrepareScene()
	{
		SDL_SetRenderDrawColor(_rendererUsed, 0, 0, 0, 255);
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
	}

	void RendererSDL::DrawCircle(const Vec2i& pos, const Circle& circle)
	{
	}

	void RendererSDL::DrawText(const Vec2i& pos, const Text& text)
	{
	}

}
