#include <gui/chat_screen_screen/chat_screenView.hpp>

chat_screenView::chat_screenView()
{

}

void chat_screenView::setupScreen()
{
    chat_screenViewBase::setupScreen();
}

void chat_screenView::tearDownScreen()
{
    chat_screenViewBase::tearDownScreen();
}

void chat_screenView::handleTickEvent()
{
	//focus.handleTickEvent();

	static uint32_t count = 0;
	static bool state = false;
	static char caret_on[] = "|";
	static char caret_off[] = "";

	count++;

	if (count % 25 == 0) {
		if(state == false)
		{
			Unicode::snprintf(txtBuffer, 30, "%s", caret_on);
			state = true;
		}
		else
		{
			Unicode::snprintf(txtBuffer, 30, "%s", caret_off);
			state = false;
		}

		txtField.invalidate();
	}
}

void chat_screenView::handleKeyEvent(uint8_t key)
{
	chat_screenViewBase::handleKeyEvent(key);
}
