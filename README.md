### microblaze-drivers

Vivado 2020.1 Custom_AXI_IP  issue- Vitis won't build a generic axi driver autogenerated by Vivado and you will get makefile errors for even attempting to build a project using a custom ip.



Solution- https://forums.xilinx.com/t5/High-Level-Synthesis-HLS/Create-IP-AXI4-Lite/td-p/1139280
In Vitis, select your platform and open the Makefile(s) below and make the edits listed below. Unfortunately, you have to do this everytime you update the platform.

1. platform/microblaze_0/standalone_domain/bsp/microblaze_0/libsrc/YOUR_IP/src/Makefile
2. platform/hw/drivers/YOUR_IP/src/Makefile
3. platform/tempdsa/drivers/YOUR_IP/src/Makefile



Replace :
>libs:
>
>echo "Compiling YOUR_IP"
>
>$(COMPILER) $(COMPILER_FLAGS) $(EXTRA_COMPILER_FLAGS) $(INCLUDES) $(LIBSOURCES)
>
>$(ARCHIVER) -r ${RELEASEDIR}/${LIB} ${OUT}
>make clean'

By :
>OBJECTS = $(addsuffix .o, $(basename $(wildcard *.c)))
>
>libs:
>
>echo "Compiling YOUR_IP"
>
>$(COMPILER) $(COMPILER_FLAGS) $(EXTRA_COMPILER_FLAGS) $(INCLUDES) $(LIBSOURCES)
>
>$(ARCHIVER) -r ${RELEASEDIR}/${LIB} ${OBJECTS}'



Do a make clean and build your platform.



### example code
custom_interrupt_example.c - A simple driver for vhdl_misc\mb_helper3 based on- https://www.xilinx.com/support/answers/60837.html 




axigpiodriver.c - Drive io using a command / menu system from Microblaze stdio using Xilinx's AXI GPIO
