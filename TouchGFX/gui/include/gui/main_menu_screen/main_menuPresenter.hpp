#ifndef MAIN_MENUPRESENTER_HPP
#define MAIN_MENUPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class main_menuView;

class main_menuPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    main_menuPresenter(main_menuView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~main_menuPresenter() {};

private:
    main_menuPresenter();

    main_menuView& view;
};

#endif // MAIN_MENUPRESENTER_HPP
