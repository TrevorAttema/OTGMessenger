/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/addressbook_screen_screen/addressbook_screenViewBase.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>

addressbook_screenViewBase::addressbook_screenViewBase()
{

    __background.setPosition(0, 0, 320, 240);
    __background.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));

    box1.setPosition(0, 0, 320, 240);
    box1.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));

    title_container1.setXY(0, 0);

    textArea1.setXY(90, 111);
    textArea1.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    textArea1.setLinespacing(0);
    textArea1.setTypedText(touchgfx::TypedText(T_SINGLEUSEID14));

    add(__background);
    add(box1);
    add(title_container1);
    add(textArea1);
}

void addressbook_screenViewBase::setupScreen()
{
    title_container1.initialize();
}
