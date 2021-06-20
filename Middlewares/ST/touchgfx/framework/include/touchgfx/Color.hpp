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
 * @file touchgfx/Color.hpp
 *
 * Declares the touchgfx::Color class
 */
#ifndef COLOR_HPP
#define COLOR_HPP

#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/lcd/LCD.hpp>

namespace touchgfx
{
/** Contains functionality for color conversion. */
class Color
{
public:
    /**
     * Generates a color representation to be used on the LCD, based on 24 bit RGB values.
     * Depending on your display color bit depth, the color might be interpreted internally
     * as fewer than 24 bits with a loss of color precision.
     *
     * @param  red   Value of the red part (0-255).
     * @param  green Value of the green part (0-255).
     * @param  blue  Value of the blue part (0-255).
     *
     * @return The color representation depending on LCD color format.
     *
     * @see LCD::getColorFrom24BitRGB, LCD16bpp::getColorFromRGB
     *
     * @note This function is not available to call before the LCD has been setup, because the
     *       color depth is required. Consider using the function getColorFromRGB for a
     *       specific class, e.g. LCD16::getColorFromRGB().
     */
    static colortype getColorFrom24BitRGB(uint8_t red, uint8_t green, uint8_t blue)
    {
        assert(HAL::getInstance() && "Cannot set color before HAL is initialized");
        return HAL::lcd().getColorFrom24BitRGB(red, green, blue);
    }

    /**
     * Gets the red color part of a color. As this function must work for all color depths,
     * it can be somewhat slow if used in speed critical sections. Consider finding the
     * color in another way, if possible. If the color depth of the display is known,
     * consider using function getRedFromColor() from the current LCD.
     *
     * @param  color The color value.
     *
     * @return The red part of the color.
     *
     * @see LCD16bpp::getRedFromColor
     */
    FORCE_INLINE_FUNCTION static uint8_t getRedColor(colortype color)
    {
        return HAL::lcd().getRedColor(color);
    }

    /**
     * Gets the green color part of a color. As this function must work for all color depths,
     * it can be somewhat slow if used in speed critical sections. Consider finding the
     * color in another way, if possible. If the color depth of the display is known,
     * consider using function getGreenFromColor() from the current LCD.
     *
     * @param  color The color value.
     *
     * @return The green part of the color.
     *
     * @see LCD16bpp::getGreenFromColor
     */
    FORCE_INLINE_FUNCTION static uint8_t getGreenColor(colortype color)
    {
        return HAL::lcd().getGreenColor(color);
    }

    /**
     * Gets the blue color part of a color. As this function must work for all color depths,
     * it can be somewhat slow if used in speed critical sections. Consider finding the
     * color in another way, if possible. If the color depth of the display is known,
     * consider using function getBlueFromColor() from the current LCD.
     *
     * @param  color The color value.
     *
     * @return The blue part of the color.
     *
     * @see LCD16bpp::getBlueFromColor
     */
    FORCE_INLINE_FUNCTION static uint8_t getBlueColor(colortype color)
    {
        return HAL::lcd().getBlueColor(color);
    }

    /**
     * Convert a given color from HSV (Hue, Saturation, Value) to RGB (Red, Green, Blue).
     *
     * @param       hue        The input Hue (0 to 255).
     * @param       saturation The input Saturation (0 to 255).
     * @param       value      The input Value (0 to 255).
     * @param [out] red        The output Red (0 to 255).
     * @param [out] green      The output Green (0 to 255).
     * @param [out] blue       The output Blue (0 to 255).
     *
     * @note The conversion is an approximation.
     */
    static void getRGBFrom24BitHSV(uint8_t hue, uint8_t saturation, uint8_t value, uint8_t& red, uint8_t& green, uint8_t& blue)
    {
        if (saturation == 0)
        {
            red = green = blue = value;
            return;
        }

        const uint8_t region = (hue * 6) >> 8;
        const int next_region_start = ((((region + 1) << 8) + 5) / 6); // Can go up to 256, uint8_t not enough
        const uint8_t region_size = next_region_start - (((region << 8) + 5) / 6);
        const uint8_t remainder = 255 - (next_region_start - hue) * 255 / region_size;

        const uint8_t p = touchgfx::LCD::div255(value * (255 - saturation));
        const uint8_t q = touchgfx::LCD::div255(value * (255 - touchgfx::LCD::div255(saturation * remainder)));
        const uint8_t t = touchgfx::LCD::div255(value * (255 - touchgfx::LCD::div255(saturation * (255 - remainder))));

        switch (region)
        {
        case 0:
            red = value, green = t, blue = p;
            break;
        case 1:
            red = q, green = value, blue = p;
            break;
        case 2:
            red = p, green = value, blue = t;
            break;
        case 3:
            red = p, green = q, blue = value;
            break;
        case 4:
            red = t, green = p, blue = value;
            break;
        default:
            red = value, green = p, blue = q;
            break;
        }
    }

    /**
     * Convert a given color from RGB (Red, Green, Blue) to HSV (Hue, Saturation, Value).
     *
     * @param       red        The input Red.
     * @param       green      The input Green.
     * @param       blue       The input Blue.
     * @param [out] hue        The output Hue.
     * @param [out] saturation The output Saturation.
     * @param [out] value      The output Value.
     *
     * @note The conversion is an approximation.
     */
    static void getHSVFrom24BitRGB(uint8_t red, uint8_t green, uint8_t blue, uint8_t& hue, uint8_t& saturation, uint8_t& value)
    {
        const uint8_t rgbMin = MIN(MIN(red, green), blue);
        const uint8_t rgbMax = MAX(MAX(red, green), blue);
        const uint8_t rgbRange = rgbMax - rgbMin;

        value = rgbMax;
        if (value == 0)
        {
            hue = 0;
            saturation = 0;
            return;
        }

        saturation = 255 * rgbRange / value;
        if (saturation == 0)
        {
            hue = 0;
        }
        else if (rgbMax == red)
        {
            if (green < blue)
            {
                hue = 0 + (42 * (green - blue) + rgbRange / 2) / rgbRange; // [0-42; 0] = [214; 0]
            }
            else
            {
                hue = 0 + (43 * (green - blue) + rgbRange / 2) / rgbRange; // [0; 0+43] = [0; 43]
            }
        }
        else if (rgbMax == green)
        {
            if (blue < red)
            {
                hue = 86 + (43 * (blue - red) + rgbRange / 2) / rgbRange; // [86-43; 86] = [43; 86]
            }
            else
            {
                hue = 86 + (42 * (blue - red) + rgbRange / 2) / rgbRange; // [86; 86+42] = [86; 128]
            }
        }
        else
        {
            hue = 171 + (43 * (red - green) + rgbRange / 2) / rgbRange; // [171-43; 171+43] = [128; 214]
        }
    }

    /**
     * Convert a given color from HSV (Hue, Saturation, Value) to colortype.
     *
     * @param       hue        The input Hue (0 to 255).
     * @param       saturation The input Saturation (0 to 255).
     * @param       value      The input Value (0 to 255).
     *
     * @return The colortype color.
     * @note The conversion is an approximation.
     */
    static colortype getColorFrom24BitHSV(uint8_t hue, uint8_t saturation, uint8_t value)
    {
        uint8_t red, green, blue;
        getRGBFrom24BitHSV(hue, saturation, value, red, green, blue);
        return Color::getColorFrom24BitRGB(red, green, blue);
    }

    /**
     * Convert a given colortype color to HSV (Hue, Saturation, Value).
     *
     * @param       color      The input color.
     * @param [out] hue        The output Hue (0 to 255).
     * @param [out] saturation The output Saturation (0 to 255).
     * @param [out] value      The output Value (0 to 255).
     *
     * @note The conversion is an approximation.
     */
    static void getHSVFromColor(colortype color, uint8_t& hue, uint8_t& saturation, uint8_t& value)
    {
        getHSVFrom24BitRGB(Color::getRedColor(color), Color::getGreenColor(color), Color::getBlueColor(color), hue, saturation, value);
    }

    /**
     * Convert HSL (Hue, Saturation, Luminance) to HSV (Hue, Saturation, Value). The Hue is unaltered,
     * the Saturation is changed and the Value is calculated.
     *
     * @param          hue        The hue (0 to 255).
     * @param [in,out] saturation The saturation (0 to 255).
     * @param          luminance  The luminance (0 to 255).
     * @param [out]    value      The value (0 to 255).
     */
    static void getHSVFromHSL(uint8_t hue, uint8_t& saturation, uint8_t luminance, uint8_t& value)
    {
        value = luminance + LCD::div255(saturation * MIN(luminance, 255 - luminance));
        saturation = value == 0 ? 0 : 2 * (255 - LCD::div255((luminance * 255) / value));
    }

    /**
     * Convert HSV (Hue, Saturation, Value) to HSL (Hue, Saturation, Luminance). The Hue is unaltered,
     * the Saturation is changed and the Luminance is calculated.
     *
     * @param          hue        The hue (0 to 255).
     * @param [in,out] saturation The saturation (0 to 255).
     * @param          value      The value (0 to 255).
     * @param [out]    luminance  The luminance (0 to 255).
     */
    static void getHSLFromHSV(uint8_t hue, uint8_t& saturation, uint8_t value, uint8_t& luminance)
    {
        luminance = LCD::div255(value * (255 - saturation / 2));
        saturation = luminance == 0 || luminance == 255 ? 0 : (((value - luminance) * 255) / MIN(luminance, 255 - luminance));
    }

    /**
     * Convert a given color from HSV (Hue, Saturation, Value) to RGB (Red, Green, Blue).
     *
     * @param       hue        The input Hue (0 to 255).
     * @param       saturation The input Saturation (0 to 255).
     * @param       luminance  The input Value (0 to 255).
     * @param [out] red        The output Red (0 to 255).
     * @param [out] green      The output Green (0 to 255).
     * @param [out] blue       The output Blue (0 to 255).
     *
     * @note The conversion is an approximation.
     */
    static void getRGBFrom24BitHSL(uint8_t hue, uint8_t saturation, uint8_t luminance, uint8_t& red, uint8_t& green, uint8_t& blue)
    {
        uint8_t value;
        getHSVFromHSL(hue, saturation, luminance, value);
        getRGBFrom24BitHSV(hue, saturation, value, red, green, blue);
    }

    /**
     * Convert a given color from RGB (Red, Green, Blue) to HSV (Hue, Saturation, Value).
     *
     * @param       red        The input Red (0 to 255).
     * @param       green      The input Green (0 to 255).
     * @param       blue       The input Blue (0 to 255).
     * @param [out] hue        The output Hue (0 to 255).
     * @param [out] saturation The output Saturation (0 to 255).
     * @param [out] luminance  The output Value (0 to 255).
     *
     * @note The conversion is an approximation.
     */
    static void getHSLFrom24BitRGB(uint8_t red, uint8_t green, uint8_t blue, uint8_t& hue, uint8_t& saturation, uint8_t& luminance)
    {
        uint8_t value;
        getHSVFrom24BitRGB(red, green, blue, hue, saturation, value);
        getHSLFromHSV(hue, saturation, value, luminance);
    }

    /**
     * Convert a given color from HSV (Hue, Saturation, Value) to colortype.
     *
     * @param  hue        The input Hue (0 to 255).
     * @param  saturation The input Saturation (0 to 255).
     * @param  luminance  The input Value (0 to 255).
     *
     * @return The colortype color.
     *
     * @note The conversion is an approximation.
     */
    static colortype getColorFrom24BitHSL(uint8_t hue, uint8_t saturation, uint8_t luminance)
    {
        uint8_t red, green, blue;
        getRGBFrom24BitHSL(hue, saturation, luminance, red, green, blue);
        return Color::getColorFrom24BitRGB(red, green, blue);
    }

    /**
     * Convert a given colortype color to HSV (Hue, Saturation, Value).
     *
     * @param       color      The input color.
     * @param [out] hue        The output Hue (0 to 255).
     * @param [out] saturation The output Saturation (0 to 255).
     * @param [out] luminance  The output Value (0 to 255).
     *
     * @note The conversion is an approximation.
     */
    static void getHSLFromColor(colortype color, uint8_t& hue, uint8_t& saturation, uint8_t& luminance)
    {
        getHSLFrom24BitRGB(Color::getRedColor(color), Color::getGreenColor(color), Color::getBlueColor(color), hue, saturation, luminance);
    }
};

} // namespace touchgfx

#endif // COLOR_HPP
