#ifndef RENDERERSDL_H_
#define RENDERERSDL_H_

// STD
#include <list>

// GXEZ
#include "GXEZ/Graphic/IRenderer.h"
#include "GXEZ/Graphic/SDL/WindowSDL.h"
#include "GXEZ/Graphic/SDL/Texture2DSDL.h"

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
		void			SetRenderTarget(WindowSDL* windowSDL);
		virtual void	SetRenderTarget(ATexture2D* texture) override;
		void			SetRenderTarget(Texture2DSDL* textureSDL);

		// Textures
		virtual ATexture2D* CreateTexture2D(const ATexture2D::Definition& definition, ATexture2D* texture = NULL) override;


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

		/// 2D Textures ///
		virtual void					DrawTexture(const Vec2i& pos, ATexture2D* texture, float rotation) override;
		void							DrawTexture(const Vec2i& pos, Texture2DSDL* textureSDL, float rotation);

		// SDL //
		SDL_Renderer*					GetRendererUsed();

	private:

		// Search in linked window a renderer to be used for creating a Texture2D for exemple
		bool		FindRendererSDL();

		RendererSDL(WindowSDL* windowSDL);

		std::list<WindowSDL*>		_windowLinked;
		SDL_Renderer*				_rendererUsed;
		SDL_Rect					_rectSDL;
		SDL_Point					_pointSDL;;
	};
}

#endif // !RENDERERSDL_H_

