/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/title_containerBase.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"

title_containerBase::title_containerBase()
{
    setWidth(320);
    setHeight(22);
    box1.setPosition(0, 0, 320, 22);
    box1.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));

    batt_charging.setXY(281, 3);
    batt_charging.setBitmap(touchgfx::Bitmap(BITMAP_BATTERY_CHARGING_ID));

    batt_0.setXY(281, 2);
    batt_0.setBitmap(touchgfx::Bitmap(BITMAP_BATTERY_0_ID));

    batt_2.setXY(281, 2);
    batt_2.setBitmap(touchgfx::Bitmap(BITMAP_BATTERY_2_ID));

    batt_4.setXY(281, 2);
    batt_4.setBitmap(touchgfx::Bitmap(BITMAP_BATTERY_4_ID));

    batt_6.setXY(281, 1);
    batt_6.setBitmap(touchgfx::Bitmap(BITMAP_BATTERY_6_ID));

    batt_8.setXY(281, 2);
    batt_8.setBitmap(touchgfx::Bitmap(BITMAP_BATTERY_8_ID));

    batt_10.setXY(281, 2);
    batt_10.setBitmap(touchgfx::Bitmap(BITMAP_BATTERY_10_ID));

    radio_locked.setXY(9, 1);
    radio_locked.setBitmap(touchgfx::Bitmap(BITMAP_SIGNAL_LOCKED_ID));

    radio_off.setXY(9, 1);
    radio_off.setBitmap(touchgfx::Bitmap(BITMAP_SIGNAL_OFF_ID));

    radio_1.setXY(9, 1);
    radio_1.setBitmap(touchgfx::Bitmap(BITMAP_SIGNAL_1_ID));

    radio_2.setXY(9, 1);
    radio_2.setBitmap(touchgfx::Bitmap(BITMAP_SIGNAL_2_ID));

    radio_3.setXY(9, 1);
    radio_3.setBitmap(touchgfx::Bitmap(BITMAP_SIGNAL_3_ID));

    radio_4.setXY(9, 1);
    radio_4.setBitmap(touchgfx::Bitmap(BITMAP_SIGNAL_4_ID));

    radio_5.setXY(9, 1);
    radio_5.setBitmap(touchgfx::Bitmap(BITMAP_SIGNAL_5_ID));

    add(box1);
    add(batt_charging);
    add(batt_0);
    add(batt_2);
    add(batt_4);
    add(batt_6);
    add(batt_8);
    add(batt_10);
    add(radio_locked);
    add(radio_off);
    add(radio_1);
    add(radio_2);
    add(radio_3);
    add(radio_4);
    add(radio_5);
}

void title_containerBase::initialize()
{

}

