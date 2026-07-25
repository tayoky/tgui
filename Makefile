TOP = $(CURDIR)
TMAKE_DIR = $(TOP)/make
include $(TMAKE_DIR)/tmake-init.mk

LIB = tgui
INCS = include/tgui
SRCS = $(wildcard src/*.c) platform/$(PLATFORM).c
CFLAGS += -Iinclude/tgui
include $(TMAKE_DIR)/tmake-lib.mk 

SUBDIRS = test
include $(TMAKE_DIR)/tmake-subdir.mk 

all-test : $(BUILDDIR)/$(SHARED_LIB)
