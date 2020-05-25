#ifndef RENDERERSDL_H_
#define RENDERERSDL_H_

// STD
#include <list>

// GXEZ
#include "GXEZ/Graphic/IRenderer.h"
#include "GXEZ/Graphic/SDL/WindowSDL.h"
#include "GXEZ/Graphic/SDL/Texture2DSDL.h"
#include "GXEZ/Graphic/SDL/Texture2DManagerSDL.h"

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
		bool			SetRenderTarget(IWindow* window) override;
		bool			SetRenderTarget(WindowSDL* windowSDL);
		bool			SetRenderTarget(ATexture2D* texture) override;
		bool			SetRenderTarget(Texture2DSDL* textureSDL);

		// Textures
		ATexture2D*		CreateTexture2D(const ATexture2D::Definition& definition) override;
		void			RemoveTexture2D(ATexture2D* texture) override;

		// Scene Init and Render
		void			PrepareScene() override;
		void			PresentScene() override;

		/// 2D PRIMITIVE ///

		// Point
		virtual void					DrawPoint(const Vec2i& pos, const Color& color) override;
		// Line
		virtual void					DrawLine(const Vec2i& a, const Vec2i& b, const Color& color) override;
		// Rect
		//virtual void					DrawRectBorder(const Vec2i& pos, const RectBorder& borderdef) override;
		//virtual void					DrawRect(const Vec2i& pos, const Rect& rect) override;
		// Circle
		//virtual void					DrawCircle(const Vec2i& pos, const Circle& circle) override;
		// Text
		virtual void					DrawText(const Vec2i& pos, const Text& text) override;

		/// 2D Textures ///
		virtual void					DrawTexture(const Vec2i& pos, ATexture2D* texture, float rotation) override;
		void							DrawTexture(const Vec2i& pos, Texture2DSDL* textureSDL, float rotation);

		// SDL //
		SDL_Renderer*					GetSDLRendererUsed();
		const uint32_t&					GetSDLPixelFormat() const;

	private:

		////////////
		// Window //

		// Events
		void					OnWindowResize(WindowSDL* window);

		/////////////////////// Inherit From ADrawer2DExtended ///////////////////
		// Point
		void					InternDrawPoint(const Vec2i& pos, const Color& color) override;
		// Line
		void					InternDrawLine(const Vec2i& a, const Vec2i& b, const Color& color) override;
		// Rect
		void					InternDrawRectBorder(const Vec2i& pos, const IDrawer2D::RectBorder& borderdef) override;
		void					InternDrawRect(const Vec2i& pos, const IDrawer2D::Rect& rect) override;

		// Search in linked window a renderer to be used for creating a Texture2D for exemple
		bool		FindSDLRenderer();

		//////////////////////////////////////////////////////////////////////////
		RendererSDL(WindowSDL* windowSDL); // SINGLETON - (Private Constructor) // 

		std::list<WindowSDL*>		_windowLinked;
		Texture2DManagerSDL*		_textureManager;
		SDL_Renderer*				_rendererUsed;
		uint32_t					_pixelFormat;
		SDL_Rect					_rectSDL;
		SDL_Point					_pointSDL;;
	};
}

#endif // !RENDERERSDL_H_

