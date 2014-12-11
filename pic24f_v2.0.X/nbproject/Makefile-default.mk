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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_v2.0.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_v2.0.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=uart2.c i2c.c spi2.c blink_LED.c IR_control.c timer1.c mqtt/mqtt.c nrf24l01/nrf24l01.c nrf24l01/nrf24l01_link.c telnetd/shell.c telnetd/telnetd.c telnetd/memb.c uip/psock.c uip/timer.c uip/uip-fw.c uip/uip-neighbor.c uip/uip-split.c uip/uip.c uip/uip_arp.c uip/uiplib.c main.c tcp/tcp_server.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/uart2.o ${OBJECTDIR}/i2c.o ${OBJECTDIR}/spi2.o ${OBJECTDIR}/blink_LED.o ${OBJECTDIR}/IR_control.o ${OBJECTDIR}/timer1.o ${OBJECTDIR}/mqtt/mqtt.o ${OBJECTDIR}/nrf24l01/nrf24l01.o ${OBJECTDIR}/nrf24l01/nrf24l01_link.o ${OBJECTDIR}/telnetd/shell.o ${OBJECTDIR}/telnetd/telnetd.o ${OBJECTDIR}/telnetd/memb.o ${OBJECTDIR}/uip/psock.o ${OBJECTDIR}/uip/timer.o ${OBJECTDIR}/uip/uip-fw.o ${OBJECTDIR}/uip/uip-neighbor.o ${OBJECTDIR}/uip/uip-split.o ${OBJECTDIR}/uip/uip.o ${OBJECTDIR}/uip/uip_arp.o ${OBJECTDIR}/uip/uiplib.o ${OBJECTDIR}/main.o ${OBJECTDIR}/tcp/tcp_server.o
POSSIBLE_DEPFILES=${OBJECTDIR}/uart2.o.d ${OBJECTDIR}/i2c.o.d ${OBJECTDIR}/spi2.o.d ${OBJECTDIR}/blink_LED.o.d ${OBJECTDIR}/IR_control.o.d ${OBJECTDIR}/timer1.o.d ${OBJECTDIR}/mqtt/mqtt.o.d ${OBJECTDIR}/nrf24l01/nrf24l01.o.d ${OBJECTDIR}/nrf24l01/nrf24l01_link.o.d ${OBJECTDIR}/telnetd/shell.o.d ${OBJECTDIR}/telnetd/telnetd.o.d ${OBJECTDIR}/telnetd/memb.o.d ${OBJECTDIR}/uip/psock.o.d ${OBJECTDIR}/uip/timer.o.d ${OBJECTDIR}/uip/uip-fw.o.d ${OBJECTDIR}/uip/uip-neighbor.o.d ${OBJECTDIR}/uip/uip-split.o.d ${OBJECTDIR}/uip/uip.o.d ${OBJECTDIR}/uip/uip_arp.o.d ${OBJECTDIR}/uip/uiplib.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/tcp/tcp_server.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/uart2.o ${OBJECTDIR}/i2c.o ${OBJECTDIR}/spi2.o ${OBJECTDIR}/blink_LED.o ${OBJECTDIR}/IR_control.o ${OBJECTDIR}/timer1.o ${OBJECTDIR}/mqtt/mqtt.o ${OBJECTDIR}/nrf24l01/nrf24l01.o ${OBJECTDIR}/nrf24l01/nrf24l01_link.o ${OBJECTDIR}/telnetd/shell.o ${OBJECTDIR}/telnetd/telnetd.o ${OBJECTDIR}/telnetd/memb.o ${OBJECTDIR}/uip/psock.o ${OBJECTDIR}/uip/timer.o ${OBJECTDIR}/uip/uip-fw.o ${OBJECTDIR}/uip/uip-neighbor.o ${OBJECTDIR}/uip/uip-split.o ${OBJECTDIR}/uip/uip.o ${OBJECTDIR}/uip/uip_arp.o ${OBJECTDIR}/uip/uiplib.o ${OBJECTDIR}/main.o ${OBJECTDIR}/tcp/tcp_server.o

# Source Files
SOURCEFILES=uart2.c i2c.c spi2.c blink_LED.c IR_control.c timer1.c mqtt/mqtt.c nrf24l01/nrf24l01.c nrf24l01/nrf24l01_link.c telnetd/shell.c telnetd/telnetd.c telnetd/memb.c uip/psock.c uip/timer.c uip/uip-fw.c uip/uip-neighbor.c uip/uip-split.c uip/uip.c uip/uip_arp.c uip/uiplib.c main.c tcp/tcp_server.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_v2.0.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ64GB002
MP_LINKER_FILE_OPTION=,--script="p24FJ64GB002.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/uart2.o: uart2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/uart2.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uart2.c  -o ${OBJECTDIR}/uart2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uart2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/uart2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/i2c.o: i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/i2c.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  i2c.c  -o ${OBJECTDIR}/i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/i2c.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/spi2.o: spi2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/spi2.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  spi2.c  -o ${OBJECTDIR}/spi2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/spi2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/spi2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/blink_LED.o: blink_LED.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/blink_LED.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  blink_LED.c  -o ${OBJECTDIR}/blink_LED.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/blink_LED.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/blink_LED.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/IR_control.o: IR_control.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/IR_control.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  IR_control.c  -o ${OBJECTDIR}/IR_control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/IR_control.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/IR_control.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/timer1.o: timer1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/timer1.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  timer1.c  -o ${OBJECTDIR}/timer1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/timer1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/timer1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mqtt/mqtt.o: mqtt/mqtt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mqtt 
	@${RM} ${OBJECTDIR}/mqtt/mqtt.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mqtt/mqtt.c  -o ${OBJECTDIR}/mqtt/mqtt.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mqtt/mqtt.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/mqtt/mqtt.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/nrf24l01/nrf24l01.o: nrf24l01/nrf24l01.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/nrf24l01 
	@${RM} ${OBJECTDIR}/nrf24l01/nrf24l01.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  nrf24l01/nrf24l01.c  -o ${OBJECTDIR}/nrf24l01/nrf24l01.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/nrf24l01/nrf24l01.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/nrf24l01/nrf24l01.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/nrf24l01/nrf24l01_link.o: nrf24l01/nrf24l01_link.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/nrf24l01 
	@${RM} ${OBJECTDIR}/nrf24l01/nrf24l01_link.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  nrf24l01/nrf24l01_link.c  -o ${OBJECTDIR}/nrf24l01/nrf24l01_link.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/nrf24l01/nrf24l01_link.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/nrf24l01/nrf24l01_link.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/telnetd/shell.o: telnetd/shell.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/telnetd 
	@${RM} ${OBJECTDIR}/telnetd/shell.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  telnetd/shell.c  -o ${OBJECTDIR}/telnetd/shell.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/telnetd/shell.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/telnetd/shell.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/telnetd/telnetd.o: telnetd/telnetd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/telnetd 
	@${RM} ${OBJECTDIR}/telnetd/telnetd.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  telnetd/telnetd.c  -o ${OBJECTDIR}/telnetd/telnetd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/telnetd/telnetd.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/telnetd/telnetd.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/telnetd/memb.o: telnetd/memb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/telnetd 
	@${RM} ${OBJECTDIR}/telnetd/memb.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  telnetd/memb.c  -o ${OBJECTDIR}/telnetd/memb.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/telnetd/memb.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/telnetd/memb.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/uip/psock.o: uip/psock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uip 
	@${RM} ${OBJECTDIR}/uip/psock.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uip/psock.c  -o ${OBJECTDIR}/uip/psock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uip/psock.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/uip/psock.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/uip/timer.o: uip/timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uip 
	@${RM} ${OBJECTDIR}/uip/timer.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uip/timer.c  -o ${OBJECTDIR}/uip/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uip/timer.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/uip/timer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/uip/uip-fw.o: uip/uip-fw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uip 
	@${RM} ${OBJECTDIR}/uip/uip-fw.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uip/uip-fw.c  -o ${OBJECTDIR}/uip/uip-fw.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uip/uip-fw.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/uip/uip-fw.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/uip/uip-neighbor.o: uip/uip-neighbor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uip 
	@${RM} ${OBJECTDIR}/uip/uip-neighbor.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uip/uip-neighbor.c  -o ${OBJECTDIR}/uip/uip-neighbor.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uip/uip-neighbor.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/uip/uip-neighbor.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/uip/uip-split.o: uip/uip-split.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uip 
	@${RM} ${OBJECTDIR}/uip/uip-split.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uip/uip-split.c  -o ${OBJECTDIR}/uip/uip-split.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uip/uip-split.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/uip/uip-split.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/uip/uip.o: uip/uip.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uip 
	@${RM} ${OBJECTDIR}/uip/uip.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uip/uip.c  -o ${OBJECTDIR}/uip/uip.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uip/uip.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/uip/uip.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/uip/uip_arp.o: uip/uip_arp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uip 
	@${RM} ${OBJECTDIR}/uip/uip_arp.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uip/uip_arp.c  -o ${OBJECTDIR}/uip/uip_arp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uip/uip_arp.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/uip/uip_arp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/uip/uiplib.o: uip/uiplib.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uip 
	@${RM} ${OBJECTDIR}/uip/uiplib.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uip/uiplib.c  -o ${OBJECTDIR}/uip/uiplib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uip/uiplib.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/uip/uiplib.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/tcp/tcp_server.o: tcp/tcp_server.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/tcp 
	@${RM} ${OBJECTDIR}/tcp/tcp_server.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  tcp/tcp_server.c  -o ${OBJECTDIR}/tcp/tcp_server.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/tcp/tcp_server.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/tcp/tcp_server.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/uart2.o: uart2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/uart2.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uart2.c  -o ${OBJECTDIR}/uart2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uart2.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/uart2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/i2c.o: i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/i2c.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  i2c.c  -o ${OBJECTDIR}/i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/i2c.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/spi2.o: spi2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/spi2.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  spi2.c  -o ${OBJECTDIR}/spi2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/spi2.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/spi2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/blink_LED.o: blink_LED.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/blink_LED.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  blink_LED.c  -o ${OBJECTDIR}/blink_LED.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/blink_LED.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/blink_LED.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/IR_control.o: IR_control.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/IR_control.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  IR_control.c  -o ${OBJECTDIR}/IR_control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/IR_control.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/IR_control.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/timer1.o: timer1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/timer1.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  timer1.c  -o ${OBJECTDIR}/timer1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/timer1.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/timer1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mqtt/mqtt.o: mqtt/mqtt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/mqtt 
	@${RM} ${OBJECTDIR}/mqtt/mqtt.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mqtt/mqtt.c  -o ${OBJECTDIR}/mqtt/mqtt.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mqtt/mqtt.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/mqtt/mqtt.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/nrf24l01/nrf24l01.o: nrf24l01/nrf24l01.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/nrf24l01 
	@${RM} ${OBJECTDIR}/nrf24l01/nrf24l01.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  nrf24l01/nrf24l01.c  -o ${OBJECTDIR}/nrf24l01/nrf24l01.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/nrf24l01/nrf24l01.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/nrf24l01/nrf24l01.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/nrf24l01/nrf24l01_link.o: nrf24l01/nrf24l01_link.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/nrf24l01 
	@${RM} ${OBJECTDIR}/nrf24l01/nrf24l01_link.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  nrf24l01/nrf24l01_link.c  -o ${OBJECTDIR}/nrf24l01/nrf24l01_link.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/nrf24l01/nrf24l01_link.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/nrf24l01/nrf24l01_link.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/telnetd/shell.o: telnetd/shell.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/telnetd 
	@${RM} ${OBJECTDIR}/telnetd/shell.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  telnetd/shell.c  -o ${OBJECTDIR}/telnetd/shell.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/telnetd/shell.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/telnetd/shell.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/telnetd/telnetd.o: telnetd/telnetd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/telnetd 
	@${RM} ${OBJECTDIR}/telnetd/telnetd.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  telnetd/telnetd.c  -o ${OBJECTDIR}/telnetd/telnetd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/telnetd/telnetd.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/telnetd/telnetd.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/telnetd/memb.o: telnetd/memb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/telnetd 
	@${RM} ${OBJECTDIR}/telnetd/memb.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  telnetd/memb.c  -o ${OBJECTDIR}/telnetd/memb.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/telnetd/memb.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/telnetd/memb.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/uip/psock.o: uip/psock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uip 
	@${RM} ${OBJECTDIR}/uip/psock.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uip/psock.c  -o ${OBJECTDIR}/uip/psock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uip/psock.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/uip/psock.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/uip/timer.o: uip/timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uip 
	@${RM} ${OBJECTDIR}/uip/timer.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uip/timer.c  -o ${OBJECTDIR}/uip/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uip/timer.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/uip/timer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/uip/uip-fw.o: uip/uip-fw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uip 
	@${RM} ${OBJECTDIR}/uip/uip-fw.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uip/uip-fw.c  -o ${OBJECTDIR}/uip/uip-fw.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uip/uip-fw.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/uip/uip-fw.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/uip/uip-neighbor.o: uip/uip-neighbor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uip 
	@${RM} ${OBJECTDIR}/uip/uip-neighbor.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uip/uip-neighbor.c  -o ${OBJECTDIR}/uip/uip-neighbor.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uip/uip-neighbor.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/uip/uip-neighbor.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/uip/uip-split.o: uip/uip-split.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uip 
	@${RM} ${OBJECTDIR}/uip/uip-split.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uip/uip-split.c  -o ${OBJECTDIR}/uip/uip-split.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uip/uip-split.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/uip/uip-split.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/uip/uip.o: uip/uip.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uip 
	@${RM} ${OBJECTDIR}/uip/uip.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uip/uip.c  -o ${OBJECTDIR}/uip/uip.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uip/uip.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/uip/uip.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/uip/uip_arp.o: uip/uip_arp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uip 
	@${RM} ${OBJECTDIR}/uip/uip_arp.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uip/uip_arp.c  -o ${OBJECTDIR}/uip/uip_arp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uip/uip_arp.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/uip/uip_arp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/uip/uiplib.o: uip/uiplib.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/uip 
	@${RM} ${OBJECTDIR}/uip/uiplib.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  uip/uiplib.c  -o ${OBJECTDIR}/uip/uiplib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/uip/uiplib.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/uip/uiplib.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/tcp/tcp_server.o: tcp/tcp_server.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/tcp 
	@${RM} ${OBJECTDIR}/tcp/tcp_server.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  tcp/tcp_server.c  -o ${OBJECTDIR}/tcp/tcp_server.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/tcp/tcp_server.o.d"      -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/tcp/tcp_server.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_v2.0.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    p24FJ64GB002.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_v2.0.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PICKIT2=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_v2.0.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   p24FJ64GB002.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_v2.0.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}/xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_v2.0.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf 
	
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
