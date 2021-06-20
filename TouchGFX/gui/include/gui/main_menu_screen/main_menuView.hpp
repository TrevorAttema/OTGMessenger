#ifndef MAIN_MENUVIEW_HPP
#define MAIN_MENUVIEW_HPP

#include <gui_generated/main_menu_screen/main_menuViewBase.hpp>
#include <gui/main_menu_screen/main_menuPresenter.hpp>
#include <FocusController.h>

class main_menuView : public main_menuViewBase
{
public:
    main_menuView();
    virtual ~main_menuView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

protected:
    virtual void handleKeyEvent(uint8_t key);

private:
    FocusBox		focus;
};

#endif // MAIN_MENUVIEW_HPP
