/**
  ******************************************************************************
  * File Name          : TouchGFXGeneratedHAL.cpp
  ******************************************************************************
  * @attention
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

#include <TouchGFXGeneratedHAL.hpp>
#include <touchgfx/hal/OSWrappers.hpp>
#include <gui/common/FrontendHeap.hpp>

#include "stm32h7xx.h"

using namespace touchgfx;

/* ******************************************************
 * Functions required by Partial Frame Buffer Strategy
 * ******************************************************
 *
 *  int touchgfxDisplayDriverTransmitActive() must return whether or not data is currently being transmitted, over e.g. SPI.
 *  void touchgfxDisplayDriverTransmitBlock(const uint8_t* pixels, uint16_t x, uint16_t y, uint16_t w, uint16_t h) will be called
 *  when the framework wants to send a block. The user must then transfer the data represented by the arguments.
 *
 *  A user must call touchgfx::startNewTransfer(); once touchgfxDisplayDriverTransmitBlock() has successfully sent a block.
 *  E.g. if using DMA to transfer the block, this could be called in the "Transfer Completed" interrupt handler.
 *
 */

extern "C" int touchgfxDisplayDriverTransmitActive();
extern "C" void touchgfxDisplayDriverTransmitBlock(const uint8_t* pixels, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
extern "C" void touchgfxSignalVSync(void);

// Block Allocator for Partial Framebuffer strategy
static ManyBlockAllocator<11520, /* block size */
                   2, /* number of blocks */
                   3  /* bytes per pixel */
                   > blockAllocator;

void TouchGFXGeneratedHAL::initialize()
{
    HAL::initialize();

    registerEventListener(*(Application::getInstance()));
    // Partial framebuffer strategy
    setFrameBufferAllocator(&blockAllocator);
    setFrameRefreshStrategy(HAL::REFRESH_STRATEGY_PARTIAL_FRAMEBUFFER);
}

void TouchGFXGeneratedHAL::configureInterrupts()
{
}

void TouchGFXGeneratedHAL::enableInterrupts()
{
}

void TouchGFXGeneratedHAL::disableInterrupts()
{
}

void TouchGFXGeneratedHAL::enableLCDControllerInterrupt()
{
}

bool TouchGFXGeneratedHAL::beginFrame()
{
    return HAL::beginFrame();
}

void TouchGFXGeneratedHAL::endFrame()
{
  // We must guard the next frame until we're done transferring all blocks over our display interface
  // through either a semaphore if user is running an OS or a simple variable if not
  // Wait until all blocks have been transmitted
  while(touchgfxDisplayDriverTransmitActive()){}

  HAL::endFrame();
  touchgfx::OSWrappers::signalRenderingDone();
}

uint16_t* TouchGFXGeneratedHAL::getTFTFrameBuffer() const
{
    //getTFTFrameBuffer() not used for selected Frame Buffer Strategy
    return 0;
}

void TouchGFXGeneratedHAL::setTFTFrameBuffer(uint16_t* adr)
{
    //setTFTFrameBuffer() not used for selected display interface
}

void TouchGFXGeneratedHAL::flushFrameBuffer(const touchgfx::Rect& rect)
{
  HAL::flushFrameBuffer(rect);
  // Once flushFrameBuffer() is called by the framework a block is already for transfer
  // Mark it ready for transfer and transmit it if user defined method isTransmittingData() does not return false
  // If data is not being transmitted, transfer the data with user defined method transmitFrameBufferBlock().
  frameBufferAllocator->markBlockReadyForTransfer();
  if (!touchgfxDisplayDriverTransmitActive())
  {
    touchgfx::Rect r;
    // Get pointer to block buffer and coordinates of the rect
    const uint8_t* pixels = frameBufferAllocator->getBlockForTransfer(r);
    // Start transmission of the block
    touchgfxDisplayDriverTransmitBlock((uint8_t*)pixels, r.x, r.y, r.width, r.height);
  }
// If the framebuffer is placed in Write Through cached memory (e.g. SRAM) then we need
// to flush the Dcache prior to letting DMA2D accessing it. That's done
// using SCB_CleanInvalidateDCache().
SCB_CleanInvalidateDCache();
}

bool TouchGFXGeneratedHAL::blockCopy(void* RESTRICT dest, const void* RESTRICT src, uint32_t numBytes)
{
  return HAL::blockCopy(dest, src, numBytes);
}

void TouchGFXGeneratedHAL::InvalidateCache()
{
// If the framebuffer is placed in Write Through cached memory (e.g. SRAM) then we need
// to flush the Dcache prior to letting DMA2D accessing it. That's done
// using SCB_CleanInvalidateDCache().
SCB_CleanInvalidateDCache();
}

void TouchGFXGeneratedHAL::FlushCache()
{
// If the framebuffer is placed in Write Through cached memory (e.g. SRAM) then we need
// to flush the Dcache prior to letting DMA2D accessing it. That's done
// using SCB_CleanInvalidateDCache().
SCB_CleanInvalidateDCache();
}

/*
 * ******************************************************
 * Required by Partial Frame Buffer Strategy
 * ******************************************************
 */
namespace touchgfx
{
/**
 * This function is called by FrameBufferAllocator if no block is
 * available.
 */
__weak void FrameBufferAllocatorWaitOnTransfer()
{
  /* NOTE: This function should not be modified, when the fuction is needed,
   *       FrameBufferAllocatorWaitOnTransfer should be implemented in the user file
   */
}

/**
 * Called by FrameBufferAllocator when a block is drawn and
 * therefore ready for transfer. The LCD driver should use this
 * method to start a transfer.
 */
__weak void FrameBufferAllocatorSignalBlockDrawn()
{
  /* NOTE: This function should not be modified, when the fuction is needed,
   *       FrameBufferAllocatorSignalBlockDrawn should be implemented in the user file
   */
}

// A user must call touchgfx::startNewTransfer(); once transmitFrameBufferBlock() has successfully sent a block.
void startNewTransfer()
{
  FrameBufferAllocator* fba = HAL::getInstance()->getFrameBufferAllocator();

  // Free the previous transmitted block, marking it ready for rendering
  fba->freeBlockAfterTransfer();
  if (fba->hasBlockReadyForTransfer())
  {
    touchgfx::Rect r;
    // Get pointer to block buffer and coordinates of the rect
    const uint8_t* pixels = fba->getBlockForTransfer(r);
    // Start transmission of the block
    touchgfxDisplayDriverTransmitBlock((uint8_t*)pixels, r.x, r.y, r.width, r.height);
  }
}

extern "C"
void DisplayDriver_TransferCompleteCallback()
{
  // After completed transmission start new transfer if blocks are ready.
  touchgfx::startNewTransfer();
}
}

extern "C"
void touchgfxSignalVSync(void)
{
  /* VSync has occurred, increment TouchGFX engine vsync counter */
  touchgfx::HAL::getInstance()->vSync();

  /* VSync has occurred, signal TouchGFX engine */
  touchgfx::OSWrappers::signalVSync();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
