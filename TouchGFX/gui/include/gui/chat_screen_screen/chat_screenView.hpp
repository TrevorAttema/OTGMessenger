#ifndef CHAT_SCREENVIEW_HPP
#define CHAT_SCREENVIEW_HPP

#include <gui_generated/chat_screen_screen/chat_screenViewBase.hpp>
#include <gui/chat_screen_screen/chat_screenPresenter.hpp>

class chat_screenView : public chat_screenViewBase
{
public:
    chat_screenView();
    virtual ~chat_screenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
protected:
    Unicode::UnicodeChar txtBuffer[255];

    virtual void handleKeyEvent(uint8_t key);
};

#endif // CHAT_SCREENVIEW_HPP
