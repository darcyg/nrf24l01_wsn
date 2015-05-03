#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_RTOS_adv.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_RTOS_adv.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=app/src/main.c app/src/board_config.c common/src/logger.c common/src/device.c common/src/panic.s common/src/panic_handler.c drivers/src/pic24f_spi2.c drivers/src/pic24f_uart2.c drivers/src/nrf24l01.c drivers/src/pic24f_ext_irq.c drivers/src/pic24f_gpio.c drivers/src/pic24f_adc.c drivers/src/ir_control.c drivers/src/pic24f_i2c.c framework/mqtt/mqtt.c framework/serial/serial.c framework/tcp_app/tcp_server.c framework/uip/psock.c framework/uip/timer.c framework/uip/uip-fw.c framework/uip/uip-split.c framework/uip/uip.c framework/uip/uiplib.c framework/uip/nrf24l01_link.c os/croutine.c os/event_groups.c os/list.c os/queue.c os/tasks.c os/timers.c os/portable/MPLAB/PIC24_dsPIC/port.c os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S os/portable/MemMang/heap_1.c common/src/spi.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/app/src/main.o ${OBJECTDIR}/app/src/board_config.o ${OBJECTDIR}/common/src/logger.o ${OBJECTDIR}/common/src/device.o ${OBJECTDIR}/common/src/panic.o ${OBJECTDIR}/common/src/panic_handler.o ${OBJECTDIR}/drivers/src/pic24f_spi2.o ${OBJECTDIR}/drivers/src/pic24f_uart2.o ${OBJECTDIR}/drivers/src/nrf24l01.o ${OBJECTDIR}/drivers/src/pic24f_ext_irq.o ${OBJECTDIR}/drivers/src/pic24f_gpio.o ${OBJECTDIR}/drivers/src/pic24f_adc.o ${OBJECTDIR}/drivers/src/ir_control.o ${OBJECTDIR}/drivers/src/pic24f_i2c.o ${OBJECTDIR}/framework/mqtt/mqtt.o ${OBJECTDIR}/framework/serial/serial.o ${OBJECTDIR}/framework/tcp_app/tcp_server.o ${OBJECTDIR}/framework/uip/psock.o ${OBJECTDIR}/framework/uip/timer.o ${OBJECTDIR}/framework/uip/uip-fw.o ${OBJECTDIR}/framework/uip/uip-split.o ${OBJECTDIR}/framework/uip/uip.o ${OBJECTDIR}/framework/uip/uiplib.o ${OBJECTDIR}/framework/uip/nrf24l01_link.o ${OBJECTDIR}/os/croutine.o ${OBJECTDIR}/os/event_groups.o ${OBJECTDIR}/os/list.o ${OBJECTDIR}/os/queue.o ${OBJECTDIR}/os/tasks.o ${OBJECTDIR}/os/timers.o ${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/port.o ${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o ${OBJECTDIR}/os/portable/MemMang/heap_1.o ${OBJECTDIR}/common/src/spi.o
POSSIBLE_DEPFILES=${OBJECTDIR}/app/src/main.o.d ${OBJECTDIR}/app/src/board_config.o.d ${OBJECTDIR}/common/src/logger.o.d ${OBJECTDIR}/common/src/device.o.d ${OBJECTDIR}/common/src/panic.o.d ${OBJECTDIR}/common/src/panic_handler.o.d ${OBJECTDIR}/drivers/src/pic24f_spi2.o.d ${OBJECTDIR}/drivers/src/pic24f_uart2.o.d ${OBJECTDIR}/drivers/src/nrf24l01.o.d ${OBJECTDIR}/drivers/src/pic24f_ext_irq.o.d ${OBJECTDIR}/drivers/src/pic24f_gpio.o.d ${OBJECTDIR}/drivers/src/pic24f_adc.o.d ${OBJECTDIR}/drivers/src/ir_control.o.d ${OBJECTDIR}/drivers/src/pic24f_i2c.o.d ${OBJECTDIR}/framework/mqtt/mqtt.o.d ${OBJECTDIR}/framework/serial/serial.o.d ${OBJECTDIR}/framework/tcp_app/tcp_server.o.d ${OBJECTDIR}/framework/uip/psock.o.d ${OBJECTDIR}/framework/uip/timer.o.d ${OBJECTDIR}/framework/uip/uip-fw.o.d ${OBJECTDIR}/framework/uip/uip-split.o.d ${OBJECTDIR}/framework/uip/uip.o.d ${OBJECTDIR}/framework/uip/uiplib.o.d ${OBJECTDIR}/framework/uip/nrf24l01_link.o.d ${OBJECTDIR}/os/croutine.o.d ${OBJECTDIR}/os/event_groups.o.d ${OBJECTDIR}/os/list.o.d ${OBJECTDIR}/os/queue.o.d ${OBJECTDIR}/os/tasks.o.d ${OBJECTDIR}/os/timers.o.d ${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/port.o.d ${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o.d ${OBJECTDIR}/os/portable/MemMang/heap_1.o.d ${OBJECTDIR}/common/src/spi.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/app/src/main.o ${OBJECTDIR}/app/src/board_config.o ${OBJECTDIR}/common/src/logger.o ${OBJECTDIR}/common/src/device.o ${OBJECTDIR}/common/src/panic.o ${OBJECTDIR}/common/src/panic_handler.o ${OBJECTDIR}/drivers/src/pic24f_spi2.o ${OBJECTDIR}/drivers/src/pic24f_uart2.o ${OBJECTDIR}/drivers/src/nrf24l01.o ${OBJECTDIR}/drivers/src/pic24f_ext_irq.o ${OBJECTDIR}/drivers/src/pic24f_gpio.o ${OBJECTDIR}/drivers/src/pic24f_adc.o ${OBJECTDIR}/drivers/src/ir_control.o ${OBJECTDIR}/drivers/src/pic24f_i2c.o ${OBJECTDIR}/framework/mqtt/mqtt.o ${OBJECTDIR}/framework/serial/serial.o ${OBJECTDIR}/framework/tcp_app/tcp_server.o ${OBJECTDIR}/framework/uip/psock.o ${OBJECTDIR}/framework/uip/timer.o ${OBJECTDIR}/framework/uip/uip-fw.o ${OBJECTDIR}/framework/uip/uip-split.o ${OBJECTDIR}/framework/uip/uip.o ${OBJECTDIR}/framework/uip/uiplib.o ${OBJECTDIR}/framework/uip/nrf24l01_link.o ${OBJECTDIR}/os/croutine.o ${OBJECTDIR}/os/event_groups.o ${OBJECTDIR}/os/list.o ${OBJECTDIR}/os/queue.o ${OBJECTDIR}/os/tasks.o ${OBJECTDIR}/os/timers.o ${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/port.o ${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o ${OBJECTDIR}/os/portable/MemMang/heap_1.o ${OBJECTDIR}/common/src/spi.o

# Source Files
SOURCEFILES=app/src/main.c app/src/board_config.c common/src/logger.c common/src/device.c common/src/panic.s common/src/panic_handler.c drivers/src/pic24f_spi2.c drivers/src/pic24f_uart2.c drivers/src/nrf24l01.c drivers/src/pic24f_ext_irq.c drivers/src/pic24f_gpio.c drivers/src/pic24f_adc.c drivers/src/ir_control.c drivers/src/pic24f_i2c.c framework/mqtt/mqtt.c framework/serial/serial.c framework/tcp_app/tcp_server.c framework/uip/psock.c framework/uip/timer.c framework/uip/uip-fw.c framework/uip/uip-split.c framework/uip/uip.c framework/uip/uiplib.c framework/uip/nrf24l01_link.c os/croutine.c os/event_groups.c os/list.c os/queue.c os/tasks.c os/timers.c os/portable/MPLAB/PIC24_dsPIC/port.c os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S os/portable/MemMang/heap_1.c common/src/spi.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_RTOS_adv.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ64GB002
MP_LINKER_FILE_OPTION=,--script="bootloader/pic24f_usb_hid.X/linker/hid_boot_p24FJ64GB002.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/app/src/main.o: app/src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/app/src 
	@${RM} ${OBJECTDIR}/app/src/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  app/src/main.c  -o ${OBJECTDIR}/app/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/app/src/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/app/src/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/app/src/board_config.o: app/src/board_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/app/src 
	@${RM} ${OBJECTDIR}/app/src/board_config.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  app/src/board_config.c  -o ${OBJECTDIR}/app/src/board_config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/app/src/board_config.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/app/src/board_config.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/common/src/logger.o: common/src/logger.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/common/src 
	@${RM} ${OBJECTDIR}/common/src/logger.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  common/src/logger.c  -o ${OBJECTDIR}/common/src/logger.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/common/src/logger.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/common/src/logger.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/common/src/device.o: common/src/device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/common/src 
	@${RM} ${OBJECTDIR}/common/src/device.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  common/src/device.c  -o ${OBJECTDIR}/common/src/device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/common/src/device.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/common/src/device.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/common/src/panic_handler.o: common/src/panic_handler.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/common/src 
	@${RM} ${OBJECTDIR}/common/src/panic_handler.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  common/src/panic_handler.c  -o ${OBJECTDIR}/common/src/panic_handler.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/common/src/panic_handler.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/common/src/panic_handler.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/drivers/src/pic24f_spi2.o: drivers/src/pic24f_spi2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/drivers/src 
	@${RM} ${OBJECTDIR}/drivers/src/pic24f_spi2.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  drivers/src/pic24f_spi2.c  -o ${OBJECTDIR}/drivers/src/pic24f_spi2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/drivers/src/pic24f_spi2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/drivers/src/pic24f_spi2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/drivers/src/pic24f_uart2.o: drivers/src/pic24f_uart2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/drivers/src 
	@${RM} ${OBJECTDIR}/drivers/src/pic24f_uart2.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  drivers/src/pic24f_uart2.c  -o ${OBJECTDIR}/drivers/src/pic24f_uart2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/drivers/src/pic24f_uart2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/drivers/src/pic24f_uart2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/drivers/src/nrf24l01.o: drivers/src/nrf24l01.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/drivers/src 
	@${RM} ${OBJECTDIR}/drivers/src/nrf24l01.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  drivers/src/nrf24l01.c  -o ${OBJECTDIR}/drivers/src/nrf24l01.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/drivers/src/nrf24l01.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/drivers/src/nrf24l01.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/drivers/src/pic24f_ext_irq.o: drivers/src/pic24f_ext_irq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/drivers/src 
	@${RM} ${OBJECTDIR}/drivers/src/pic24f_ext_irq.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  drivers/src/pic24f_ext_irq.c  -o ${OBJECTDIR}/drivers/src/pic24f_ext_irq.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/drivers/src/pic24f_ext_irq.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/drivers/src/pic24f_ext_irq.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/drivers/src/pic24f_gpio.o: drivers/src/pic24f_gpio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/drivers/src 
	@${RM} ${OBJECTDIR}/drivers/src/pic24f_gpio.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  drivers/src/pic24f_gpio.c  -o ${OBJECTDIR}/drivers/src/pic24f_gpio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/drivers/src/pic24f_gpio.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/drivers/src/pic24f_gpio.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/drivers/src/pic24f_adc.o: drivers/src/pic24f_adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/drivers/src 
	@${RM} ${OBJECTDIR}/drivers/src/pic24f_adc.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  drivers/src/pic24f_adc.c  -o ${OBJECTDIR}/drivers/src/pic24f_adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/drivers/src/pic24f_adc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/drivers/src/pic24f_adc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/drivers/src/ir_control.o: drivers/src/ir_control.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/drivers/src 
	@${RM} ${OBJECTDIR}/drivers/src/ir_control.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  drivers/src/ir_control.c  -o ${OBJECTDIR}/drivers/src/ir_control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/drivers/src/ir_control.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/drivers/src/ir_control.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/drivers/src/pic24f_i2c.o: drivers/src/pic24f_i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/drivers/src 
	@${RM} ${OBJECTDIR}/drivers/src/pic24f_i2c.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  drivers/src/pic24f_i2c.c  -o ${OBJECTDIR}/drivers/src/pic24f_i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/drivers/src/pic24f_i2c.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/drivers/src/pic24f_i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/mqtt/mqtt.o: framework/mqtt/mqtt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/framework/mqtt 
	@${RM} ${OBJECTDIR}/framework/mqtt/mqtt.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/mqtt/mqtt.c  -o ${OBJECTDIR}/framework/mqtt/mqtt.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/mqtt/mqtt.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/framework/mqtt/mqtt.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/serial/serial.o: framework/serial/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/framework/serial 
	@${RM} ${OBJECTDIR}/framework/serial/serial.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/serial/serial.c  -o ${OBJECTDIR}/framework/serial/serial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/serial/serial.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/framework/serial/serial.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/tcp_app/tcp_server.o: framework/tcp_app/tcp_server.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/framework/tcp_app 
	@${RM} ${OBJECTDIR}/framework/tcp_app/tcp_server.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/tcp_app/tcp_server.c  -o ${OBJECTDIR}/framework/tcp_app/tcp_server.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/tcp_app/tcp_server.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/framework/tcp_app/tcp_server.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/uip/psock.o: framework/uip/psock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/framework/uip 
	@${RM} ${OBJECTDIR}/framework/uip/psock.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/uip/psock.c  -o ${OBJECTDIR}/framework/uip/psock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/uip/psock.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/framework/uip/psock.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/uip/timer.o: framework/uip/timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/framework/uip 
	@${RM} ${OBJECTDIR}/framework/uip/timer.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/uip/timer.c  -o ${OBJECTDIR}/framework/uip/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/uip/timer.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/framework/uip/timer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/uip/uip-fw.o: framework/uip/uip-fw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/framework/uip 
	@${RM} ${OBJECTDIR}/framework/uip/uip-fw.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/uip/uip-fw.c  -o ${OBJECTDIR}/framework/uip/uip-fw.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/uip/uip-fw.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/framework/uip/uip-fw.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/uip/uip-split.o: framework/uip/uip-split.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/framework/uip 
	@${RM} ${OBJECTDIR}/framework/uip/uip-split.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/uip/uip-split.c  -o ${OBJECTDIR}/framework/uip/uip-split.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/uip/uip-split.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/framework/uip/uip-split.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/uip/uip.o: framework/uip/uip.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/framework/uip 
	@${RM} ${OBJECTDIR}/framework/uip/uip.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/uip/uip.c  -o ${OBJECTDIR}/framework/uip/uip.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/uip/uip.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/framework/uip/uip.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/uip/uiplib.o: framework/uip/uiplib.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/framework/uip 
	@${RM} ${OBJECTDIR}/framework/uip/uiplib.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/uip/uiplib.c  -o ${OBJECTDIR}/framework/uip/uiplib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/uip/uiplib.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/framework/uip/uiplib.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/uip/nrf24l01_link.o: framework/uip/nrf24l01_link.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/framework/uip 
	@${RM} ${OBJECTDIR}/framework/uip/nrf24l01_link.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/uip/nrf24l01_link.c  -o ${OBJECTDIR}/framework/uip/nrf24l01_link.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/uip/nrf24l01_link.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/framework/uip/nrf24l01_link.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/os/croutine.o: os/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/croutine.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  os/croutine.c  -o ${OBJECTDIR}/os/croutine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/os/croutine.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/os/croutine.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/os/event_groups.o: os/event_groups.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/event_groups.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  os/event_groups.c  -o ${OBJECTDIR}/os/event_groups.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/os/event_groups.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/os/event_groups.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/os/list.o: os/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/list.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  os/list.c  -o ${OBJECTDIR}/os/list.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/os/list.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/os/list.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/os/queue.o: os/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/queue.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  os/queue.c  -o ${OBJECTDIR}/os/queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/os/queue.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/os/queue.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/os/tasks.o: os/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/tasks.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  os/tasks.c  -o ${OBJECTDIR}/os/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/os/tasks.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/os/tasks.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/os/timers.o: os/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/timers.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  os/timers.c  -o ${OBJECTDIR}/os/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/os/timers.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/os/timers.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/port.o: os/portable/MPLAB/PIC24_dsPIC/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC 
	@${RM} ${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/port.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  os/portable/MPLAB/PIC24_dsPIC/port.c  -o ${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/port.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/os/portable/MemMang/heap_1.o: os/portable/MemMang/heap_1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os/portable/MemMang 
	@${RM} ${OBJECTDIR}/os/portable/MemMang/heap_1.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  os/portable/MemMang/heap_1.c  -o ${OBJECTDIR}/os/portable/MemMang/heap_1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/os/portable/MemMang/heap_1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/os/portable/MemMang/heap_1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/common/src/spi.o: common/src/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/common/src 
	@${RM} ${OBJECTDIR}/common/src/spi.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  common/src/spi.c  -o ${OBJECTDIR}/common/src/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/common/src/spi.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/common/src/spi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/app/src/main.o: app/src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/app/src 
	@${RM} ${OBJECTDIR}/app/src/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  app/src/main.c  -o ${OBJECTDIR}/app/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/app/src/main.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/app/src/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/app/src/board_config.o: app/src/board_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/app/src 
	@${RM} ${OBJECTDIR}/app/src/board_config.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  app/src/board_config.c  -o ${OBJECTDIR}/app/src/board_config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/app/src/board_config.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/app/src/board_config.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/common/src/logger.o: common/src/logger.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/common/src 
	@${RM} ${OBJECTDIR}/common/src/logger.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  common/src/logger.c  -o ${OBJECTDIR}/common/src/logger.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/common/src/logger.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/common/src/logger.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/common/src/device.o: common/src/device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/common/src 
	@${RM} ${OBJECTDIR}/common/src/device.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  common/src/device.c  -o ${OBJECTDIR}/common/src/device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/common/src/device.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/common/src/device.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/common/src/panic_handler.o: common/src/panic_handler.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/common/src 
	@${RM} ${OBJECTDIR}/common/src/panic_handler.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  common/src/panic_handler.c  -o ${OBJECTDIR}/common/src/panic_handler.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/common/src/panic_handler.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/common/src/panic_handler.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/drivers/src/pic24f_spi2.o: drivers/src/pic24f_spi2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/drivers/src 
	@${RM} ${OBJECTDIR}/drivers/src/pic24f_spi2.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  drivers/src/pic24f_spi2.c  -o ${OBJECTDIR}/drivers/src/pic24f_spi2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/drivers/src/pic24f_spi2.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/drivers/src/pic24f_spi2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/drivers/src/pic24f_uart2.o: drivers/src/pic24f_uart2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/drivers/src 
	@${RM} ${OBJECTDIR}/drivers/src/pic24f_uart2.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  drivers/src/pic24f_uart2.c  -o ${OBJECTDIR}/drivers/src/pic24f_uart2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/drivers/src/pic24f_uart2.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/drivers/src/pic24f_uart2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/drivers/src/nrf24l01.o: drivers/src/nrf24l01.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/drivers/src 
	@${RM} ${OBJECTDIR}/drivers/src/nrf24l01.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  drivers/src/nrf24l01.c  -o ${OBJECTDIR}/drivers/src/nrf24l01.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/drivers/src/nrf24l01.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/drivers/src/nrf24l01.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/drivers/src/pic24f_ext_irq.o: drivers/src/pic24f_ext_irq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/drivers/src 
	@${RM} ${OBJECTDIR}/drivers/src/pic24f_ext_irq.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  drivers/src/pic24f_ext_irq.c  -o ${OBJECTDIR}/drivers/src/pic24f_ext_irq.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/drivers/src/pic24f_ext_irq.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/drivers/src/pic24f_ext_irq.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/drivers/src/pic24f_gpio.o: drivers/src/pic24f_gpio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/drivers/src 
	@${RM} ${OBJECTDIR}/drivers/src/pic24f_gpio.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  drivers/src/pic24f_gpio.c  -o ${OBJECTDIR}/drivers/src/pic24f_gpio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/drivers/src/pic24f_gpio.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/drivers/src/pic24f_gpio.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/drivers/src/pic24f_adc.o: drivers/src/pic24f_adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/drivers/src 
	@${RM} ${OBJECTDIR}/drivers/src/pic24f_adc.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  drivers/src/pic24f_adc.c  -o ${OBJECTDIR}/drivers/src/pic24f_adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/drivers/src/pic24f_adc.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/drivers/src/pic24f_adc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/drivers/src/ir_control.o: drivers/src/ir_control.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/drivers/src 
	@${RM} ${OBJECTDIR}/drivers/src/ir_control.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  drivers/src/ir_control.c  -o ${OBJECTDIR}/drivers/src/ir_control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/drivers/src/ir_control.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/drivers/src/ir_control.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/drivers/src/pic24f_i2c.o: drivers/src/pic24f_i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/drivers/src 
	@${RM} ${OBJECTDIR}/drivers/src/pic24f_i2c.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  drivers/src/pic24f_i2c.c  -o ${OBJECTDIR}/drivers/src/pic24f_i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/drivers/src/pic24f_i2c.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/drivers/src/pic24f_i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/mqtt/mqtt.o: framework/mqtt/mqtt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/framework/mqtt 
	@${RM} ${OBJECTDIR}/framework/mqtt/mqtt.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/mqtt/mqtt.c  -o ${OBJECTDIR}/framework/mqtt/mqtt.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/mqtt/mqtt.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/framework/mqtt/mqtt.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/serial/serial.o: framework/serial/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/framework/serial 
	@${RM} ${OBJECTDIR}/framework/serial/serial.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/serial/serial.c  -o ${OBJECTDIR}/framework/serial/serial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/serial/serial.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/framework/serial/serial.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/tcp_app/tcp_server.o: framework/tcp_app/tcp_server.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/framework/tcp_app 
	@${RM} ${OBJECTDIR}/framework/tcp_app/tcp_server.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/tcp_app/tcp_server.c  -o ${OBJECTDIR}/framework/tcp_app/tcp_server.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/tcp_app/tcp_server.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/framework/tcp_app/tcp_server.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/uip/psock.o: framework/uip/psock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/framework/uip 
	@${RM} ${OBJECTDIR}/framework/uip/psock.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/uip/psock.c  -o ${OBJECTDIR}/framework/uip/psock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/uip/psock.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/framework/uip/psock.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/uip/timer.o: framework/uip/timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/framework/uip 
	@${RM} ${OBJECTDIR}/framework/uip/timer.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/uip/timer.c  -o ${OBJECTDIR}/framework/uip/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/uip/timer.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/framework/uip/timer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/uip/uip-fw.o: framework/uip/uip-fw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/framework/uip 
	@${RM} ${OBJECTDIR}/framework/uip/uip-fw.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/uip/uip-fw.c  -o ${OBJECTDIR}/framework/uip/uip-fw.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/uip/uip-fw.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/framework/uip/uip-fw.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/uip/uip-split.o: framework/uip/uip-split.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/framework/uip 
	@${RM} ${OBJECTDIR}/framework/uip/uip-split.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/uip/uip-split.c  -o ${OBJECTDIR}/framework/uip/uip-split.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/uip/uip-split.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/framework/uip/uip-split.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/uip/uip.o: framework/uip/uip.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/framework/uip 
	@${RM} ${OBJECTDIR}/framework/uip/uip.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/uip/uip.c  -o ${OBJECTDIR}/framework/uip/uip.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/uip/uip.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/framework/uip/uip.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/uip/uiplib.o: framework/uip/uiplib.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/framework/uip 
	@${RM} ${OBJECTDIR}/framework/uip/uiplib.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/uip/uiplib.c  -o ${OBJECTDIR}/framework/uip/uiplib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/uip/uiplib.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/framework/uip/uiplib.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/uip/nrf24l01_link.o: framework/uip/nrf24l01_link.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/framework/uip 
	@${RM} ${OBJECTDIR}/framework/uip/nrf24l01_link.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/uip/nrf24l01_link.c  -o ${OBJECTDIR}/framework/uip/nrf24l01_link.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/uip/nrf24l01_link.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/framework/uip/nrf24l01_link.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/os/croutine.o: os/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/croutine.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  os/croutine.c  -o ${OBJECTDIR}/os/croutine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/os/croutine.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/os/croutine.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/os/event_groups.o: os/event_groups.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/event_groups.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  os/event_groups.c  -o ${OBJECTDIR}/os/event_groups.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/os/event_groups.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/os/event_groups.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/os/list.o: os/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/list.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  os/list.c  -o ${OBJECTDIR}/os/list.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/os/list.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/os/list.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/os/queue.o: os/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/queue.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  os/queue.c  -o ${OBJECTDIR}/os/queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/os/queue.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/os/queue.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/os/tasks.o: os/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/tasks.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  os/tasks.c  -o ${OBJECTDIR}/os/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/os/tasks.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/os/tasks.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/os/timers.o: os/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os 
	@${RM} ${OBJECTDIR}/os/timers.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  os/timers.c  -o ${OBJECTDIR}/os/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/os/timers.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/os/timers.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/port.o: os/portable/MPLAB/PIC24_dsPIC/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC 
	@${RM} ${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/port.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  os/portable/MPLAB/PIC24_dsPIC/port.c  -o ${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/port.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/os/portable/MemMang/heap_1.o: os/portable/MemMang/heap_1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os/portable/MemMang 
	@${RM} ${OBJECTDIR}/os/portable/MemMang/heap_1.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  os/portable/MemMang/heap_1.c  -o ${OBJECTDIR}/os/portable/MemMang/heap_1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/os/portable/MemMang/heap_1.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/os/portable/MemMang/heap_1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/common/src/spi.o: common/src/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/common/src 
	@${RM} ${OBJECTDIR}/common/src/spi.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  common/src/spi.c  -o ${OBJECTDIR}/common/src/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/common/src/spi.o.d"      -g -omf=elf -O2 -I"os/include" -I"os/portable/MPLAB/PIC24_dsPIC" -I"drivers/include" -I"app/include" -I"common/include" -I"framework" -DMPLAB_PIC24_PORT -msmart-io=1 -Werror -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/common/src/spi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/common/src/panic.o: common/src/panic.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/common/src 
	@${RM} ${OBJECTDIR}/common/src/panic.o.d 
	${MP_CC} $(MP_EXTRA_AS_PRE)  common/src/panic.s  -o ${OBJECTDIR}/common/src/panic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -Wa,-MD,"${OBJECTDIR}/common/src/panic.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PICKIT2=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/common/src/panic.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/common/src/panic.o: common/src/panic.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/common/src 
	@${RM} ${OBJECTDIR}/common/src/panic.o.d 
	${MP_CC} $(MP_EXTRA_AS_PRE)  common/src/panic.s  -o ${OBJECTDIR}/common/src/panic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -Wa,-MD,"${OBJECTDIR}/common/src/panic.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/common/src/panic.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o: os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC 
	@${RM} ${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o.d 
	${MP_CC} $(MP_EXTRA_AS_PRE)  os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S  -o ${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o.d"  -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -Wa,-MD,"${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o.asm.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PICKIT2=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o.d" "${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o.asm.d"  -t $(SILENT)  -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o: os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC 
	@${RM} ${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o.d 
	${MP_CC} $(MP_EXTRA_AS_PRE)  os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.S  -o ${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o.d"  -omf=elf -Wa,-MD,"${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o.asm.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o.d" "${OBJECTDIR}/os/portable/MPLAB/PIC24_dsPIC/portasm_PIC24.o.asm.d"  -t $(SILENT)  -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_RTOS_adv.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    bootloader/pic24f_usb_hid.X/linker/hid_boot_p24FJ64GB002.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_RTOS_adv.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PICKIT2=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_RTOS_adv.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   bootloader/pic24f_usb_hid.X/linker/hid_boot_p24FJ64GB002.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_RTOS_adv.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}/xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_RTOS_adv.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
