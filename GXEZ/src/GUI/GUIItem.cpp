#include "GXEZ/GUI/GUIItem.h"

// GXEZ
#include "GXEZ/GUI/GUIManager.h"

// STD
#include <sstream> // For Texture Image IDS


namespace GXEZ
{

	/////////////////////////////
	///////////////////////
	////// GUI ITEM 
	//// 
	///

	GUIItem::GUIItem(GUIManager* manager, GUIItem* parent, IRenderer* renderer, GUIItem::ItemType type, const GUIItem::Definition& definition)
	{
		_id = 0;
		_type = type;
		_renderer = renderer;
		_parent = NULL;
		_realPosition.x = 0;
		_realPosition.y = 0;
		_realSize.x = 0;
		_realSize.y = 0;
		_isVisible = true;
		_isHovered = false;
		_isClicked = false;
		_isSelected = false;
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

	GUIItem::Handler::Handler(std::function<void()> f) : _function(f)
	{
	}

	void GUIItem::Definition::LoadDefault()
	{
	}

	void GUIItem::Resize(const Definition::Size& size)
	{
		_definition.size = size;
		OnResizeParent();
	}

	void GUIItem::Draw(IRenderer* renderer)
	{
		if (_isVisible) {
			//std::cout << "GUIItem::Draw (" << _id << ")" << std::endl;
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
		_definition.LoadDefault();
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
			OnChanged();
			OnClick(state);
		}
	}

	const bool& GUIItem::IsClicked() const
	{
		return (_isClicked);
	}


	void GUIItem::SetHovered(bool state)
	{
		if (_isHovered != state)
		{
			std::cout << "GUIItem(" << _id << ")::SetHovered (" << state << ")" << std::endl;
			_isHovered = state;
			// Call Overridable Method On Click
			OnChanged();
			OnHovered(state);
		}
	}

	const bool& GUIItem::IsHovered() const
	{
		return (_isHovered);
	}

	void GUIItem::SetSelected(bool state)
	{
	}

	const bool& GUIItem::IsSelected() const
	{
		return (_isSelected);
	}

	void GUIItem::SetVisible(bool state)
	{
		if (_isVisible != state)
		{
			std::cout << "GUIItem(" << _id << ")::SetVisible (" << state << ")" << std::endl;
			_isVisible = state;
			// Call Overridable Method On Click
			OnChanged();
			OnVisible(state);
		}
	}

	const bool& GUIItem::IsVisible() const
	{
		return (_isVisible);
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
		delete (_texture);
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
			OnChanged();
		}
		for (size_t i = 0; i < _children.size(); i += 1)
		{
			_children.at(i)->OnResizeParent();
		}
	}



	void GUIItem::OnChanged()
	{
		std::cout << "GUIItem::OnChanged()" << std::endl;
		CreateTexture();
	}

	void GUIItem::ComputeRealPosition()
	{
		if (_definition.position.type == UnitType::ABSOLUTE_PX)
		{
			_realPosition.x = (int)_definition.position.x.value();
			_realPosition.y = (int)_definition.position.y.value();
		}
		else if (_definition.position.type == UnitType::RELATIVE_TO_PARENT && _parent != NULL) {
			_realPosition.x = (int)((float)_parent->GetRealPosition().x + ((float)_parent->GetRealSize().x * _definition.position.x.value()));
			_realPosition.y = (int)((float)_parent->GetRealPosition().y + ((float)_parent->GetRealSize().y * _definition.position.y.value()));
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
			_realSize.x = (int)_definition.size.width.value();
			_realSize.y = (int)_definition.size.height.value();
		}
		else if (_definition.size.type == UnitType::RELATIVE_TO_PARENT && _parent != NULL) {
			_realSize.x = (int)((float)_parent->GetRealSize().x * _definition.size.width.value());
			_realSize.y = (int)((float)_parent->GetRealSize().y * _definition.size.height.value());
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

#define GUIBUTTON_DEFAULT_BACKGROUND_COLOR_NORMAL Color(225, 225, 225, 255)
#define GUIBUTTON_DEFAULT_BACKGROUND_COLOR_HOVERED Color(229, 241, 251, 255)
#define GUIBUTTON_DEFAULT_BORDER_COLOR_NORMAL Color(173, 173, 173, 255)
#define GUIBUTTON_DEFAULT_BORDER_COLOR_HOVERED Color(0, 120, 215, 255)

	GUIButton::Definition::Definition()
	{
		// Defaults Values - Mandatories - Will be set even if user do not set them
		size.width = 60;
		size.height = 20;
		border.radius = 0;
		border.size = 2;
	}

	void GUIButton::Definition::LoadDefault()
	{	
		std::cout << "GUIButton::Definition::LoadDefault()" << std::endl;
		// Default Before
		LoadDefaultModificator(hovered, GUIButton::ModificatorID::GUIBUTTONMODIFICATOR_HOVERED_ID);
		LoadDefaultModificator(clicked, GUIButton::ModificatorID::GUIBUTTONMODIFICATOR_CLICKED_ID);
		LoadDefaultModificator(selected, GUIButton::ModificatorID::GUIBUTTONMODIFICATOR_SELECTED_ID);
		// Default After 
		if (!border.color.has_value()) {
			border.color = Color(GUIBUTTON_DEFAULT_BORDER_COLOR_NORMAL);
		}
		if (!background.color.has_value()) {
			background.color = Color(GUIBUTTON_DEFAULT_BACKGROUND_COLOR_NORMAL);
		}
	}

	void GUIButton::Definition::LoadDefaultModificator(GUIButton::DefinitionModificator& modificator, const GUIButton::ModificatorID& id)
	{
		std::cout << "LoadDefaultModificator (" << id << ")" << std::endl;
		if (!modificator.background.color.has_value())
		{
			if (background.color.has_value()) {
				modificator.background.color = background.color.value();
			} else { // DEFAULT Color Panels
				if (id == GUIButton::ModificatorID::GUIBUTTONMODIFICATOR_HOVERED_ID)
					modificator.background.color = GUIBUTTON_DEFAULT_BACKGROUND_COLOR_HOVERED;
				else if (id == GUIButton::ModificatorID::GUIBUTTONMODIFICATOR_SELECTED_ID)
					modificator.background.color = GUIBUTTON_DEFAULT_BACKGROUND_COLOR_NORMAL;
				else if (id == GUIButton::ModificatorID::GUIBUTTONMODIFICATOR_CLICKED_ID)
					modificator.background.color = Color(229, 241, 251);
			}
		}
		if (!modificator.border.color.has_value())
		{
			if (border.color.has_value()) {
				modificator.border.color = border.color.value();
			} else { // DEFAULT Color Panels
				if (id == GUIButton::ModificatorID::GUIBUTTONMODIFICATOR_HOVERED_ID)
					modificator.border.color = GUIBUTTON_DEFAULT_BORDER_COLOR_HOVERED;
				else if (id == GUIButton::ModificatorID::GUIBUTTONMODIFICATOR_SELECTED_ID)
					modificator.border.color = GUIBUTTON_DEFAULT_BORDER_COLOR_NORMAL;
				else if (id == GUIButton::ModificatorID::GUIBUTTONMODIFICATOR_CLICKED_ID)
					modificator.border.color = Color(229, 241, 251);
			}
		}
		if (!modificator.border.size.has_value())
			modificator.border.size = border.size.value();
		if (!modificator.border.sizeType.has_value())
			modificator.border.sizeType = border.sizeType.value();
		if (!modificator.border.radius.has_value())
			modificator.border.radius = border.radius.value();
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

	void GUIButton::CreateTexture()
	{
		ClearTextures();
		/*CreateTextureNormal();
		CreateTextureHovered();
		CreateTextureClicked();*/
		//std::cout << "GUIButton::Draw() Texture Size (" << _textures.size() << ")" << std::endl;
		if (IsClicked()) // Draw Cliked Texture
			CreateTextureClicked();
		else if (IsHovered()) // Draw Hovered Texture
			CreateTextureHovered();
		else if (IsSelected()) // Draw Selected Texture
			CreateTextureSelected();
		else // Draw Normal Texture
			CreateTextureNormal();
	}

	ATexture2D* GUIButton::CreateTextureBase(const GUIItem::Definition::Border& border, const GUIItem::Definition::Background& background)
	{
		// Create Image & USE it
		ATexture2D* texture = _renderer->CreateTexture2D(ATexture2D::Definition(GetRealSize()));
		texture->UseAsRenderTarget();

		////// DRAW //////
		Vec2i pos;
		pos.x = 0;
		pos.y = 0;

		std::cout << "GUIButton::RECT BORDER SIZE (" << border.size.value() << ")" << std::endl;;

		// Button Borders
		IDrawer2D::RectBorder rectBorder;

		rectBorder.width = GetRealSize().x;
		rectBorder.height = GetRealSize().y;
		rectBorder.color = border.color.value();
		rectBorder.size = border.size.value(); // Convert Needed from UnitType
		rectBorder.radius = border.radius.value();

		Vec2 max;

		max.x = GetRealSize().x / 2;
		max.y = GetRealSize().y / 2;

		if (rectBorder.radius > max.x) {
			rectBorder.radius = max.x;
		}
		if (rectBorder.radius > max.y) {
			rectBorder.radius = max.y;
		}		
		// std::cout << "GUIButton::CreateTextureBase Border Radius (" << border.radius << ")" << std::endl;

		texture->DrawRectBorder(pos, rectBorder);

		// BackGround
		IDrawer2D::Rect rect;

		if (rectBorder.size > rectBorder.radius)
		{
			rect.width = (GetRealSize().x - (border.size.value() * 2));
			rect.height = (GetRealSize().y - (border.size.value() * 2));
			rect.color = background.color.value();
			pos.x = int(border.size.value());
			pos.y = int(border.size.value());
			texture->DrawRect(pos, rect);
		}
		else
		{
			float diff = (rectBorder.radius - rectBorder.size);
			Vec2 tl, tr, bl, br;

			rect.width = (GetRealSize().x - (border.size.value() * 2)) + 2;
			rect.height = (GetRealSize().y - (border.size.value() * 2)) + 2;
			rect.color = background.color.value();
			rect.radius = diff;
			pos.x = int(border.size.value()) - 1;
			pos.y = int(border.size.value()) - 1;
			texture->DrawRect(pos, rect);
		}

		texture->UseAsRenderTarget(false);

		return (texture);
	}

	void GUIButton::CreateTextureNormal()
	{
		std::cout << "GUIButton::CreateTextureNormal()" << std::endl;
		_texture = CreateTextureBase(_definitionButton.border, _definitionButton.background);
	}

	void GUIButton::CreateTextureHovered()
	{
		std::cout << "GUIButton::CreateTextureHovered()" << std::endl;
		_texture = CreateTextureBase(_definitionButton.hovered.border, _definitionButton.hovered.background);
	}

	void GUIButton::CreateTextureClicked()
	{
		std::cout << "GUIButton::CreateTextureClicked()" << std::endl;
		_texture = CreateTextureBase(_definitionButton.clicked.border, _definitionButton.clicked.background);
	}

	void GUIButton::CreateTextureSelected()
	{
		std::cout << "GUIButton::CreateTextureSelected()" << std::endl;
		_texture = CreateTextureBase(_definitionButton.selected.border, _definitionButton.selected.background);
	}

	void GUIButton::OnDraw(IRenderer* renderer)
	{
		renderer->DrawTexture(GetRealPosition(), _texture);
	}

	void GUIButton::SetDefinitionButton(const GUIButton::Definition& definition)
	{
		_definitionButton = definition;
		_definitionButton.LoadDefault();
		CreateTexture();
	}

	GUICanvas::GUICanvas(GUIManager* manager, GUIItem* parent, IRenderer* renderer, const GUIItem::Definition& definition) : GUIItem(manager, parent, renderer, GUIItem::ItemType::BUTTON, definition)
	{
	}

	void GUICanvas::OnDraw(IRenderer* renderer)
	{
		// Do Nothing
	}

	void GUICanvas::CreateTexture()
	{
		// No Texture
	}



}