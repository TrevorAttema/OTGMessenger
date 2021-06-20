#ifndef SPASH_SCREENPRESENTER_HPP
#define SPASH_SCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class spash_screenView;

class spash_screenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    spash_screenPresenter(spash_screenView& v);

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

    virtual ~spash_screenPresenter() {};

private:
    spash_screenPresenter();

    spash_screenView& view;
};

#endif // SPASH_SCREENPRESENTER_HPP
