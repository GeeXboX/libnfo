ifeq (,$(wildcard ../config.mak))
$(error "../config.mak is not present, run configure !")
endif
include ../config.mak

all: doxygen

doxygen:
ifeq ($(DOC),yes)
ifeq (,$(wildcard doxygen))
	PROJECT_NUMBER="$(VERSION)" doxygen Doxyfile
endif
endif

clean:
	rm -rf doxygen

install:
ifeq ($(DOC),yes)
	if [ -d doxygen/html ]; then \
		$(INSTALL) -d $(docdir)/libnfo; \
		$(INSTALL) -c -m 755 doxygen/html/* $(docdir)/libnfo; \
	fi
endif

uninstall:
	rm -rf $(docdir)/libnfo

.PHONY: clean doxygen *install
