#include <gui/setup_screen_screen/setup_screenView.hpp>

setup_screenView::setup_screenView()
{
	focus.attachFocusBox(focusBox);
	focus.attachFocusable(background, container1, flexButton1);
	focus.attachFocusable(background, container2, flexButton2);
	focus.attachFocusable(background, container3, flexButton3);
	focus.attachFocusable(background, container4, flexButton4);
	focus.attachFocusable(background, container5, flexButton5);
	focus.attachFocusable(background, container6, flexButton6);
	focus.attachFocusable(background, container7, flexButton7);
	focus.attachFocusable(background, container8, flexButton8);
	focus.attachFocusable(background, container9, flexButton9);
	focus.attachFocusable(background, container10, flexButton10);
}

void setup_screenView::setupScreen()
{
    setup_screenViewBase::setupScreen();
}

void setup_screenView::tearDownScreen()
{
    setup_screenViewBase::tearDownScreen();
}

void setup_screenView::handleKeyEvent(uint8_t key)
{
	focus.processKey(key);
	setup_screenViewBase::handleKeyEvent(key);
}
