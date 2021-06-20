/*
 * MP2667.h
 *
 *  Created on: 21 May 2021
 *      Author: Trevor Attema
 */

#ifndef BSP_INC_MP2667_H_
#define BSP_INC_MP2667_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#define MP2667_I2C			((uint16_t)0x9 << 1)

//#define MP2667_I2C			((uint16_t)0x71<<1)

#define MP2667_REG_TST	((uint16_t)0x0E)

#define MP2667_REG_SCR	((uint16_t)0x0)									// Source Control Register
#define MP2667_REG_POC	((uint16_t)0x1)									// Power On Config Register
#define MP2667_REG_CCR	((uint16_t)0x2)									// Current Control Register
#define MP2667_REG_DTC	((uint16_t)0x3)									// Discharge Termination Current
#define MP2667_REG_CGC	((uint16_t)0x4)									// Charge Control Register
#define MP2667_REG_TCR	((uint16_t)0x5)									// Charge Termination / Timer Control Register
#define MP2667_REG_MIS	((uint16_t)0x6)									// Misc operation control
#define MP2667_REG_STA	((uint16_t)0x7)									// Misc operation control
#define MP2667_REG_ERR	((uint16_t)0x8)									// Misc operation control


HAL_StatusTypeDef MP2667_ReadReg(uint8_t reg, uint8_t * value);
HAL_StatusTypeDef MP2667_Read_Src_Ctl_Reg(uint8_t * value);
HAL_StatusTypeDef MP2667_Read_Poc_Ctl_Reg(uint8_t * value);
HAL_StatusTypeDef MP2667_Read_Ccr_Ctl_Reg(uint8_t * value);
HAL_StatusTypeDef MP2667_Read_Dtc_Ctl_Reg(uint8_t * value);
HAL_StatusTypeDef MP2667_Read_Cgc_Ctl_Reg(uint8_t * value);
HAL_StatusTypeDef MP2667_Read_Tcr_Ctl_Reg(uint8_t * value);
HAL_StatusTypeDef MP2667_Read_Mis_Ctl_Reg(uint8_t * value);
HAL_StatusTypeDef MP2667_Read_Sta_Ctl_Reg(uint8_t * value);
HAL_StatusTypeDef MP2667_Read_Err_Ctl_Reg(uint8_t * value);

#ifdef __cplusplus
}
#endif

#endif /* BSP_INC_MP2667_H_ */
