#include <gui/main_menu_screen/main_menuView.hpp>

main_menuView::main_menuView()
{
	//add(focusBox);
	focus.attachFocusBox(focusBox);
	focus.attachFocusable(background, chat_container, chat_button);
	focus.attachFocusable(background, settings_container, settings_button);
	focus.attachFocusable(background, left_container, left_button);
	focus.attachFocusable(background, right_container, right_button);
}

void main_menuView::setupScreen()
{
	focus.setFocus(2);
  main_menuViewBase::setupScreen();

}

void main_menuView::tearDownScreen()
{

	main_menuViewBase::tearDownScreen();
}

void main_menuView::handleKeyEvent(uint8_t key)
{
	focus.processKey(key);
	main_menuViewBase::handleKeyEvent(key);
}
