## src/Pcode/Pomega/Makefile.am
## This file is (ultimately) included by the top level Makefile.am, so all
## files need to be specified relative to the top level directory.

## List the files generated in this directory.  Binaries go under _PRG,
## libraries under _LIB.  Higher level Makefiles use these variables
## unconditionally, so both _PRG and _LIB need to be defined.
src_Pcode_Pomega_PRG = src/Pcode/Pomega/Pomega
src_Pcode_Pomega_LIB = 
src_Pcode_Pomega_DST = src/Pcode/Pomega/doxygen_config.doc \
	src/Pcode/Pomega/doxygen_config.tag

## A target to support building everything under this subdirectory.  This
## should refer to anything defined above.  There should be two targets; one
## with and one without the trailing slash.  The trailing slash will allow
## the use of tab completion.
src/Pcode/Pomega src/Pcode/Pomega/: \
	include $(src_Pcode_Pomega_PRG) $(src_Pcode_Pomega_LIB)

## A target to support cleaning everything under this subdirectory.
src/Pcode/Pomega/clean:
	rm -f $(top_builddir)/src/Pcode/Pomega/*.$(OBJEXT)
	rm -f $(src_Pcode_Pomega_PRG)

## List all source and header files under _SOURCES.  
src_Pcode_Pomega_Pomega_SOURCES = \
	src/Pcode/Pomega/do_pcode.c

## A target to build the tag file for this subdirectory's documentation.
## The tag file contains information on every documented symbol so that
## documentation in other directories can be linked to documentation in
## this one.
$(srcdir)/src/Pcode/Pomega/Pomega.tag: \
		$(srcdir)/src/Pcode/Pomega/doxygen_config.tag \
		$(src_Pcode_Pomega_SOURCES)
	cd $(srcdir)/src/Pcode/Pomega && doxygen doxygen_config.tag

## A target to build all documentation under this subdirectory.  This
## build the human readable documentation and links it to documentation
## in other directories.
src/Pcode/Pomega/doc $(srcdir)/src/Pcode/Pomega/html \
	$(srcdir)/src/Pcode/Pomega/latex: \
		$(srcdir)/src/Pcode/Pomega/doxygen_config.doc \
		$(srcdir)/src/Pcode/Pcode/libpcode.tag \
		$(srcdir)/src/Pcode/Plib_DD/libpcode_dd.tag \
		$(srcdir)/src/Pcode/Plib_SS/libpcode_ssa.tag \
		$(srcdir)/src/Pcode/Plib_CF/libpcode_cf.tag \
		$(srcdir)/src/Pcode/Plib_probe/libpcode_probe.tag \
		$(srcdir)/src/library/libimpact.tag
	cd $(srcdir)/src/Pcode/Pomega && doxygen doxygen_config.doc

## List all include files that should be symlinked into include under 
## noinst_..._INC.  These include files are not installed when the user
## does 'make install'.  This is appropriate for most include files.  If
## you have headers that need to be installed, see src/library/Makefile.am
## for an example.
## noinst_src_Pcode_Pomega_INC = 

## The class for the include files.  This is the directory under the include
## directory where the symlinks will be created.  This needs to have the same
## variable name as the _INC variable, only with a _CLASS suffix.
## noinst_src_Pcode_Pomega_CLASS = 

## List any libraries needed when linking under _LDADD.  If a library is built
## as part of impact, list it as $(top_builddir)/lib/<library> so that it can
## be detected as a dependency automatically.  Automake assumes libraries
## listed as -l<lib> are system libraries and does not detect them as
## dependencies.
src_Pcode_Pomega_Pomega_LDADD = -L$(top_builddir)/lib -lm \
	$(top_builddir)/lib/libpcode_dd.a \
	$(top_builddir)/lib/libpcode_ssa.a \
	$(top_builddir)/lib/libpcode_cf.a \
	$(top_builddir)/lib/libpcode_probe.a \
	$(top_builddir)/lib/libpcode.a \
	$(top_builddir)/lib/libimpact.a

## List any dependencies other than libraries under _DEPENDENDIES.  Defining
## this variable disables automatic dependency detection for this target, so
## you will need to relist dependencies that appear in _LDADD.
## src_Pcode_Pomega_DEPENDENCIES = 

## List any special compiler flags under _CFLAGS.
## src_Pcode_Pomega_CFLAGS = 
