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

/**
 * @file touchgfx/widgets/Box.hpp
 *
 * Declares the touchgfx::Box class.
 */
#ifndef BOX_HPP
#define BOX_HPP

#include <touchgfx/Bitmap.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/widgets/Widget.hpp>

#include <touchgfx/widgets/Button.hpp>

namespace touchgfx
{
/**
 * Simple widget capable of showing a rectangle of a specific color and an optional alpha.
 */
class Box : public Widget
{
public:
    /** Construct a new Box with a default alpha value of 255 (solid) */
    Box()
        : Widget(), alpha(255), color(0)
    {
    }

    /**
     * Construct a Box with the given size and color (and optionally alpha).
     *
     * @param  width  The width of the box.
     * @param  height The height of the box.
     * @param  color  The color of the box.
     * @param  alpha  (Optional) The alpha of the box. Default is 255 (solid).
     */
    Box(uint16_t width, uint16_t height, colortype color, uint8_t alpha = 255)
        : Widget(),
          alpha(alpha), color(color)
    {
        rect.width = width;
        rect.height = height;
    }

    virtual Rect getSolidRect() const;

    /**
     * Sets the color of the Box.
     *
     * @param  color The color of the box.
     *
     * @see getColor, Color::getColorFrom24BitRGB
     */
    void setColor(colortype color)
    {
        this->color = color;
    }

    /**
     * Gets the current color of the Box.
     *
     * @return The current color of the box.
     *
     * @see setColor, Color::getRedColor, Color::getGreenColor, Color::getRedColor
     */
    FORCE_INLINE_FUNCTION colortype getColor() const
    {
        return color;
    }

    /**
     * @copydoc Image::setAlpha
     */
    void setAlpha(uint8_t newAlpha)
    {
        alpha = newAlpha;
    }

    /**
     * @copydoc Image::getAlpha
     */
    FORCE_INLINE_FUNCTION uint8_t getAlpha() const
    {
        return alpha;
    }

    virtual void draw(const Rect& area) const;

protected:
    uint8_t alpha;   ///< The alpha value used for this Box.
    colortype color; ///< The fill color for this Box
};

} // namespace touchgfx

#endif // BOX_HPP
