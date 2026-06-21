# automatically generated from tmakegen
# DO NOT EDIT
MAKEFLAGS += --no-builtin-rules

# tconf might have generated a config.mk
-include config.mk

# a few standard variables
VERSION := $(shell git describe --tags --always 2>/dev/null || echo unknown)
NAME ?= tgui
BUILDDIR ?= build
PREFIX ?= /usr/local
STATIC ?= yes
SHARED ?= no
CFLAGS ?= -Wall -Wextra
CFLAGS += -DVERSION='"$(VERSION)"'

ifeq ($(HAVE_MMD) $(HAVE_MP),yes yes)
	CFLAGS += -MMD -MP
endif

ifeq ($(V),1)
	Q =
else
	Q = @
endif

.PHONY : all
all :

.PHONY : install
install :

.PHONY : uninstall
uninstall :

# ==== shared-libtgui target ====

ALL_shared-libtgui =
LINK_shared-libtgui =
ifeq ($(SHARED),yes)
ALL_shared-libtgui += $(BUILDDIR)/shared-libtgui/libtgui.so
LINK_shared-libtgui = $(BUILDDIR)/shared-libtgui/libtgui.so
ALL_libtgui += $(BUILDDIR)/shared-libtgui/libtgui.so
LINK_libtgui = $(BUILDDIR)/shared-libtgui/libtgui.so
SRC_shared-libtgui = src/action.c src/box.c src/button.c src/canva.c src/checkbox.c src/color.c src/dropdown.c src/expander.c src/factory.c src/font.c src/grid.c src/icon.c src/image.c src/inputs.c src/label.c src/list.c src/listbase.c src/listmodel.c src/listview.c src/log.c src/main.c src/menuitem.c src/paned.c src/popover.c src/popoverbutton.c src/popovermenu.c src/rect.c src/render.c src/scrollbar.c src/scrolledwindow.c src/separator.c src/slider.c src/stack.c src/stackswitcher.c src/stringfactory.c src/stringlist.c src/style.c src/submenubutton.c src/surface.c src/text.c src/textbuffer.c src/textview.c src/theme.c src/titlebar.c src/tobject.c src/togglebutton.c src/vector.c src/viewport.c src/widget.c src/window.c platform/$(PLATFORM).c
OBJ_shared-libtgui = $(SRC_shared-libtgui:%=$(BUILDDIR)/shared-libtgui/%.o)
DEPS_shared-libtgui = $(SRC_shared-libtgui:%=$(BUILDDIR)/shared-libtgui/%.d)

.PHONY : all-shared-libtgui
all : all-shared-libtgui
all-shared-libtgui : $(ALL_shared-libtgui)

# include dependencies files
-include $(DEPS_shared-libtgui)

.PHONY : install-shared-libtgui
install : install-shared-libtgui
install-shared-libtgui : all-shared-libtgui
	@mkdir -p "$(DESTDIR)$(PREFIX)/lib"
	@echo "INSTALL $(ALL_shared-libtgui)"
	$(Q)cp $(ALL_shared-libtgui) "$(DESTDIR)$(PREFIX)/lib"

.PHONY : uninstall-shared-libtgui
uninstall : uninstall-shared-libtgui
uninstall-shared-libtgui :
	@echo "UNINSTALL $(DESTDIR)$(PREFIX)/lib/libtgui.so"
	$(Q)rm -f "$(DESTDIR)$(PREFIX)/lib/libtgui.so"

.PHONY : clean-shared-libtgui
clean-shared-libtgui :
	@echo "CLEAN $(BUILDDIR)/shared-libtgui"
	$(Q)rm -fr "$(BUILDDIR)/shared-libtgui"

$(BUILDDIR)/shared-libtgui/%.c.o : %.c
	@mkdir -p "$(@D)"
	@echo "CC $<"
	$(Q)$(CC) $(CFLAGS) -fPIC -Iinclude -Iinclude/tgui -o $@ -c $<

$(BUILDDIR)/shared-libtgui/libtgui.so : $(OBJ_shared-libtgui)
	@mkdir -p "$(@D)"
	@echo "CCLD libtgui.so"
	$(Q)$(CC) -shared $(CFLAGS) -fPIC -Iinclude -Iinclude/tgui $(LDFLAGS) -lm -o $@ $^

.PHONY : all-libtgui
all-libtgui : all-shared-libtgui

.PHONY : install-libtgui
install-libtgui : install-shared-libtgui

.PHONY : uninstall-libtgui
uninstall-libtgui : uninstall-shared-libtgui

.PHONY : clean-libtgui
clean-libtgui : clean-shared-libtgui
endif

# ==== headers target ====
SRC_headers = include/tgui
FILES_headers = $(shell for I in $(SRC_headers) ; do basename "$$I"; done)
DEST_headers = $(FILES_headers:%=$(DESTDIR)$(PREFIX)/include/%)

.PHONY : install-headers
install : install-headers
install-headers : $(SRC_headers)
	@mkdir -p "$(DESTDIR)$(PREFIX)/include"
	@echo "INSTALL_DATA $(SRC_headers)"
	$(Q)cp -r $(SRC_headers) "$(DESTDIR)$(PREFIX)/include"

.PHONY : uninstall-headers
uninstall : uninstall-headers
uninstall-headers :
	@echo "UNINSTALL $(DEST_headers)"
	$(Q)rm -fr $(DEST_headers)

# ==== tgui-demo target ====

ALL_tgui-demo = $(BUILDDIR)/tgui-demo/tgui-demo
SRC_tgui-demo = test/demo.c
OBJ_tgui-demo = $(SRC_tgui-demo:%=$(BUILDDIR)/tgui-demo/%.o)
DEPS_tgui-demo = $(SRC_tgui-demo:%=$(BUILDDIR)/tgui-demo/%.d)

.PHONY : all-tgui-demo
all : all-tgui-demo
all-tgui-demo : $(ALL_tgui-demo)

# include dependencies files
-include $(DEPS_tgui-demo)

.PHONY : install-tgui-demo
install : install-tgui-demo
install-tgui-demo : all-tgui-demo install-libtgui
	@mkdir -p "$(DESTDIR)$(PREFIX)/bin"
	@echo "INSTALL $(ALL_tgui-demo)"
	$(Q)cp $(ALL_tgui-demo) "$(DESTDIR)$(PREFIX)/bin"

.PHONY : uninstall-tgui-demo
uninstall : uninstall-tgui-demo
uninstall-tgui-demo :
	@echo "UNINSTALL $(DESTDIR)$(PREFIX)/bin/tgui-demo"
	$(Q)rm -f "$(DESTDIR)$(PREFIX)/bin/tgui-demo"

.PHONY : clean-tgui-demo
clean-tgui-demo :
	@echo "CLEAN $(BUILDDIR)/tgui-demo"
	$(Q)rm -fr "$(BUILDDIR)/tgui-demo"

$(BUILDDIR)/tgui-demo/%.c.o : %.c
	@mkdir -p "$(@D)"
	@echo "CC $<"
	$(Q)$(CC) $(CFLAGS) -Iinclude -o $@ -c $<

$(BUILDDIR)/tgui-demo/tgui-demo : $(LINK_libtgui) $(OBJ_tgui-demo)
	@mkdir -p "$(@D)"
	@echo "CCLD tgui-demo"
	$(Q)$(CC) $(CFLAGS) -Iinclude $(LDFLAGS) -o $@ $^

.PHONY : targets
targets :
	@echo "====== tgui targets ======"
	@echo "====== globals targets ======"
	@echo "all       : build every component"
	@echo "install   : install every component"
	@echo "uninstall : uninstall every component"
	@echo "clean     : clean every component"
	@echo "====== shared-libtgui targets ======"
	@echo "all-shared-libtgui       : build shared-libtgui"
	@echo "install-shared-libtgui   : install shared-libtgui"
	@echo "uninstall-shared-libtgui : uninstall shared-libtgui"
	@echo "clean-shared-libtgui     : clean shared-libtgui"
	@echo "====== tgui-demo targets ======"
	@echo "all-tgui-demo       : build tgui-demo"
	@echo "install-tgui-demo   : install tgui-demo"
	@echo "uninstall-tgui-demo : uninstall tgui-demo"
	@echo "clean-tgui-demo     : clean tgui-demo"

Makefile : tmakegen tmake.sh
	@echo "GEN Makefile"
	$(Q)./tmakegen

.PHONY : clean
clean :
	@echo "CLEAN $(BUILDDIR)"
	$(Q)rm -fr "$(BUILDDIR)"
