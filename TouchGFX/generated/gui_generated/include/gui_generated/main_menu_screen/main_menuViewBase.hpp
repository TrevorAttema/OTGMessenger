/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef MAIN_MENUVIEWBASE_HPP
#define MAIN_MENUVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/main_menu_screen/main_menuPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <gui/containers/title_container.hpp>
#include <touchgfx/widgets/BoxWithBorder.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/containers/buttons/Buttons.hpp>

class main_menuViewBase : public touchgfx::View<main_menuPresenter>
{
public:
    main_menuViewBase();
    virtual ~main_menuViewBase() {}
    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    title_container title_container1;
    touchgfx::Box background;
    touchgfx::BoxWithBorder focusBox;
    touchgfx::Container chat_container;
    touchgfx::TextArea text_chat;
    touchgfx::ImageButtonStyle< touchgfx::ClickButtonTrigger > chat_button;
    touchgfx::Container settings_container;
    touchgfx::TextArea text_settings;
    touchgfx::ImageButtonStyle< touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger > > settings_button;
    touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger > left_button;
    touchgfx::Container left_container;
    touchgfx::Container right_container;
    touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger > right_button;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<main_menuViewBase, const touchgfx::AbstractButtonContainer&> flexButtonCallback;

    /*
     * Callback Handler Declarations
     */
    void flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src);

};

#endif // MAIN_MENUVIEWBASE_HPP
