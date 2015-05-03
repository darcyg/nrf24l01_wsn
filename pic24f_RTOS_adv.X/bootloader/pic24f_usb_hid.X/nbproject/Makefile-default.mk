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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_usb_hid.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_usb_hid.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=src/system_config/app_hid_boot_loader.c src/system_config/system.c src/app_led_usb_status.c src/main.c src/usb_descriptors.c bsp/buttons.c bsp/leds.c ../../framework/usb/src/usb_device.c ../../framework/usb/src/usb_device_hid.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/system_config/app_hid_boot_loader.o ${OBJECTDIR}/src/system_config/system.o ${OBJECTDIR}/src/app_led_usb_status.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/usb_descriptors.o ${OBJECTDIR}/bsp/buttons.o ${OBJECTDIR}/bsp/leds.o ${OBJECTDIR}/_ext/1354599976/usb_device.o ${OBJECTDIR}/_ext/1354599976/usb_device_hid.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/system_config/app_hid_boot_loader.o.d ${OBJECTDIR}/src/system_config/system.o.d ${OBJECTDIR}/src/app_led_usb_status.o.d ${OBJECTDIR}/src/main.o.d ${OBJECTDIR}/src/usb_descriptors.o.d ${OBJECTDIR}/bsp/buttons.o.d ${OBJECTDIR}/bsp/leds.o.d ${OBJECTDIR}/_ext/1354599976/usb_device.o.d ${OBJECTDIR}/_ext/1354599976/usb_device_hid.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/system_config/app_hid_boot_loader.o ${OBJECTDIR}/src/system_config/system.o ${OBJECTDIR}/src/app_led_usb_status.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/usb_descriptors.o ${OBJECTDIR}/bsp/buttons.o ${OBJECTDIR}/bsp/leds.o ${OBJECTDIR}/_ext/1354599976/usb_device.o ${OBJECTDIR}/_ext/1354599976/usb_device_hid.o

# Source Files
SOURCEFILES=src/system_config/app_hid_boot_loader.c src/system_config/system.c src/app_led_usb_status.c src/main.c src/usb_descriptors.c bsp/buttons.c bsp/leds.c ../../framework/usb/src/usb_device.c ../../framework/usb/src/usb_device_hid.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_usb_hid.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ64GB002
MP_LINKER_FILE_OPTION=,--script="linker/hid_boot_p24FJ64GB002.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/system_config/app_hid_boot_loader.o: src/system_config/app_hid_boot_loader.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/system_config 
	@${RM} ${OBJECTDIR}/src/system_config/app_hid_boot_loader.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/system_config/app_hid_boot_loader.c  -o ${OBJECTDIR}/src/system_config/app_hid_boot_loader.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/system_config/app_hid_boot_loader.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -Os -I"src" -I"src/system_config" -I"../../framework" -I"bsp" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/system_config/app_hid_boot_loader.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/system_config/system.o: src/system_config/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/system_config 
	@${RM} ${OBJECTDIR}/src/system_config/system.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/system_config/system.c  -o ${OBJECTDIR}/src/system_config/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/system_config/system.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -Os -I"src" -I"src/system_config" -I"../../framework" -I"bsp" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/system_config/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_led_usb_status.o: src/app_led_usb_status.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/app_led_usb_status.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_led_usb_status.c  -o ${OBJECTDIR}/src/app_led_usb_status.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_led_usb_status.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -Os -I"src" -I"src/system_config" -I"../../framework" -I"bsp" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/app_led_usb_status.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -Os -I"src" -I"src/system_config" -I"../../framework" -I"bsp" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/usb_descriptors.o: src/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/usb_descriptors.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_descriptors.c  -o ${OBJECTDIR}/src/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/usb_descriptors.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -Os -I"src" -I"src/system_config" -I"../../framework" -I"bsp" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/usb_descriptors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/bsp/buttons.o: bsp/buttons.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/bsp 
	@${RM} ${OBJECTDIR}/bsp/buttons.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  bsp/buttons.c  -o ${OBJECTDIR}/bsp/buttons.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/bsp/buttons.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -Os -I"src" -I"src/system_config" -I"../../framework" -I"bsp" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/bsp/buttons.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/bsp/leds.o: bsp/leds.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/bsp 
	@${RM} ${OBJECTDIR}/bsp/leds.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  bsp/leds.c  -o ${OBJECTDIR}/bsp/leds.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/bsp/leds.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -Os -I"src" -I"src/system_config" -I"../../framework" -I"bsp" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/bsp/leds.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1354599976/usb_device.o: ../../framework/usb/src/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1354599976 
	@${RM} ${OBJECTDIR}/_ext/1354599976/usb_device.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../framework/usb/src/usb_device.c  -o ${OBJECTDIR}/_ext/1354599976/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1354599976/usb_device.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -Os -I"src" -I"src/system_config" -I"../../framework" -I"bsp" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1354599976/usb_device.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1354599976/usb_device_hid.o: ../../framework/usb/src/usb_device_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1354599976 
	@${RM} ${OBJECTDIR}/_ext/1354599976/usb_device_hid.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../framework/usb/src/usb_device_hid.c  -o ${OBJECTDIR}/_ext/1354599976/usb_device_hid.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1354599976/usb_device_hid.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -Os -I"src" -I"src/system_config" -I"../../framework" -I"bsp" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1354599976/usb_device_hid.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/src/system_config/app_hid_boot_loader.o: src/system_config/app_hid_boot_loader.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/system_config 
	@${RM} ${OBJECTDIR}/src/system_config/app_hid_boot_loader.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/system_config/app_hid_boot_loader.c  -o ${OBJECTDIR}/src/system_config/app_hid_boot_loader.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/system_config/app_hid_boot_loader.o.d"      -g -omf=elf -Os -I"src" -I"src/system_config" -I"../../framework" -I"bsp" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/system_config/app_hid_boot_loader.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/system_config/system.o: src/system_config/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src/system_config 
	@${RM} ${OBJECTDIR}/src/system_config/system.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/system_config/system.c  -o ${OBJECTDIR}/src/system_config/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/system_config/system.o.d"      -g -omf=elf -Os -I"src" -I"src/system_config" -I"../../framework" -I"bsp" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/system_config/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_led_usb_status.o: src/app_led_usb_status.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/app_led_usb_status.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_led_usb_status.c  -o ${OBJECTDIR}/src/app_led_usb_status.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_led_usb_status.o.d"      -g -omf=elf -Os -I"src" -I"src/system_config" -I"../../framework" -I"bsp" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/app_led_usb_status.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/main.o.d"      -g -omf=elf -Os -I"src" -I"src/system_config" -I"../../framework" -I"bsp" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/usb_descriptors.o: src/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/usb_descriptors.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/usb_descriptors.c  -o ${OBJECTDIR}/src/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/usb_descriptors.o.d"      -g -omf=elf -Os -I"src" -I"src/system_config" -I"../../framework" -I"bsp" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/src/usb_descriptors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/bsp/buttons.o: bsp/buttons.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/bsp 
	@${RM} ${OBJECTDIR}/bsp/buttons.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  bsp/buttons.c  -o ${OBJECTDIR}/bsp/buttons.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/bsp/buttons.o.d"      -g -omf=elf -Os -I"src" -I"src/system_config" -I"../../framework" -I"bsp" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/bsp/buttons.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/bsp/leds.o: bsp/leds.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/bsp 
	@${RM} ${OBJECTDIR}/bsp/leds.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  bsp/leds.c  -o ${OBJECTDIR}/bsp/leds.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/bsp/leds.o.d"      -g -omf=elf -Os -I"src" -I"src/system_config" -I"../../framework" -I"bsp" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/bsp/leds.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1354599976/usb_device.o: ../../framework/usb/src/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1354599976 
	@${RM} ${OBJECTDIR}/_ext/1354599976/usb_device.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../framework/usb/src/usb_device.c  -o ${OBJECTDIR}/_ext/1354599976/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1354599976/usb_device.o.d"      -g -omf=elf -Os -I"src" -I"src/system_config" -I"../../framework" -I"bsp" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1354599976/usb_device.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1354599976/usb_device_hid.o: ../../framework/usb/src/usb_device_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1354599976 
	@${RM} ${OBJECTDIR}/_ext/1354599976/usb_device_hid.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../framework/usb/src/usb_device_hid.c  -o ${OBJECTDIR}/_ext/1354599976/usb_device_hid.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1354599976/usb_device_hid.o.d"      -g -omf=elf -Os -I"src" -I"src/system_config" -I"../../framework" -I"bsp" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1354599976/usb_device_hid.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
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
dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_usb_hid.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    linker/hid_boot_p24FJ64GB002.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_usb_hid.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1  -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PICKIT2=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,-D__BOOTLOADER,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_usb_hid.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   linker/hid_boot_p24FJ64GB002.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_usb_hid.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,-D__BOOTLOADER,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}/xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/pic24f_usb_hid.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf 
	
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
