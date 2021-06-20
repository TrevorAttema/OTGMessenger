#ifndef TITLE_CONTAINER_HPP
#define TITLE_CONTAINER_HPP

#include <gui_generated/containers/title_containerBase.hpp>

class title_container : public title_containerBase
{
public:
    title_container();
    virtual ~title_container() {}

    virtual void initialize();
protected:
};

#endif // TITLE_CONTAINER_HPP
