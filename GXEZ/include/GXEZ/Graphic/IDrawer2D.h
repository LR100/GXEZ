#ifndef IDRAWER2D_H_
#define IDRAWER2D_H_

// STD
#include <string>

// GXEZ
#include "GXEZ/Graphic/Color.h"
#include "GXEZ/Graphic/Sprite.h"
#include "GXEZ/Graphic/IImage.h"

#ifdef DrawText
#undef DrawText
#endif

namespace GXEZ
{

	class IDrawer2D
	{
	public:
		/// SHOULD INIT HIS TEXTURE AND SHAPES FROM A FILE AT LAUNCHING
		//
		enum class UnitDraw
		{
			ABSOLUTE_PX,
			RELATIVE_TO_SHAPE,
		};

		/*
		EVERY Shapes will always be drawn from the top left pos to the bot right pos!!
		*/

		virtual ~IDrawer2D() {};

		virtual void 					Quit() = 0;

		// INIT
		virtual void					SetDefaultFont(const std::string& name) = 0;
		virtual void					SetDefaultFontSize(const unsigned int& size) = 0;

		// Images
		virtual bool					SetCurrentImage(IImage* image) = 0;
		virtual IImage* GetCurrentImage() const = 0;
		virtual void					SetDefaultImage(const std::string& name) = 0;
		virtual const std::string& GetDefaultImageName() = 0;
		virtual void					UseDefaultImage() = 0;

		virtual void					SetDefaultFormatImage(const ColorFormat& format) = 0;
		virtual const ColorFormat& GetDefaultFormatImage() = 0;

		virtual IImage* CreateImage(const std::string& name, const unsigned int& w, const unsigned int& h, const ColorFormat& format = ColorFormat::DEFAULT) = 0;
		virtual void					UseImage(const std::string& name) = 0;
		virtual IImage* GetImage(const std::string& name) = 0;
		virtual IImage* RemoveImage(const std::string& name) = 0;

		// Tools
		virtual void					ScaleImage(const unsigned int& sW, const unsigned int& sH) = 0;
		virtual void					ClearImage() = 0;
		virtual void					CopyImage(const std::string& nameToDraw) = 0;
		virtual void					DrawImage(const std::string& nameToDraw, const int& xPos, const int& yPos) = 0;

		// Shapes
		virtual void					DrawPoint(const int& x, const int& y, const Color& color) = 0;
		virtual void					DrawLine(const int& x, const int& y, const int& xE, const int& yE, const Color& color) = 0;
		virtual void					DrawLine(const int& x, const int& y, const int& xE, const int& yE, const Color& colorB, const Color& colorE) = 0;

		// Rect 
		struct Rect
		{
			Rect();
			Rect(unsigned int width, unsigned int height, Color color);

			unsigned int	width;
			unsigned int	height;
			Color			color;
			float			radius;
			UnitDraw		radiusType;
		};

		struct RectBorder : public Rect
		{
			RectBorder() {
				size = 0;
			}
			float			size;
		};


		virtual void					DrawRectBorder(const int& x, const int& y, const RectBorder& borderdef) = 0;
		virtual void					DrawRect(const int& x, const int& y, const Rect& rect) = 0;
		virtual void					DrawRectFill(const int& x, const int& y, const unsigned int& width, const unsigned int& height, float fillscale, const Color& color) = 0;


		struct Circle
		{
			enum class Part {
				FULL = 0,
				TOP,
				TOP_LEFT,
				TOP_RIGHT,
				BOTTOM,
				BOTTOM_LEFT,
				BOTTOM_RIGHT
			};

			Circle();
			Circle(unsigned int diameter, Color color, unsigned int thickness = 1, Circle::Part part = IDrawer2D::Circle::Part::FULL);

			Part			part;
			unsigned int	diameter;
			unsigned int	thickness;
			Color			color;
		};

		virtual void					DrawCircleFill(const int& x, const int& y, const unsigned int& diameter, const Color& color) = 0;
		virtual void					DrawCircleFill(const int& x, const int& y, const unsigned int& diameter, const Color& colora, const Color& colorb) = 0;
		virtual void					DrawCircle(const int& x, const int& y, const unsigned int& diameter, const Color& color) = 0;
		virtual void					DrawCircle(const int& x, const int& y, const Circle& circle) = 0;

		// Text Part
		virtual int						LoadText(const std::string& text, const unsigned int& sFont, const std::string& font, const Color& color) = 0;
		virtual void					DrawText(const int& x, const int& y) = 0;
		virtual void					GetSizeText(unsigned int& w, unsigned int& h) = 0;

		// Sprites

		virtual void					AddSprite(const std::string& id, Sprite* sprite) = 0;
		virtual	void					LoadSprite(const std::string& id, const std::string& path) = 0;
		// If NULL -> No Color Transparency
		virtual void					SetSpriteColorTransparency(const Color& color) = 0;
		virtual void					SetSprite(const std::string& id) = 0; // (Set Current Sprite and use it for next Draw);
		virtual void					DrawSprite(const int& x, const int& y) = 0;
		virtual void					DrawSprite(const int& x, const int& y, Sprite* sprite) = 0;
	};

}

#endif /* !IDRAWER2D_H_ */