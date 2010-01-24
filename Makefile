ifeq (,$(wildcard config.mak))
$(error "config.mak is not present, run configure !")
endif
include config.mak

PKGCONFIG_DIR = $(libdir)/pkgconfig
PKGCONFIG_FILE = libnfo.pc

NFO_READER      = libnfo-reader
NFO_READER_SRCS = libnfo-reader.c
NFO_READER_OBJS = $(NFO_READER_SRCS:.c=.o)
NFO_READER_MAN  = $(NFO_READER).1

MANS = $(NFO_READER_MAN)

override CPPFLAGS += -Isrc
override LDFLAGS += -Lsrc -lnfo

ifeq ($(BUILD_STATIC),yes)
  override LDFLAGS += $(EXTRALIBS)
endif

DISTFILE = libnfo-$(VERSION).tar.bz2

EXTRADIST = \
	AUTHORS \
	ChangeLog \
	configure \
	COPYING \
	README \
	$(MANS) \

SUBDIRS = \
	DOCS \
	src \

.SUFFIXES: .c .o

all: lib apps docs

.c.o:
	$(CC) -c $(OPTFLAGS) $(CFLAGS) $(CPPFLAGS) -o $@ $<

config.mak: configure
	@echo "############################################################"
	@echo "####### Please run ./configure again - it's changed! #######"
	@echo "############################################################"

lib:
	$(MAKE) -C src

$(NFO_READER): $(NFO_READER_OBJS)
	$(CC) $(NFO_READER_OBJS) $(LDFLAGS) -o $(NFO_READER)

apps-dep:
	$(CC) -MM $(CFLAGS) $(CPPFLAGS) $(NFO_READER_SRCS) 1>.depend

apps: apps-dep lib
	$(MAKE) $(NFO_READER)

docs:
	$(MAKE) -C DOCS

docs-clean:
	$(MAKE) -C DOCS clean

clean:
	$(MAKE) -C src clean
	rm -f *.o
	rm -f $(NFO_READER)
	rm -f .depend

distclean: clean docs-clean
	rm -f config.log
	rm -f config.mak
	rm -f $(DISTFILE)
	rm -f $(PKGCONFIG_FILE)

install: install-lib install-pkgconfig install-apps install-docs install-man

install-lib: lib
	$(MAKE) -C src install

install-pkgconfig: $(PKGCONFIG_FILE)
	$(INSTALL) -d "$(PKGCONFIG_DIR)"
	$(INSTALL) -m 644 $< "$(PKGCONFIG_DIR)"

install-apps: apps
	$(INSTALL) -d $(bindir)
	$(INSTALL) -c -m 755 $(NFO_READER) $(bindir)

install-docs: docs
	$(MAKE) -C DOCS install

install-man: $(MANS)
	for m in $(MANS); do \
	  section=`echo $$m | sed -e 's/^.*\\.//'`; \
	  $(INSTALL) -d $(mandir)/man$$section; \
	  $(INSTALL) -m 644 $$m $(mandir)/man$$section; \
	done

uninstall: uninstall-lib uninstall-pkgconfig uninstall-apps uninstall-docs uninstall-man

uninstall-lib:
	$(MAKE) -C src uninstall

uninstall-pkgconfig:
	rm -f $(PKGCONFIG_DIR)/$(PKGCONFIG_FILE)

uninstall-apps:
	rm -f $(bindir)/$(NFO_READER)

uninstall-docs:
	$(MAKE) -C DOCS uninstall

uninstall-man:
	for m in $(MANS); do \
	  section=`echo $$m | sed -e 's/^.*\\.//'`; \
	  rm -f $(mandir)/man$$section/$$m; \
	done

.PHONY: *clean *install* docs apps*

dist:
	-$(RM) $(DISTFILE)
	dist=$(shell pwd)/libnfo-$(VERSION) && \
	for subdir in . $(SUBDIRS); do \
		mkdir -p "$$dist/$$subdir"; \
		$(MAKE) -C $$subdir dist-all DIST="$$dist/$$subdir"; \
	done && \
	tar cjf $(DISTFILE) libnfo-$(VERSION)
	-$(RM) -rf libnfo-$(VERSION)

dist-all:
	cp $(EXTRADIST) $(NFO_READER_SRCS) Makefile $(DIST)

.PHONY: dist dist-all

#
# include dependency files if they exist
#
ifneq ($(wildcard .depend),)
include .depend
endif
