# This makefile contains the lookup path for all the external dependencies to
# toolchains and libraries.


################# helper functions ##############

# Finds the first occurence of a file under a search path. Returns the first
# entry in the directories list that has file under it, or empty string if none
# found.
# $(1): file or dir to look for, $(2); list of paths
findfirst=$(firstword $(foreach dir,$(2),$(if $(wildcard $(dir)/$(1)),$(wildcard $(dir)))))

# Finds missing dependencies from a list.
#
# Accepts as $(1) a list of variable NAMEs. Returns a string containing the
# name of those variables that have no or empty value. Returns an empty string
# if all dependencies are met.
#
# Usage:
# DEPS += TOOLPATH FREERTOSPATH
# MISSING_DEPS:=$(call find_missing_deps,$(DEPS))
# ifneq (,$(MISSING_DEPS))
# all:
# 	@echo missing dependencies: $(MISSING_DEPS)
# else
# all: build-deps
# endif
find_missing_deps=$(strip $(foreach depvar,$(1),$(if $(value $(depvar)),,$(depvar))))

################# mbed library ##################

ifndef MBEDPATH
SEARCHPATH := \
  $(HOME)/lpc-workspace/libmbed_2387/mbed \
  /opt/mbed/default/libraries \


TRYPATH:=$(call findfirst,USBDevice,$(SEARCHPATH))
ifneq ($(TRYPATH),)
MBEDPATH:=$(TRYPATH)
endif
endif #ifndef MBED_PATH

################### FreeRTOS ####################
ifndef FREERTOSPATH
SEARCHPATH := \
  /opt/FreeRTOS \
  /opt/FreeRTOS/default \
  $(HOME)/FreeRTOS \

TRYPATH:=$(call findfirst,Source,$(SEARCHPATH))
ifneq ($(TRYPATH),)
FREERTOSPATH:=$(TRYPATH)
endif
endif #FREERTOSPATH

################# lpcxpresso ####################
ifndef LPCXPRESSOPATH
SEARCHPATH := \
  /usr/local/lpcxpresso_*/lpcxpresso \

TRYPATH:=$(call findfirst,tools/bin,$(SEARCHPATH))
ifneq ($(TRYPATH),)
LPCXPRESSOPATH:=$(TRYPATH)
endif
endif #LPCXPRESSOPATH

################## checksum #####################
ifndef CHECKSUM
SEARCHPATH := \
  $(LPCXPRESSOPATH)/bin \

TRYPATH:=$(call findfirst,checksum,$(SEARCHPATH))
ifneq ($(TRYPATH),)
CHECKSUM:=$(TRYPATH)/checksum
endif
endif #CHECKSUM

ifndef CHECKSUM
CHECKSUM:=@echo No CHECKSUM binary available - skipping writing header checksum. It is possible that the MCU will drop into bootloader when using this binary. \#
endif

################### ARM-GCC #####################
ifndef ARMGCCPATH
SEARCHPATH := \
  /opt/armgcc/default \

TRYPATH:=$(call findfirst,bin/arm-none-eabi-g++,$(SEARCHPATH))
ifneq ($(TRYPATH),)
ARMGCCPATH:=$(TRYPATH)
endif
endif #ARMGCCPATH

############### CMSIS-LPC11xx ###################
ifndef CMSIS_LPC11_PATH
SEARCHPATH := \
  $(HOME)/lpc-workspace/CMSISv2p00_LPC11xx \

TRYPATH:=$(call findfirst,inc/LPC11xx.h,$(SEARCHPATH))
ifneq ($(TRYPATH),)
CMSIS_LPC11_PATH:=$(TRYPATH)
endif
endif #CMSIS_LPC11_PATH


############### GMOCK ###################
ifndef GMOCKPATH
SEARCHPATH := \
  /opt/gmock/gmock-1.6.0 \
  /usr \

TRYPATH:=$(call findfirst,include/gmock/gmock.h,$(SEARCHPATH))
ifneq ($(TRYPATH),)
GMOCKPATH:=$(TRYPATH)
endif
endif #GMOCKPATH

ifndef GMOCKSRCPATH
SEARCHPATH := \
  $(GMOCKPATH) \
  /usr/src/gmock \
  /opt/gmock/gmock-1.6.0 \

TRYPATH:=$(call findfirst,src/gmock-all.cc,$(SEARCHPATH))
ifneq ($(TRYPATH),)
GMOCKSRCPATH:=$(TRYPATH)
endif
endif #GMOCKSRCPATH


############### GTEST ###################
ifndef GTESTPATH
SEARCHPATH := \
  $(GMOCKPATH)/gtest \
  /opt/gmock/gmock-1.6.0/gtest \
  /opt/gtest/gtest-1.6.0 \
  /usr \

TRYPATH:=$(call findfirst,include/gtest/gtest.h,$(SEARCHPATH))
ifneq ($(TRYPATH),)
GTESTPATH:=$(TRYPATH)
endif
endif #GTESTPATH

ifndef GTESTSRCPATH
SEARCHPATH := \
  $(GTESTPATH) \
  /opt/gmock/gmock-1.6.0/gtest \
  /usr/src/gtest \
  /opt/gtest/gtest-1.6.0 \

TRYPATH:=$(call findfirst,src/gtest-all.cc,$(SEARCHPATH))
ifneq ($(TRYPATH),)
GTESTSRCPATH:=$(TRYPATH)
endif
endif #GTESTSRCPATH