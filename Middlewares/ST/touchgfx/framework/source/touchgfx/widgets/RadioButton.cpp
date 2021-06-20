/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.16.1 distribution.
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#include <touchgfx/widgets/RadioButton.hpp>
#include <touchgfx/hal/HAL.hpp>

namespace touchgfx
{
void RadioButton::draw(const Rect& invalidatedArea) const
{
    Bitmap bitmap = getCurrentlyDisplayedBitmap();
    if (bitmap.getId() != BITMAP_INVALID)
    {
        Rect meAbs;
        translateRectToAbsolute(meAbs); //To find our x and y coords in absolute.

        // Calculate intersection between bitmap rect and invalidated area.
        Rect dirtyBitmapArea = bitmap.getRect() & invalidatedArea;

        if (!dirtyBitmapArea.isEmpty())
        {
            HAL::lcd().drawPartialBitmap(bitmap, meAbs.x, meAbs.y, dirtyBitmapArea, alpha);
        }
    }
}

void RadioButton::handleClickEvent(const ClickEvent& event)
{
    bool wasPressed = pressed;
    pressed = (event.getType() == ClickEvent::PRESSED);
    if ((pressed && !wasPressed) || (!pressed && wasPressed))
    {
        invalidate();
    }
    if (wasPressed && (event.getType() == ClickEvent::RELEASED))
    {
        if (deselectionEnabled)
        {
            setSelected(!getSelected());
        }
        else if (!getSelected())
        {
            setSelected(true);
        }
    }
}

void RadioButton::setBitmaps(const Bitmap& bmpUnselected, const Bitmap& bmpUnselectedPressed, const Bitmap& bmpSelected, const Bitmap& bmpSelectedPressed)
{
    bitmapUnselected = bmpUnselected;
    bitmapUnselectedPressed = bmpUnselectedPressed;
    bitmapSelected = bmpSelected;
    bitmapSelectedPressed = bmpSelectedPressed;

    RadioButton::setWidthHeight(bitmapUnselected);
}

Rect RadioButton::getSolidRect() const
{
    if (alpha < 255)
    {
        return Rect(0, 0, 0, 0);
    }

    return getCurrentlyDisplayedBitmap().getSolidRect();
}

void RadioButton::setSelected(bool newSelected)
{
    bool wasSelected = selected;
    selected = newSelected;

    if (wasSelected && !newSelected)
    {
        executeDeselectedAction();
    }

    if (!wasSelected && newSelected)
    {
        executeAction();
    }

    invalidate();
}
} // namespace touchgfx
