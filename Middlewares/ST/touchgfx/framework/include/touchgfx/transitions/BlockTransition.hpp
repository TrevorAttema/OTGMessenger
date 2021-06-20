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

#ifndef BLOCKTRANSITION_HPP
#define BLOCKTRANSITION_HPP

#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/transitions/Transition.hpp>
#include <touchgfx/widgets/Widget.hpp>

namespace touchgfx
{
/**
 * A Transition that draws two small blocks in every frame. It is
 * therefore very usefull on MCUs with limited performance.
 */
class BlockTransition : public Transition
{
public:
    /**
     * Initializes a new instance of the BlockTransition class.
     *
     * @param  transitionSteps (Optional) Number of steps in the transition animation.
     */
    BlockTransition()
        : Transition(),
          animationCounter(0)
    {
        //8x6 blocks, with 8 blocks on the longest edge
        if (HAL::DISPLAY_WIDTH > HAL::DISPLAY_HEIGHT)
        {
            blockWidth = (HAL::DISPLAY_WIDTH + 7) / 8;
            blockHeight = (HAL::DISPLAY_HEIGHT + 5) / 6;
            blocksHorizontal = 8;
        }
        else
        {
            blockWidth = (HAL::DISPLAY_WIDTH + 5) / 6;
            blockHeight = (HAL::DISPLAY_HEIGHT + 7) / 8;
            blocksHorizontal = 6;
        }
    }

    /**
     * Handles the tick event when transitioning. It uncovers and
     * invalidates two blocks in every frame, for a total of 24
     * frames.
     */
    virtual void handleTickEvent()
    {
        const int blocks = 48;
        //"random" sequence of blocks to invalidate
        const int indeces[blocks] = { 20, 11, 47, 14, 10, 0, 18, 28, 13, 6, 2, 41,
                                      44, 5, 3, 17, 36, 46, 26, 15, 29, 39, 25, 12,
                                      19, 24, 7, 38, 37, 30, 9, 43, 4, 31, 22, 23,
                                      35, 16, 32, 42, 8, 1, 40, 33, 21, 27, 34, 45
                                    };

        Transition::handleTickEvent();

        if (animationCounter == 0 && HAL::USE_DOUBLE_BUFFERING)
        {
            Application::getInstance()->copyInvalidatedAreasFromTFTToClientBuffer();
        }

        if (animationCounter < blocks)
        {
            int blocks_per_tick = 2;
            while (blocks_per_tick-- > 0 && animationCounter < blocks)
            {
                //Invalidate next block in sequence
                const int index = indeces[animationCounter];

                const int16_t x = (index % blocksHorizontal) * blockWidth;
                const int16_t y = (index / blocksHorizontal) * blockHeight;

                Rect invRect(x, y, blockWidth, blockHeight);
                screenContainer->invalidateRect(invRect);
                animationCounter++;
            }
        }
        else
        {
            done = true;
        }
    }

    virtual void tearDown()
    {
    }

    virtual void init()
    {
        Transition::init();
    }

    /**
     * Block transition does not require an invalidation. Invalidation
     * is handled by the class. Do no invalidation initially.
     */
    virtual void invalidate()
    {
        //nop
    }

private:
    uint16_t blockWidth;
    uint16_t blockHeight;
    uint16_t blocksHorizontal;
    uint8_t animationCounter; ///< Current step in the transition animation.
};

} // namespace touchgfx

#endif // BLOCKTRANSITION_HPP
