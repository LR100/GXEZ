#ifndef _DRAWER2DSDL_H_
#define _DRAWER2DSDL_H_

// STD
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <sstream>

// GXEZ
#include "GXEZ/GXEZDefinitions.h"
#include "GXEZ/Graphic/SDL/ImageSDL.h"
#include "GXEZ/Graphic/IDrawer2D.h"

// MXEZ
#include "MXEZ/Value/ValueTools.h"
#include "MXEZ/Vec/Vec.h"
using namespace MXEZ;

#if GXEZ_SDL

// SDL
#include <SDL_image.h> // Using SDL_Image Lib for Loading images files (.jpeg, .png, ...)
#include <SDL_ttf.h> // Using TTF SDL (for text drawing)

#ifdef DrawText
#undef DrawText
#endif

namespace GXEZ
{

	class GXEZContextSDL;

#define GXEZ_DRAWER2DSDL_TRSPR 255
#define GXEZ_DRAWER2DSDL_TRSPG 24
#define GXEZ_DRAWER2DSDL_TRSPB 244

	class Drawer2DSDL : public IDrawer2D
	{
	public:
		Drawer2DSDL(GXEZContextSDL* context, const ColorFormat& format);


		// SINGLETON BY CREATING
		void* operator new(size_t size);

	private:

		Drawer2DSDL(const Drawer2DSDL& other);
		Drawer2DSDL(const Drawer2DSDL&& other);

		virtual ~Drawer2DSDL();
		static Drawer2DSDL* sptr; // SINGLETON INSTANCE	

	public:

		virtual void 					Quit() override;

		// INIT
		virtual void					SetDefaultFont(const std::string& name) override;
		virtual void					SetDefaultFontSize(const unsigned int& size) override;

		// Images
		virtual bool					SetCurrentImage(IImage* image) override;
		virtual IImage* GetCurrentImage() const override;
		virtual void					SetDefaultImage(const std::string& name) override;
		virtual const std::string& GetDefaultImageName() override;
		virtual void					UseDefaultImage() override;

		virtual void					SetDefaultFormatImage(const ColorFormat& format) override;
		virtual const ColorFormat& GetDefaultFormatImage() override;

		virtual IImage* CreateImage(const std::string& name, const unsigned int& w, const unsigned int& h, const ColorFormat& format) override;
		virtual void					UseImage(const std::string& name) override;
		virtual IImage* GetImage(const std::string& name) override;
		virtual IImage* RemoveImage(const std::string& name) override;

		// Tools
		virtual void					ScaleImage(const unsigned int& sW, const unsigned int& sH) override;
		virtual void					ClearImage() override;
		virtual void					CopyImage(const std::string& nameToDraw) override;
		virtual void					DrawImage(const std::string& nameToDraw, const int& xPos, const int& yPos) override;

		// Shapes
		virtual void					DrawPoint(const int& x, const int& y, const Color& color) override;
		virtual void					DrawLine(const int& x, const int& y, const int& xE, const int& yE, const Color& color) override;
		virtual void					DrawLine(const int& x, const int& y, const int& xE, const int& yE, const Color& colorB, const Color& colorE) override;

		virtual void					DrawRectBorder(const int& x, const int& y, const RectBorder& border) override;
		virtual void					DrawRect(const int& x, const int& y, const Rect& rect) override;
		virtual void					DrawRectFill(const int& x, const int& y, const unsigned int& width, const unsigned int& height, float fillscale, const Color& color) override;

		virtual void					DrawCircleFill(const int& x, const int& y, const unsigned int& diameter, const Color& color) override;
		virtual void					DrawCircleFill(const int& x, const int& y, const unsigned int& diameter, const Color& colora, const Color& colorb) override;
		virtual void					DrawCircle(const int& x, const int& y, const unsigned int& diameter, const Color& color) override;
		virtual void					DrawCircle(const int& x, const int& y, const Circle& circle) override;

		// Text Part
		virtual int						LoadText(const std::string& text, const unsigned int& sFont, const std::string& font, const Color& color) override;
		virtual void					GetSizeText(unsigned int& w, unsigned int& h) override;
		virtual void					DrawText(const int& xPos, const int& yPos) override;

		// Sprites
		virtual	void					AddSprite(const std::string& id, Sprite* sprite) override;
		virtual	void					LoadSprite(const std::string& id, const std::string& path) override;
		virtual void					SetSprite(const std::string& id) override; // (Set Current Sprite and use it for next Draw);
		virtual void					SetSpriteColorTransparency(const Color& color) override;
		virtual void					DrawSprite(const int& x, const int& y) override;
		virtual void					DrawSprite(const int& x, const int& y, Sprite* sprite) override;
		///
		// OTHERS -
		///
		// SDL Specification
		void	AddImage(const std::string& name, ImageSDL* img);

		/// //////////// ///
		///  DRAW STRUCT ///
		/// //////////// ///

		// Rectangle 
		struct DrawRectSnakeAttrib
		{
			unsigned int minX;
			unsigned int minY;
			unsigned int maxX;
			unsigned int maxY;
			unsigned int x;
			unsigned int y;
			char dir;
			unsigned int filledRect;
			unsigned int rectToFill;

			void Init(unsigned int width, unsigned int height, unsigned int xPos, unsigned int yPos, float fillScale);
			void IncDrawRectangle(char& dir, unsigned int& filledRect, unsigned int& x, unsigned int& y, unsigned int& minX, unsigned int& minY, unsigned int& maxX, unsigned int& maxY);
		};

		struct PolyEdge
		{
			int		min_y;
			int		max_y;
			int		x_to_min_y;
			int		sign;
			int		dx;
			int		dy;
			float	sum;
		};

	private:

		inline void CorrectXY(int& x, int& y);
		inline int	CorrectX(const int& x);
		inline int	CorrectY(const int& y);

		inline void	InternSetPixel(const int& x, const int& y, const unsigned int& color);
		inline void	InternCheckBFSetPixel(const int& x, const int& y, const unsigned int& color);
		inline void InternSetLine(const int& x, const int& y, const int& xe, const Sprite::Line& line);

		struct Font
		{
			TTF_Font* font;
		};

		struct Text
		{
			SDL_Surface* text;
		};

		// INITIALISATION
		void		Init();

		// Images
		ColorFormat										_defaultImageFormat;
		ImageSDL* _currImage;
		std::string										_currImageName;
		ImageSDL* _defaultImage;
		std::string										_defaultImageName;
		std::unordered_map<std::string, ImageSDL*>		_images;
		// Text
		std::unordered_map<std::string, Font>			_fonts;
		Text											_textActive;
		// Sprites
		std::unordered_map<std::string, Sprite*>		_sprites;
		Sprite* _spriteActive;
		Color* _spriteColorTransparency;

		// Drawing Optimization STRUCT //
		struct InfoLine
		{
			int		deltaX, deltaY;
			int		incX, incY;
			int		i;
			int		sum;
			int		xi, yi;
		};

		struct InfoRect
		{
			bool	Init(const int& xr, const int& yr, const unsigned int& widthMax, const unsigned int& heightMax, const unsigned int& width, const unsigned int& height);

			int		xMax, yMax;
			int		xi, yi;
			int		iw, ih;
			int		cX, cY;
			int		cXMax, cYMax;
		};

		struct InfoCircle
		{
			int		xi, yi;
			int		d;
			int		dx, dy;
			int		radius;
		};

		struct InfoCircleBressenham
		{
			void	Init(const float& diameter);
			int		r;
			int		x, y;
			int		d;
		};

		class DrawerCircleBressenham
		{
		public:
			DrawerCircleBressenham(Drawer2DSDL* drawer);
			// Use X and Y as center for each parts
			void	Draw(const int& _x, const int& _y, const Circle& circle);
			// Use Part Parts Position
			void	Draw(const Circle& circle);
			void	SetPartsPosition(const Vec2i& pos, const Circle::Part& part);

		private:

			Vec2i	pTL, pTR, pBL, pBR;
			int		t;
			int		r;
			int		x, y;
			int		d;

			typedef			void(DrawerCircleBressenham::* DrawCircleBressenhamFunction)();

			inline void	DrawFull();
			inline void	DrawTop();
			inline void	DrawTopLeft();
			inline void	DrawTopRight();
			inline void	DrawBottom();
			inline void	DrawBottomLeft();
			inline void	DrawBottomRight();

			Circle							_circle;
			DrawCircleBressenhamFunction	_drawFct;
			Drawer2DSDL* _drawer;
		};

		class IDrawerRect
		{
		public:
			IDrawerRect(Drawer2DSDL* drawer);

		protected:
			virtual void	DrawStart(const int& _x, const int& _y, Rect& rect);
			// Can verify additional things on init if everything was fine on classic init
			virtual bool	OnDrawInit() { return (true); };

			virtual void	DrawClassic() = 0;
			virtual void	DrawRadius() = 0;

			int				xr, yr;
			InfoRect		_iR;
			Drawer2DSDL* _drawer;


		private:
			bool			DrawInit(const int& _x, const int& _y, Rect& rect);
		};

		class DrawerRect : public IDrawerRect
		{
		public:
			DrawerRect(Drawer2DSDL* drawer) : IDrawerRect(drawer) {};
			void	Draw(const int& _x, const int& _y, const Rect& rect);
		private:
			virtual void	DrawClassic() override;
			virtual void	DrawRadius() override;

			Rect			_rect;
		};

		class DrawerRectBorder : public IDrawerRect
		{
		public:
			DrawerRectBorder(Drawer2DSDL* drawer) : IDrawerRect(drawer) {};
			void	Draw(const int& _x, const int& _y, const RectBorder& rectBorder);
		private:
			virtual void	DrawClassic() override;
			virtual void	DrawRadius() override;

			RectBorder		_rectBorder;
		};

		struct InfoInternLine
		{
			int		x;
			int		xmem;
		};

		// Line
		InfoLine										_iL;
		InfoInternLine									_iIL;
		// Rect
		InfoRect										_iR;
		DrawerRect										_dR;
		// Rect Border
		DrawerRectBorder								_dRB;
		// Circle
		InfoCircle										_iC;
		DrawerCircleBressenham							_dCBr;


		// Colors Manipulation attributs
		float											_offsetColor;
		float											_stepColor;
		ColorFDef										_colorFA;
		ColorFDef										_colorFB;
		ColorFDef										_colorFC;
		GXEZContextSDL*									_context;
	};

}

#endif // GXEX_SDL2

#endif /* !_DRAWER2DSDL_H_ */