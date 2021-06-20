#ifndef CHAT_SCREENPRESENTER_HPP
#define CHAT_SCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class chat_screenView;

class chat_screenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    chat_screenPresenter(chat_screenView& v);

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

    virtual ~chat_screenPresenter() {};

private:
    chat_screenPresenter();

    chat_screenView& view;
};

#endif // CHAT_SCREENPRESENTER_HPP
