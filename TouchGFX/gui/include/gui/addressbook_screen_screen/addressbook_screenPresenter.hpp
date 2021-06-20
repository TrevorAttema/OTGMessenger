#ifndef ADDRESSBOOK_SCREENPRESENTER_HPP
#define ADDRESSBOOK_SCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class addressbook_screenView;

class addressbook_screenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    addressbook_screenPresenter(addressbook_screenView& v);

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

    virtual ~addressbook_screenPresenter() {};

private:
    addressbook_screenPresenter();

    addressbook_screenView& view;
};

#endif // ADDRESSBOOK_SCREENPRESENTER_HPP
