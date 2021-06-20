#ifndef NAV_CONTAINER_HPP
#define NAV_CONTAINER_HPP

#include <gui_generated/containers/nav_containerBase.hpp>

class nav_container : public nav_containerBase
{
public:
    nav_container();
    virtual ~nav_container() {}

    virtual void initialize();
protected:
};

#endif // NAV_CONTAINER_HPP
