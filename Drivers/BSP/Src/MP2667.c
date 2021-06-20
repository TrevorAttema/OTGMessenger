/*
 * MP2667.c
 *
 *  Created on: 21 May 2021
 *      Author: Trevor Attema
 */


#include <i2c.h>

#include <MP2667.h>

HAL_StatusTypeDef MP2667_ReadReg(uint8_t reg, uint8_t * value) {

	HAL_StatusTypeDef status;
	status = HAL_I2C_Mem_Read(&hi2c1, MP2667_I2C, reg, I2C_MEMADD_SIZE_8BIT, value, 1, 10000);

	return status;
}

HAL_StatusTypeDef MP2667_Read_Src_Ctl_Reg(uint8_t * value) {

	return MP2667_ReadReg(MP2667_REG_SCR, value);
}

HAL_StatusTypeDef MP2667_Read_Poc_Ctl_Reg(uint8_t * value) {
	return MP2667_ReadReg(MP2667_REG_POC, value);
}

HAL_StatusTypeDef MP2667_Read_Ccr_Ctl_Reg(uint8_t * value){
	return MP2667_ReadReg(MP2667_REG_CCR, value);
}

HAL_StatusTypeDef MP2667_Read_Dtc_Ctl_Reg(uint8_t * value){
	return MP2667_ReadReg(MP2667_REG_DTC, value);
}

HAL_StatusTypeDef MP2667_Read_Cgc_Ctl_Reg(uint8_t * value){
	return MP2667_ReadReg(MP2667_REG_CGC, value);
}

HAL_StatusTypeDef MP2667_Read_Tcr_Ctl_Reg(uint8_t * value){
	return MP2667_ReadReg(MP2667_REG_TCR, value);
}

HAL_StatusTypeDef MP2667_Read_Mis_Ctl_Reg(uint8_t * value){
	return MP2667_ReadReg(MP2667_REG_MIS, value);
}

HAL_StatusTypeDef MP2667_Read_Sta_Ctl_Reg(uint8_t * value){
	return MP2667_ReadReg(MP2667_REG_STA, value);
}

HAL_StatusTypeDef MP2667_Read_Err_Ctl_Reg(uint8_t * value) {
	return MP2667_ReadReg(MP2667_REG_ERR, value);
}
