## intrinsic.lib/Makefile.am
## This file is (ultimately) included by the top level Makefile.am, so all
## files need to be specified relative to the top level directory.

intrinsic_lib_DST = intrinsic.lib/ETSI_convert.h intrinsic.lib/intrinsic.h \
		    intrinsic.lib/intrinsic.c intrinsic.lib/typedef.h

intrinsic_libdir = $(prefix)/intrinsic.lib

intrinsic_lib_DATA = intrinsic.lib/intrinsic_db.hmdes2pp \
		     intrinsic.lib/intrinsic_db.lmdes2
dist_intrinsic_lib_DATA = intrinsic.lib/intrinsic_db

## Specify the files that are created by this make file so that 'make clean'
## will delete them properly.
intrinsic_lib_CLN = $(intrinsic_lib_DATA)

## A target to support building everything under this subdirectory.  This
## should refer to anything defined above.  There should be two targets; one
## with and one without the trailing slash.  The trailing slash will allow
## the use of tab completion.
intrinsic.lib intrinsic.lib/: $(intrinsic_lib_DATA)

## A target to support cleaning everything under this subdirectory.
intrinsic.lib/clean:
	rm -f $(intrinsic_lib_CLN)

intrinsic.lib/intrinsic_db.hmdes2pp: $(abs_srcdir)/intrinsic.lib/intrinsic_db \
		bin/md_preprocessor
	mkdir -p intrinsic.lib
	md_preprocessor $(abs_srcdir)/intrinsic.lib/intrinsic_db \
		-o intrinsic.lib/intrinsic_db.hmdes2pp

intrinsic.lib/intrinsic_db.lmdes2: intrinsic.lib/intrinsic_db.hmdes2pp \
		bin/md_compiler
	md_compiler intrinsic.lib/intrinsic_db.hmdes2pp \
		-o intrinsic.lib/intrinsic_db.lmdes2
