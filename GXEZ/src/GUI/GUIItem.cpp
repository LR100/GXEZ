#include "GXEZ/GUI/GUIItem.h"

// GXEZ
#include "GXEZ/GUI/GUIManager.h"

// STD
#include <sstream> // For Sprite Image IDS


namespace GXEZ
{
	GUIItem::GUIItem(GUIManager* manager, GUIItem* parent, IImageDrawer2D* drawer, GUIItem::ItemType type, const GUIItem::Definition& definition)
	{
		_id = 0;
		_type = type;
		_drawer = drawer;
		_parent = NULL;
		_realPositionX = 0;
		_realPositionY = 0;
		_realSizeWidth = 0;
		_realSizeHeight = 0;
		_isVisible = true;
		_isHovered = false;
		_isClicked = false;
		SetParent(parent);
		SetDefinition(definition);
		// Must Be At End !! LinkItem finish the init
		if (manager) {
			_manager = manager;
			_id = manager->LinkItem(this); // Set ID And Link Item
		}
		else {
			_id = 0;
			_manager = NULL;
		}
	}

	void GUIItem::Draw(IImageDrawer2D* drawer)
	{
		if (_isVisible) {
			OnDraw(drawer);
		}
	}

	void GUIItem::DrawTree(IImageDrawer2D* drawer)
	{
		// std::cout << "GUIItem::DrawTree()" << std::endl;
		// Draw This
		Draw(drawer);
		// Then Draw Children
		for (size_t i = 0; i < _children.size(); i += 1) {
			_children.at(i)->DrawTree(drawer);
		}
	}

	const unsigned int& GUIItem::GetID() const
	{
		return (_id);
	}

	const GUIItem::ItemType& GUIItem::GetType() const
	{
		return (_type);
	}

	const AABB2i& GUIItem::GetAABB() const
	{
		return (_aabb);
	}

	void GUIItem::SetDefinition(const GUIItem::Definition& definition)
	{
		_definition = definition;
		ComputeRealPosition();
		ComputeRealSize();
		ComputeAABB();
	}

	const GUIItem::Definition& GUIItem::GetDefinition() const
	{
		return (_definition);
	}

	void GUIItem::SetParent(GUIItem* item)
	{
		if (item)
		{
			item->AddChild(this);
		}
	}

	GUIItem* GUIItem::GetParent() const
	{
		return (_parent);
	}

	void GUIItem::AddChild(GUIItem* item)
	{
		if (item)
		{
			if (item->GetParent()) {
				item->GetParent()->RemoveChildren(item);
			}
			item->_parent = this;
			if (item->_definition.zIndex == 0) {
				item->_definition.zIndex = (_definition.zIndex + 1);
			}
			_children.push_back(item);
		}
	}

	void GUIItem::RemoveChildren(GUIItem* item)
	{
		if (item)
		{
			for (size_t i = 0; i < _children.size(); i += 1)
			{
				if (_children.at(i) == item)
				{
					_children.erase(_children.begin() + i);
					return; // true ?
				}
			}
		}
	}

	void GUIItem::SetClicked(bool state)
	{
		if (_isClicked != state)
		{
			std::cout << "Item Clicked (" << state << ")" << std::endl;
			_isClicked = state;
			// Call Overridable Method On Click
			OnClick(state);
		}
	}

	const bool& GUIItem::IsClicked() const
	{
		return (_isClicked);
	}

	void GUIItem::SetVisibile(bool state)
	{
		if (_isVisible != state)
		{
			_isVisible = state;
			// Call Overridable Method On Click
			OnVisible(state);
		}
	}

	void GUIItem::SetHovered(bool state)
	{
		if (_isHovered != state)
		{
			_isHovered = state;
			// Call Overridable Method On Click
			OnHovered(state);
		}
	}

	const bool& GUIItem::IsHovered() const
	{
		return (_isHovered);
	}

	void GUIItem::SetZIndex(const int& zindex)
	{
		_definition.zIndex = zindex;
	}

	const int& GUIItem::GetZIndex() const
	{
		return (_definition.zIndex);
	}

	void GUIItem::ClearSprites()
	{
		for (size_t i = 0; i < _sprites.size(); i += 1)
		{
			if (_sprites.at(i) != NULL) {
				delete (_sprites.at(i));
			}
		}
		_sprites.clear();
	}

	const int& GUIItem::GetRealPositionX() const
	{
		return (_realPositionX);
	}

	const int& GUIItem::GetRealPositionY() const
	{
		return (_realPositionY);
	}

	const int& GUIItem::GetRealSizeWidth() const
	{
		return (_realSizeWidth);
	}

	const int& GUIItem::GetRealSizeHeight() const
	{
		return (_realSizeHeight);
	}

	void GUIItem::ComputeRealPosition()
	{
		if (_definition.position.type == UnitType::ABSOLUTE_PX)
		{
			_realPositionX = (int)_definition.position.x;
			_realPositionY = (int)_definition.position.y;
		}
		else if (_definition.position.type == UnitType::RELATIVE_TO_PARENT && _parent != NULL) {
			_realPositionX = (int)((float)_parent->GetRealPositionX() + ((float)_parent->GetRealSizeWidth() * _definition.position.x));
			_realPositionY = (int)((float)_parent->GetRealPositionY() + ((float)_parent->GetRealSizeHeight() * _definition.position.y));
		}
		else {
			_realPositionX = 0;
			_realPositionY = 0;
		}
		std::cout << "GUIItem::ComputeRealPosition() X (" << _realPositionX << ") Y (" << _realPositionY << ")" << std::endl;
	}

	void GUIItem::ComputeRealSize()
	{
		if (_definition.size.type == UnitType::ABSOLUTE_PX)
		{
			_realSizeWidth = (int)_definition.size.width;
			_realSizeHeight = (int)_definition.size.height;
		}
		else if (_definition.size.type == UnitType::RELATIVE_TO_PARENT && _parent != NULL) {
			_realSizeWidth = (int)((float)_parent->GetRealSizeWidth() * _definition.size.width);
			_realSizeHeight = (int)((float)_parent->GetRealSizeHeight() * _definition.size.height);
		}
		else {
			_realSizeWidth = 0;
			_realSizeHeight = 0;
		}
		std::cout << "GUIItem::ComputeRealSize() W (" << _realSizeWidth << ") H (" << _realSizeHeight << ")" << std::endl;
	}

	void GUIItem::ComputeAABB()
	{
		_aabb.halfSize.x = (_realSizeWidth / 2);
		_aabb.halfSize.y = (_realSizeHeight / 2);
		_aabb.center.x = _aabb.halfSize.x + _realPositionX;
		_aabb.center.y = _aabb.halfSize.y + _realPositionY;
		_aabb.ComputeMinMax();
	}

	GUIButton::GUIButton(GUIManager* manager, GUIItem* parent, IImageDrawer2D* drawer, const GUIButton::Definition& definition) : GUIItem(manager, parent, drawer, GUIItem::ItemType::BUTTON, definition)
	{
		SetDefinitionButton(definition);
		_onClickHandler = NULL;
	}

	void GUIButton::OnClick(bool state)
	{
		if (state == false) // was previously true -> trigger the click (could even check if button is still hovered)
		{
			if (_onClickHandler) {
				_onClickHandler->_function();
			}
		}
	}

	void GUIButton::CreateSprites()
	{
		ClearSprites();
		CreateSpriteNormal();
		CreateSpriteHovered();
		CreateSpriteClicked();
	}

	Sprite* GUIButton::CreateSpriteBase(const GUIItem::Definition::Border& border, const GUIItem::Definition::Background& background)
	{
		Sprite* sprite;

		std::stringstream	ss;
		ss << GetID();
		ss << "buttonSB";
		// Create Image & USE it
		IImage* imageBuffer = _drawer->CreateImage(ss.str(), GetRealSizeWidth(), GetRealSizeHeight());
		_drawer->UseImage(ss.str());

		////// DRAW //////
		// Button Borders
		IImageDrawer2D::RectBorder rectBorder;

		rectBorder.width = GetRealSizeWidth();
		rectBorder.height = GetRealSizeHeight();
		rectBorder.color = border.color;
		rectBorder.size = border.size; // Convert Needed from UnitType
		rectBorder.radius = border.radius;
		std::cout << "GUIButton::CreateSpriteBase Border Radius (" << border.radius << ")" << std::endl;

		_drawer->DrawRectBorder(0, 0, rectBorder);

		// BackGround
		IImageDrawer2D::Rect rect;

		rect.width = (GetRealSizeWidth() - (border.size * 2));
		rect.height = (GetRealSizeHeight() - (border.size * 2));
		rect.color = background.color;
		rect.radius = border.radius;

		_drawer->DrawRect(int(border.size), int(border.size), rect);

		///// CIRCLE TEST /////

		//IImageDrawer2D::Circle circle;

		//circle.diameter = border.radius * 2;

		//Vec2i pc(int(GetRealSizeWidth() / 2), int(GetRealSizeHeight() / 2));

		//circle.color = Color::RED();
		//circle.part = IImageDrawer2D::Circle::Part::TOP_LEFT;
		//_drawer->DrawCircle(pc.x, pc.y, circle);

		//circle.color = Color::GREEN();
		//circle.part = IImageDrawer2D::Circle::Part::TOP_RIGHT;
		//_drawer->DrawCircle(pc.x, pc.y, circle);

		//circle.color = Color::YELLOW();
		//circle.part = IImageDrawer2D::Circle::Part::BOTTOM_LEFT;
		//_drawer->DrawCircle(pc.x, pc.y, circle);

		//circle.color = Color::BLUE();
		//circle.part = IImageDrawer2D::Circle::Part::BOTTOM_RIGHT;
		//_drawer->DrawCircle(pc.x, pc.y, circle);

		imageBuffer->Export("test");

		ColorDef	colorDefTransparency;
		sprite = new Sprite(imageBuffer, colorDefTransparency);
		// Remove Image
		_drawer->RemoveImage(ss.str());

		return (sprite);
	}

	void GUIButton::CreateSpriteNormal()
	{
		_sprites.push_back(CreateSpriteBase(_definitionButton.border, _definitionButton.background));
	}

	void GUIButton::CreateSpriteHovered()
	{
		_sprites.push_back(CreateSpriteBase(_definitionButton.hovered.border, _definitionButton.hovered.background));
	}

	void GUIButton::CreateSpriteClicked()
	{
		_sprites.push_back(CreateSpriteBase(_definitionButton.clicked.border, _definitionButton.clicked.background));
	}

	void GUIButton::OnDraw(IImageDrawer2D* drawer)
	{
		//std::cout << "GUIButton::Draw()" << std::endl;
		if (IsClicked())
		{ // Draw Cliked Sprite
			drawer->DrawSprite(GetRealPositionX(), GetRealPositionY(), _sprites.at(2));
		}
		else if (IsHovered())
		{ // Draw Hovered Sprite
			drawer->DrawSprite(GetRealPositionX(), GetRealPositionY(), _sprites.at(1));
		}
		else
		{ // Draw Normal Sprite
			drawer->DrawSprite(GetRealPositionX(), GetRealPositionY(), _sprites.at(0));
		}
	}

	void GUIButton::SetDefinitionButton(const GUIButton::Definition& definition)
	{
		_definitionButton = definition;
		CreateSprites();
	}

	GUICanvas::GUICanvas(GUIManager* manager, GUIItem* parent, IImageDrawer2D* drawer, const GUIItem::Definition& definition) : GUIItem(manager, parent, drawer, GUIItem::ItemType::BUTTON, definition)
	{
	}

	void GUICanvas::OnDraw(IImageDrawer2D* drawer)
	{
		// Do Nothing
	}

	void GUICanvas::CreateSprites()
	{
		// No Sprite
	}

	GUIItem::Handler::Handler(std::function<void()> f) : _function(f)
	{
	}

}