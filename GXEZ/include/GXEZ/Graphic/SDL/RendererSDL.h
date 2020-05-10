#ifndef RENDERERSDL_H_
#define RENDERERSDL_H_

// STD
#include <list>

// GXEZ
#include "GXEZ/Graphic/IRenderer.h"
#include "GXEZ/Graphic/SDL/WindowSDL.h"

namespace GXEZ
{
	// Drawer do not depends of IWindow // Renderer does
	class RendererSDL : public IRenderer
	{
		static RendererSDL* _singleton;

	public:

		static RendererSDL* Get(WindowSDL* windowSDL) noexcept { 
			if (_singleton == NULL)
			{
				_singleton = new RendererSDL(windowSDL);
			}
			else 
			{
				_singleton->LinkWindow(windowSDL);
			}
			return (_singleton);
		}

		virtual ~RendererSDL() {};

		// Window
		virtual void	LinkWindow(IWindow* window) override;

		// Render Target
		virtual void	SetRenderTarget(IWindow* window) override;
		virtual void	SetRenderTarget(ATexture* texture) override;

		// Textures
		virtual ATexture* CreateTexture(const ATexture::Definition& definition, ATexture* texture = NULL) override;

		// Scene Init and Render
		virtual void	PrepareScene() override;
		virtual void	PresentScene() override;

		/// 2D PRIMITIVE ///

		// Point
		virtual void					DrawPoint(const Vec2i& pos, const Color& color) override;
		// Line
		virtual void					DrawLine(const Vec2i& a, const Vec2i& b, const Color& color) override;
		// Rect
		virtual void					DrawRectBorder(const Vec2i& pos, const RectBorder& borderdef) override;
		virtual void					DrawRect(const Vec2i& pos, const Rect& rect) override;
		// Circle
		virtual void					DrawCircle(const Vec2i& pos, const Circle& circle) override;
		// Text
		virtual void					DrawText(const Vec2i& pos, const Text& text) override;

	private:

		RendererSDL(WindowSDL* windowSDL);

		std::list<WindowSDL*>		_windowLinked;
		SDL_Renderer*				_rendererUsed;
	};
}

#endif // !RENDERERSDL_H_

