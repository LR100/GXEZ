#ifndef IRENDERER_H_
#define IRENDERER_H_

#include "GXEZ/Graphic/ADrawer2DExtended.h"
#include "GXEZ/Graphic/IWindow.h"
#include "GXEZ/Graphic/ATexture2D.h"

namespace GXEZ
{
	class IRenderer : public ADrawer2DExtended
	{
	public:
		IRenderer(IWindow* window) {};
		virtual ~IRenderer() {};

		virtual void LinkWindow(IWindow* window) = 0;

		// Renders targets can be or a window or a texture

		// Remind to use the target window before any operation (texture creating, 
		virtual bool		SetRenderTarget(IWindow* window) = 0;
		virtual bool		SetRenderTarget(ATexture2D* texture) = 0;

		// Textures
		virtual ATexture2D* CreateTexture2D(const ATexture2D::Definition& definition) = 0;
		virtual void		RemoveTexture2D(ATexture2D* texture) = 0;

		virtual void		DrawTexture(const Vec2i& pos, ATexture2D* texture, float rotation = 0) = 0;

		/// Scenes
		virtual void		PrepareScene() = 0;
		virtual void		PresentScene() = 0;
	};
}

#endif // !IRENDERER_H_

//#ifndef IRENDERER_H_
//#define IRENDERER_H_
//#endif // !IRENDERER_H_