ifeq (,$(wildcard config.mak))
$(error "config.mak is not present, run configure !")
endif
include config.mak

PKGCONFIG_DIR = $(libdir)/pkgconfig
PKGCONFIG_FILE = libnfo.pc

NFO_READER      = nfo-reader
NFO_READER_SRCS = nfo-reader.c

CFLAGS += -Isrc
LDFLAGS += -Lsrc -lnfo

ifeq ($(BUILD_STATIC),yes)
  LDFLAGS += $(EXTRALIBS)
endif

DISTFILE = libnfo-$(VERSION).tar.bz2

EXTRADIST = \
	AUTHORS \
	configure \
	COPYING \
	README \

SUBDIRS = \
	DOCS \
	src \

all: lib $(NFO_READER) docs

lib:
	$(MAKE) -C src

$(NFO_READER): lib
	$(CC) $(NFO_READER_SRCS) $(OPTFLAGS) $(CFLAGS) $(EXTRACFLAGS) $(LDFLAGS) -o $(NFO_READER)

docs:
	$(MAKE) -C DOCS

docs-clean:
	$(MAKE) -C DOCS clean

clean:
	$(MAKE) -C src clean
	rm -f $(NFO_READER)

distclean: clean docs-clean
	rm -f config.log
	rm -f config.mak
	rm -f $(PKGCONFIG_FILE)

install: install-pkgconfig install-docs
	$(MAKE) -C src install
	$(INSTALL) -d $(bindir)
	$(INSTALL) -c -m 755 $(NFO_READER) $(bindir)

install-docs: docs
	$(MAKE) -C DOCS install

install-pkgconfig: $(PKGCONFIG_FILE)
	$(INSTALL) -d "$(PKGCONFIG_DIR)"
	$(INSTALL) -m 644 $< "$(PKGCONFIG_DIR)"

uninstall: uninstall-docs
	$(MAKE) -C src uninstall
	rm -f $(bindir)/$(NFO_READER)
	rm -f $(PKGCONFIG_DIR)/$(PKGCONFIG_FILE)

uninstall-docs:
	$(MAKE) -C DOCS uninstall

.PHONY: clean distclean
.PHONY: *install*
.PHONY: docs

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
