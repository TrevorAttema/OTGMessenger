/*
 * KeypadController.h
 *
 *  Created on: 14 Jun 2021
 *      Author: Trevor Attema
 */

#ifndef TARGET_KEYPADCONTROLLER_H_
#define TARGET_KEYPADCONTROLLER_H_

#include <platform/driver/button/ButtonController.hpp>

namespace touchgfx
{

	class I2CKeypadController : public touchgfx::ButtonController
	{
	public:
    virtual void init() {  }
    virtual bool sample(uint8_t& key);

	};

} /* namespace touchgfx */

#endif /* TARGET_KEYPADCONTROLLER_H_ */
