#include "GXEZ/GUI/GUIItem.h"

// GXEZ
#include "GXEZ/GUI/GUIManager.h"

// STD
#include <sstream> // For Texture Image IDS


namespace GXEZ
{
	GUIItem::GUIItem(GUIManager* manager, GUIItem* parent, IRenderer* renderer, GUIItem::ItemType type, const GUIItem::Definition& definition)
	{
		_id = 0;
		_type = type;
		_renderer = renderer;
		_parent = NULL;
		_realPosition.x = 0;
		_realPosition.y = 0;
		_realSize.x= 0;
		_realSize.y = 0;
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

	void GUIItem::Resize(const Definition::Size& size)
	{
		_definition.size = size;
		OnResizeParent();
	}

	void GUIItem::Draw(IRenderer* renderer)
	{
		if (_isVisible) {
			OnDraw(renderer);
		}
	}

	void GUIItem::DrawTree(IRenderer* renderer)
	{
		// std::cout << "GUIItem::DrawTree()" << std::endl;
		// Draw This
		Draw(renderer);
		// Then Draw Children
		for (size_t i = 0; i < _children.size(); i += 1) {
			_children.at(i)->DrawTree(renderer);
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
			// std::cout << "Item Clicked (" << state << ")" << std::endl;
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

	void GUIItem::ClearTextures()
	{
		// std::cout << "GUIItem::ClearTextures()" << std::endl;
		for (size_t i = 0; i < _textures.size(); i += 1)
		{
			if (_textures.at(i) != NULL) {
				delete (_textures.at(i));
			}
		}
		_textures.clear();
	}

	const Vec2i& GUIItem::GetRealPosition() const
	{
		return (_realPosition);
	}

	const Vec2i& GUIItem::GetRealSize() const
	{
		return (_realSize);
	}

	void GUIItem::OnResizeParent()
	{
		ComputeRealPosition();
		Vec2i previousSize = GetRealSize();
		ComputeRealSize();
		ComputeAABB();
		if (previousSize != GetRealSize())
		{
			OnSizeChanged();
		}
		for (size_t i = 0; i < _children.size(); i += 1)
		{
			_children.at(i)->OnResizeParent();
		}
	}

	void GUIItem::OnSizeChanged()
	{
		std::cout << "GUIItem::OnSizeChanged()" << std::endl;
		CreateTextures();
	}

	void GUIItem::ComputeRealPosition()
	{
		if (_definition.position.type == UnitType::ABSOLUTE_PX)
		{
			_realPosition.x = (int)_definition.position.x;
			_realPosition.y = (int)_definition.position.y;
		}
		else if (_definition.position.type == UnitType::RELATIVE_TO_PARENT && _parent != NULL) {
			_realPosition.x = (int)((float)_parent->GetRealPosition().x + ((float)_parent->GetRealSize().x* _definition.position.x));
			_realPosition.y = (int)((float)_parent->GetRealPosition().y + ((float)_parent->GetRealSize().y * _definition.position.y));
		}
		else {
			_realPosition.x = 0;
			_realPosition.y = 0;
		}
		// std::cout << "GUIItem::ComputeRealPosition() X (" << _realPosition.x << ") Y (" << _realPosition.y << ")" << std::endl;
	}

	void GUIItem::ComputeRealSize()
	{
		if (_definition.size.type == UnitType::ABSOLUTE_PX)
		{
			_realSize.x = (int)_definition.size.width;
			_realSize.y = (int)_definition.size.height;
		}
		else if (_definition.size.type == UnitType::RELATIVE_TO_PARENT && _parent != NULL) {
			_realSize.x = (int)((float)_parent->GetRealSize().x * _definition.size.width);
			_realSize.y = (int)((float)_parent->GetRealSize().y * _definition.size.height);
		}
		else {
			_realSize.x = 0;
			_realSize.y = 0;
		}
		// std::cout << "GUIItem::ComputeRealSize() W (" << _realSize.x << ") H (" << _realSize.y << ")" << std::endl;
	}

	void GUIItem::ComputeAABB()
	{
		_aabb.halfSize.x = (_realSize.x / 2);
		_aabb.halfSize.y = (_realSize.y / 2);
		_aabb.center.x = _aabb.halfSize.x + _realPosition.x;
		_aabb.center.y = _aabb.halfSize.y + _realPosition.y;
		_aabb.ComputeMinMax();
		// TODO: Relocate In AABB Quadtree
	}

	GUIButton::GUIButton(GUIManager* manager, GUIItem* parent, IRenderer* renderer, const GUIButton::Definition& definition) : GUIItem(manager, parent, renderer, GUIItem::ItemType::BUTTON, definition)
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

	void GUIButton::CreateTextures()
	{
		ClearTextures();
		CreateTextureNormal();
		CreateTextureHovered();
		CreateTextureClicked();
	}

	ATexture2D* GUIButton::CreateTextureBase(const GUIItem::Definition::Border& border, const GUIItem::Definition::Background& background)
	{
		// Create Image & USE it
		ATexture2D* texture = _renderer->CreateTexture2D(ATexture2D::Definition(GetRealSize()));
		texture->UseAsRenderTarget();

		////// DRAW //////
		Vec2i pos;

		// Button Borders
		IDrawer2D::RectBorder rectBorder;

		rectBorder.width = GetRealSize().x;
		rectBorder.height = GetRealSize().y;
		rectBorder.color = border.color;
		rectBorder.size = border.size; // Convert Needed from UnitType
		rectBorder.radius = border.radius;
		// std::cout << "GUIButton::CreateTextureBase Border Radius (" << border.radius << ")" << std::endl;

		texture->DrawRectBorder(pos, rectBorder);

		// BackGround
		IDrawer2D::Rect rect;

		rect.width = (GetRealSize().x - (border.size * 2));
		rect.height = (GetRealSize().y - (border.size * 2));
		rect.color = background.color;
		rect.radius = border.radius;
		pos.x = int(border.size);
		pos.y = pos.x;

		texture->DrawRect(pos, rect);

		///// CIRCLE TEST /////

		//IImageDrawer2D::Circle circle;

		//circle.diameter = border.radius * 2;

		//Vec2i pc(int(GetRealSizeWidth() / 2), int(GetRealSizeHeight() / 2));

		//circle.color = Color::RED();
		//circle.part = IImageDrawer2D::Circle::Part::TOP_LEFT;
		//_renderer->DrawCircle(pc.x, pc.y, circle);

		//circle.color = Color::GREEN();
		//circle.part = IImageDrawer2D::Circle::Part::TOP_RIGHT;
		//_renderer->DrawCircle(pc.x, pc.y, circle);

		//circle.color = Color::YELLOW();
		//circle.part = IImageDrawer2D::Circle::Part::BOTTOM_LEFT;
		//_renderer->DrawCircle(pc.x, pc.y, circle);

		//circle.color = Color::BLUE();
		//circle.part = IImageDrawer2D::Circle::Part::BOTTOM_RIGHT;
		//_renderer->DrawCircle(pc.x, pc.y, circle);

		// imageBuffer->Export("test");

		// ColorDef	colorDefTransparency;
		//sprite = new Texture(imageBuffer, colorDefTransparency);
		

		return (texture);
	}

	void GUIButton::CreateTextureNormal()
	{
		_textures.push_back(CreateTextureBase(_definitionButton.border, _definitionButton.background));
	}

	void GUIButton::CreateTextureHovered()
	{
		_textures.push_back(CreateTextureBase(_definitionButton.hovered.border, _definitionButton.hovered.background));
	}

	void GUIButton::CreateTextureClicked()
	{
		_textures.push_back(CreateTextureBase(_definitionButton.clicked.border, _definitionButton.clicked.background));
	}

	void GUIButton::OnDraw(IRenderer* renderer)
	{
		// std::cout << "GUIButton::Draw()" << std::endl;
		if (IsClicked())
		{ // Draw Cliked Texture
			renderer->DrawTexture(GetRealPosition(), _textures.at(2));
		}
		else if (IsHovered())
		{ // Draw Hovered Texture
			renderer->DrawTexture(GetRealPosition(), _textures.at(1));
		}
		else
		{ // Draw Normal Texture
			renderer->DrawTexture(GetRealPosition(), _textures.at(0));
		}
	}

	void GUIButton::SetDefinitionButton(const GUIButton::Definition& definition)
	{
		_definitionButton = definition;
		CreateTextures();
	}

	GUICanvas::GUICanvas(GUIManager* manager, GUIItem* parent, IRenderer* renderer, const GUIItem::Definition& definition) : GUIItem(manager, parent, renderer, GUIItem::ItemType::BUTTON, definition)
	{
	}

	void GUICanvas::OnDraw(IRenderer* renderer)
	{
		// Do Nothing
	}

	void GUICanvas::CreateTextures()
	{
		// No Texture
	}

	GUIItem::Handler::Handler(std::function<void()> f) : _function(f)
	{
	}

}