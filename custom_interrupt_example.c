/******Heavily modified verison of Xilinx's example code**********************/
/*** <https://github.com/Xilinx/embeddedsw/blob/master/XilinxProcessorIPLib>**/
/***Original is here <>/drivers/intc/examples/xintc_low_level_example.c*******/
#include "mb_helper3.h"
#include "xparameters.h"
#include "xstatus.h"
#include "xintc_l.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "xil_types.h"

/************************** Constant Definitions *****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define INTC_BASEADDR		XPAR_INTC_0_BASEADDR
#define INTC_DEVICE_ID		XPAR_INTC_0_DEVICE_ID
#define INTC_DEVICE_INTR_ID	XPAR_INTC_0_MB_HELPER3_0_VEC_ID
#define INTC_DEVICE_INT_MASK	XPAR_MB_HELPER3_0_IRQ_MASK
#define MB_HELPER3_BASEADDR     0x44a10000
volatile int count = 0;
void MB_HELPER3_EnableInterrupt(void * baseaddr_p)
{
  u32 baseaddr;
  baseaddr = (u32) baseaddr_p;
  /*
   * Enable all interrupt source from user logic.
   */
  MB_HELPER3_mWriteReg(baseaddr, 0x4, 0x1);
  /*
   * Set global interrupt enable.
   */
  MB_HELPER3_mWriteReg(baseaddr, 0x0, 0x1);
}


void MB_HELPER3_INTERRUPT_ACK(void * baseaddr_p)
{
  u32 baseaddr;
  baseaddr = (u32) baseaddr_p;

  /*
  * ACK interrupts on MYIP_WITH_INTERRUPTS.
  */
  MB_HELPER3_mWriteReg(baseaddr, 0xc, 0x1);
}
void handler()
{
   count++;
   xil_printf("interrupt %d \n\r", count);
   MB_HELPER3_INTERRUPT_ACK(MB_HELPER3_BASEADDR);
}
int main()
{
  microblaze_enable_interrupts();

   XIntc_RegisterHandler(XPAR_INTC_SINGLE_BASEADDR,INTC_DEVICE_INTR_ID,
   (XInterruptHandler)handler, XPAR_INTC_0_DEVICE_ID);

   XIntc_MasterEnable(XPAR_INTC_SINGLE_BASEADDR);

   XIntc_EnableIntr(XPAR_INTC_SINGLE_BASEADDR, INTC_DEVICE_INT_MASK);

   MB_HELPER3_EnableInterrupt(MB_HELPER3_BASEADDR);

   xil_printf("Wait for Interrupts.... \n\r");
   while(count < 10);
}
