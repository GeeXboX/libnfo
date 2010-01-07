ifeq (,$(wildcard ../config.mak))
$(error "../config.mak is not present, run configure !")
endif
include ../config.mak

LIBNAME = libnfo
STATIC_LIBNAME = $(LIBNAME).a

ifeq ($(BUILD_STATIC),yes)
  BUILD_RULES += lib_static
endif
ifeq ($(BUILD_SHARED),yes)
  BUILD_RULES += lib_shared
  ifeq ($(BUILD_DYLIB),yes)
    SHARED_LIBNAME         = ${LIBNAME}.dylib
    SHARED_LIBNAME_VERSION = $(LIBNAME).$(VERSION).dylib
    SHARED_LIBNAME_MAJOR   = $(LIBNAME).$(shell echo $(VERSION) | cut -f1 -d.).dylib
    SHARED_LIBNAME_FLAGS   = -dynamiclib -Wl,-headerpad_max_install_names,-undefined,dynamic_lookup,-install_name,$(SHARED_LIBNAME_VERSION)
  else
    SHARED_LIBNAME         = $(LIBNAME).so
    SHARED_LIBNAME_VERSION = $(SHARED_LIBNAME).$(VERSION)
    SHARED_LIBNAME_MAJOR   = $(SHARED_LIBNAME).$(shell echo $(VERSION) | cut -f1 -d.)
    SHARED_LIBNAME_FLAGS   = -shared -Wl,-soname,$(SHARED_LIBNAME_MAJOR)
  endif
endif

SRCS =  \
	nfo.c \
	nfo_priv.c \
	nfo_xml_utils.c \
	nfo_xml_parser.c \

EXTRADIST = \
	nfo.h \
	nfo_priv.h \
	nfo_xml_parser.h \
	nfo_xml_utils.h \

OBJS = $(SRCS:.c=.o)

.SUFFIXES: .c .o

all: depend $(BUILD_RULES)

.c.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(OPTFLAGS) -o $@ $<

lib_static: $(STATIC_LIBNAME)

lib_shared: $(SHARED_LIBNAME)

$(STATIC_LIBNAME): $(OBJS)
	$(AR) r $(STATIC_LIBNAME) $(OBJS)
	$(RANLIB) $(STATIC_LIBNAME)

$(SHARED_LIBNAME): $(OBJS)
	$(CC) $(SHARED_LIBNAME_FLAGS) \
	  $(OBJS) $(LDFLAGS) $(EXTRALIBS) -o $(SHARED_LIBNAME_VERSION)
	$(LN) -sf $(SHARED_LIBNAME_VERSION) $(SHARED_LIBNAME_MAJOR)
	$(LN) -sf $(SHARED_LIBNAME_MAJOR) $(SHARED_LIBNAME)

clean:
	rm -f *.o *.a *.so*
	rm -f .depend

install: $(BUILD_RULES)
	$(INSTALL) -d $(libdir)
	[ $(BUILD_STATIC) = yes ] && $(INSTALL) -c $(STATIC_LIBNAME) $(libdir); \
	if [ $(BUILD_SHARED) = yes ]; then \
	  $(INSTALL) -c $(SHARED_LIBNAME_VERSION) $(libdir); \
	  $(LN) -sf $(SHARED_LIBNAME_VERSION) $(libdir)/$(SHARED_LIBNAME_MAJOR); \
	  $(LN) -sf $(SHARED_LIBNAME_MAJOR) $(libdir)/$(SHARED_LIBNAME); \
	fi
	$(INSTALL) -d $(includedir)
	$(INSTALL) -c -m 644 nfo.h $(includedir)

uninstall:
	rm -f $(libdir)/$(STATIC_LIBNAME)
	rm -f $(libdir)/$(SHARED_LIBNAME)*
	rm -f $(includedir)/nfo.h

depend:
	$(CC) -MM $(CFLAGS) $(CPPFLAGS) $(SRCS) 1>.depend

.PHONY: clean depend
.PHONY: uninstall

dist-all:
	cp $(EXTRADIST) $(SRCS) Makefile $(DIST)

.PHONY: dist-all

#
# include dependency files if they exist
#
ifneq ($(wildcard .depend),)
include .depend
endif
