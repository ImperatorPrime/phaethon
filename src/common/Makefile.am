include $(top_srcdir)/Makefile.common

# Pass the git version information to the compiler

VER_REVDESC = $(shell cat $(top_srcdir)/gitstamp/gitstamp)
VERSION_FLAGS = $(shell if test x$(VER_REVDESC) != "x"; then echo "-DPHAETHON_REVDESC=\\\"$(VER_REVDESC)\\\""; fi)

VER_REV = $(shell cat $(top_srcdir)/gitstamp/gitstamp | cut -d '+' -f 2-)
VERSION_FLAGS += $(shell if test x$(VER_REV) != "x"; then echo "-DPHAETHON_REV=\\\"$(VER_REV)\\\""; fi)

VER_DATE = $(shell date -u '+%Y-%m-%dT%H:%M:%S')
VERSION_FLAGS += $(shell if test x$(VER_DATE) != "x"; then echo "-DPHAETHON_BUILDDATE=\\\"$(VER_DATE)\\\""; fi)

AM_CXXFLAGS += $(VERSION_FLAGS)
AM_CFLAGS   += $(VERSION_FLAGS)

# Rebuild the version.lo when the git version changed
version.lo: $(top_srcdir)/gitstamp/gitstamp

SUBDIRS = \
          $(EMPTY)

noinst_LTLIBRARIES = libcommon.la

noinst_HEADERS = \
                 endianness.h \
                 system.h \
                 types.h \
                 util.h \
                 maths.h \
                 noncopyable.h \
                 singleton.h \
                 error.h \
                 ustring.h \
                 stream.h \
                 file.h \
                 version.h \
                 hash.h \
                 filepath.h \
                 filelist.h \
                 filetree.h \
                 zipfile.h \
                 bitstream.h \
                 huffman.h \
                 sinewindows.h \
                 cosinetables.h \
                 fft.h \
                 mdct.h \
                 mutex.h \
                 thread.h \
                 $(EMPTY)

libcommon_la_SOURCES = \
                       util.cpp \
                       maths.cpp \
                       error.cpp \
                       ustring.cpp \
                       stream.cpp \
                       file.cpp \
                       version.cpp \
                       filepath.cpp \
                       filelist.cpp \
                       filetree.cpp \
                       zipfile.cpp \
                       huffman.cpp \
                       sinewindows.cpp \
                       cosinetables.cpp \
                       fft.cpp \
                       mdct.cpp \
                       mutex.cpp \
                       thread.cpp \
                       $(EMPTY)

libcommon_la_LIBADD = \
                      $(EMPTY)
