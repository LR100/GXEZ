#pragma once

// GXEZ
#include "GXEZ/Event/IEventHandler.h"
#include "GXEZ/Graphic/IDrawer2D.h"
#include "GXEZ/GUI/GUIItem.h"

// STD
#include <unordered_map>
#include <vector>

// MXEZ
#include "MXEZ/AABB/QuadTree.h"

namespace GXEZ
{
	class GUIManager
	{
	public:
		GUIManager(const unsigned int& width, const unsigned int& height);

		// Basics
		void			Resize(const unsigned int& width, const unsigned int& height);
		void			Draw();

		// Link/Init
		void			LinkEventHandler(IEventHandler* eventHandler);
		void			LinkDrawer2D(IDrawer2D* drawer2D);
		unsigned int	LinkItem(GUIItem* item);

		// Items
		GUIItem* GetItemHovered() const;
		GUIButton* CreateButton(const GUIButton::Definition& definition, GUIItem* parent = NULL);

	private:

		class GUIScreen : public GUIItem
		{
		public:
			GUIScreen(GUIManager* manager, IDrawer2D* drawer, const GUIItem::Definition& definition);
		private:
			virtual void	OnDraw(IDrawer2D* drawer) override;
			virtual void	CreateSprites() override;
		};
		 
		// Event Handling //
		void			EventMouseMoved();
		void			EventMouseLeftPress();
		void			EventMouseLeftRelease();

		// Event Trigger
		void			UpdateItemsHoveredByMouse();
		void			InsertItemsClickedByMouse();
		void			RemoveItemsClickedByMouse();


		// Items
		std::vector<GUIItem*>						_items;
		std::unordered_map<unsigned int, GUIItem*>	_itemsByID;
		std::vector<unsigned int>					_itemsAvailablesIDs;
		std::vector<GUIItem*>						_itemsHovered;
		std::vector<GUIItem*>						_itemsClicked;
		GUIItem* _itemHovered; // Top Foreground
		// Root Item
		GUIItem* _itemScreen;

		QuadTreeAABB2T<int, GUIItem*>* _tree;

		// Basics
		unsigned int			_width, _height;
		IEventHandler* _eventHandler;
		IDrawer2D*	 _drawer2D;
	};

}