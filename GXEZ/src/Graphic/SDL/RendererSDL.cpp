#include "GXEZ/Graphic/SDL/RendererSDL.h"

// STD
#include <iostream>

namespace GXEZ
{
	RendererSDL* RendererSDL::_singleton = NULL;

	RendererSDL::RendererSDL(WindowSDL* windowSDL) : IRenderer(windowSDL)
	{
		std::cout << "RendererSDL::RendererSDL()" << std::endl;
		_rendererUsed = NULL;
		
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best")) {
			std::cerr << "RendererSDL::SDL_Hint SDL_HINT_RENDER_SCALE_QUALITY (best) : ERROR : " << SDL_GetError() << std::endl;
			if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"))
				std::cerr << "RendererSDL::SDL_Hint SDL_HINT_RENDER_SCALE_QUALITY (linear) : ERROR : " << SDL_GetError() << std::endl;
			else 
				std::cout << "RendererSDL::SDL_Hint SDL_HINT_RENDER_SCALE_QUALITY (linear) : OK" << std::endl;
		}
		else
			std::cout << "RendererSDL::SDL_Hint SDL_HINT_RENDER_SCALE_QUALITY (best) : OK" << std::endl;
		// Link the first window to have at least one renderer created
		LinkWindow(windowSDL);
		// and set the pixel format from the first window as reference pixel format
		_pixelFormat = SDL_GetWindowPixelFormat(windowSDL->GetSDLWindow()); 
		// Then init other components
		_textureManager = new Texture2DManagerSDL(this);
	}

	void RendererSDL::LinkWindow(IWindow* window)
	{
		WindowSDL* windowSDL = dynamic_cast<WindowSDL*>(window);
		if (windowSDL)
		{
			int paramsRenderer = SDL_RENDERER_ACCELERATED; // | SDL_RENDERER_PRESENTVSYNC
			_rendererUsed = SDL_CreateRenderer(windowSDL->GetSDLWindow(), -1, paramsRenderer);
			// Window Renderer
			windowSDL->SetSDLRenderer(_rendererUsed);
			// Window Events
			windowSDL->LinkEvent(ControlKey::WINDOW_RESIZE, &RendererSDL::OnWindowResize, this, windowSDL);
			_windowLinked.push_back(windowSDL);
		}
	}

	bool RendererSDL::SetRenderTarget(IWindow* window)
	{
		WindowSDL* windowSDL = dynamic_cast<WindowSDL*>(window);
		if (windowSDL) {
			return (SetRenderTarget(windowSDL));
		}
		return (false);
	}

	bool RendererSDL::SetRenderTarget(WindowSDL* windowSDL)
	{
		// std::cout << "SetRenderTarget(Window) SDL" << std::endl;
		if (windowSDL->GetSDLRenderer())
		{
			_rendererUsed = windowSDL->GetSDLRenderer();
			SDL_SetRenderDrawBlendMode(_rendererUsed, SDL_BLENDMODE_NONE);
			SDL_SetRenderTarget(_rendererUsed, NULL);
			return (true);
		}
		return (false);
	}

	bool RendererSDL::SetRenderTarget(ATexture2D* texture)
	{
		Texture2DSDL* textureSDL = dynamic_cast<Texture2DSDL*>(texture);
		if (textureSDL) {
			return (SetRenderTarget(textureSDL));
		}
		return (false);
	}

	bool RendererSDL::SetRenderTarget(Texture2DSDL* texture)
	{
		// std::cout << "SetRenderTarget(Texture) SDL" << std::endl;
		//// If no renderer currently used - use one from any linked window (SDL have one renderer per window)
		if (FindSDLRenderer()) {
			// SDL_SetRenderDrawBlendMode(_rendererUsed, SDL_BLENDMODE_NONE);
			SDL_SetRenderTarget(_rendererUsed, texture->GetSDLTexture());
			return (true);
		}
		return (false);
	}

	ATexture2D* RendererSDL::CreateTexture2D(const ATexture2D::Definition& definition)
	{
		return (_textureManager->CreateTexture2D(definition));
	}

	void RendererSDL::RemoveTexture2D(ATexture2D* texture)
	{
		_textureManager->RemoveTexture2D(texture);
	}
	void RendererSDL::PrepareScene()
	{
		SDL_SetRenderDrawColor(_rendererUsed, 20, 20, 20, 255);
		SDL_SetRenderDrawBlendMode(_rendererUsed, SDL_BLENDMODE_BLEND);
		SDL_RenderClear(_rendererUsed);
	}

	void RendererSDL::PresentScene()
	{
		SDL_RenderPresent(_rendererUsed);
	}

	void RendererSDL::DrawPoint(const Vec2i& pos, const Color& color)
	{
		SDL_SetRenderDrawColor(_rendererUsed, color.r(), color.g(), color.b(), color.a());
		SDL_RenderDrawPoint(_rendererUsed, pos.x, pos.y);
	}

	void RendererSDL::DrawLine(const Vec2i& a, const Vec2i& b, const Color& color)
	{
		SDL_SetRenderDrawColor(_rendererUsed, color.r(), color.g(), color.b(), color.a());
		SDL_RenderDrawLine(_rendererUsed, a.x, a.x, b.x, b.y);
	}

	//void RendererSDL::DrawRectBorder(const Vec2i& pos, const RectBorder& borderdef)
	//{

	//}

	//void RendererSDL::DrawRect(const Vec2i& pos, const Rect& rect)
	//{
	//	SDL_SetRenderDrawColor(_rendererUsed, rect.color.r(), rect.color.g(), rect.color.b(), rect.color.a());
	//	_rectSDL.x = pos.x;
	//	_rectSDL.y = pos.y;
	//	_rectSDL.w = rect.width;
	//	_rectSDL.h = rect.height;
	//	SDL_RenderDrawRect(_rendererUsed, &_rectSDL);
	//}

	//void RendererSDL::DrawCircle(const Vec2i& pos, const Circle& circle)
	//{
	//}

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
			SDL_SetTextureBlendMode(textureSDL->GetSDLTexture(), SDL_BLENDMODE_BLEND);
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

	SDL_Renderer* RendererSDL::GetSDLRendererUsed()
	{
		if (FindSDLRenderer()) {
			return (_rendererUsed);
		}
		return (NULL);
	}

	const uint32_t& RendererSDL::GetSDLPixelFormat() const
	{
		return (_pixelFormat);
	}

	void RendererSDL::OnWindowResize(WindowSDL* window)
	{
		std::cout << "RendererSDL::OnWindowResize Window ID(" << window->GetID() << ")" << std::endl;
		// SDL seems to destroy content of SDL_Texture with SDL_TEXTUREACCESS_TARGET flag on Window Resize so restore them ...
		_textureManager->RestoreTextureWithSDLTextureAccess(SDL_TEXTUREACCESS_TARGET);
	}

	/////////////////////////////
	/// ADRAWER2D EXTENDED /////////
	/////////////////////////////

	void RendererSDL::InternDrawPoint(const Vec2i& pos, const Color& color)
	{
		SDL_SetRenderDrawColor(_rendererUsed, color.r(), color.g(), color.b(), color.a());
		SDL_RenderDrawPoint(_rendererUsed, pos.x, pos.y);
	}

	void RendererSDL::InternDrawLine(const Vec2i& a, const Vec2i& b, const Color& color)
	{
		SDL_SetRenderDrawColor(_rendererUsed, color.r(), color.g(), color.b(), color.a());
		SDL_RenderDrawLine(_rendererUsed, a.x, a.y, b.x, b.y);
	}

	void RendererSDL::InternDrawRectBorder(const Vec2i& pos, const IDrawer2D::RectBorder& borderdef)
	{
		SDL_SetRenderDrawColor(_rendererUsed, borderdef.color.r(), borderdef.color.g(), borderdef.color.b(), borderdef.color.a());
		_rectSDL.x = pos.x;
		_rectSDL.y = pos.y;
		_rectSDL.w = borderdef.width;
		_rectSDL.h = borderdef.height;
		SDL_RenderDrawRect(_rendererUsed, &_rectSDL);
	}

	void RendererSDL::InternDrawRect(const Vec2i& pos, const IDrawer2D::Rect& rect)
	{
		SDL_SetRenderDrawColor(_rendererUsed, rect.color.r(), rect.color.g(), rect.color.b(), rect.color.a());
		_rectSDL.x = pos.x;
		_rectSDL.y = pos.y;
		_rectSDL.w = rect.width;
		_rectSDL.h = rect.height;
		SDL_RenderFillRect(_rendererUsed, &_rectSDL);
	}

	/////////////////////////////
	/// PRIVATE METHODS /////////
	/////////////////////////////


	bool RendererSDL::FindSDLRenderer()
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
