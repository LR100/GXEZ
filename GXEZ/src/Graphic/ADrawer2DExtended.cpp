#include "GXEZ/Graphic/ADrawer2DExtended.h"

namespace GXEZ
{
	ADrawer2DExtended::ADrawer2DExtended() : _dCB(this), _dR(this), _dRB(this)
	{
	}

	void ADrawer2DExtended::DrawRectBorder(const Vec2i& pos, const IDrawer2D::RectBorder& borderdef)
	{
		_dRB.Draw(pos.x, pos.y, borderdef);
	}

	void ADrawer2DExtended::DrawRect(const Vec2i& pos, const IDrawer2D::Rect& rect)
	{
		_dR.Draw(pos.x, pos.y, rect);
	}

	void ADrawer2DExtended::DrawCircle(const Vec2i& pos, const IDrawer2D::Circle& circle)
	{
		_dCB.Draw(pos.x, pos.y, circle);
	}


	//////////////////////////////
	/////                  ///////
	/////      CIRCLES     ///////
	/////                  ///////
	//////////////////////////////


	ADrawer2DExtended::DrawerCircleBressenham::DrawerCircleBressenham(ADrawer2DExtended* drawer)
	{
		_drawFct = NULL;
		_drawer = drawer;
		// Vec2i are init on 0
		r = 1;
		xo = 0;
		xi = 0;
		ym = 0;
	}

	void ADrawer2DExtended::DrawerCircleBressenham::Draw(const int& _x, const int& _y, const IDrawer2D::Circle& circle)
	{
		SetPartsPosition(Vec2i(_x, _y), Circle::Part::FULL);
		Draw(circle);
	}

	void ADrawer2DExtended::DrawerCircleBressenham::Draw(const IDrawer2D::Circle& circle)
	{
		_circle = circle;

	
		if (circle.part == IDrawer2D::Circle::Part::FULL) {
			_drawFct = &ADrawer2DExtended::DrawerCircleBressenham::DrawFull;
		}
		else if (circle.part == IDrawer2D::Circle::Part::TOP) {
			_drawFct = &ADrawer2DExtended::DrawerCircleBressenham::DrawTop;
		}
		else if (circle.part == IDrawer2D::Circle::Part::TOP_LEFT) {
			_drawFct = &ADrawer2DExtended::DrawerCircleBressenham::DrawTopLeft;
		}
		else if (circle.part == IDrawer2D::Circle::Part::TOP_RIGHT) {
			_drawFct = &ADrawer2DExtended::DrawerCircleBressenham::DrawTopRight;
		}
		else if (circle.part == IDrawer2D::Circle::Part::BOTTOM) {
			_drawFct = &ADrawer2DExtended::DrawerCircleBressenham::DrawBottom;
		}
		else if (circle.part == IDrawer2D::Circle::Part::BOTTOM_LEFT) {
			_drawFct = &ADrawer2DExtended::DrawerCircleBressenham::DrawBottomLeft;
		}
		else if (circle.part == IDrawer2D::Circle::Part::BOTTOM_RIGHT) {
			_drawFct = &ADrawer2DExtended::DrawerCircleBressenham::DrawBottomRight;
		}
		if (_drawFct) {

			r = int((float)_circle.diameter * 0.5f);
			xo = (circle.thickness - 1) + r;
			xi = r;
			inner = r;
			ym = 0;
			erro = 1 - xo;
			erri = 1 - xi;

			// Start To Draw
			while (xo >= ym)
			{
				// for each pixel we will 
				// draw all eight pixels 
				(this->*_drawFct)();
				ym++;

				if (erro < 0) {
					erro += 2 * ym + 1;
				}
				else {
					xo--;
					erro += 2 * (ym - xo + 1);
				}

				if (ym > inner) {
					xi = ym;
				}
				else {
					if (erri < 0) {
						erri += 2 * ym + 1;
					}
					else {
						xi--;
						erri += 2 * (ym - xi + 1);
					}
				}
			}
		}
	}

	void ADrawer2DExtended::DrawerCircleBressenham::SetPartsPosition(const Vec2i& pos, const IDrawer2D::Circle::Part& part)
	{
		if (part == Circle::Part::FULL) {
			pBR = pos;
			pBL = pos;
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
			pBR = pos;
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


	inline void ADrawer2DExtended::DrawerCircleBressenham::DrawXLine(int x1, int x2, int y, const Color& color)
	{
		while (x1 <= x2) _drawer->InternDrawPoint(Vec2i(x1++, y), color);
	}

	inline void ADrawer2DExtended::DrawerCircleBressenham::DrawYLine(int x, int y1, int y2, const Color& color)
	{
		while (y1 <= y2) _drawer->InternDrawPoint(Vec2i(x, y1++), color);
	}

	void ADrawer2DExtended::DrawerCircleBressenham::DrawFull()
	{
		DrawTop();
		DrawBottom();
	}

	void ADrawer2DExtended::DrawerCircleBressenham::DrawTop()
	{
		DrawTopLeft();
		DrawTopRight();
	}

	void ADrawer2DExtended::DrawerCircleBressenham::DrawTopLeft()
	{
		DrawXLine(pTL.x - xo, pTL.x - xi, pTL.y - ym, _circle.color);
		DrawYLine(pTL.x - ym, pTL.y - xo, pTL.y - xi, _circle.color);
	}

	void ADrawer2DExtended::DrawerCircleBressenham::DrawTopRight()
	{
		DrawXLine(pTR.x + xi, pTR.x + xo, pTR.y - ym, _circle.color);
		DrawYLine(pTR.x + ym, pTR.y - xo, pTR.y - xi, _circle.color);
	}

	void ADrawer2DExtended::DrawerCircleBressenham::DrawBottom()
	{
		DrawBottomLeft();
		DrawBottomRight();
	}

	void ADrawer2DExtended::DrawerCircleBressenham::DrawBottomLeft()
	{
		DrawXLine(pBL.x - xo, pBL.x - xi, pBL.y + ym, _circle.color);
		DrawYLine(pBL.x - ym, pBL.y + xi, pBL.y + xo, _circle.color);
	}

	void ADrawer2DExtended::DrawerCircleBressenham::DrawBottomRight()
	{
		DrawXLine(pBR.x + xi, pBR.x + xo, pBR.y + ym, _circle.color);
		DrawYLine(pBR.x + ym, pBR.y + xi, pBR.y + xo, _circle.color);
	}

	//////////////////////////////
	/////                  ///////
	///// RECT and BORDERS ///////
	/////                  ///////
	//////////////////////////////

	bool ADrawer2DExtended::InfoRect::Init(const int& xr, const int& yr, const unsigned int& widthMax, const unsigned int& heightMax, const unsigned int& width, const unsigned int& height)
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

	ADrawer2DExtended::IDrawerRect::IDrawerRect(ADrawer2DExtended* drawer) : _drawerCircle(drawer)
	{
		_drawer = drawer;
	}

	void ADrawer2DExtended::IDrawerRect::DrawStart(const int& _x, const int& _y, Rect& rect)
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
	bool ADrawer2DExtended::IDrawerRect::DrawInit(const int& _x, const int& _y, Rect& rect)
	{
		_pos.x = _x;
		_pos.y = _y;
		//Init Rect and check if (at least partly) inside image
		//if (!_iR.Init(xr, yr, _drawer->_currImage->GetWidth(), _drawer->_currImage->GetHeight(), rect.width, rect.height))
			//return (false);
		// Correct The Border Radius to avoid being larger than the size of the border
		if ((rect.radius * 2) > rect.height) {
			rect.radius = (rect.height / 2);
		}
		if ((rect.radius * 2) > rect.width) {
			rect.radius = (rect.width / 2);
		}
		return (OnDrawInit());
	}

	void ADrawer2DExtended::DrawerRect::Draw(const int& _x, const int& _y, const Rect& rect)
	{
		_rect = rect;
		DrawStart(_x, _y, _rect);
	}

	void ADrawer2DExtended::DrawerRect::DrawClassic()
	{
		//std::cout << "DrawerRect::DrawClassic()" << std::endl;
		_drawer->InternDrawRect(_pos, _rect);
	}

	void ADrawer2DExtended::DrawerRect::DrawRadius()
	{
		std::cout << "DrawerRect::DrawRadius()" << std::endl;
		int idiameter = (float)_rect.radius * 2.0f;
		int iradius = (int)_rect.radius;
		
		// Use Drawer Circle for Borders

		Circle					borderCircle;

		borderCircle.thickness = iradius;
		borderCircle.diameter = 0;
		borderCircle.color = _rect.color;
		borderCircle.part = IDrawer2D::Circle::Part::FULL;

		//_drawerCircle.SetPartsPosition(Vec2i(_pos.x, _pos.y), IDrawer2D::Circle::Part::FULL);
		_drawerCircle.SetPartsPosition(Vec2i(_pos.x + iradius, _pos.y + iradius), IDrawer2D::Circle::Part::TOP_LEFT);
		_drawerCircle.SetPartsPosition(Vec2i(_pos.x - iradius + (int)_rect.width - 1, _pos.y + iradius), IDrawer2D::Circle::Part::TOP_RIGHT);
		_drawerCircle.SetPartsPosition(Vec2i(_pos.x + iradius, _pos.y - iradius + (int)_rect.height - 1), IDrawer2D::Circle::Part::BOTTOM_LEFT);
		_drawerCircle.SetPartsPosition(Vec2i(_pos.x - iradius + (int)_rect.width - 1, _pos.y - iradius + (int)_rect.height - 1), IDrawer2D::Circle::Part::BOTTOM_RIGHT);
		_drawerCircle.Draw(borderCircle);

		std::cout << "IDIAMETER (" << idiameter << ") RECT HEIGHT (" << _rect.height << ")" << std::endl;
		// Center
		if (idiameter < _rect.height) {
			
			_posA.x = _pos.x;
			_posA.y = _rect.radius + _pos.y;
			_rectI.color = _rect.color;
			_rectI.width = _rect.width;
			_rectI.height = (_rect.height - idiameter);
			_drawer->InternDrawRect(_posA, _rectI);
		}
		// Top
		_posA.x = (_pos.x + iradius);
		_posA.y = _pos.y;
		_rectI.color = _rect.color;
		_rectI.width = (_rect.width - idiameter);
		_rectI.height = iradius;
		_drawer->InternDrawRect(_posA, _rectI);

		// Bot
		_posA.y = (_pos.y + _rect.height - iradius);
		_rectI.color = _rect.color;
		_rectI.width = (_rect.width - idiameter);
		_rectI.height = iradius;
		_drawer->InternDrawRect(_posA, _rectI);
	}

	void ADrawer2DExtended::DrawerRectBorder::Draw(const int& _x, const int& _y, const RectBorder& rectBorder)
	{
		_rectBorder = rectBorder;
		DrawStart(_x, _y, _rectBorder);
	}

	void ADrawer2DExtended::DrawerRectBorder::DrawClassic()
	{
		// std::cout << "DrawerRectBorder::DrawClassic()" << std::endl;
		if (_rectBorder.size > 1)
		{
			for (int i = 0; i < _rectBorder.size; i += 1) {
				_drawer->InternDrawRectBorder(_pos, _rectBorder);
				_pos.x += 1;
				_pos.y += 1;
				_rectBorder.width -= 2;
				_rectBorder.height -= 2;
			}
		}
		else
		{
			_drawer->InternDrawRectBorder(_pos, _rectBorder);
		}
	}

	void ADrawer2DExtended::DrawerRectBorder::DrawRadius()
	{
		//std::cout << "DrawerRectBorder::DrawRadius() Border Size(" << _rectBorder.size << ")" << std::endl;
		int iradius = (int)_rectBorder.radius;
		int idiameter = (iradius * 2);

		_rectI.color = _rectBorder.color;

		// TOP and BOT
		_posA.x = (_pos.x + iradius);
		_posA.y = _pos.y;
		
		if (_rectBorder.size > 1)
		{
			// TOP
			_rectI.width = (_rectBorder.width - idiameter);
			_rectI.height = _rectBorder.size;
			_drawer->InternDrawRect(_posA, _rectI);
			// BOT
			_posA.y = (_pos.y + (_rectBorder.height - _rectBorder.size));
			_drawer->InternDrawRect(_posA, _rectI);
		}
		else
		{	
			// TOP
			_posB.x = (_posA.x + (_rectBorder.width - idiameter));
			_posB.y = _posA.y;
			_drawer->InternDrawLine(_posA, _posB, _rectBorder.color);
			// BOT
			_posA.y = (_pos.y + (_rectBorder.height - _rectBorder.size));
			_posB.y = _posA.y;
			_drawer->InternDrawLine(_posA, _posB, _rectBorder.color);
		}

		// RIGHT and LEFT
		_posA.x = _pos.x;
		_posA.y = (_pos.y + iradius);

		if (_rectBorder.size > 1)
		{
			// RIGHT	
			_rectI.width = _rectBorder.size;
			_rectI.height = (_rectBorder.height - idiameter);
			_drawer->InternDrawRect(_posA, _rectI);
			// LEFT
			_posA.x = (_pos.x + (_rectBorder.width - _rectBorder.size));;
			_drawer->InternDrawRect(_posA, _rectI);
		}
		else
		{
			// RIGHT
			_posB.x = _posA.x;
			_posB.y = (_posA.y + (_rectBorder.height - idiameter));
			_drawer->InternDrawLine(_posA, _posB, _rectBorder.color);
			// LEFT 
			_posA.x = (_pos.x + (_rectBorder.width - _rectBorder.size));
			_posB.x = _posA.x;
			_drawer->InternDrawLine(_posA, _posB, _rectBorder.color);
		}

		// Use Drawer Circle for Borders
		//DrawerCircleBressenham	drawerBorderCircle(_drawer);
		Circle					borderCircle;


		borderCircle.thickness = (_rectBorder.size);
		if (borderCircle.thickness > iradius) {
			borderCircle.thickness = iradius;
		}
		borderCircle.diameter = ((iradius + 1) - borderCircle.thickness) * 2;
		borderCircle.color = _rectBorder.color;
		borderCircle.part = IDrawer2D::Circle::Part::FULL;

		//_drawerCircle.SetPartsPosition(Vec2i(_pos.x, _pos.y), IDrawer2D::Circle::Part::FULL);
		_drawerCircle.SetPartsPosition(Vec2i(_pos.x + iradius, _pos.y + iradius), IDrawer2D::Circle::Part::TOP_LEFT);
		_drawerCircle.SetPartsPosition(Vec2i(_pos.x - iradius + (int)_rectBorder.width - 1, _pos.y + iradius), IDrawer2D::Circle::Part::TOP_RIGHT);
		_drawerCircle.SetPartsPosition(Vec2i(_pos.x + iradius, _pos.y - iradius + (int)_rectBorder.height - 1), IDrawer2D::Circle::Part::BOTTOM_LEFT);
		_drawerCircle.SetPartsPosition(Vec2i(_pos.x - iradius + (int)_rectBorder.width - 1, _pos.y - iradius + (int)_rectBorder.height - 1), IDrawer2D::Circle::Part::BOTTOM_RIGHT);

		// DEBUG COLORS
		/*borderCircle.color = Color::RED();
		borderCircle.part = IDrawer2D::Circle::Part::TOP_LEFT;
		_drawerCircle.Draw(borderCircle);
		borderCircle.color = Color::GREEN();
		borderCircle.part = IDrawer2D::Circle::Part::TOP_RIGHT;
		_drawerCircle.Draw(borderCircle);
		borderCircle.color = Color::YELLOW();
		borderCircle.part = IDrawer2D::Circle::Part::BOTTOM_LEFT;
		_drawerCircle.Draw(borderCircle);
		borderCircle.color = Color::BLUE();
		borderCircle.part = IDrawer2D::Circle::Part::BOTTOM_RIGHT;
		_drawerCircle.Draw(borderCircle);*/

		_drawerCircle.Draw(borderCircle);

	}

}