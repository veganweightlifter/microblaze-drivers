/*
 *
 * Simple gpio output driver for a dual-channel AXI GPIO Xilinx block.
 * Both channels have 8 bit outputs driving LEDs
 * Tested and works in Vivado-Vitis 2020.1
 *
 */

#include <stdio.h>
#include "xparameters.h"
#include "platform.h"
#include "xil_printf.h"
#include "xgpio.h"
#include "xil_types.h"

#define GPIO_REG_BASEADDR XPAR_GPIO_0_BASEADDR
#define CHANNEL1 1
#define CHANNEL2 2

/****************************************************************************/
/**
* Takes in 2 chars and converts them to 8-bit hex.
* @param	char charhigh
* @param	char charlow
*
* @return	u8 which is 0x<charhigh><charlow>
*
* @note
*
* This function takes in 2 chars that are in the format :
* 0x<char1><char2> and converts them to 8-bit hex.
*
*****************************************************************************/
u8 char2hex(char char_high, char char_low)
{
	u8 h1, h2;
	if(('0' <= char_low) && ('9' >= char_low)) {
		h1 = char_low - '0';
	} else if (('a' <= char_low) && ('f' >= char_low)) {
		h1 = charlow - ('a'-10);
	} else if (('A' <= char_low) && ('F' >= char_low)) {
		h1 = charlow - ('A'-10);
	} else {
		h1 = 0xFF;
	}
	if(('0' <= char_high) && ('9' >= char_high)) {
		h2 = char_high - '0';
	} else if (('a' <= char_high) && ('f' >= char_high)) {
		h2 = charhigh - ('a'-10);
	} else if (('A' <= char_high) && ('F' >= char_high)) {
		h2 = char_high - ('A'-10);
	} else {
		h2 = 0xFF;
	}
	return (h1 & 0x0F)+(h2<<4);
}

int main()
{
	u32 data;
	XGpio_Config *cfg_ptr;
	XGpio gpio_device;
	// Initialize LED Device
	cfg_ptr = XGpio_LookupConfig(XPAR_GPIO_0_DEVICE_ID);
	XGpio_CfgInitialize(&gpio_device, cfg_ptr, cfg_ptr->BaseAddress);

    unsigned char led1, led2, led3, led4;
    while(1)
    {
        xil_printf("\n\rHello World!\n\rEnter 2 hex chars 0x<char1><char2> which will represent lower LED sequence: ");
        led1 = inbyte();// like getchar(); but better
        led2 = inbyte();
        data = (u32)char2hex(led1,led2);
        xil_printf("\n\rThe chars you entered are: 0x%02X\n\r",(u8)data);
    	XGpio_WriteReg((GPIO_REG_BASEADDR), ((CHANNEL1 - 1) * XGPIO_CHAN_OFFSET) + XGPIO_DATA_OFFSET, data);
        //XGpio_DiscreteWrite(&led_device, LED_CHANNEL, data);

        xil_printf("Enter 2 hex chars 0x<char1><char2> which will represent upper LED sequence: ");
        led3 = inbyte();// like getchar(); but better
        led4 = inbyte();
        data = (u32)char2hex(led3,led4);
        xil_printf("\n\rThe chars you entered are: 0x%02X\n\r",(u8)data);
    	XGpio_WriteReg((GPIO_REG_BASEADDR), ((CHANNEL2 - 1) * XGPIO_CHAN_OFFSET) + XGPIO_DATA_OFFSET, data);

    }

    cleanup_platform();
    return 0;
}
