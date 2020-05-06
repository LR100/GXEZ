// GXEZ
#include "GXEZ/Graphic/SDL/Drawer2DSDL.h"
#include "GXEZ/SDL/GXEZContextSDL.h"

// STD
#include <bitset>
#include <iostream> // TMP

#if GXEZ_SDL

namespace GXEZ
{

	Drawer2DSDL* Drawer2DSDL::sptr = NULL;    //initialize Singleton pointer to NULL

	Drawer2DSDL::Drawer2DSDL(GXEZContextSDL* context, const ColorFormat& format) : _dCBr(this), _dRB(this), _dR(this)
	{
		_context = context;
		_defaultImage = NULL;
		std::cout << "DRAWER 2D ADDR (" << this << ")" << std::endl;
		if (sptr != this)
		{
			std::cout << std::endl << "-Drawer2DSDL()-" << std::endl;
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
			std::cout << "Drawer2DSDL : Default Image Format (" << ColorFormatToString(_defaultImageFormat) << ")" << std::endl;
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

	Drawer2DSDL::~Drawer2DSDL()
	{
	}

	void* Drawer2DSDL::operator new(size_t size)
	{
		if (sptr != NULL)    //if already one object is created return reference to same object
		{
			std::cout << "1 + Nth Creation Drawer2DSDL" << std::endl;
			std::cout << "Return Drawer 2D Img Format(" << ColorFormatToString(sptr->_defaultImageFormat) << ")" << std::endl;
			return (sptr);
		}
		else
		{
			std::cout << "FIRST Drawer2DSDL" << std::endl;

			return (calloc(1, size));    //else allocate memory for one (first) object
		}
	}


	void Drawer2DSDL::Quit()
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

	void Drawer2DSDL::SetDefaultFont(const std::string& name)
	{
	}

	void Drawer2DSDL::SetDefaultFontSize(const unsigned int& size)
	{
	}

	bool Drawer2DSDL::SetCurrentImage(IImage* image)
	{
		if (image) {
			if (image->GetFormat() == _defaultImageFormat)
			{
				_currImage = (ImageSDL*)image;
				return (true);
			}
			else
			{
				std::cerr << "Drawer2DSDL::SetCurrentImage() : Bad Image Format D(" << _defaultImageFormat << ") != I(" << image->GetFormat() << ")" << std::endl;
			}
		}
		else {
			_currImage = NULL;
			return (true);
		}
		return (false);
	}

	IImage* Drawer2DSDL::GetCurrentImage() const
	{
		return (_currImage);
	}

	void Drawer2DSDL::SetDefaultImage(const std::string& name)
	{
		std::cerr << "Drawer2DSDL::SetDefaultImage() " << std::endl;
		if (_images.count(name))
		{
			_defaultImageName = name;
			_defaultImage = _images.at(name);
		}
	}

	const std::string& Drawer2DSDL::GetDefaultImageName()
	{
		return (_defaultImageName);
	}

	void Drawer2DSDL::UseDefaultImage()
	{
		_currImage = _defaultImage;
		_currImageName = _defaultImageName;
	}

	void Drawer2DSDL::SetDefaultFormatImage(const ColorFormat& format)
	{
		std::cout << "SET DEFAULT FORMAT IMAGE (" << format << ")" << std::endl;
		_defaultImageFormat = format;
	}

	const ColorFormat& Drawer2DSDL::GetDefaultFormatImage()
	{
		return (_defaultImageFormat);
	}

	IImage* Drawer2DSDL::CreateImage(const std::string& name, const unsigned int& w, const unsigned int& h, const ColorFormat& format)
	{
		ImageSDL* img = new ImageSDL;

		img->Init(w, h, format);
		_images[name] = img;
		return (img);
	}

	void Drawer2DSDL::UseImage(const std::string& name)
	{
		if (_images.count(name))
		{
			_currImage = _images.at(name);
			_currImageName = name;
		}
	}

	IImage* Drawer2DSDL::GetImage(const std::string& name)
	{
		if (_images.count(name))
			return ((IImage*)(_images.at(name)));
		return (NULL);
	}

	IImage* Drawer2DSDL::RemoveImage(const std::string& name)
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

	void Drawer2DSDL::ScaleImage(const unsigned int& sW, const unsigned int& sH)
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

	void Drawer2DSDL::ClearImage()
	{
		if (!_currImage)
			return;
		_currImage->Clear();
	}

	void Drawer2DSDL::CopyImage(const std::string& nameToDraw)
	{
		if (!_currImage || !_images.count(nameToDraw))
			return;
		ImageSDL* img = _images.at(nameToDraw);

		SDL_BlitScaled(img->GetSurface(), NULL, _currImage->GetSurface(), NULL);
	}

	void Drawer2DSDL::DrawImage(const std::string& nameToDraw, const int& xPos, const int& yPos)
	{
		if (!_currImage || !_images.count(nameToDraw))
			return;
		ImageSDL* img = _images.at(nameToDraw);
		SDL_Rect rect;

		rect.x = xPos;
		rect.y = yPos;
		SDL_BlitSurface(img->GetSurface(), NULL, _currImage->GetSurface(), &rect);
	}

	void Drawer2DSDL::DrawPoint(const int& x, const int& y, const Color& color)
	{
		if (x < 0 || y < 0)
			return;
		if (x >= _currImage->GetWidth() || y >= _currImage->GetHeight())
			return;
		InternSetPixel(x, y, color.value());
	}

	void Drawer2DSDL::DrawLine(const int& xB, const int& yB, const int& xE, const int& yE, const Color& color)
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

	void Drawer2DSDL::DrawLine(const int& x, const int& y, const int& xE, const int& yE, const Color& colorB, const Color& colorE)
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


	void Drawer2DSDL::DrawRectBorder(const int& x, const int& y, const RectBorder& border)
	{
		// std::cout << "Draw Rect Border at (" << x << ") (" << y << ") Size (" << border.height << ") (" << border.width << ")" << std::endl;
		if (!_currImage)
			return;
		_dRB.Draw(x, y, border);
	}

	/////////////////////////////////
	///////////////
	///
	///
	// 

	void Drawer2DSDL::DrawRect(const int& x, const int& y, const Rect& rect)
	{
		// std::cout << "Draw Rect  at (" << x << ") (" << y << ") Size (" << rect.height << ") (" << rect.width << ")" << std::endl;
		if (!_currImage)
			return;
		_dR.Draw(x, y, rect);
	}


	void Drawer2DSDL::DrawRectFill(const int& x, const int& y, const unsigned int& width, const unsigned int& height, float fillscale, const Color& color)
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

	void Drawer2DSDL::DrawCircleFill(const int& x, const int& y, const unsigned int& diameter, const Color& color)
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

	void Drawer2DSDL::DrawCircleFill(const int& x, const int& y, const unsigned int& diameter, const Color& colora, const Color& colorb)
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

	void Drawer2DSDL::DrawCircle(const int& x, const int& y, const unsigned int& diameter, const Color& color)
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

	void Drawer2DSDL::DrawCircle(const int& x, const int& y, const Circle& circle)
	{
		if (!_currImage)
			return;
		_dCBr.Draw(x, y, circle);
	}

	//void Drawer2DSDL::DrawPolygon(const unsigned int & xPos, const unsigned int & yPos, const std::vector<Vec2>& points, const Color & color)
	//{
	//	if (!_currImage)
	//		return;
	//	std::vector<PolyEdge*> edges;
	//
	//	//CreateEdges(points, edges);
	//	//DrawEdges(edges, xPos, yPos, color);
	//}

	int Drawer2DSDL::LoadText(const std::string& text, const unsigned int& sFont, const std::string& font, const Color& color)
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
				std::cerr << "IDrawer2D: Cannot Opent Font: (" << font << ") of size: (" << sFont << ")\n";
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
			std::cerr << "IDrawer2D: Cannot Render Font: (" << font << ") (" << SDL_GetError() << ")\n";
			return (-1);
		}
		return (0);
	}

	void Drawer2DSDL::GetSizeText(unsigned int& w, unsigned int& h)
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

	void Drawer2DSDL::DrawText(const int& xPos, const int& yPos)
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

	bool CmpYMinPolyEdge(const Drawer2DSDL::PolyEdge* pA, const Drawer2DSDL::PolyEdge* pB)
	{
		if (pA->min_y < pB->min_y)
			return (true);
		return (false);
	}

	///////////////////////
	// TEST DRAW POLYGON //
	///////////////////////
	/*

	bool CmpYMaxPolyEdge(const Drawer2DSDL::PolyEdge* pA, const Drawer2DSDL::PolyEdge* pB)
	{
	if (pA->max_y < pB->max_y)
	return (true);
	return (false);
	}

	bool CmpXPolyEdge(const Drawer2DSDL::PolyEdge* pA, const Drawer2DSDL::PolyEdge* pB)
	{
	if (pA->x_to_min_y < pB->x_to_min_y)
	return (true);
	return (false);
	}

	void Drawer2DSDL::DrawPolygon(const unsigned int & xPos, const unsigned int & yPos, const std::vector<Vec2>& points, const Color & color)
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


	void Drawer2DSDL::CreateEdges(const std::vector<Vec2>& points, std::vector<PolyEdge>& edges)
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

	void Drawer2DSDL::CreateSortedEdges(std::vector<PolyEdge>& edges, std::vector<std::vector<PolyEdge*>>& sorted_edges)
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

	void Drawer2DSDL::UpdateXEdges(std::vector<PolyEdge*> edges)
	{
	for (unsigned int inc = 0; inc < edges.size(); inc++)
	edges.at(inc)->x_to_min_y += edges.at(inc)->slope;
	}


	*/

	void Drawer2DSDL::AddSprite(const std::string& id, Sprite* sprite)
	{
		if (_sprites.count(id))
			return;
		_sprites.emplace(id, sprite);
	}

	void Drawer2DSDL::LoadSprite(const std::string& id, const std::string& path)
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

	void Drawer2DSDL::SetSprite(const std::string& id)
	{
		if (!_sprites.count(id))
			return;
		_spriteActive = _sprites.at(id);
	}

	void Drawer2DSDL::SetSpriteColorTransparency(const Color& color)
	{
		_spriteColorTransparency = new Color(color);
	}

	void Drawer2DSDL::DrawSprite(const int& x, const int& y)
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

	void Drawer2DSDL::DrawSprite(const int& x, const int& y, Sprite* sprite)
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

	void Drawer2DSDL::AddImage(const std::string& name, ImageSDL* img)
	{
		_images[name] = img;
		UseImage(name);
	}


	inline void Drawer2DSDL::CorrectXY(int& x, int& y)
	{
		x = CorrectX(x);
		y = CorrectY(y);
	}

	inline int Drawer2DSDL::CorrectX(const int& x)
	{
		if (x < 0)
			return (0);
		else if (x >= (int)_currImage->GetWidth())
			return ((int)_currImage->GetWidth() - 1);
		return (x);
	}

	inline int Drawer2DSDL::CorrectY(const int& y)
	{
		if (y < 0)
			return (0);
		else if (y >= (int)_currImage->GetHeight())
			return ((int)_currImage->GetHeight() - 1);
		return (y);
	}

	inline void Drawer2DSDL::InternSetPixel(const int& x, const int& y, const unsigned int& color)
	{
		//std::cout << "Curr Image Format : (" << ColorFormatToString(_currImage->GetFormat()) << ")" << std::endl;
		_currImage->SetPixel((unsigned int)x, (unsigned int)y, color);
	}

	inline void Drawer2DSDL::InternCheckBFSetPixel(const int& x, const int& y, const unsigned int& color)
	{
		if (x < 0 || y < 0)
			return;
		if (x >= (int)_currImage->GetWidth() || y >= (int)_currImage->GetHeight())
			return;
		_currImage->SetPixel(x, y, color);
	}

	inline void Drawer2DSDL::InternSetLine(const int& x, const int& y, const int& xe, const Sprite::Line& line)
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

	void Drawer2DSDL::Init()
	{
	}

	void Drawer2DSDL::DrawRectSnakeAttrib::IncDrawRectangle(char& dir, unsigned int& filledRect, unsigned int& x, unsigned int& y, unsigned int& minX, unsigned int& minY, unsigned int& maxX, unsigned int& maxY)
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

	void Drawer2DSDL::DrawRectSnakeAttrib::Init(unsigned int width, unsigned int height, unsigned int xPos, unsigned int yPos, float fillScale)
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

	//////////////////////////////
	/////                  ///////
	/////      CIRCLES     ///////
	/////                  ///////
	//////////////////////////////

	Drawer2DSDL::DrawerCircleBressenham::DrawerCircleBressenham(Drawer2DSDL* drawer)
	{
		_drawFct = NULL;
		_drawer = drawer;
		// Vec2i are init on 0
		r = 1;
		x = 0;
		y = r;
		d = 0;
	}

	void Drawer2DSDL::DrawerCircleBressenham::Draw(const int& _x, const int& _y, const Circle& circle)
	{
		SetPartsPosition(Vec2i(_x, _y), Circle::Part::FULL);
		Draw(circle);
	}

	void Drawer2DSDL::DrawerCircleBressenham::Draw(const Circle& circle)
	{
		_circle = circle;
		for (t = 0; t < _circle.thickness; t += 1)
		{
			r = int((float)_circle.diameter * 0.5f);
			x = 0;
			y = r;
			d = 3 - 2 * r;
			if (circle.part == IDrawer2D::Circle::Part::FULL) {
				_drawFct = &Drawer2DSDL::DrawerCircleBressenham::DrawFull;
			}
			else if (circle.part == IDrawer2D::Circle::Part::TOP) {
				_drawFct = &Drawer2DSDL::DrawerCircleBressenham::DrawTop;
			}
			else if (circle.part == IDrawer2D::Circle::Part::TOP_LEFT) {
				_drawFct = &Drawer2DSDL::DrawerCircleBressenham::DrawTopLeft;
			}
			else if (circle.part == IDrawer2D::Circle::Part::TOP_RIGHT) {
				_drawFct = &Drawer2DSDL::DrawerCircleBressenham::DrawTopRight;
			}
			else if (circle.part == IDrawer2D::Circle::Part::BOTTOM) {
				_drawFct = &Drawer2DSDL::DrawerCircleBressenham::DrawBottom;
			}
			else if (circle.part == IDrawer2D::Circle::Part::BOTTOM_LEFT) {
				_drawFct = &Drawer2DSDL::DrawerCircleBressenham::DrawBottomLeft;
			}
			else if (circle.part == IDrawer2D::Circle::Part::BOTTOM_RIGHT) {
				_drawFct = &Drawer2DSDL::DrawerCircleBressenham::DrawBottomRight;
			}
			if (_drawFct) {
				// Start To Draw
				(this->*_drawFct)();
				while (y >= x)
				{
					// for each pixel we will 
					// draw all eight pixels 

					x++;

					// check for decision parameter 
					// and correspondingly  
					// update d, x, y 
					if (d > 0)
					{
						y--;
						d = d + 4 * (x - y) + 10;
					}
					else {
						d = d + 4 * x + 6;
					}
					(this->*_drawFct)();
				}
			}
			_circle.diameter += 2;
		}
	}

	void Drawer2DSDL::DrawerCircleBressenham::SetPartsPosition(const Vec2i& pos, const Circle::Part& part)
	{
		if (part == Circle::Part::FULL) {
			pBL = pos;
			pBR = pos;
			pTL = pos;
			pTR = pos;
		}
		else if (part == Circle::Part::BOTTOM_LEFT) {
			pBL = pos;
		}
		else if (part == Circle::Part::BOTTOM_RIGHT) {
			pBR = pos;
		}
		else if (part == Circle::Part::BOTTOM) {
			pBL = pos;
			pBR = pos;
		}
		else if (part == Circle::Part::TOP_LEFT) {
			pTL = pos;
		}
		else if (part == Circle::Part::TOP_RIGHT) {
			pTR = pos;
		}
		else if (part == Circle::Part::TOP) {
			pTL = pos;
			pTR = pos;
		}
	}

	void Drawer2DSDL::DrawerCircleBressenham::DrawFull()
	{
		DrawTop();
		DrawBottom();
	}

	void Drawer2DSDL::DrawerCircleBressenham::DrawTop()
	{
		DrawTopLeft();
		DrawTopRight();
	}

	void Drawer2DSDL::DrawerCircleBressenham::DrawTopLeft()
	{
		_drawer->InternCheckBFSetPixel(pTL.x - y, pTL.y - x, _circle.color.value());
		_drawer->InternCheckBFSetPixel(pTL.x - x, pTL.y - y, _circle.color.value());
	}

	void Drawer2DSDL::DrawerCircleBressenham::DrawTopRight()
	{
		_drawer->InternCheckBFSetPixel(pTR.x + x, pTR.y - y, _circle.color.value());
		_drawer->InternCheckBFSetPixel(pTR.x + y, pTR.y - x, _circle.color.value());
	}

	void Drawer2DSDL::DrawerCircleBressenham::DrawBottom()
	{
		DrawBottomLeft();
		DrawBottomRight();
	}

	void Drawer2DSDL::DrawerCircleBressenham::DrawBottomLeft()
	{
		_drawer->InternCheckBFSetPixel(pBL.x - x, pBL.y + y, _circle.color.value());
		_drawer->InternCheckBFSetPixel(pBL.x - y, pBL.y + x, _circle.color.value());
	}

	void Drawer2DSDL::DrawerCircleBressenham::DrawBottomRight()
	{
		_drawer->InternCheckBFSetPixel(pBR.x + y, pBR.y + x, _circle.color.value());
		_drawer->InternCheckBFSetPixel(pBR.x + x, pBR.y + y, _circle.color.value());
	}

	//////////////////////////////
	/////                  ///////
	///// RECT and BORDERS ///////
	/////                  ///////
	//////////////////////////////

	Drawer2DSDL::IDrawerRect::IDrawerRect(Drawer2DSDL* drawer)
	{
		_drawer = drawer;
	}

	void Drawer2DSDL::IDrawerRect::DrawStart(const int& _x, const int& _y, Rect& rect)
	{
		DrawInit(_x, _y, rect);
		// Use corresponding method to type of rect)
		if (rect.radius == 0) {
			DrawClassic();
		}
		else {
			DrawRadius();
		}
	}

	// Can Modify rect
	bool Drawer2DSDL::IDrawerRect::DrawInit(const int& _x, const int& _y, Rect& rect)
	{
		xr = _x;
		yr = _y;
		// Init Rect and check if (at least partly) inside image
		if (!_iR.Init(xr, yr, _drawer->_currImage->GetWidth(), _drawer->_currImage->GetHeight(), rect.width, rect.height))
			return (false);
		// Correct The Border Radius to avoid being larger than the size of the border
		if ((rect.radius * 2) > rect.height) {
			rect.radius = (rect.height / 2);
		}
		if ((rect.radius * 2) > rect.width) {
			rect.radius = (rect.width / 2);
		}
		return (OnDrawInit());
	}


	void Drawer2DSDL::DrawerRect::Draw(const int& _x, const int& _y, const Rect& rect)
	{
		_rect = rect;
		DrawStart(_x, _y, _rect);
	}

	void Drawer2DSDL::DrawerRect::DrawClassic()
	{
		//std::cout << "DrawerRect::DrawClassic()" << std::endl;
		for (_iR.xi = _iR.cX; _iR.xi < _iR.cXMax; _iR.xi++)
			for (_iR.yi = _iR.cY; _iR.yi < _iR.cYMax; _iR.yi++) {
				_drawer->InternCheckBFSetPixel(_iR.xi, _iR.yi, _rect.color.value());
			}
	}

	void Drawer2DSDL::DrawerRect::DrawRadius()
	{
		//std::cout << "DrawerRect::DrawRadius()" << std::endl;
		float diameter = (float)_rect.radius * 2.0f;
		// Border Part
		InfoCircleBressenham c;
		c.Init(diameter);
		while (c.y >= c.x)
		{
			// for each pixel we will 
			// draw all eight pixels 
			c.x++;
			// check for decision parameter 
			// and correspondingly  
			// update d, x, y 
			if (c.d > 0)
			{
				c.y--;
				c.d = c.d + 4 * (c.x - c.y) + 10;
			}
			else {
				c.d = c.d + 4 * c.x + 6;
			}

		}
	}

	void Drawer2DSDL::DrawerRectBorder::Draw(const int& _x, const int& _y, const RectBorder& rectBorder)
	{
		_rectBorder = rectBorder;
		DrawStart(_x, _y, _rectBorder);
	}

	void Drawer2DSDL::DrawerRectBorder::DrawClassic()
	{
		//std::cout << "DrawerRectBorder::DrawClassic()" << std::endl;
		if (yr >= 0)
			for (_iR.xi = _iR.cX; _iR.xi < _iR.cXMax; _iR.xi++)
				_drawer->InternCheckBFSetPixel(_iR.xi, yr, _rectBorder.color.value());
		if (xr >= 0)
			for (_iR.yi = _iR.cY; _iR.yi < _iR.cYMax; _iR.yi++)
				_drawer->InternCheckBFSetPixel(xr, _iR.yi, _rectBorder.color.value());
		// Remove One to max to avoid declaring other var or 
		_iR.cXMax -= 1;
		_iR.cYMax -= 1;
		_iR.xMax -= 1;
		_iR.yMax -= 1;
		// std::cout << "IRcXMAX (" << cXMax << ") IRcYMAX (" << cYMax << ")" << std::endl;
		if (_iR.yMax < _iR.ih)
			for (_iR.xi = _iR.cX; _iR.xi <= _iR.cXMax; _iR.xi++)
				_drawer->InternCheckBFSetPixel(_iR.xi, _iR.yMax, _rectBorder.color.value());
		if (_iR.xMax < _iR.iw)
			for (_iR.yi = _iR.cY; _iR.yi <= _iR.cYMax; _iR.yi++)
				_drawer->InternCheckBFSetPixel(_iR.xMax, _iR.yi, _rectBorder.color.value());
	}

	void Drawer2DSDL::DrawerRectBorder::DrawRadius()
	{
		//std::cout << "DrawerRectBorder::DrawRadius()" << std::endl;
		int iradius = (int)_rectBorder.radius;

		// Draw Lines 
		_iR.cX += iradius;
		_iR.cXMax -= iradius;
		if (yr >= 0)
			for (_iR.xi = _iR.cX; _iR.xi < _iR.cXMax; _iR.xi++)
				_drawer->InternCheckBFSetPixel(_iR.xi, yr, _rectBorder.color.value());

		_iR.cY += iradius;
		_iR.cYMax -= iradius;
		if (xr >= 0)
			for (_iR.yi = _iR.cY; _iR.yi < _iR.cYMax; _iR.yi++)
				_drawer->InternCheckBFSetPixel(xr, _iR.yi, _rectBorder.color.value());

		//// Remove One to max to avoid declaring other var or 

		//cX -= 2;
		//cXMax += 2;
		//cY -= 2;
		//cYMax += 2;
		//xMax -= 1;
		//yMax -= 1;

		_iR.cXMax -= 1;
		_iR.cYMax -= 1;
		_iR.xMax -= 1;
		_iR.yMax -= 1;
		//// std::cout << "IRcXMAX (" << cXMax << ") IRcYMAX (" << cYMax << ")" << std::endl;

		if (_iR.yMax < _iR.ih) // Bottom
			for (_iR.xi = _iR.cX; _iR.xi <= _iR.cXMax; _iR.xi++)
				_drawer->InternCheckBFSetPixel(_iR.xi, _iR.yMax, _rectBorder.color.value());

		if (_iR.xMax < _iR.iw) // Right
			for (_iR.yi = _iR.cY; _iR.yi <= _iR.cYMax; _iR.yi++)
				_drawer->InternCheckBFSetPixel(_iR.xMax, _iR.yi, _rectBorder.color.value());

		// Use Drawer Circle for Borders
		DrawerCircleBressenham	drawerBorderCircle(_drawer);
		Circle					borderCircle;

		borderCircle.diameter = (_rectBorder.radius * 2);
		borderCircle.color = _rectBorder.color;
		borderCircle.part = IDrawer2D::Circle::Part::FULL;

		drawerBorderCircle.SetPartsPosition(Vec2i(xr + iradius, yr + iradius), IDrawer2D::Circle::Part::TOP_LEFT);
		drawerBorderCircle.SetPartsPosition(Vec2i(xr - iradius + (int)_rectBorder.width - 1, yr + iradius), IDrawer2D::Circle::Part::TOP_RIGHT);
		drawerBorderCircle.SetPartsPosition(Vec2i(xr + iradius, yr - iradius + (int)_rectBorder.height - 1), IDrawer2D::Circle::Part::BOTTOM_LEFT);
		drawerBorderCircle.SetPartsPosition(Vec2i(xr - iradius + (int)_rectBorder.width - 1, yr - iradius + (int)_rectBorder.height - 1), IDrawer2D::Circle::Part::BOTTOM_RIGHT);

		// DEBUG COLORS
		/*borderCircle.color = Color::RED();
		borderCircle.part = IDrawer2D::Circle::Part::TOP_LEFT;
		drawerBorderCircle.Draw(borderCircle);
		borderCircle.color = Color::GREEN();
		borderCircle.part = IDrawer2D::Circle::Part::TOP_RIGHT;
		drawerBorderCircle.Draw(borderCircle);
		borderCircle.color = Color::YELLOW();
		borderCircle.part = IDrawer2D::Circle::Part::BOTTOM_LEFT;
		drawerBorderCircle.Draw(borderCircle);
		borderCircle.color = Color::BLUE();
		borderCircle.part = IDrawer2D::Circle::Part::BOTTOM_RIGHT;
		drawerBorderCircle.Draw(borderCircle);*/

		drawerBorderCircle.Draw(borderCircle);
	}


	bool Drawer2DSDL::InfoRect::Init(const int& xr, const int& yr, const unsigned int& widthMax, const unsigned int& heightMax, const unsigned int& width, const unsigned int& height)
	{
		iw = (int)widthMax;
		ih = (int)heightMax;

		// Inside Image Verification
		if (xr >= iw || yr >= ih)
			return (false);

		xMax = xr + (int)width;
		yMax = yr + (int)height;

		if (xMax < 0 || yMax < 0)
			return (false);

		// Set Correct Min X and Y
		(xr <= 0) ? cX = 0 : cX = xr;
		(yr <= 0) ? cY = 0 : cY = yr;
		// Set Correct Max X and Y
		(xMax > iw) ? cXMax = (iw - 1) : cXMax = xMax;
		(yMax > ih) ? cYMax = (ih - 1) : cYMax = yMax;

		return (true);
	}

	void Drawer2DSDL::InfoCircleBressenham::Init(const float& diameter)
	{
		r = int(diameter / 2.0f);
		x = 0;
		y = r;
		d = 3 - 2 * r;
	}

}

#endif // GXEZ_SDL