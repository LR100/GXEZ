// GXEZ
#include "GXEZ/Graphic/SDL/ImageDrawer2DSDL.h"
#include "GXEZ/SDL/GXEZContextSDL.h"

// STD
#include <bitset>
#include <iostream> // TMP
#include "..\..\..\include\GXEZ\Graphic\SDL\ImageDrawer2DSDL.h"

#if GXEZ_SDL

namespace GXEZ
{

	ImageDrawer2DSDL* ImageDrawer2DSDL::sptr = NULL;    //initialize Singleton pointer to NULL

	ImageDrawer2DSDL::ImageDrawer2DSDL(GXEZContextSDL* context, const ColorFormat& format)
	{
		_context = context;
		_defaultImage = NULL;
		std::cout << "DRAWER 2D ADDR (" << this << ")" << std::endl;
		if (sptr != this)
		{
			std::cout << std::endl << "-ImageDrawer2DSDL()-" << std::endl;
			_spriteActive = NULL;
			_spriteColorTransparency = new Color();
			_currImage = NULL;
			std::cout << "Parameter Format (" << format << ")" << std::endl;
			if (format == ColorFormat::DEFAULT) // Get Image Format From Screen
			{
				_defaultImageFormat = ColorFormat::RGB; // Glitchy image if 4 bytes but at least no crash
			}
			else
			{
				std::cout << "SET DEF FORMAT" << std::endl;
				_defaultImageFormat = format;
			}
			std::cout << "ImageDrawer2DSDL : Default Image Format (" << ColorFormatToString(_defaultImageFormat) << ")" << std::endl;
		}

		uint32_t a;
		std::bitset<32> x;
		Color rouge(255, 0, 0);

		a = rouge.value();
		x = std::bitset<32>(a);
		std::cout << "rouge" << x << ' ' << a << ")\n";

		Color bleu(0, 0, 255);
		a = bleu.value();
		x = std::bitset<32>(a);
		std::cout << "bleu" << x << ' ' << a << ")\n";


		Color vert(0, 255, 0);
		a = vert.value();
		x = std::bitset<32>(a);
		std::cout << "vert" << x << ' ' << a << ")\n";

		sptr = this;
	}

	ImageDrawer2DSDL::~ImageDrawer2DSDL()
	{
	}

	void* ImageDrawer2DSDL::operator new(size_t size)
	{
		if (sptr != NULL)    //if already one object is created return reference to same object
		{
			std::cout << "1 + Nth Creation ImageDrawer2DSDL" << std::endl;
			std::cout << "Return Drawer 2D Img Format(" << ColorFormatToString(sptr->_defaultImageFormat) << ")" << std::endl;
			return (sptr);
		}
		else
		{
			std::cout << "FIRST ImageDrawer2DSDL" << std::endl;

			return (calloc(1, size));    //else allocate memory for one (first) object
		}
	}


	void ImageDrawer2DSDL::Quit()
	{
		// Images
		std::unordered_map<std::string, ImageSDL*>::iterator itImage = _images.begin();
		std::unordered_map<std::string, ImageSDL*>::iterator itImageEnd = _images.end();

		for (; itImage != itImageEnd; itImage++)
		{
			ImageSDL* img = itImage->second;
			if (img != NULL)
				delete (img);
		}
		_images.clear();

		_currImage = NULL;
		_currImageName = "";
		_defaultImage = NULL;
		_defaultImageName = "";

		// Text
		//std::unordered_map<std::string, Font>			_fonts;
		//Text											_textActive;
		//// Texture
		//std::unordered_map<std::string, Texture*>		_textures;
		//Texture*										_textActiveure;
	}

	void ImageDrawer2DSDL::SetDefaultFont(const std::string& name)
	{
	}

	void ImageDrawer2DSDL::SetDefaultFontSize(const unsigned int& size)
	{
	}

	bool ImageDrawer2DSDL::SetCurrentImage(IImage* image)
	{
		if (image) {
			if (image->GetFormat() == _defaultImageFormat)
			{
				_currImage = (ImageSDL*)image;
				return (true);
			}
			else
			{
				std::cerr << "ImageDrawer2DSDL::SetCurrentImage() : Bad Image Format D(" << _defaultImageFormat << ") != I(" << image->GetFormat() << ")" << std::endl;
			}
		}
		else {
			_currImage = NULL;
			return (true);
		}
		return (false);
	}

	IImage* ImageDrawer2DSDL::GetCurrentImage() const
	{
		return (_currImage);
	}

	void ImageDrawer2DSDL::SetDefaultImage(const std::string& name)
	{
		std::cerr << "ImageDrawer2DSDL::SetDefaultImage() " << std::endl;
		if (_images.count(name))
		{
			_defaultImageName = name;
			_defaultImage = _images.at(name);
		}
	}

	const std::string& ImageDrawer2DSDL::GetDefaultImageName()
	{
		return (_defaultImageName);
	}

	void ImageDrawer2DSDL::UseDefaultImage()
	{
		_currImage = _defaultImage;
		_currImageName = _defaultImageName;
	}

	void ImageDrawer2DSDL::SetDefaultFormatImage(const ColorFormat& format)
	{
		std::cout << "SET DEFAULT FORMAT IMAGE (" << format << ")" << std::endl;
		_defaultImageFormat = format;
	}

	const ColorFormat& ImageDrawer2DSDL::GetDefaultFormatImage()
	{
		return (_defaultImageFormat);
	}

	IImage* ImageDrawer2DSDL::CreateImage(const std::string& name, const unsigned int& w, const unsigned int& h, const ColorFormat& format)
	{
		ImageSDL* img = new ImageSDL;

		img->Init(w, h, format);
		_images[name] = img;
		return (img);
	}

	void ImageDrawer2DSDL::UseImage(const std::string& name)
	{
		if (_images.count(name))
		{
			_currImage = _images.at(name);
			_currImageName = name;
		}
	}

	IImage* ImageDrawer2DSDL::GetImage(const std::string& name)
	{
		if (_images.count(name))
			return ((IImage*)(_images.at(name)));
		return (NULL);
	}

	IImage* ImageDrawer2DSDL::RemoveImage(const std::string& name)
	{
		IImage* img;

		if (_images.count(name))
		{
			img = _images.at(name);
			if (img == _currImage)
				_currImage = NULL;
			_images.erase(name);
		}
		else
			img = NULL;
		return (img);
	}

	void ImageDrawer2DSDL::ScaleImage(const unsigned int& sW, const unsigned int& sH)
	{
		if (!_currImage)
			return;
		ImageSDL* tmp = new ImageSDL();

		tmp->Init(sW, sH, _currImage->GetFormat());
		if (tmp)
		{
			SDL_BlitScaled(_currImage->GetSurface(), NULL, tmp->GetSurface(), NULL);
			delete _currImage;
			_currImage = tmp;
			_images[_currImageName] = tmp;
		}

	}

	void ImageDrawer2DSDL::ClearImage()
	{
		if (!_currImage)
			return;
		_currImage->Clear();
	}

	void ImageDrawer2DSDL::CopyImage(const std::string& nameToDraw)
	{
		if (!_currImage || !_images.count(nameToDraw))
			return;
		ImageSDL* img = _images.at(nameToDraw);

		SDL_BlitScaled(img->GetSurface(), NULL, _currImage->GetSurface(), NULL);
	}

	void ImageDrawer2DSDL::DrawImage(const std::string& nameToDraw, const int& xPos, const int& yPos)
	{
		if (!_currImage || !_images.count(nameToDraw))
			return;
		ImageSDL* img = _images.at(nameToDraw);
		SDL_Rect rect;

		rect.x = xPos;
		rect.y = yPos;
		SDL_BlitSurface(img->GetSurface(), NULL, _currImage->GetSurface(), &rect);
	}

	void ImageDrawer2DSDL::DrawPoint(const Vec2i& pos, const Color& color)
	{
		DrawPoint(pos.x, pos.y, color);
	}

	void ImageDrawer2DSDL::DrawLine(const Vec2i& a, const Vec2i& b, const Color& color)
	{
		DrawLine(a.x, a.y, b.x, b.y, color);
	}

	/*void ImageDrawer2DSDL::DrawRect(const Vec2i& pos, const Rect& rect)
	{
		DrawRect(pos.x, pos.y, rect);
	}*/

	/*void ImageDrawer2DSDL::DrawRectBorder(const Vec2i& pos, const RectBorder& borderdef)
	{
		DrawRectBorder(pos.x, pos.y, borderdef);
	}

	void ImageDrawer2DSDL::DrawRect(const Vec2i& pos, const Rect& rect)
	{
		DrawRect(pos.x, pos.y, rect);
	}*/

	/*void ImageDrawer2DSDL::DrawCircle(const Vec2i& pos, const Circle& circle)
	{
	}*/

	void ImageDrawer2DSDL::DrawText(const Vec2i& pos, const Text& text)
	{
		std::cerr << "ImageDrawer2DSDL::DrawText() NOT IMPLEMENTED" << std::endl;
	}

	void ImageDrawer2DSDL::DrawPoint(const int& x, const int& y, const Color& color)
	{
		if (x < 0 || y < 0)
			return;
		if (x >= _currImage->GetWidth() || y >= _currImage->GetHeight())
			return;
		InternSetPixel(x, y, color.value());
	}

	void ImageDrawer2DSDL::DrawLine(const int& xB, const int& yB, const int& xE, const int& yE, const Color& color)
	{
		if (!_currImage)
			return;
		_iL.i = 1;

		_iL.xi = xB;
		_iL.yi = yB;

		_iL.deltaX = (xE - _iL.xi);
		_iL.deltaY = (yE - _iL.yi);

		if (_iL.deltaX < 0)
			_iL.incX = -1;
		else
			_iL.incX = 1;

		if (_iL.deltaY < 0)
			_iL.incY = -1;
		else
			_iL.incY = 1;

		_iL.deltaX = abs(_iL.deltaX);
		_iL.deltaY = abs(_iL.deltaY);

		if (_iL.deltaX > _iL.deltaY)
		{
			// Little Endian Optimization
			_iL.sum = (_iL.deltaX >> 1);
			while (_iL.i <= _iL.deltaX)
			{
				_iL.xi += _iL.incX;
				_iL.sum += _iL.deltaY;
				if (_iL.sum >= _iL.deltaX)
				{
					_iL.sum -= _iL.deltaX;
					_iL.yi += _iL.incY;
				}
				InternCheckBFSetPixel(_iL.xi, _iL.yi, color.value());
				_iL.i += 1;
			}
		}
		else
		{
			// Little Endian Optimization
			_iL.sum = (_iL.deltaY >> 1);
			while (_iL.i <= _iL.deltaY)
			{
				_iL.yi += _iL.incY;
				_iL.sum += _iL.deltaX;
				if (_iL.sum >= _iL.deltaY)
				{
					_iL.sum -= _iL.deltaY;
					_iL.xi += _iL.incX;
				}

				InternCheckBFSetPixel(_iL.xi, _iL.yi, color.value());
				_iL.i += 1;
			}
		}
	}

	void ImageDrawer2DSDL::DrawLine(const int& x, const int& y, const int& xE, const int& yE, const Color& colorB, const Color& colorE)
	{
		if (!_currImage)
			return;

		_iL.i = 1;

		_iL.xi = CorrectX(x);
		_iL.yi = CorrectY(y);

		_iL.deltaX = (CorrectX(xE) - _iL.xi);
		_iL.deltaY = (CorrectY(yE) - _iL.yi);

		if (_iL.deltaX < 0)
			_iL.incX = -1;
		else
			_iL.incX = 1;

		if (_iL.deltaY < 0)
			_iL.incY = -1;
		else
			_iL.incY = 1;

		_iL.deltaX = abs(_iL.deltaX);
		_iL.deltaY = abs(_iL.deltaY);

		_colorFA = ColorFDef(colorB.r(), colorB.g(), colorB.b(), colorB.a());
		_colorFB = ColorFDef(colorE.r(), colorE.g(), colorE.b(), colorE.a());

		_stepColor = 0;

		if (_iL.deltaX > _iL.deltaY)
		{
			_offsetColor = (1.0f / (float)_iL.deltaX);
			// Little Endian Optimization
			_iL.sum = (_iL.deltaX >> 1);
			while (_iL.i <= _iL.deltaX)
			{
				_iL.xi += _iL.incX;
				_iL.sum += _iL.deltaY;
				if (_iL.sum >= _iL.deltaX)
				{
					_iL.sum -= _iL.deltaX;
					_iL.yi += _iL.incY;
				}
				// Step Color Part
				_colorFC = _colorFA * (1.0f - _stepColor);
				_colorFC += _colorFB * _stepColor;

				_stepColor += _offsetColor; // END Step Color Part

				InternSetPixel(_iL.xi, _iL.yi, Color(_colorFC).value());
				_iL.i += 1;
			}
		}
		else
		{
			_offsetColor = (1.0f / (float)_iL.deltaY);
			// Little Endian Optimization
			_iL.sum = (_iL.deltaY >> 1);
			while (_iL.i <= _iL.deltaY)
			{
				_iL.yi += _iL.incY;
				_iL.sum += _iL.deltaX;
				if (_iL.sum >= _iL.deltaY)
				{
					_iL.sum -= _iL.deltaY;
					_iL.xi += _iL.incX;
				}

				// Step Color Part
				_colorFC = _colorFA * (1.0f - _stepColor);
				_colorFC += _colorFB * _stepColor;

				_stepColor += _offsetColor; // END Step Color Part

				InternSetPixel(_iL.xi, _iL.yi, Color(_colorFC).value());

				_iL.i += 1;
			}
		}
	}

	void ImageDrawer2DSDL::DrawRectBorder(const int& x, const int& y, const RectBorder& border)
	{
		//ADrawer2DExtended::DrawRectBorder(Vec2i(x, y), border);
	}

	void ImageDrawer2DSDL::DrawRect(const int& x, const int& y, const Rect& rect)
	{
		//ADrawer2DExtended::DrawRect(Vec2i(x, y), rect);
	}

	void ImageDrawer2DSDL::DrawRectFill(const int& x, const int& y, const unsigned int& width, const unsigned int& height, float fillscale, const Color& color)
	{
		if (!_currImage)
			return;

		// Draw Width
		int isx, iex, ex;
		int isy, iey, ey, eey;

		isy = y;
		isx = x;
		iex = isx + (int)(width - 1);
		iey = isy + (int)(height - 1);

		ex = (x + (int)((float)(width - 1) * fillscale * 0.5f));
		ey = (y + (int)((float)(height - 1) * fillscale * 0.5f));
		eey = isy + (int)(height - 2);

		while (isx < ex)
		{
			while (isy < iey)
			{
				InternSetPixel(isx, isy, color.value());
				InternSetPixel(iex, isy, color.value());
				isy += 1;
			}
			isx += 1;
			iex -= 1;
			isy = y;
		}

		while (isx <= iex)
		{
			iey = eey;
			while (isy < ey)
			{
				InternSetPixel(isx, isy, color.value());
				InternSetPixel(iex, isy, color.value());

				InternSetPixel(isx, iey, color.value());
				InternSetPixel(iex, iey, color.value());
				isy += 1;
				iey -= 1;
			}
			isx += 1;
			iex -= 1;
			isy = y;
		}


		// Draw Height

		// Then Last Part Of Height


		//bool drawed = false;

		//DrawRectSnakeAttrib drawAttrb;
		//drawAttrb.Init(width, height, x, y, fillscale);


		//unsigned int totalPixel = (width * height);
		//unsigned int pixelDrawed = 0;

		//while (!drawed)
		//{
		//	// Draw
		//	InternSetPixel(drawAttrb.x, drawAttrb.y, color.value());
		//	if (drawAttrb.filledRect >= drawAttrb.rectToFill)
		//		drawed = true;
		//	drawAttrb.IncDrawRectangle(drawAttrb.dir, drawAttrb.filledRect, drawAttrb.x, drawAttrb.y, drawAttrb.minX, drawAttrb.minY, drawAttrb.maxX, drawAttrb.maxY);
		//	if (pixelDrawed == totalPixel)
		//		drawed = true;
		//	// ---
		//	pixelDrawed += 1;
		//}
	}

	void ImageDrawer2DSDL::DrawCircleFill(const int& x, const int& y, const unsigned int& diameter, const Color& color)
	{
		if (!_currImage)
			return;
		if (diameter <= 1)
		{
			DrawPoint(x, y, color);
			return;
		}


		/*uint32_t a;
		std::bitset<32> bb;

		a = color.value();
		bb = std::bitset<32>(a);
		std::cout << "color circle Fill (" << bb << ")" << a << ") format (" << color.GetFormat() << ")\n";*/

		_iC.radius = (int)((float)diameter * 0.5f);
		_iC.xi = 0;
		_iC.yi = _iC.radius;
		_iC.d = (1 - _iC.radius);
		_iC.dx = 0;
		_iC.dy = (-2 * _iC.radius);

		DrawLine(x, y - _iC.radius, x, y + _iC.radius, color);
		while (_iC.xi <= _iC.yi)
		{
			if (_iC.d >= 0)
			{
				_iC.yi--;
				_iC.dy += 2;
				_iC.d += _iC.dy;
			}
			else
			{
				_iC.xi++;
				_iC.dx += 2;
				_iC.d += (_iC.dx + 1);
			}
			DrawLine(x + _iC.xi, y - _iC.yi, x + _iC.xi, y + _iC.yi, color);
			DrawLine(x - _iC.xi, y - _iC.yi, x - _iC.xi, y + _iC.yi, color);
			DrawLine(x + _iC.yi, y - _iC.xi, x + _iC.yi, y + _iC.xi, color);
			DrawLine(x - _iC.yi, y - _iC.xi, x - _iC.yi, y + _iC.xi, color);
		}
	}

	void ImageDrawer2DSDL::DrawCircleFill(const int& x, const int& y, const unsigned int& diameter, const Color& colora, const Color& colorb)
	{
		if (!_currImage)
			return;

		if (diameter <= 1)
		{
			DrawPoint(x, y, colora);
			return;
		}


		int r = (int)((float)diameter * 0.5f);
		Color ncolor;
		int nx, ny;
		int distx, disty;
		int dist;
		float ratio;

		for (int yi = -r; yi <= r; yi++)
			for (int xi = -r; xi <= r; xi++)
				if (xi * xi + yi * yi < r * r)
				{
					nx = (xi + x);
					ny = (yi + y);
					distx = nx;
					distx -= x;
					distx = ValueTools::Abs(distx);
					disty = ny;
					disty -= y;
					disty = ValueTools::Abs(disty);
					dist = (ValueTools::Max(distx, disty) + 1);
					ratio = ((float)dist / (float)(r + 1));

					ncolor = Color(((colora.r() * ratio) + (colorb.r() * (1.0f - ratio))), \
						((colora.g() * ratio) + (colorb.g() * (1.0f - ratio))), \
						((colora.b() * ratio) + (colorb.b() * (1.0f - ratio))));

					InternSetPixel(nx, ny, ncolor.value());
				}
	}

	void ImageDrawer2DSDL::DrawCircle(const int& x, const int& y, const unsigned int& diameter, const Color& color)
	{
		if (!_currImage)
			return;

		if (diameter <= 1)
		{
			DrawPoint(x, y, color);
			return;
		}
		_iC.radius = (int)((float)diameter * 0.5f);
		_iC.xi = 0;
		_iC.yi = _iC.radius;
		_iC.d = (1 - _iC.radius);
		_iC.dx = 0;
		_iC.dy = (-2 * _iC.radius);

		//DrawLine(x, y - _iC.radius, x, y + _iC.radius, color);

		DrawPoint(x, y - _iC.radius, color);
		DrawPoint(x, y + _iC.radius, color);

		//uint32_t a;
		//std::bitset<32> bb;

		//a = color.value();
		//bb = std::bitset<32>(a);
		//std::cout << "color circle (" << bb << ")" << a << ") format (" << color.GetFormat() << ")\n";


		while (_iC.xi <= _iC.yi)
		{
			DrawPoint(x + _iC.xi, y - _iC.yi, color);
			DrawPoint(x + _iC.xi, y + _iC.yi, color);

			DrawPoint(x - _iC.xi, y - _iC.yi, color);
			DrawPoint(x - _iC.xi, y + _iC.yi, color);

			DrawPoint(x + _iC.yi, y - _iC.xi, color);
			DrawPoint(x + _iC.yi, y + _iC.xi, color);

			DrawPoint(x - _iC.yi, y - _iC.xi, color);
			DrawPoint(x - _iC.yi, y + _iC.xi, color);

			if (_iC.d >= 0)
			{
				_iC.yi--;
				_iC.dy += 2;
				_iC.d += _iC.dy;
			}
			else
			{
				_iC.xi++;
				_iC.dx += 2;
				_iC.d += (_iC.dx + 1);
			}
		}
	}

	void ImageDrawer2DSDL::DrawCircle(const int& x, const int& y, const Circle& circle)
	{
		// ADrawer2DExtended::DrawCircle(Vec2i(x, y), circle);
	}

	//void ImageDrawer2DSDL::DrawPolygon(const unsigned int & xPos, const unsigned int & yPos, const std::vector<Vec2>& points, const Color & color)
	//{
	//	if (!_currImage)
	//		return;
	//	std::vector<PolyEdge*> edges;
	//
	//	//CreateEdges(points, edges);
	//	//DrawEdges(edges, xPos, yPos, color);
	//}

	int ImageDrawer2DSDL::LoadText(const std::string& text, const unsigned int& sFont, const std::string& font, const Color& color)
	{
		if (!_currImage)
			return (-1);
		std::stringstream	ss;
		TTF_Font* tfont;
		ss << font;
		ss << sFont;
		if (!_fonts.count(ss.str()))
		{
			if (!(tfont = TTF_OpenFont(font.c_str(), sFont)))
			{
				std::cerr << "IImageDrawer2D: Cannot Opent Font: (" << font << ") of size: (" << sFont << ")\n";
				return (-1);
			}
			_fonts[ss.str()].font = tfont;
		}
		else
			tfont = _fonts[ss.str()].font;
		SDL_Color		tmp_color = { color.r(), color.g(), color.b() };
		if (_textActive.text)
		{
			SDL_FreeSurface(_textActive.text);
			_textActive.text = NULL;
		}
		if (!(_textActive.text = TTF_RenderText_Blended(tfont, text.c_str(), tmp_color)))
		{
			std::cerr << "IImageDrawer2D: Cannot Render Font: (" << font << ") (" << SDL_GetError() << ")\n";
			return (-1);
		}
		return (0);
	}

	void ImageDrawer2DSDL::GetSizeText(unsigned int& w, unsigned int& h)
	{
		if (!_textActive.text)
		{
			w = 0;
			h = 0;
		}
		else
		{
			w = _textActive.text->w;
			h = _textActive.text->h;
		}
	}

	void ImageDrawer2DSDL::DrawText(const int& xPos, const int& yPos)
	{
		if (!_currImage || !_textActive.text)
			return;
		SDL_Rect rect;

		rect.x = xPos;
		rect.y = yPos;
		SDL_SetColorKey(_textActive.text, SDL_TRUE, SDL_MapRGB(_textActive.text->format, GXEZ_DRAWER2DSDL_TRSPR, GXEZ_DRAWER2DSDL_TRSPG, GXEZ_DRAWER2DSDL_TRSPB));
		SDL_BlitSurface(_textActive.text, NULL, _currImage->GetSurface(), &rect);
		SDL_SetColorKey(_textActive.text, SDL_FALSE, SDL_MapRGB(_textActive.text->format, GXEZ_DRAWER2DSDL_TRSPR, GXEZ_DRAWER2DSDL_TRSPG, GXEZ_DRAWER2DSDL_TRSPB));
	}

	bool CmpYMinPolyEdge(const ImageDrawer2DSDL::PolyEdge* pA, const ImageDrawer2DSDL::PolyEdge* pB)
	{
		if (pA->min_y < pB->min_y)
			return (true);
		return (false);
	}

	///////////////////////
	// TEST DRAW POLYGON //
	///////////////////////
	/*

	bool CmpYMaxPolyEdge(const ImageDrawer2DSDL::PolyEdge* pA, const ImageDrawer2DSDL::PolyEdge* pB)
	{
	if (pA->max_y < pB->max_y)
	return (true);
	return (false);
	}

	bool CmpXPolyEdge(const ImageDrawer2DSDL::PolyEdge* pA, const ImageDrawer2DSDL::PolyEdge* pB)
	{
	if (pA->x_to_min_y < pB->x_to_min_y)
	return (true);
	return (false);
	}

	void ImageDrawer2DSDL::DrawPolygon(const unsigned int & xPos, const unsigned int & yPos, const std::vector<Vec2>& points, const Color & color)
	{
	if (!_currImage)
	return;
	std::vector<PolyEdge>	edges;
	std::vector<std::vector<PolyEdge*>>  sorted_edges;
	unsigned int					scanline;
	unsigned int					x;
	std::vector<PolyEdge*>	active_edges;
	bool					parity = true;
	PolyEdge*				current;



	CreateEdges(points, edges);
	CreateSortedEdges(edges, sorted_edges);

	scanline = edges.front().min_y;

	for (unsigned int inc = 0; inc < sorted_edges.size(); inc++)
	{
	while (!sorted_edges.at(inc).empty())
	{
	parity = true;
	for (unsigned int j = 0; j < (sorted_edges.at(inc).size() - 1); j++)
	{
	if (parity)
	for (x = sorted_edges.at(inc).at(j)->x_to_min_y; x < sorted_edges.at(inc).at(j + 1)->x_to_min_y; x++)
	InternSetPixel(x + xPos, scanline + yPos, color.value());
	parity = !parity;
	}
	scanline++;
	while (!sorted_edges.at(inc).empty() && sorted_edges.at(inc).front()->max_y <= scanline)
	sorted_edges.at(inc).erase(sorted_edges.at(inc).begin());
	UpdateXEdges(sorted_edges.at(inc));
	Sorting::QuickSort(sorted_edges.back(), 0, (sorted_edges.back().size() - 1), &CmpXPolyEdge);
	}



	}
	}


	void ImageDrawer2DSDL::CreateEdges(const std::vector<Vec2>& points, std::vector<PolyEdge>& edges)
	{
	PolyEdge				edge;
	float					tmp;
	unsigned int					inc = 0;
	int						xx;

	for (; inc < (points.size() - 1); inc++)
	{
	if (points.at(inc).y >= points.at(inc + 1).y)
	{
	edge.min_y = points.at(inc + 1).y;
	edge.max_y = points.at(inc).y;
	edge.x_to_min_y = points.at(inc + 1).x;
	xx = points.at(inc).x;
	}
	else
	{
	edge.min_y = points.at(inc).y;
	edge.max_y = points.at(inc + 1).y;
	edge.x_to_min_y = points.at(inc).x;
	xx = points.at(inc + 1).x;
	}
	if (!isinf(edge.slope = ((float)(xx - edge.x_to_min_y) / (float)(edge.max_y - edge.min_y))))
	edges.push_back(edge);
	}
	// Last To First (Duplicate to avoid if)
	if (points.at(inc).y >= points.at(0).y)
	{
	edge.min_y = points.at(0).y;
	edge.max_y = points.at(inc).y;
	edge.x_to_min_y = points.at(0).x;
	}
	else
	{
	edge.min_y = points.at(inc).y;
	edge.max_y = points.at(0).y;
	edge.x_to_min_y = points.at(inc).x;
	}
	if (!isinf(edge.slope = ((float)(xx - edge.x_to_min_y) / (float)(edge.max_y - edge.min_y))))
	edges.push_back(edge);
	}

	void ImageDrawer2DSDL::CreateSortedEdges(std::vector<PolyEdge>& edges, std::vector<std::vector<PolyEdge*>>& sorted_edges)
	{
	std::vector<PolyEdge*>	empty;
	int						yInc;

	Sorting::QuickSort(edges, 0, (edges.size() - 1), &CmpYMinPolyEdge);
	yInc = edges.front().min_y;
	sorted_edges.push_back(empty);
	for (unsigned int inc = 0; inc < edges.size(); inc++)
	{
	if (yInc != edges.at(inc).min_y)
	{
	if (sorted_edges.back().size())
	{
	Sorting::QuickSort(sorted_edges.back(), 0, (sorted_edges.back().size() - 1), &CmpYMaxPolyEdge);
	Sorting::QuickSort(sorted_edges.back(), 0, (sorted_edges.back().size() -1), &CmpXPolyEdge);
	}
	yInc = edges.at(inc).min_y;
	sorted_edges.push_back(empty);
	}
	sorted_edges.back().push_back(&edges.at(inc));
	}
	if (sorted_edges.back().size())
	{
	Sorting::QuickSort(sorted_edges.back(), 0, (sorted_edges.back().size() - 1), &CmpYMaxPolyEdge);
	Sorting::QuickSort(sorted_edges.back(), 0, (sorted_edges.back().size() - 1), &CmpXPolyEdge);
	}
	}

	void ImageDrawer2DSDL::UpdateXEdges(std::vector<PolyEdge*> edges)
	{
	for (unsigned int inc = 0; inc < edges.size(); inc++)
	edges.at(inc)->x_to_min_y += edges.at(inc)->slope;
	}


	*/

	void ImageDrawer2DSDL::AddSprite(const std::string& id, Sprite* sprite)
	{
		if (_sprites.count(id))
			return;
		_sprites.emplace(id, sprite);
	}

	void ImageDrawer2DSDL::LoadSprite(const std::string& id, const std::string& path)
	{
		if (_sprites.count(id) || !_currImage)
			return;
		SDL_Surface* tmp;

		if ((tmp = IMG_Load(path.c_str())) != NULL)
		{
			ImageSDL* img = new ImageSDL();
			img->InitFromSurface(tmp);


			Sprite* sprite;
			if (_spriteColorTransparency)
				sprite = new Sprite(img, _spriteColorTransparency->GetDef());
			else
				sprite = new Sprite(img);

			if (sprite)
				_sprites.emplace(id, sprite);
		}
	}

	void ImageDrawer2DSDL::SetSprite(const std::string& id)
	{
		if (!_sprites.count(id))
			return;
		_spriteActive = _sprites.at(id);
	}

	void ImageDrawer2DSDL::SetSpriteColorTransparency(const Color& color)
	{
		_spriteColorTransparency = new Color(color);
	}

	void ImageDrawer2DSDL::DrawSprite(const int& x, const int& y)
	{
		if (!_spriteActive || !_currImage)
			return;

		int mx, my, mxe;
		/*
		for (size_t i = 0; i < _spriteActive->GetPixels().size(); i += 1)
		{
			const Sprite::Pixel& pixel = _spriteActive->GetPixels().at(i);
			mx = x + pixel.x;
			my = y + pixel.y;
			InternCheckBFSetPixel(mx, my, pixel.color.value());
		}*/

		const std::vector<Sprite::Line>& lines = _spriteActive->GetLines();

		for (size_t i = 0; i < lines.size(); i += 1)
		{
			const Sprite::Line& line = lines.at(i);
			mx = x + (int)line.x;
			my = y + (int)line.y;
			mxe = x + (int)line.xE;
			InternSetLine(mx, my, mxe, line);
		}
	}

	void ImageDrawer2DSDL::DrawSprite(const int& x, const int& y, Sprite* sprite)
	{
		if (!sprite || !_currImage)
			return;

		int mx, my, mxe;

		const std::vector<Sprite::Line>& lines = sprite->GetLines();

		for (size_t i = 0; i < lines.size(); i += 1)
		{
			const Sprite::Line& line = lines.at(i);
			mx = x + (int)line.x;
			my = y + (int)line.y;
			mxe = x + (int)line.xE;
			InternSetLine(mx, my, mxe, line);
		}
	}

	void ImageDrawer2DSDL::InternDrawPoint(const Vec2i& pos, const Color& color)
	{
		InternCheckBFSetPixel(pos.x, pos.y, color.value());
	}

	void ImageDrawer2DSDL::InternDrawLine(const Vec2i& a, const Vec2i& b, const Color& color)
	{
		DrawLine(a.x, a.y, b.x, b.y, color, color);
	}

	void ImageDrawer2DSDL::InternDrawRectBorder(const Vec2i& pos, const IDrawer2D::RectBorder& borderdef)
	{
	}

	void ImageDrawer2DSDL::InternDrawRect(const Vec2i& pos, const IDrawer2D::Rect& rect)
	{
	}

	void ImageDrawer2DSDL::AddImage(const std::string& name, ImageSDL* img)
	{
		_images[name] = img;
		UseImage(name);
	}


	inline void ImageDrawer2DSDL::CorrectXY(int& x, int& y)
	{
		x = CorrectX(x);
		y = CorrectY(y);
	}

	inline int ImageDrawer2DSDL::CorrectX(const int& x)
	{
		if (x < 0)
			return (0);
		else if (x >= (int)_currImage->GetWidth())
			return ((int)_currImage->GetWidth() - 1);
		return (x);
	}

	inline int ImageDrawer2DSDL::CorrectY(const int& y)
	{
		if (y < 0)
			return (0);
		else if (y >= (int)_currImage->GetHeight())
			return ((int)_currImage->GetHeight() - 1);
		return (y);
	}

	inline void ImageDrawer2DSDL::InternSetPixel(const int& x, const int& y, const unsigned int& color)
	{
		//std::cout << "Curr Image Format : (" << ColorFormatToString(_currImage->GetFormat()) << ")" << std::endl;
		_currImage->SetPixel((unsigned int)x, (unsigned int)y, color);
	}

	inline void ImageDrawer2DSDL::InternCheckBFSetPixel(const int& x, const int& y, const unsigned int& color)
	{
		if (x < 0 || y < 0)
			return;
		if (x >= (int)_currImage->GetWidth() || y >= (int)_currImage->GetHeight())
			return;
		_currImage->SetPixel(x, y, color);
	}

	inline void ImageDrawer2DSDL::InternSetLine(const int& x, const int& y, const int& xe, const Sprite::Line& line)
	{
		if (y < 0)
			return;
		if (x >= (int)_currImage->GetWidth() || y >= (int)_currImage->GetHeight())
			return;
		if (x < 0 && xe > 0) {
			_iIL.xmem = ((int)line.lineSize - xe) * _currImage->GetBytesPerPixel();
			_currImage->SetLine(0, y, &line.line[_iIL.xmem], line.lineBytesSize - _iIL.xmem);
		}
		else if (xe >= (int)_currImage->GetWidth()) {
			_iIL.xmem = (xe - (int)_currImage->GetWidth() + 1) * _currImage->GetBytesPerPixel();
			_currImage->SetLine(x, y, line.line, line.lineBytesSize - _iIL.xmem);
		}
		else {
			_currImage->SetLine(x, y, line.line, line.lineBytesSize);
		}
	}

	void ImageDrawer2DSDL::Init()
	{
	}

	void ImageDrawer2DSDL::DrawRectSnakeAttrib::IncDrawRectangle(char& dir, unsigned int& filledRect, unsigned int& x, unsigned int& y, unsigned int& minX, unsigned int& minY, unsigned int& maxX, unsigned int& maxY)
	{
		if (dir == 0) // DOWN
		{
			y++;
			if (y == maxY)
			{
				maxY -= 1;
				dir = 1;
			}
		}
		else if (dir == 1) // RIGHT
		{
			x++;
			if (x == maxX)
			{
				maxX -= 1;
				dir = 2;
			}
		}
		else if (dir == 2) // UP
		{
			y--;
			if (y == minY)
			{
				minY += 1;
				dir = 3;
			}
		}
		else if (dir == 3) // LEFT
		{
			x--;
			if (x == (minX + 1))
			{
				minX += 1;
				dir = 0;
				filledRect += 1;
			}
		}
	}

	void ImageDrawer2DSDL::DrawRectSnakeAttrib::Init(unsigned int width, unsigned int height, unsigned int xPos, unsigned int yPos, float fillScale)
	{
		minX = xPos;
		minY = yPos;
		x = xPos;
		y = yPos;
		maxX = (xPos + width) - 1;
		maxY = (yPos + height) - 1;
		dir = 0;
		filledRect = 0;
		if (height > width)
			rectToFill = ((height * fillScale) + 1.0f);
		else
			rectToFill = ((width * fillScale) + 1.0f);
	}

	void ImageDrawer2DSDL::InfoCircleBressenham::Init(const float& diameter)
	{
		r = int(diameter / 2.0f);
		x = 0;
		y = r;
		d = 3 - 2 * r;
	}

}

#endif // GXEZ_SDL