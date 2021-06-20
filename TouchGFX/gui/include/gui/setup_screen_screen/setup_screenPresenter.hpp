#ifndef SETUP_SCREENPRESENTER_HPP
#define SETUP_SCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class setup_screenView;

class setup_screenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    setup_screenPresenter(setup_screenView& v);

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

    virtual ~setup_screenPresenter() {};

private:
    setup_screenPresenter();

    setup_screenView& view;
};

#endif // SETUP_SCREENPRESENTER_HPP
