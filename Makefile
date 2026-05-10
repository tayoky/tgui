# automatically generated from tmakegen
# DO NOT EDIT

# tconf might have generated a config.mk
-include config.mk

# a few standard variables
VERSION ?= $(shell git describe --tags --always 2>/dev/null || echo unknown)
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

# ==== libtgui target ====

ALL_libtgui = $(BUILDDIR)/libtgui/libtgui.so
SRC_libtgui = src/action.c src/box.c src/button.c src/canva.c src/color.c src/dropdown.c src/expander.c src/factory.c src/font.c src/grid.c src/icon.c src/image.c src/inputs.c src/label.c src/list.c src/listbase.c src/listmodel.c src/listview.c src/log.c src/main.c src/menuitem.c src/paned.c src/popover.c src/popoverbutton.c src/popovermenu.c src/rect.c src/render.c src/scrollbar.c src/scrolledwindow.c src/separator.c src/slider.c src/stack.c src/stackswitcher.c src/stringfactory.c src/stringlist.c src/style.c src/submenubutton.c src/surface.c src/text.c src/textbuffer.c src/textview.c src/theme.c src/titlebar.c src/tobject.c src/togglebutton.c src/vector.c src/viewport.c src/widget.c src/window.c platform/$(PLATFORM).c
OBJ_libtgui = $(SRC_libtgui:%=$(BUILDDIR)/libtgui/%.o)
DEPS_libtgui = $(SRC_libtgui:%=$(BUILDDIR)/libtgui/%.d)

.PHONY : all-libtgui
all : all-libtgui
all-libtgui : $(ALL_libtgui)

# include dependencies files
-include $(DEPS_libtgui)

.PHONY : install-libtgui
install : install-libtgui
install-libtgui : all-libtgui
	@mkdir -p "$(DESTDIR)$(PREFIX)/lib"
	@echo "INSTALL $(ALL_libtgui)"
	$(Q)cp $(ALL_libtgui) "$(DESTDIR)$(PREFIX)/lib"

.PHONY : uninstall-libtgui
uninstall : uninstall-libtgui
uninstall-libtgui :
	@echo "UNINSTALL $(DESTDIR)$(PREFIX)/lib/libtgui.so"
	$(Q)rm -f "$(DESTDIR)$(PREFIX)/lib/libtgui.so"

.PHONY : clean-libtgui
clean-libtgui :
	@echo "CLEAN $(BUILDDIR)/libtgui"
	$(Q)rm -fr "$(BUILDDIR)/libtgui"

$(BUILDDIR)/libtgui/%.c.o : %.c
	@mkdir -p "$(@D)"
	@echo "CC $<"
	$(Q)$(CC) $(CFLAGS) -fPIC -Iinclude -Iinclude/tgui -o $@ -c $<

$(BUILDDIR)/libtgui/libtgui.so : $(OBJ_libtgui)
	@mkdir -p "$(@D)"
	@echo "CCLD libtgui.so"
	$(Q)$(CC) -shared $(CFLAGS) -fPIC -Iinclude -Iinclude/tgui $(LDFLAGS) -o $@ $^

# ==== headers target ====
SRC_headers = include/tgui
FILES_headers = tgui
DEST_headers = $(FILES_headers:%=$(DESTDIR)$(PREFIX)/include/%)

.PHONY : install-headers
install : install-headers
install-headers :
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

$(BUILDDIR)/tgui-demo/tgui-demo : $(OBJ_tgui-demo) $(ALL_libtgui)
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
	@echo "====== libtgui targets ======"
	@echo "all-libtgui       : build libtgui"
	@echo "install-libtgui   : install libtgui"
	@echo "uninstall-libtgui : uninstall libtgui"
	@echo "clean-libtgui     : clean libtgui"
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
