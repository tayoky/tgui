MAKEFLAGS += --no-builtin-rules

include config.mk

OS = $(word 2, $(subst -, ,$(HOST)))
BUILDDIR = build
SRC = $(wildcard src/*.c) platform/$(PLATFORM).c
OBJ = $(SRC:%.c=$(BUILDDIR)/%.o)

ifeq ($(OS),stanix)
	PLATFORM = twm
else
	PLATFORM = x11
endif

# need to link with some libs
ifeq ($(PLATFORM),x11)
	LDFLAGS += -lX11 -lXft
	CFLAGS += -I$(PREFIX)/include/freetype2
endif
ifeq ($(PLATFORM),twm)
	LDFLAGS += -ltwm -lgfx
endif

CFLAGS += -Wall -Wextra -std=c99
CFLAGS += -Iinclude -Iinclude/tgui
DYNFLAGS = -fpic

all : $(BUILDDIR)/libtgui.so $(BUILDDIR)/tgui-demo

$(BUILDDIR)/libtgui.so : $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o $@ $^

$(BUILDDIR)/tgui-demo : $(BUILDDIR)/libtgui.so $(BUILDDIR)/test/demo.o
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(BUILDDIR)/test/demo.o -L$(BUILDDIR) -ltgui

$(BUILDDIR)/%.o : %.c
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -o $@ -c $^

$(BUILDDIR)/src/%.o : CFLAGS += $(DYNFLAGS)
$(BUILDDIR)/platform/%.o : CFLAGS += $(DYNFLAGS)
	
install : all
	@echo '[install headers]'
	@mkdir -p $(PREFIX)/include/tgui
	@cp include/tgui/*.h $(PREFIX)/include/tgui
	@echo '[install libtgui.so]'
	@mkdir -p $(PREFIX)/lib
	@cp $(BUILDDIR)/libtgui.so $(PREFIX)/lib
	@echo '[install tgui-demo]'
	@mkdir -p $(PREFIX)/bin
	@cp $(BUILDDIR)/tgui-demo $(PREFIX)/bin

uninstall :
	rm -fr $(PREFIX)/include/tgui $(PREFIX)/lib/libtgui.so

clean : 
	rm -rf $(BUILDDIR)

.PHONY : install uninstall all clean
