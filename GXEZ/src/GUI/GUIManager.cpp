#include "GXEZ/GUI/GUIManager.h"

#define GUI_MANAGER_ITEMS_MAX 42000
namespace GXEZ
{

	GUIManager::GUIManager(const unsigned int& width, const unsigned int& height)
	{
		_width = width;
		_height = height;
		_eventHandler = NULL;
		_itemHovered = NULL;
		for (unsigned int i = 1; i < GUI_MANAGER_ITEMS_MAX; i += 1) { // Items IDs start at 1 !!!!!
			_itemsAvailablesIDs.push_back(i);
		}

		_screenDef.size.width = (float)_width;
		_screenDef.size.height = (float)_height;
		_screenDef.size.type = GUIItem::UnitType::ABSOLUTE_PX;

		AABB2i	screenMaxAABB(Vec2i(5000, 5000), Vec2i(5000, 5000));

		_tree = new QuadTreeAABB2T<int, GUIItem*>(screenMaxAABB, 10);
		_itemScreen = new GUIScreen(this, NULL, _screenDef);

	}

	void GUIManager::Resize(const unsigned int& width, const unsigned int& height)
	{
		_width = width;
		_height = height;
		_screenDef.size.width = (float)_width;
		_screenDef.size.height = (float)_height;
		// TODO : Resize all Items
		_itemScreen->Resize(_screenDef.size);
	}

	void GUIManager::Draw()
	{
		//std::cout << "GUIManager::Draw()" << std::endl;
		// MUST BE OPTIMIZED
		_itemScreen->DrawTree(_renderer);

	}

	void GUIManager::LinkEventHandler(IEventHandler* eventHandler)
	{
		_eventHandler = eventHandler;
		_eventHandler->AddHandlerToEvent(ControlKey::MOUSE_MOTION, &GUIManager::EventMouseMoved, this);
		_eventHandler->AddHandlerToEvent(ControlKey::MOUSE_LEFT, ControlKeyState::PRESSED, &GUIManager::EventMouseLeftPress, this);
		_eventHandler->AddHandlerToEvent(ControlKey::MOUSE_LEFT, ControlKeyState::RELEASED, &GUIManager::EventMouseLeftRelease, this);
	}

	void GUIManager::LinkRenderer(IRenderer* renderer)
	{
		_renderer = renderer;
	}

	//void GUIManager::LinkDrawer2D(IDrawer2D* drawer2D)
	//{
	//	_renderer = drawer2D;
	//}

	unsigned int GUIManager::LinkItem(GUIItem* item)
	{
		// Not Linked Yet
		if (item->GetID() == 0) {
			std::cout << "Link New Item With AABB (" << item->GetAABB().ToString() << ")" << std::endl;
			unsigned int id = _itemsAvailablesIDs.back();
			_itemsAvailablesIDs.pop_back();
			// Add Items In The List
			_items.push_back(item);
			_tree->InsertData(item->GetAABB(), item);
			return (id);
		}
		// Item is Already Linked !
		return (item->GetID());
	}

	GUIItem* GUIManager::GetItemHovered() const
	{
		return (_itemHovered);
	}

	GUIButton* GUIManager::CreateButton(const GUIButton::Definition& definition, GUIItem* parent)
	{
		GUIItem* nparent;
		if (parent == NULL) {
			nparent = _itemScreen;
		}
		else {
			nparent = parent;
		}
		GUIButton* button = new GUIButton(this, nparent, _renderer, definition);
		std::cout << "Create GUIButton with AABB (" << button->GetAABB().ToString() << ")" << std::endl;
		return (button);
	}

	void GUIManager::EventMouseMoved()
	{
		UpdateItemsHoveredByMouse();
	}

	void GUIManager::EventMouseLeftPress()
	{
		RemoveItemsClickedByMouse();
		InsertItemsClickedByMouse();
	}

	void GUIManager::EventMouseLeftRelease()
	{
		RemoveItemsClickedByMouse();
	}

	void GUIManager::UpdateItemsHoveredByMouse()
	{
		_itemHovered = NULL;

		// Keep previous hovered
		std::list<GUIItem*> previousHovered = _itemsHovered;
		std::list<GUIItem*> currentHovered;
		auto it = _itemsHovered.begin();
		auto itEnd = _itemsHovered.end();

		// Set New Ones
		if (_eventHandler) {
			const Mouse* mouse = _eventHandler->GetMouse();
			Vec2i pos;
			pos.x = mouse->x;
			pos.y = mouse->y;
			// std::cout << "Mouse Moved At Pos (" << pos.ToString() << ")" << std::endl;
			auto indexes = _tree->GetIndexesAtPosition(pos);
			int zIndexMax = -9999;
			GUIItem* itemCursor;
			// Set Items as Hovered and search for Item at top Foreground
			for (size_t i = 0; i < indexes.size(); i += 1)
			{
				itemCursor = indexes.at(i)->GetData();
				itemCursor->SetHovered(true);
				if (itemCursor->GetZIndex() > zIndexMax) {
					_itemHovered = itemCursor;
					zIndexMax = itemCursor->GetZIndex();
				}
				if (previousHovered.size())
				{
					it = previousHovered.begin();
					for (; it != previousHovered.end();) {
						if ((*it) == itemCursor)
							it = previousHovered.erase(it);
						else 
							it++;
					}
				}
				currentHovered.push_back(itemCursor);				
				// std::cout << "Item Hovered:" << indexes.at(i)->GetData()->GetType() << std::endl;
			}
		}

		_itemsHovered = currentHovered;

		// Clear Previous Items Hovered
		it = previousHovered.begin();
		itEnd = previousHovered.end();
		for (; it != itEnd; it++) {
			(*it)->SetHovered(false);
		}
		previousHovered.clear();
	}

	void GUIManager::InsertItemsClickedByMouse()
	{
		// std::cout << "Clicked" << std::endl;
		auto it = _itemsHovered.begin();
		auto itEnd = _itemsHovered.end();

		for (; it != itEnd; it++)
		{
			(*it)->SetClicked(true);
			_itemsClicked.push_back((*it));
		}
	}

	void GUIManager::RemoveItemsClickedByMouse()
	{
		// std::cout << "Remove" << std::endl;
		for (size_t i = 0; i < _itemsClicked.size(); i += 1)
		{
			_itemsClicked.at(i)->SetClicked(false);
		}
		_itemsClicked.clear();
	}

	GUIManager::GUIScreen::GUIScreen(GUIManager* manager, IRenderer* renderer, const GUIItem::Definition& definition) : GUIItem(NULL, NULL, renderer, GUIItem::ItemType::NONE, definition)
	{
		GUIItem::SetDefinition(definition);
	}

	void GUIManager::GUIScreen::OnDraw(IRenderer* renderer)
	{
	}

	void GUIManager::GUIScreen::CreateTexture()
	{
	}

}