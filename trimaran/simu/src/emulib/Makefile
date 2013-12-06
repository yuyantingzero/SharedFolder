#  LICENSE NOTICE
#  --------------

#  IT IS  A BREACH OF  THE LICENSE AGREEMENT  TO REMOVE THIS  NOTICE FROM
#  THIS  FILE  OR SOFTWARE  OR  ANY MODIFIED  VERSIONS  OF  THIS FILE  OR
#  SOFTWARE.

#  Copyright notices/Licensor(s) Identification
#  --------------------------------------------
#  Each of  the entity(ies) whose name properly  appear immediately below
#  in connection with a copyright notice is a Licensor(s) under the terms
#  that follow.

#  Copyright  1998  New  York  University.  All rights  reserved  by  the
#  foregoing, respectively.

#  Copyright 2001 Center for  Research on Embedded Systems and Technology
#  at the  Georgia Institute  of Technology. All  rights reserved  by the
#  foregoing, respectively.

#  License agreement
#  -----------------

#  The  code contained  in this  file  including both  binary and  source
#  (hereafter,  Software)  is subject  to  copyright  by Licensor(s)  and
#  ownership remains with Licensor(s).

#  Licensor(s)  grants you  (hereafter, Licensee)  a license  to  use the
#  Software for  academic, research and internal  business purposes only,
#  without  a  fee.  "Internal  business  use"  means  that Licensee  may
#  install, use and execute the Software for the purpose of designing and
#  evaluating products.   Licensee may also disclose  results obtained by
#  executing  the  Software,  as  well as  algorithms  embodied  therein.
#  Licensee may  distribute the Software  to third parties  provided that
#  the copyright notice and this statement appears on all copies and that
#  no  charge  is  associated  with  such copies.   No  patent  or  other
#  intellectual property license is granted or implied by this Agreement,
#  and this  Agreement does not  license any acts except  those expressly
#  recited.

#  Licensee may  make derivative works,  which shall also be  governed by
#  the  terms of  this  License Agreement.  If  Licensee distributes  any
#  derivative work based  on or derived from the  Software, then Licensee
#  will abide by the following terms.  Both Licensee and Licensor(s) will
#  be  considered joint  owners of  such derivative  work  and considered
#  Licensor(s) for  the purpose of distribution of  such derivative work.
#  Licensee shall  not modify this  agreement except that  Licensee shall
#  clearly  indicate  that  this  is  a  derivative  work  by  adding  an
#  additional copyright notice in  the form "Copyright <year> <Owner>" to
#  other copyright notices above, before the line "All rights reserved by
#  the foregoing, respectively".   A party who is not  an original author
#  of such derivative works within  the meaning of US Copyright Law shall
#  not modify or add his name to the copyright notices above.

#  Any Licensee  wishing to  make commercial use  of the  Software should
#  contact each and every Licensor(s) to negotiate an appropriate license
#  for  such  commercial  use;  permission  of all  Licensor(s)  will  be
#  required for such a  license.  Commercial use includes (1) integration
#  of all or part  of the source code into a product  for sale or license
#  by or on  behalf of Licensee to third parties,  or (2) distribution of
#  the Software  to third  parties that need  it to utilize  a commercial
#  product sold or licensed by or on behalf of Licensee.

#  LICENSOR (S)  MAKES NO REPRESENTATIONS  ABOUT THE SUITABILITY  OF THIS
#  SOFTWARE FOR ANY  PURPOSE.  IT IS PROVIDED "AS  IS" WITHOUT EXPRESS OR
#  IMPLIED WARRANTY.   LICENSOR (S) SHALL  NOT BE LIABLE FOR  ANY DAMAGES
#  SUFFERED BY THE USERS OF THIS SOFTWARE.

#  IN NO EVENT UNLESS REQUIRED BY  APPLICABLE LAW OR AGREED TO IN WRITING
#  WILL ANY  COPYRIGHT HOLDER, OR ANY  OTHER PARTY WHO  MAY MODIFY AND/OR
#  REDISTRIBUTE THE  PROGRAM AS PERMITTED  ABOVE, BE LIABLE  FOR DAMAGES,
#  INCLUDING  ANY GENERAL, SPECIAL,  INCIDENTAL OR  CONSEQUENTIAL DAMAGES
#  ARISING OUT OF THE USE OR  INABILITY TO USE THE PROGRAM (INCLUDING BUT
#  NOT  LIMITED TO  LOSS OF  DATA OR  DATA BEING  RENDERED  INACCURATE OR
#  LOSSES SUSTAINED BY  YOU OR THIRD PARTIES OR A  FAILURE OF THE PROGRAM
#  TO  OPERATE WITH ANY  OTHER PROGRAMS),  EVEN IF  SUCH HOLDER  OR OTHER
#  PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

#  By using  or copying  this Software, Licensee  agrees to abide  by the
#  copyright law and all other applicable laws of the U.S., and the terms
#  of  this license  agreement. Any  individual Licensor  shall  have the
#  right  to terminate this  license immediately  by written  notice upon
#  Licensee's   breach   of,  or   non-compliance   with,   any  of   its
#  terms.  Licensee may  be held  legally responsible  for  any copyright
#  infringement  that is caused  or encouraged  by Licensee's  failure to
#  abide     by    the     terms    of     this     license    agreement.


# track statistics per-operation
C_FLAGS   = -D__PD_TRACK_OP_STATS_

# support control flow and memory tracing
#C_FLAGS += -D__PD_DEBUG

# support program tracing
C_FLAGS += -D__PD_TRACE_

# full speculation  support (most  accurate simulation) is  supported in
# the simulator, although the impact code generation is not correct - it
# is recommended you do not use full speculation support - the simulator
# (by  default) provides some  speculation support to ensure correctness
C_FLAGS += -D__FULL_SPECULATION_SUPPORT_

# interface to M5E cache simulator:
# set BUILD_M5E=0 to omit M5E components  
# set BUILD_M5E=1 if you want to use M5E
BUILD_M5E=0

# generate custom library extension 
LIBEXTN =

# static standard emulation library
SSTDTARGET  = libequals$(LIBEXTN).a
# static extended emulation library
SEXTTARGET  = libequals$(LIBEXTN)+.a

# dynamic standard emulation library
DSTDTARGET = libequals$(LIBEXTN).sl
# dynamic extended emulation library
DEXTTARGET = libequals$(LIBEXTN)+.sl

# include path for standard library
STDINCLUDE = -I$(SIMU_HOME)/include/ -I$(DINERO_HOME)/include/

# include path for M5E cache simulator
ifeq (${BUILD_M5E}, 1)
	STDINCLUDE += -I$(M5_ROOT)/m5
endif

# recommended CC Flags
CC_FLAGS_R  = +DA1.1 -Wall -Aa
CC_FLAGS_D  = +DA1.1 -Wall -Aa -g

CC_FLAGS    = $(CC_FLAGS_R)

# recommended GCC Flags
GCC_FLAGS_R  = -Wall -malign-double
GCC_FLAGS_O  = -m32 -Wall -malign-double -O3
GCC_FLAGS_D  = -m32 -Wall -malign-double -g -fno-inline
GCC_FLAGS_DV = -Wall -malign-double -g -fno-inline -D__PD_DEBUG

# GCC flags to be used
GCC_FLAGS    = $(GCC_FLAGS_O)

# platform specific flags
ifeq ($(TRIMARAN_HOST_TYPE), x86lin)
	C_FLAGS += -DLINUX
else
	ifeq ($(TRIMARAN_HOST_TYPE), x86win)
		C_FLAGS += -DWINUX
	else
		ifeq ($(TRIMARAN_HOST_TYPE), sunsol)
			C_FLAGS += -DSOLARIS
		else 
			ifeq ($(TRIMARAN_HOST_TYPE), hp)
				C_FLAGS  += -DHPUX
			endif
		endif
	endif
endif

# shared library
ifdef GEN_SHARED_PD_LIBRARY
    CC_FLAGS  += +z
    GCC_FLAGS += -fPIC
    GEN_LIB    = $(LD)
    GEN_FLAGS  = -b -o
    TARGET_LIB = $(DSTDTRAGET)
    OBJS_DIR   = shared_lib$(LIBEXTN)
# static library
else
    GEN_LIB    = $(AR)
    GEN_FLAGS  = r
    TARGET_LIB = $(SSTDTARGET)
    OBJS_DIR   = static_lib$(LIBEXTN)
endif

# chose the relevant compiler flags
ifeq ($(CC), cc)
	C_FLAGS += $(CC_FLAGS)  $(STDINCLUDE)
else
	C_FLAGS += $(GCC_FLAGS) $(STDINCLUDE)
endif


########### DO NOT NEED TO CHANGE ANYTHING BELOW THIS LINE UNLESS.... ###########

SOURCE_DIR = ../src/emulib
TARGET_DIR = ../../lib

CORE_SRCS = \
            PD_init.c \
            PD_kernel.c \
            PD_reg.c \
            PD_queue.c \
            PD_stack.c \
            PD_stats.c \
            PD_parms.c \
            PD_trace.c \
            PD_debug.c \
            PD_timer.c \
            PD_error.c \
            PD_signals.c \
            PD_setjmp.c \
            PD_longjmp.c \
            PD_varargs.c \
            PD_alloc.c \
            PD_instrument_memops.c \
	    PD_alloca.c

ifeq (${BUILD_M5E}, 1)
	CORE_SRCS += PD_cache.c PD_m5e.c
	C_FLAGS  += -D__PD_CACHE_ -D__PD_M5E_
endif

EMUL_SRCS = \
PD_branch_ops.c PD_conversion_ops.c PD_disambiguation_01_ops.c PD_disambiguation_02_ops.c PD_disambiguation_03_ops.c PD_int_logic_ops.c PD_int_shift_ops.c PD_load_store_ops.c PD_move_ops.c PD_postinc_load_store_ops.c PD_loop_branch_ops.c PD_predicate_ops.c \
PD_float_arith_ops.c PD_float_cmpp_dp_01_ops.c PD_float_cmpp_dp_02_ops.c PD_float_cmpp_dp_03_ops.c PD_float_cmpp_dp_04_ops.c PD_float_cmpp_dp_05_ops.c PD_float_cmpp_dp_06_ops.c PD_float_cmpp_dp_07_ops.c PD_float_cmpp_dp_08_ops.c PD_float_cmpp_dp_09_ops.c PD_float_cmpp_dp_10_ops.c PD_float_cmpp_dp_11_ops.c PD_float_cmpp_dp_12_ops.c PD_float_cmpp_dp_13_ops.c PD_float_cmpp_dp_14_ops.c PD_float_cmpp_dp_15_ops.c PD_float_cmpp_dp_16_ops.c PD_float_cmpp_dp_17_ops.c PD_float_cmpp_dp_18_ops.c PD_float_cmpp_dp_19_ops.c PD_float_cmpp_dp_20_ops.c \
PD_float_cmpp_sp_01_ops.c PD_float_cmpp_sp_02_ops.c PD_float_cmpp_sp_03_ops.c PD_float_cmpp_sp_04_ops.c PD_float_cmpp_sp_05_ops.c PD_float_cmpp_sp_06_ops.c PD_float_cmpp_sp_07_ops.c PD_float_cmpp_sp_08_ops.c PD_float_cmpp_sp_09_ops.c PD_float_cmpp_sp_10_ops.c PD_float_cmpp_sp_11_ops.c PD_float_cmpp_sp_12_ops.c PD_float_cmpp_sp_13_ops.c PD_float_cmpp_sp_14_ops.c PD_float_cmpp_sp_15_ops.c PD_float_cmpp_sp_16_ops.c PD_float_cmpr_ops.c \
PD_int_arith_ops.c PD_int_cmpp_01_ops.c PD_int_cmpp_02_ops.c PD_int_cmpp_03_ops.c PD_int_cmpp_04_ops.c PD_int_cmpp_05_ops.c PD_int_cmpp_06_ops.c PD_int_cmpp_07_ops.c PD_int_cmpp_08_ops.c PD_int_cmpp_09_ops.c PD_int_cmpp_10_ops.c PD_int_cmpp_11_ops.c PD_int_cmpp_12_ops.c PD_int_cmpp_13_ops.c PD_int_cmpp_14_ops.c PD_int_cmpp_15_ops.c PD_int_cmpp_16_ops.c PD_int_cmpp_17_ops.c PD_int_cmpp_18_ops.c PD_int_cmpp_19_ops.c PD_int_cmpp_20_ops.c PD_int_cmpp_21_ops.c PD_int_cmpp_22_ops.c PD_int_cmpp_23_ops.c PD_int_cmpp_24_ops.c PD_int_cmpp_25_ops.c PD_int_cmpp_26_ops.c PD_int_cmpp_27_ops.c PD_int_cmpp_28_ops.c PD_int_cmpp_29_ops.c PD_int_cmpp_30_ops.c PD_int_cmpp_31_ops.c PD_int_cmpp_32_ops.c PD_int_cmpr_ops.c PD_save_restore_ops.c\
PD_vec_int_arith_ops.c PD_vec_int_logic_ops.c PD_vec_float_arith_ops.c PD_vec_conversion_ops.c PD_vec_move_ops.c PD_vec_load_store_ops.c

EMUL_HDRS = \
PD_branch_ops.h PD_cmpp_ops.h PD_disambiguation_ops.h PD_memory_ops.h PD_ops.h 

CORE_OBJS = $(CORE_SRCS:%.c=$(OBJS_DIR)/%.o)
ifdef GEN_EXT_PD_LIBRARY
EXTN_OBJS = $(EXTN_SRCS:%.c=$(OBJS_DIR)/%.o)
endif
EMUL_OBJS = $(EMUL_SRCS:%.c=$(OBJS_DIR)/%.o)

ALL_SRCS  = $(CORE_SRCS) $(EXTN_SRCS) $(EMUL_SRCS)
ALL_OBJS  = $(CORE_OBJS) $(EXTN_OBJS) $(EMUL_OBJS)

DEPS      =  \
             PD.h \
             PD_globals.h \
             PD_op.h \
             PD_reg.h \
             PD_init.h \
             PD_kernel.h \
             PD_cache.h \
             PD_m5e.h \
             PD_parms.h \
             PD_stack.h \
             PD_queue.h \
             PD_trace_defs.h \
             PD_trace.h \
             PD_instrument_memops.h \
             PD_stats.h \
             PD_debug.h \
             PD_timer.h \
             PD_error.h \
             PD_sl.h \
             PD_alloc.h \
             PD_signals.h \
             PD_varargs.h \
             Makefile
 
.SUFFIXES:

all: $(TARGET_DIR)/$(TARGET_LIB)

$(TARGET_DIR)/$(TARGET_LIB): $(OBJS_DIR)/$(TARGET_LIB)
	ln -sf $(SOURCE_DIR)/$(OBJS_DIR)/$(TARGET_LIB) $(TARGET_DIR)/$(TARGET_LIB)

$(OBJS_DIR)/$(TARGET_LIB): $(OBJS_DIR) $(ALL_SRCS) $(ALL_OBJS) $(DEPS)
	$(GEN_LIB) $(GEN_FLAGS) $@ $(ALL_OBJS)

$(EMUL_SRCS): genops
$(EMUL_HDRS): genops

$(OBJS_DIR):
	@if test ! -d $(OBJS_DIR); then mkdir $(OBJS_DIR); fi;

$(OBJS_DIR)/%.o: %.c $(DEPS) $(EMUL_HDRS)
	$(CC) $(C_FLAGS) -o $@ -c $*.c

genops: gen/gen_functions PD_ops.list
	gen/gen_functions PD_ops.list > genops

gen/gen_functions: gen/gen_functions.c PD_globals.h
	(cd gen; $(MAKE))

core: $(CORE_OBJS)

touch_core:
	touch $(CORE_SRCS)

dir:
	@if test ! -d $(OBJS_DIR); then mkdir $(OBJS_DIR); fi;

include:
	@(echo "Linking files into include dir")
	-(cd $(SIMU_HOME)/include; ./make_simu_links)

dust:
	@rm -f \#* *~

clean:
	@rm -f \#* *~ *.o $(OBJS_DIR)/*.o
	@rm -f $(EMUL_SRCS) $(EMUL_HDRS) genops

spotless: clean
	(cd gen; $(MAKE) spotless)
	@rm -rf $(OBJS_DIR)
	@rm -f  $(TARGET_DIR)/$(TARGET_LIB)
