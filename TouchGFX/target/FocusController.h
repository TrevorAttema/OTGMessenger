/*
 * FocusController.h
 *
 *  Created on: 14 Jun 2021
 *      Author: Trevor Attema
 */

#ifndef TARGET_FOCUSCONTROLLER_H_
#define TARGET_FOCUSCONTROLLER_H_

#include <math.h>

#include <touchgfx/Screen.hpp>
#include <touchgfx/widgets/BoxWithBorder.hpp>
#include <touchgfx/Drawable.hpp>

#include <i2c.h>

#define MAX_WIDGETS			10

namespace touchgfx
{

	class FocusItem;
	class FocusFine;

	typedef enum nav_t {NAV_LEFT, NAV_UP, NAV_RIGHT, NAV_DOWN, NAV_EQUAL} nav_t;
	typedef Vector<FocusItem, MAX_WIDGETS> FocusVector_t;
	typedef Vector<FocusFine, MAX_WIDGETS> FocusFine_t;

	class FocusFine {
	public:
		FocusFine () {

		}

		FocusFine(int16_t idx, float ang, float dist) {
			itemIndex 	= idx;
			distance 		= abs(dist);
			angle 			= ang;
		}

		void reset(){
			itemIndex = 0;
			distance = 0.0;
			angle = 0.0;
		}

		int16_t itemIndex;
		float		distance;
		float		angle;
	};

	class TouchPoint {
	public:
		TouchPoint() {
			_x = 0;
			_y = 0;
		}

		TouchPoint(int32_t x, int32_t y) {
			setXY(x,y);
		}

		void setXY(int32_t x, int32_t y) {
			_x = x;
			_y = y;
		}

		void getXY( int32_t &x, int32_t &y) {
			x = _x;
			y = _y;
		}

		// center point in the rect
		void setTap(Rect rect) {
			_x = rect.x + (rect.width / 2);
			_y = rect.y + (rect.height / 2);
		}

    void operator&=(const TouchPoint& other)
    {
    		_x = other._x;
    		_y = other._y;
    }

    int32_t x() {
    	return _x;
    }

    int32_t y() {
    	return _y;
    }

		float distance( TouchPoint& other )
		{
			return distance(_x, _y, other._x, other._y);
		}

		float distance(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
		{
		    // Calculating distance
		    return sqrt(pow(x2 - x1, 2) +
		                pow(y2 - y1, 2) * 1.0);
		}

		float angle( TouchPoint& other )
		{
			return angle(_x, _y, other._x, other._y);
		}

		// Top left corner of screen is 0,0
		//
		//         -90
		//          |
		//	180-----+-----0
		//          |
		//          90
		float angle(int32_t x1, int32_t y1, int32_t x2, int32_t y2 )
		{
			int32_t x, y;

			y = y2-y1;
			x = x2-x1;

			return atan2(y, x) / PI * 180;
		}
	private:
		int32_t	_x;
		int32_t _y;
	};

	class FocusItem {
	public:
		FocusItem() {
			margin = 5;
			_backArea = NULL;
			_focusArea = NULL;
			_tapable	 = NULL;
		}

		FocusItem(Drawable & backArea, Drawable & focusArea, Drawable &tapArea, uint16_t mar = 5)
		{
			_backArea = &backArea;
			_focusArea = &focusArea;
			_tapable = &tapArea;

			margin = mar;
			_rect.expandToFit(focusArea.getAbsoluteRect());
			_rect.x 			= _rect.x - margin;
			_rect.y 			= _rect.y - margin;
			_rect.width		= _rect.width + (margin * 2);
			_rect.height 	= _rect.height + (margin * 2);

			_point.setTap(tapArea.getAbsoluteRect());
		}

    void operator&=(const FocusItem& other)
    {
    		_rect = other._rect;
    		_point = other._point;
    }

		uint16_t getX() {
			return _rect.x;
		}

		uint16_t getY() {
			return _rect.y;
		}

		uint16_t getWidth() {
			return _rect.width;
		}

		uint16_t getHeight() {
			return _rect.height;
		}

		void setMargin(uint16_t value) {
			margin = value;
		}

		void invalidate() {
			if (_backArea != NULL) _backArea->invalidate();
		}

		void tap();

		TouchPoint&	point() {
			return _point;
		}

	private:
		Rect				_rect;
		TouchPoint	_point;
		uint16_t		margin;
		Drawable *	_backArea;
		Drawable *	_focusArea;
		Drawable *	_tapable;

	};

	class FocusBox
	{
	public:
		FocusBox()
    {
			boxWithBorder = NULL;
			currentItem = 0;
    }

		virtual void 	handleTickEvent();
		void attachFocusBox(BoxWithBorder & box) {
			boxWithBorder = &box;
		}

		void attachFocusable(Drawable & backArea, Drawable & focusArea, Drawable &tapArea);
		void processKey(uint8_t key);
		void setFocus(uint8_t index) {
			if(boxWithBorder == NULL)
				return;

			FocusItem item1 = focusVector[index];
			boxWithBorder->setPosition(item1.getX(), item1.getY(), item1.getWidth(), item1.getHeight());
			boxWithBorder->setVisible(true);
			item1.invalidate();
			currentItem = index;
		}

		int16_t getNextFocusItem(int16_t currentItem, nav_t dir) {
			// if there's only one item, return it
			for(int16_t i=0; i < focusFine.size(); i++) {
				focusFine[i].reset();
			}
			focusFine.clear();


			if(focusVector.size() == 1) return 0;

			FocusItem& current = focusVector[currentItem];

			for(int16_t idx=0; idx < focusVector.size(); idx++) {
				float angle 		= current.point().angle(focusVector[idx].point());
				float distance 	= current.point().distance(focusVector[idx].point());

				// if there are only 2 items then return the other one
				if(angle == 0.0 && distance == 0.0 && focusVector.size() == 2) {
					return (idx == 0) ? 1: 0;
				}

				if( angle == 0.0 && distance == 0.0) continue;

				// -45 to 0 and 45 to 0
				if(dir == NAV_RIGHT && angle >-45.0 && angle < 45.0) {
					FocusFine finer(idx, angle, distance);
					focusFine.add(finer);
					continue;
				}
				// -135 to -44.999999
				if(dir == NAV_UP && angle >-135.0 && angle < -44.99999) {
					FocusFine fineu(idx, angle, distance);
					focusFine.add(fineu);
					continue;
				}

				// -135 to -179.9999 and 135 to 180
				if((dir == NAV_LEFT) && ((angle > 135.0) || (angle < -135.00001))) {
					FocusFine finel(idx, angle, distance);
					focusFine.add(finel);
					continue;
				}

				// 135 to 45
				if(dir == NAV_DOWN && angle > 45.0 && angle < 135.00001) {
					FocusFine fined(idx, angle, distance);
					focusFine.add(fined);
					continue;
				}
			}

			int16_t closest, selected = -1;
			for(int16_t idx2=0; idx2 < focusFine.size(); idx2++) {
				if(idx2==0) {
					selected = focusFine[idx2].itemIndex;
					closest = idx2;
				}

				if(focusFine[idx2].distance < focusFine[closest].distance) {
					selected = focusFine[idx2].itemIndex;
					closest = idx2;
				}
			}

			return selected;
		}

	private:

		nav_t					nav_button;
		BoxWithBorder *boxWithBorder;
		FocusVector_t	focusVector;
		uint8_t  			currentItem;
		FocusFine_t		focusFine;
	};

} /* namespace touchgfx */

#endif /* TARGET_FOCUSCONTROLLER_H_ */
