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

DOXYGEN =

ifeq ($(DOC),yes)
  DOXYGEN = doxygen
endif

all: lib $(NFO_READER) $(DOXYGEN)

lib:
	$(MAKE) -C src

$(NFO_READER): lib
	$(CC) $(NFO_READER_SRCS) $(OPTFLAGS) $(CFLAGS) $(EXTRACFLAGS) $(LDFLAGS) -o $(NFO_READER)

doxygen:
ifeq (,$(wildcard DOCS/doxygen))
	PROJECT_NUMBER="$(VERSION)" doxygen DOCS/Doxyfile
endif

clean:
	$(MAKE) -C src clean
	rm -f $(NFO_READER)

distclean: clean
	rm -f config.log
	rm -f config.mak
	rm -f $(PKGCONFIG_FILE)
	rm -rf DOCS/doxygen

install: install-pkgconfig
	$(MAKE) -C src install
	$(INSTALL) -d $(bindir)
	$(INSTALL) -c -m 755 $(NFO_READER) $(bindir)

install-pkgconfig: $(PKGCONFIG_FILE)
	$(INSTALL) -d "$(PKGCONFIG_DIR)"
	$(INSTALL) -m 644 $< "$(PKGCONFIG_DIR)"

uninstall:
	$(MAKE) -C src uninstall
	rm -f $(bindir)/$(NFO_READER)
	rm -f $(PKGCONFIG_DIR)/$(PKGCONFIG_FILE)

.PHONY: clean distclean
.PHONY: install install-pkgconfig uninstall
.PHONY: doxygen
