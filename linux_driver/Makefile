# my optional module name
MODULE=m_nrf24l01
 
# this two variables, depends where you have you raspberry kernel source and tools installed
 
CCPREFIX="/home/mickey/workspace/raspberry/cross/tools-master/arm-bcm2708/arm-bcm2708-linux-gnueabi/bin/arm-bcm2708-linux-gnueabi-"
CCPREFIX_COMPLETE="cc_args.py /home/mickey/workspace/raspberry/cross/tools-master/arm-bcm2708/arm-bcm2708-linux-gnueabi/bin/arm-bcm2708-linux-gnueabi-"
# KERNEL_SRC=/home/mickey/workspace/raspberry/XenoPi/linux-rpi/
KERNEL_SRC=/home/mickey/workspace/raspberry/cross/linux-rpi-3.12.y/
 
obj-m += ${MODULE}.o
${MODULE}-objs := nrfmod.o nrf24l01.o
 
MODCFLAGS := -O -Wall -DMODULE -D__KERNEL__ 

# Default rule
compile:
	make ARCH=arm CROSS_COMPILE=${CCPREFIX} -C ${KERNEL_SRC} M=$(PWD) modules
 
complete:
	make ARCH=arm CROSS_COMPILE=${CCPREFIX_COMPLETE} -C ${KERNEL_SRC} M=$(PWD) modules
	sh -c "mv ${KERNEL_SRC}/.clang_* .; generate_kernel_ycm.sh"
 
clean:
	make -C ${KERNEL_SRC} M=$(PWD) clean
	rm -f cscope* .ycm* tags .clang_*
 
# this just copies a file to raspberry
upload:
	sshpass -p "password" scp ${MODULE}.ko user@alarmpi:~/nrf_driver/
 
all: clean complete upload info

info:
	modinfo  ${MODULE}.ko

