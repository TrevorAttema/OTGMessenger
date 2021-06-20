/*
 * main.hpp
 *
 *  Created on: 12 Jun 2021
 *      Author: Trevor Attema
 */

#ifndef INC_MAIN2_H_
#define INC_MAIN2_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void gfx_thread();
void post_keypress(uint8_t key);

#ifdef __cplusplus
}
#endif

#endif /* INC_MAIN2_H_ */
