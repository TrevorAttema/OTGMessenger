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

#include <touchgfx/widgets/Gauge.hpp>

namespace touchgfx
{
Gauge::Gauge()
    : AbstractProgressIndicator(),
      algorithmMoving(TextureMapper::BILINEAR_INTERPOLATION),
      algorithmSteady(TextureMapper::BILINEAR_INTERPOLATION),
      needleStartAngle(0),
      needleEndAngle(0),
      gaugeCenterX(0),
      gaugeCenterY(0),
      needleCenterX(0),
      needleCenterY(0)
{
    remove(progressIndicatorContainer);
    add(arc);
    add(needle);
    arc.setVisible(false);
}

void Gauge::setWidth(int16_t width)
{
    AbstractProgressIndicator::setWidth(width);
    needle.setWidth(width);
    arc.setWidth(width);
}

void Gauge::setHeight(int16_t height)
{
    AbstractProgressIndicator::setHeight(height);
    needle.setHeight(height);
    arc.setHeight(height);
}

void Gauge::setBackgroundOffset(int16_t offsetX, int16_t offsetY)
{
    background.setXY(offsetX, offsetY);
}

void Gauge::setCenter(int x, int y)
{
    gaugeCenterX = x;
    gaugeCenterY = y;
    setupNeedleTextureMapper();
    arc.setPixelCenter(x - arc.getX(), y - arc.getY());
}

void Gauge::setArcPosition(int16_t x, int16_t y, int16_t width, int16_t height)
{
    arc.setPosition(x, y, width, height);
    arc.setPixelCenter(gaugeCenterX - x, gaugeCenterY - y);
}

void Gauge::setNeedle(const BitmapId bitmapId, int16_t rotationCenterX, int16_t rotationCenterY)
{
    needle.setBitmap(Bitmap(bitmapId));
    needleCenterX = rotationCenterX;
    needleCenterY = rotationCenterY;
    setupNeedleTextureMapper();
}

void Gauge::setMovingNeedleRenderingAlgorithm(TextureMapper::RenderingAlgorithm algorithm)
{
    algorithmMoving = algorithm;
}

void Gauge::setSteadyNeedleRenderingAlgorithm(TextureMapper::RenderingAlgorithm algorithm)
{
    algorithmSteady = algorithm;
}

void Gauge::setStartEndAngle(int startAngle, int endAngle)
{
    assert(startAngle != endAngle);
    needleStartAngle = startAngle;
    needleEndAngle = endAngle;
    arc.setArc(startAngle, endAngle);
    Gauge::setValue(Gauge::getValue());
}

int Gauge::getStartAngle() const
{
    return needleStartAngle;
}

int Gauge::getEndAngle() const
{
    return needleEndAngle;
}

void Gauge::setArcVisible(bool show /*= true*/)
{
    arc.setVisible(show);
}

void Gauge::putArcOnTop(bool onTop /*= true*/)
{
    if (onTop)
    {
        remove(arc);
        add(arc);
    }
    else
    {
        remove(needle);
        add(needle);
    }
}

touchgfx::Circle& Gauge::getArc()
{
    return arc;
}

void Gauge::setValue(int value)
{
    AbstractProgressIndicator::setValue(value);
    if (animationStep >= animationDuration)
    {
        needle.setRenderingAlgorithm(algorithmSteady);
    }
    else
    {
        needle.setRenderingAlgorithm(algorithmMoving);
    }
    uint16_t progress = AbstractProgressIndicator::getProgress(abs(needleEndAngle - needleStartAngle));
    if (needleEndAngle < needleStartAngle)
    {
        needle.updateZAngle((needleStartAngle - progress) / 180.0f * PI);
        arc.updateArcEnd(needleStartAngle - progress);
    }
    else
    {
        needle.updateZAngle((needleStartAngle + progress) / 180.0f * PI);
        arc.updateArcEnd(needleStartAngle + progress);
    }
}

void Gauge::setAlpha(uint8_t newAlpha)
{
    background.setAlpha(newAlpha);
    needle.setAlpha(newAlpha);
    arc.setAlpha(newAlpha);
}

uint8_t Gauge::getAlpha() const
{
    return needle.getAlpha();
}

void Gauge::setupNeedleTextureMapper()
{
    needle.setWidthHeight(*this);
    needle.setXY(0, 0);
    needle.setBitmapPosition(gaugeCenterX - needleCenterX, gaugeCenterY - needleCenterY);
    needle.setCameraDistance(300.0f);
    needle.setOrigo((float)gaugeCenterX, (float)gaugeCenterY, needle.getCameraDistance());
    needle.setCamera(needle.getOrigoX(), needle.getOrigoY());
    needle.setRenderingAlgorithm(TextureMapper::BILINEAR_INTERPOLATION);
}


void Gauge::setProgressIndicatorPosition(int16_t /*x*/, int16_t /*y*/, int16_t /*width*/, int16_t /*height*/)
{
}

} // namespace touchgfx
