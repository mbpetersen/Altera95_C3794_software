#START GENERATED

# The following TYPE comment allows tools to identify the 'type' of target this 
# makefile is associated with. 
# TYPE: APP_MAKEFILE

# This following VERSION comment indicates the version of the tool used to 
# generate this makefile. A makefile variable is provided for VERSION as well. 
# ACDS_VERSION: 11.1sp2
ACDS_VERSION := 11.1sp2

# This following BUILD_NUMBER comment indicates the build number of the tool 
# used to generate this makefile. 
# BUILD_NUMBER: 259

# Define path to the application ELF. 
# It may be used by the makefile fragments so is defined before including them. 
# 
ELF := GIGLE10.elf

# Paths to C, C++, and assembly source files.
C_SRCS += main.c
C_SRCS += link_task.c
C_SRCS += cfg_hw.c
C_SRCS += functions.c
C_SRCS += gui_link.c
C_SRCS += helpers.c
C_SRCS += init.c
C_SRCS += alt_2_wire.c
C_SRCS += config.c
C_SRCS += intr.c
C_SRCS += results.c
C_SRCS += sfp.c
C_SRCS += status.c
C_SRCS += flash.c
C_SRCS += config10g.c
C_SRCS += packet10g.c
C_SRCS += cfg10_hw.c
C_SRCS += cfg_frame.c
C_SRCS += config_multistream.c
C_SRCS += update_headers.c
C_SRCS += loopback.c
C_SRCS += c37dot94.c
C_SRCS += BERTP.C
C_SRCS += onesec_alrm.c
C_SRCS += c37dot94bert.c
C_SRCS += cfg_c37.c
C_SRCS += c37dot94_task.c
C_SRCS += onesec_task.c
CXX_SRCS :=
ASM_SRCS :=


# Path to root of object file tree.
OBJ_ROOT_DIR := obj

# Options to control objdump.
CREATE_OBJDUMP := 1
OBJDUMP_INCLUDE_SOURCE := 1
OBJDUMP_FULL_CONTENTS := 0

# Options to enable/disable optional files.
CREATE_ELF_DERIVED_FILES := 0
CREATE_LINKER_MAP := 1

# Common arguments for ALT_CFLAGSs
APP_CFLAGS_DEFINED_SYMBOLS :=
APP_CFLAGS_UNDEFINED_SYMBOLS :=
APP_CFLAGS_OPTIMIZATION := -O0
APP_CFLAGS_DEBUG_LEVEL :=
APP_CFLAGS_WARNINGS := -Wall
APP_CFLAGS_USER_FLAGS :=

APP_ASFLAGS_USER :=
APP_LDFLAGS_USER :=

# Linker options that have default values assigned later if not
# assigned here.
LINKER_SCRIPT :=
CRT0 :=
SYS_LIB :=

# Define path to the root of the BSP.
BSP_ROOT_DIR := ../GIGLE10_bsp

# List of application specific include directories, library directories and library names
APP_INCLUDE_DIRS := GIGLE_Includes/
APP_LIBRARY_DIRS :=
APP_LIBRARY_NAMES :=

# Pre- and post- processor settings.
BUILD_PRE_PROCESS :=
BUILD_POST_PROCESS :=

QUARTUS_PROJECT_DIR := ../../


#END GENERATED
