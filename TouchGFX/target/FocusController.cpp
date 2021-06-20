/*
 * FocusController.cpp
 *
 *  Created on: 14 Jun 2021
 *      Author: Trevor Attema
 */


#include <stdlib.h>
#include <FocusController.h>

#include <touchgfx/containers/Container.hpp>

#include <SX1509B.h>

int32_t	tap_x = -1, tap_y = -1;

namespace touchgfx
{

	void FocusBox::handleTickEvent() {

	}

	void FocusBox::attachFocusable( Drawable & backArea, Drawable &focusArea, Drawable &tapArea )
	{
		FocusItem e(backArea, focusArea, tapArea);
		focusVector.add(e);
	}

	void FocusBox::processKey( uint8_t key )
	{
		int16_t next;
		FocusItem item;
		switch(key)
		{
			case KEY_UP:{
				next = getNextFocusItem(currentItem, NAV_UP);
				break;
			}
			case KEY_DOWN:{
				next = getNextFocusItem(currentItem, NAV_DOWN);
				break;
			}
			case KEY_LEFT: {
				next = getNextFocusItem(currentItem, NAV_LEFT);
				break;
			}
			case KEY_RIGHT: {
				next = getNextFocusItem(currentItem, NAV_RIGHT);
				break;
			}
			case KEY_SELECT: {
				item = focusVector[currentItem];
				item.tap();
				return;
			}
		}

		if(next != -1) {
			currentItem = next;
			item = focusVector[currentItem];
			boxWithBorder->setPosition(item.getX(), item.getY(), item.getWidth(), item.getHeight());
			item.invalidate();
		}
	}

	void FocusItem::tap()
	{
		_point.getXY(tap_x, tap_y);
	}

} /* namespace touchgfx */
