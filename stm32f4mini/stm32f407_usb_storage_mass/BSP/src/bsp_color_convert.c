#include "bsp_color_convert.h"

/*private includes*/

/*private macro*/

/*private variable*/

/*public variable*/

/*private funtion decaleration*/

/*public funtion decaleration*/

/*funtion*/
uint32_t BSP_RGB242YCbCr(uint32_t RGB24)
{
	uint32_t Y,Cb,Cr,R,G,B;
	
	Y = ((77 * R + 150 * G + 29 * B) >> 8);
	Cb = ((-43 * R - 85 * G + 128 * B) >> 8) + 128;
	Cr = ((128 * R - 107 * G - 21 * B) >> 8) + 128;
	return (Y<<16|Cb<<8|Cr);
}
uint32_t BSP_YCbCr2RGB24(uint32_t YCbrCr)
{
	uint32_t Y,Cb,Cr,R,G,B; 
	R = Y + ((351*(Cr - 128)) >> 8);
	G = Y - ((179*(Cr - 128) + 86*(Cb - 128))>>8);
	B = Y + ((443*(Cb - 128)) >> 8);
	return (R<<16|G<<8|B);
}