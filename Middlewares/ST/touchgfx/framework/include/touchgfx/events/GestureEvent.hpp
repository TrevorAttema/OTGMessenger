/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.16.1 distribution.
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/**
 * @file touchgfx/events/GestureEvent.hpp
 *
 * Declares the touchgfx::GestureEvent class.
 */
#ifndef GESTUREEVENT_HPP
#define GESTUREEVENT_HPP

#include <touchgfx/Event.hpp>
#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{
/**
 * A gesture event. The only gesture events currently supported is #SWIPE_HORIZONTAL and
 * #SWIPE_VERTICAL, which will be issued every time the input system detects a swipe.
 *
 * @see Event
 */
class GestureEvent : public Event
{
public:
    /** Values that represent gesture types. */
    typedef enum
    {
        SWIPE_HORIZONTAL, ///< An enum constant representing a horizontal swipe
        SWIPE_VERTICAL    ///< An enum constant representing a vertical swipe
    } GestureEventType;

    /**
     * Constructor. Create a gesture event of the specified type with the specified
     * coordinates.
     *
     * @param  type     The type of the gesture event.
     * @param  velocity The velocity of this gesture (swipe)
     * @param  x        The x coordinate of the gesture.
     * @param  y        The y coordinate of the gesture.
     */
    GestureEvent(GestureEventType type, int16_t velocity, int16_t x, int16_t y)
        : gestureEventType(type),
          gestureVelocity(velocity),
          gestureX(x),
          gestureY(y)
    {
    }

    /**
     * Gets the velocity of this gesture event.
     *
     * @return The velocity of this gesture event.
     */
    int16_t getVelocity() const
    {
        return gestureVelocity;
    }

    /**
     * Gets the type of this gesture event.
     *
     * @return The type of this gesture event.
     */
    GestureEventType getType() const
    {
        return gestureEventType;
    }

    /**
     * Gets the x coordinate of this gesture event.
     *
     * @return The x coordinate of this gesture event.
     */
    int16_t getX() const
    {
        return gestureX;
    }

    /**
     * Gets the y coordinate of this gesture event.
     *
     * @return The y coordinate of this gesture event.
     */
    int16_t getY() const
    {
        return gestureY;
    }

    /**
     * Gets event type.
     *
     * @return The type of this event.
     */
    virtual Event::EventType getEventType()
    {
        return Event::EVENT_GESTURE;
    }

private:
    /** Initializes a new instance of the GestureEvent class. */
    GestureEvent()
    {
    }

    GestureEventType gestureEventType;
    int16_t gestureVelocity;
    int16_t gestureX;
    int16_t gestureY;
};

} // namespace touchgfx

#endif // GESTUREEVENT_HPP
