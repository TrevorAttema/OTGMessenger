#ifndef SPASH_SCREENVIEW_HPP
#define SPASH_SCREENVIEW_HPP

#include <gui_generated/spash_screen_screen/spash_screenViewBase.hpp>
#include <gui/spash_screen_screen/spash_screenPresenter.hpp>

class spash_screenView : public spash_screenViewBase
{
public:
    spash_screenView();
    virtual ~spash_screenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SPASH_SCREENVIEW_HPP
