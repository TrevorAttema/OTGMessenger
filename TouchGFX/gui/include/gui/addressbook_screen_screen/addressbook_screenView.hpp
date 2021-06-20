#ifndef ADDRESSBOOK_SCREENVIEW_HPP
#define ADDRESSBOOK_SCREENVIEW_HPP

#include <gui_generated/addressbook_screen_screen/addressbook_screenViewBase.hpp>
#include <gui/addressbook_screen_screen/addressbook_screenPresenter.hpp>

class addressbook_screenView : public addressbook_screenViewBase
{
public:
    addressbook_screenView();
    virtual ~addressbook_screenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // ADDRESSBOOK_SCREENVIEW_HPP
