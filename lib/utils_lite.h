/*----------------------------------------------------------------------------*/
#ifndef __MY1UTILS_LITE_H__
#define __MY1UTILS_LITE_H__
/*----------------------------------------------------------------------------*/
/**
 * utils_lite.h
 * - lite version of utils
**/
/*----------------------------------------------------------------------------*/
#include "vars_utils.h"
/*----------------------------------------------------------------------------*/
unsigned char uint8_2cstr(char* cstr, unsigned char that) {
	util_size = 0;
	util_temp = that/100;
	if (util_temp) {
		that %= 100;
		cstr[util_size++] = util_temp + 0x30;
	}
	util_temp = that/10;
	if (util_temp) {
		that %= 10;
		cstr[util_size++] = util_temp + 0x30;
	} else if (util_size)
		cstr[util_size++] = 0x30;
	cstr[util_size++] = that + 0x30;
	cstr[util_size] = 0x0;
	return util_size;
}
/*----------------------------------------------------------------------------*/
unsigned char int8_2cstr(char* cstr, signed char that) {
	if (that<0) {
		that = -that;
		*cstr = '-';
		cstr++;
	}
	return uint8_2cstr(cstr,(unsigned char)that);
}
/*----------------------------------------------------------------------------*/
unsigned char uint16_2cstr(char* cstr, unsigned int that) {
	util_size = 0; util_idiv = 10000;
	while (util_idiv>=10) {
		util_temp = (unsigned char)(that/util_idiv);
		if (util_temp) {
			that %= util_idiv;
			cstr[util_size++] = util_temp + 0x30;
		} else if (util_size)
			cstr[util_size++] = 0x30;
		util_idiv /= 10;
	}
	cstr[util_size++] = that + 0x30;
	cstr[util_size] = 0x0;
	return util_size;
}
/*----------------------------------------------------------------------------*/
unsigned char int16_2cstr(char* cstr, int that) {
	if (that<0) {
		that = -that;
		*cstr = '-';
		cstr++;
	}
	return uint16_2cstr(cstr,(unsigned int)that);
}
/*----------------------------------------------------------------------------*/
unsigned char cstr_2uint8(char* pstr, unsigned char* pval) {
	*pval = 0; util_loop = 0;
	while (pstr[util_loop]) {
		if (pstr[util_loop]<0x30||pstr[util_loop]>0x39) {
			util_loop = 0;
			break;
		}
		*pval = (*pval*10) + (pstr[util_loop]-0x30);
		util_loop++;
	}
	return util_loop;
}
/*----------------------------------------------------------------------------*/
unsigned char cstr_2uint16(char* pstr, unsigned int* pval) {
	*pval = 0; util_loop = 0;
	while (pstr[util_loop]) {
		if (pstr[util_loop]<0x30||pstr[util_loop]>0x39) {
			util_loop = 0;
			break;
		}
		*pval = (*pval*10) + (pstr[util_loop]-0x30);
		util_loop++;
	}
	return util_loop;
}
/*----------------------------------------------------------------------------*/
unsigned char cstr_2int16(char* pstr, int* pval) {
	util_temp = (pstr[0]=='-') ? 1 : 0;
	cstr_2uint16(&pstr[util_temp],(unsigned int*)pval);
	if (util_temp) *pval = -(*pval);
	return util_loop;
}
/*----------------------------------------------------------------------------*/
#endif /** __MY1UTILS_LITE_H__ */
/*----------------------------------------------------------------------------*/
