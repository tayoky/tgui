TOP ?= $(CURDIR)/..
TMAKE_DIR ?= $(TOP)/make
include $(TMAKE_DIR)/tmake-init.mk

PROG = tgui-demo
SRCS = demo.c
CFLAGS += -I$(TOP)/include
LDFLAGS = -L$(BUILDDIR)/..  -ltgui
include $(TMAKE_DIR)/tmake-prog.mk 
