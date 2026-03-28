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

CFLAGS += -Wall -Wextra -std=c99 -fpic
CFLAGS += -Iinclude

all : $(BUILDDIR)/libtgui.so

$(BUILDDIR)/libtgui.so : $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o $@ $^

$(BUILDDIR)/%.o : %.c
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -o $@ -c $^
	
install : all
	@echo '[install headers]'
	@mkdir -p $(PREFIX)/include/tgui
	@cp include/*.h $(PREFIX)/include/tgui
	@echo '[install libtgui.so]'
	@mkdir -p $(PREFIX)/lib
	@cp $(BUILDDIR)/libtgui.so $(PREFIX)/lib
uninstall :
	rm -fr $(PREFIX)/include/tgui $(PREFIX)/lib/libtgui.so

clean : 
	rm -rf $(BUILDDIR)

.PHONY : install uninstall all clean
