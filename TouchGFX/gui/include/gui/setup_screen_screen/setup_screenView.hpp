#ifndef SETUP_SCREENVIEW_HPP
#define SETUP_SCREENVIEW_HPP

#include <gui_generated/setup_screen_screen/setup_screenViewBase.hpp>
#include <gui/setup_screen_screen/setup_screenPresenter.hpp>
#include <FocusController.h>

class setup_screenView : public setup_screenViewBase
{
public:
    setup_screenView();
    virtual ~setup_screenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

protected:
    virtual void handleKeyEvent(uint8_t key);

private:
    FocusBox		focus;
};

#endif // SETUP_SCREENVIEW_HPP
