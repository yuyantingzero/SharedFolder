
#
# The following are various targets for compilation. If you change the 
# values of these, it is likely that the dependence checking will fail
# For example changing A_OUT from elcor to new_elcor will fail the
# target "elcor" at every make of that target


A_OUT = elcor

#
# Elcor requies the use of Gnu ar and ranlib for library creation.
# If the default ar/ranlib are not the Gnu version, these settings
# should be changed appropriately.

ifndef GNU_AR
  GNU_AR = ar
endif

#
# Installation paths
#

ELCOR_BIN_PATH = ../bin
ELCOR_LIB_PATH = ../lib

ELCOR_SRC_DIRS =  Analysis Codegen Cluster Control CPR DataCPR Graph Impact Vectorize MSched Main Mdes Opti Rebel RotReg Sched Stats Tools ScalarReg Visualize Templates Customop Examples

ELCOR_INCLUDE_DIRS = ../include $(IMPACT_REL_PATH)/include $(ELCOR_HOME)/metis-4.0/Lib

CXXFLAGS = $(foreach dir,$(ELCOR_INCLUDE_DIRS),-I$(dir))

ifeq ($(USER),trimaran)
GCC_AUXFLAGS = -Wall -Wno-unused-function -O2 -static
else
GCC_AUXFLAGS = -ggdb -m32 -Wall -Wno-unused-function
#GCC_AUXFLAGS = -static -O3 -m32 -ffast-math -fomit-frame-pointer -Wall -Wno-unused-function
endif

GCC_CXXFLAGS = $(GCC_AUXFLAGS) $(CXXFLAGS) -DGCC

LOCAL_ELCOR_LIBFLAGS = -lmschedrotreg -lcodegen -lcluster -limpactxlate -lscalarreg -lsched -lanalysis -loptimization -lrebel -lgraph -lmdes -ltools -lvectorize

NONLOCAL_ELCOR_LIBFLAGS = -llcode -lproto -lmspec -limpact -lbdd -lmetis

LIBFLAGS  = $(LOCAL_ELCOR_LIBFLAGS) $(NONLOCAL_ELCOR_LIBFLAGS)

GCC_LIBFLAGS = -L$(ELCOR_LIB_PATH) -L$(IMPACT_REL_PATH)/lib -L$(ELCOR_HOME)/metis-4.0 $(LIBFLAGS) -lstdc++ -lm

GROUPED_TOOLS_SOURCES = \
Tools/tools_all.cpp

GROUPED_MDES_SOURCES = \
Mdes/mdes_all.cpp

GROUPED_GRAPH_SOURCES = \
Graph/graph_all.cpp \
Graph/opcode.cpp \
Graph/opcode2.cpp  \
Graph/opcode3.cpp \
Graph/opcode4.cpp

GROUPED_MAIN_SOURCES = \
Main/main_all.cpp 

GROUPED_REBEL_SOURCES = \
Sched/meld.cpp \
Rebel/rebel_all.cpp

GROUPED_STATS_SOURCES = \
Stats/stats_all.cpp

GROUPED_VISUALIZE_SOURCES = \
Visualize/visualize_all.cpp

ADDITIONAL_BASIC_SOURCES = 

BASIC_SOURCES = \
$(MAIN_SOURCES) \
$(VISUALIZE_SOURCES) \
$(ADDITIONAL_BASIC_SOURCES)

GROUPED_BASIC_SOURCES = \
$(GROUPED_MAIN_SOURCES) \
$(GROUPED_VISUALIZE_SOURCES) \
$(ADDITIONAL_BASIC_SOURCES) 

GROUPED_ANALYSIS_SOURCES = \
Analysis/analysis_all.cpp

GROUPED_CONTROL_SOURCES = \
Control/control_all.cpp 

GROUPED_OPTI_SOURCES = \
Opti/opti_all.cpp

GROUPED_SCHED_SOURCES = \
Sched/sched_all.cpp

GROUPED_VECTORIZE_SOURCES = \
Vectorize/vectorize_all.cpp

GROUPED_MSCHED_SOURCES = \
MSched/msched_all.cpp

GROUPED_ROTATING_REG_SOURCES = \
RotReg/rotreg_all.cpp

GROUPED_USCALAR_REG_SOURCES = \
ScalarReg/scalarreg_all.cpp

GROUPED_DATACPR_SOURCES = \
DataCPR/dcpr_all.cpp

GROUPED_CPR_SOURCES = \
CPR/cpr_all.cpp

GROUPED_CLUSTER_SOURCES = \
Cluster/cluster_all.cpp

GROUPED_CACHE_SOURCES = \
Cache/cache_all.cpp

GROUPED_CODEGEN_SOURCES = \
Codegen/codegen_all.cpp

GROUPED_IMPACT_SOURCES = \
Impact/impact_all.cpp 

TEMPLATE_SRCS = \
Templates/template_libgraph.cpp \
Templates/template_libanalysis.cpp \
Templates/template_libtools.cpp \
Templates/template_librebel.cpp \
Templates/template_libmdes.cpp \
Templates/template_liboptimization.cpp \
Templates/template_libmschedrotreg.cpp \
Templates/template_libcluster.cpp \
Templates/template_libscalarreg.cpp \
Templates/template_libimpactxlate.cpp \
Templates/template_libsched.cpp \
Templates/template_cpr.cpp \
Templates/template_libcodegen.cpp \
Templates/template_libvectorize.cpp

GCC_TEMPLATE_SOURCES_BASIC = 

GCC_TEMPLATE_SOURCES_EXTRA = \
Templates/template_cpr.cpp \
Templates/template_customop.cpp \
Templates/template_stats.cpp

GCC_TEMPLATE_SOURCES = \
$(GCC_TEMPLATE_SOURCES_BASIC) \
$(GCC_TEMPLATE_SOURCES_EXTRA)

CC_SPECIFIC_SOURCES =

#
#  Common Area
#
ALL_ADDITIONAL_SOURCES = \
Customop/customop_all.cpp \
Examples/examples_all.cpp


ALL_SOURCES = $(GROUPED_BASIC_SOURCES) \
		$(GROUPED_STATS_SOURCES) \
		$(GROUPED_CPR_SOURCES) \
		$(ALL_ADDITIONAL_SOURCES)


# Must use ungrouped version of TOOLS to allow proper linking of multiply
# defined functions that exist in the Impact libraries.
TOOLS_LIB_SOURCES = $(GROUPED_TOOLS_SOURCES) Stats/el_clock.cpp \
	Stats/el_clock_init.cpp \
	Templates/template_libtools.cpp

MDES_LIB_SOURCES = $(GROUPED_MDES_SOURCES) Templates/template_libmdes.cpp

GRAPH_LIB_SOURCES = $(GROUPED_GRAPH_SOURCES) \
	Main/init_all.cpp Templates/template_libgraph.cpp

ANALYSIS_LIB_SOURCES = $(GROUPED_ANALYSIS_SOURCES) $(GROUPED_CONTROL_SOURCES) \
	Visualize/el_dot_tools.cpp \
	Templates/template_libanalysis.cpp

REBEL_LIB_SOURCES = $(GROUPED_REBEL_SOURCES) Templates/template_librebel.cpp

OPTI_LIB_SOURCES = $(GROUPED_OPTI_SOURCES) $(GROUPED_DATACPR_SOURCES)\
	Templates/template_liboptimization.cpp 

VECTORIZE_LIB_SOURCES = $(GROUPED_VECTORIZE_SOURCES) \
	Templates/template_libvectorize.cpp

MSCHED_LIB_SOURCES = $(GROUPED_MSCHED_SOURCES) $(GROUPED_ROTATING_REG_SOURCES) \
	Templates/template_libmschedrotreg.cpp

CLUSTER_LIB_SOURCES = $(GROUPED_CLUSTER_SOURCES) \
	Templates/template_libcluster.cpp \
	Examples/random_cluster.cpp

IMPACT_LIB_SOURCES = $(GROUPED_IMPACT_SOURCES) \
	Templates/template_libimpactxlate.cpp

USCALAR_REG_LIB_SOURCES = $(GROUPED_USCALAR_REG_SOURCES) \
	Templates/template_libscalarreg.cpp

SCHED_LIB_SOURCES = $(GROUPED_SCHED_SOURCES) $(GROUPED_CACHE_SOURCES) \
	Visualize/daVinci-interface.cpp \
	Templates/template_libsched.cpp

CODEGEN_LIB_SOURCES = $(GROUPED_CODEGEN_SOURCES) \
	Templates/template_libcodegen.cpp

GCC_MAKE_SOURCES=$(ALL_SOURCES) $(GCC_TEMPLATE_SOURCES) 

ELCOR_MAKE_LIBS=$(ELCOR_LIB_PATH)/libtools.a $(ELCOR_LIB_PATH)/libmdes.a \
	$(ELCOR_LIB_PATH)/libgraph.a $(ELCOR_LIB_PATH)/libanalysis.a \
	$(ELCOR_LIB_PATH)/librebel.a $(ELCOR_LIB_PATH)/liboptimization.a \
	$(ELCOR_LIB_PATH)/libmschedrotreg.a \
	$(ELCOR_LIB_PATH)/libcluster.a \
	$(ELCOR_LIB_PATH)/libimpactxlate.a \
	$(ELCOR_LIB_PATH)/libscalarreg.a \
	$(ELCOR_LIB_PATH)/libsched.a \
	$(ELCOR_LIB_PATH)/libcodegen.a \
	$(ELCOR_LIB_PATH)/libvectorize.a

ELCOR_MAKE_SOURCES=$(GCC_MAKE_SOURCES)
ELCOR_CXXFLAGS=$(GCC_CXXFLAGS)
ELCOR_LIBFLAGS=$(GCC_LIBFLAGS)

$(ELCOR_BIN_PATH)/elcor : $(ELCOR_MAKE_LIBS) $(ELCOR_MAKE_SOURCES:.cpp=.o)
	$(TIME) $(CC)  $(ELCOR_CXXFLAGS) \
		$(ELCOR_MAKE_SOURCES:.cpp=.o) \
		-o $(A_OUT) $(ELCOR_LIBFLAGS)
	@(echo "Installing $(A_OUT) in $(ELCOR_BIN_PATH)")
	-mv -f $(A_OUT) $(ELCOR_BIN_PATH)

elcor: $(ELCOR_BIN_PATH)/elcor

libtools : $(ELCOR_LIB_PATH)/libtools.a
libtools.a : $(ELCOR_LIB_PATH)/libtools.a
$(ELCOR_LIB_PATH)/libtools.a : $(TOOLS_LIB_SOURCES:.cpp=.o)
	rm -f libtools.a
	$(GNU_AR) crus libtools.a $(TOOLS_LIB_SOURCES:.cpp=.o)
	@(echo "Installing libtools.a in $(ELCOR_LIB_PATH)")
	@if test ! -d $(ELCOR_LIB_PATH); then mkdir $(ELCOR_LIB_PATH); fi;
	-mv -f libtools.a $(ELCOR_LIB_PATH)

libmdes : $(ELCOR_LIB_PATH)/libmdes.a
libmdes.a : $(ELCOR_LIB_PATH)/libmdes.a
$(ELCOR_LIB_PATH)/libmdes.a : $(MDES_LIB_SOURCES:.cpp=.o)
	rm -f libmdes.a
	$(GNU_AR) crus libmdes.a $(MDES_LIB_SOURCES:.cpp=.o)
	@(echo "Installing libmdes.a in $(ELCOR_LIB_PATH)")
	@if test ! -d $(ELCOR_LIB_PATH); then mkdir $(ELCOR_LIB_PATH); fi;
	-mv -f libmdes.a $(ELCOR_LIB_PATH)

libgraph : $(ELCOR_LIB_PATH)/libgraph.a
libgraph.a : $(ELCOR_LIB_PATH)/libgraph.a
$(ELCOR_LIB_PATH)/libgraph.a : $(GRAPH_LIB_SOURCES:.cpp=.o)
	rm -f libgraph.a
	$(GNU_AR) crus libgraph.a $(GRAPH_LIB_SOURCES:.cpp=.o)
	@(echo "Installing libgraph.a in $(ELCOR_LIB_PATH)")
	@if test ! -d $(ELCOR_LIB_PATH); then mkdir $(ELCOR_LIB_PATH); fi;
	-mv -f libgraph.a $(ELCOR_LIB_PATH)

libanalysis : $(ELCOR_LIB_PATH)/libanalysis.a
libanalysis.a : $(ELCOR_LIB_PATH)/libanalysis.a
$(ELCOR_LIB_PATH)/libanalysis.a : $(ANALYSIS_LIB_SOURCES:.cpp=.o)
	rm -f libanalysis.a
	$(GNU_AR) crus libanalysis.a $(ANALYSIS_LIB_SOURCES:.cpp=.o)
	@(echo "Installing libanalysis.a in $(ELCOR_LIB_PATH)")
	@if test ! -d $(ELCOR_LIB_PATH); then mkdir $(ELCOR_LIB_PATH); fi;
	-mv -f libanalysis.a $(ELCOR_LIB_PATH)

librebel : $(ELCOR_LIB_PATH)/librebel.a
librebel.a : $(ELCOR_LIB_PATH)/librebel.a
$(ELCOR_LIB_PATH)/librebel.a : $(REBEL_LIB_SOURCES:.cpp=.o)
	rm -f librebel.a
	$(GNU_AR) crus librebel.a $(REBEL_LIB_SOURCES:.cpp=.o)
	@(echo "Installing librebel.a in $(ELCOR_LIB_PATH)")
	@if test ! -d $(ELCOR_LIB_PATH); then mkdir $(ELCOR_LIB_PATH); fi;
	-mv -f librebel.a $(ELCOR_LIB_PATH)

liboptimization : $(ELCOR_LIB_PATH)/liboptimization.a
liboptimization.a : $(ELCOR_LIB_PATH)/liboptimization.a
$(ELCOR_LIB_PATH)/liboptimization.a : $(OPTI_LIB_SOURCES:.cpp=.o)
	rm -f liboptimization.a
	$(GNU_AR) crus liboptimization.a $(OPTI_LIB_SOURCES:.cpp=.o)
	@(echo "Installing liboptimization.a in $(ELCOR_LIB_PATH)")
	@if test ! -d $(ELCOR_LIB_PATH); then mkdir $(ELCOR_LIB_PATH); fi;
	-mv -f liboptimization.a $(ELCOR_LIB_PATH)

libvectorize : $(ELCOR_LIB_PATH)/libvectorize.a
libvectorize.a : $(ELCOR_LIB_PATH)/libvectorize.a
$(ELCOR_LIB_PATH)/libvectorize.a : $(VECTORIZE_LIB_SOURCES:.cpp=.o)
	rm -f libvectorize.a
	$(GNU_AR) crus libvectorize.a $(VECTORIZE_LIB_SOURCES:.cpp=.o)
	@(echo "Installing libvectorize.a in $(ELCOR_LIB_PATH)")
	@if test ! -d $(ELCOR_LIB_PATH); then mkdir $(ELCOR_LIB_PATH); fi;
	-mv -f libvectorize.a $(ELCOR_LIB_PATH)

libmschedrotreg : $(ELCOR_LIB_PATH)/libmschedrotreg.a
libmschedrotreg.a : $(ELCOR_LIB_PATH)/libmschedrotreg.a
$(ELCOR_LIB_PATH)/libmschedrotreg.a : $(MSCHED_LIB_SOURCES:.cpp=.o)
	rm -f libmschedrotreg.a
	$(GNU_AR) crus libmschedrotreg.a $(MSCHED_LIB_SOURCES:.cpp=.o)
	@(echo "Installing libmschedrotreg.a in $(ELCOR_LIB_PATH)")
	@if test ! -d $(ELCOR_LIB_PATH); then mkdir $(ELCOR_LIB_PATH); fi;
	-mv -f libmschedrotreg.a $(ELCOR_LIB_PATH)

libcluster : $(ELCOR_LIB_PATH)/libcluster.a
libcluster.a : $(ELCOR_LIB_PATH)/libcluster.a
$(ELCOR_LIB_PATH)/libcluster.a : $(CLUSTER_LIB_SOURCES:.cpp=.o)
	rm -f libcluster.a
	$(GNU_AR) crus libcluster.a $(CLUSTER_LIB_SOURCES:.cpp=.o)
	@(echo "Installing libcluster.a in $(ELCOR_LIB_PATH)")
	@if test ! -d $(ELCOR_LIB_PATH); then mkdir $(ELCOR_LIB_PATH); fi;
	-mv -f libcluster.a $(ELCOR_LIB_PATH)

libimpactxlate : $(ELCOR_LIB_PATH)/libimpactxlate.a
libimpactxlate.a : $(ELCOR_LIB_PATH)/libimpactxlate.a
$(ELCOR_LIB_PATH)/libimpactxlate.a : $(IMPACT_LIB_SOURCES:.cpp=.o)
	rm -f libimpactxlate.a
	$(GNU_AR) crus libimpactxlate.a $(IMPACT_LIB_SOURCES:.cpp=.o)
	@(echo "Installing libimpactxlate.a in $(ELCOR_LIB_PATH)")
	@if test ! -d $(ELCOR_LIB_PATH); then mkdir $(ELCOR_LIB_PATH); fi;
	-mv -f libimpactxlate.a $(ELCOR_LIB_PATH)

libscalarreg : $(ELCOR_LIB_PATH)/libscalarreg.a
libscalarreg.a : $(ELCOR_LIB_PATH)/libscalarreg.a
$(ELCOR_LIB_PATH)/libscalarreg.a : $(USCALAR_REG_LIB_SOURCES:.cpp=.o)
	rm -f libscalarreg.a
	$(GNU_AR) crus libscalarreg.a $(USCALAR_REG_LIB_SOURCES:.cpp=.o)
	@(echo "Installing libscalarreg.a in $(ELCOR_LIB_PATH)")
	@if test ! -d $(ELCOR_LIB_PATH); then mkdir $(ELCOR_LIB_PATH); fi;
	-mv -f libscalarreg.a $(ELCOR_LIB_PATH)

libsched : $(ELCOR_LIB_PATH)/libsched.a
libsched.a : $(ELCOR_LIB_PATH)/libsched.a
$(ELCOR_LIB_PATH)/libsched.a : $(SCHED_LIB_SOURCES:.cpp=.o)
	rm -f libsched.a
	$(GNU_AR) crus libsched.a $(SCHED_LIB_SOURCES:.cpp=.o)
	@(echo "Installing libsched.a in $(ELCOR_LIB_PATH)")
	@if test ! -d $(ELCOR_LIB_PATH); then mkdir $(ELCOR_LIB_PATH); fi;
	-mv -f libsched.a $(ELCOR_LIB_PATH)

libcodegen : $(ELCOR_LIB_PATH)/libcodegen.a
libcodegen.a : $(ELCOR_LIB_PATH)/libcodegen.a
$(ELCOR_LIB_PATH)/libcodegen.a : $(CODEGEN_LIB_SOURCES:.cpp=.o)
	rm -f libcodegen.a
	$(GNU_AR) crus libcodegen.a $(CODEGEN_LIB_SOURCES:.cpp=.o)
	@(echo "Installing libcodegen.a in $(ELCOR_LIB_PATH)")
	@if test ! -d $(ELCOR_LIB_PATH); then mkdir $(ELCOR_LIB_PATH); fi;
	-mv -f libcodegen.a $(ELCOR_LIB_PATH)

# special rules for the template files so that the stl runs with optimizations.
# this allows decent performance without losing the ability to debug. -ntclark
$(TEMPLATE_SRCS:.cpp=.o) : $<
	$(CC) $(ELCOR_CXXFLAGS) -c $*.cpp -o $*.o 

includedir :
	@(echo "Linking files into include dir")
	-(cd ../include; ./make_links)

default : .dependences $(ELCOR_BIN_PATH)/elcor
	$(MAKE) elcor

mdes :
	-(cd ../mdes; make)
	-(cd ../mdes/arm; make)

.PHONY: tags dust clean spotless depend GNUmakefile dep_msg

MAKE_TAG = $(shell (cd $(dir) ; etags -C *.cpp *.h))

TOP_LEVEL_TAG_STRING = etags -C $(foreach dir,$(ELCOR_SRC_DIRS),-i $(dir)/TAGS)

tags_message:
	@(echo "processing directories $(ELCOR_SRC_DIRS)")

tags: tags_message
	$(foreach dir,$(ELCOR_SRC_DIRS),$(MAKE_TAG))
	-($(TOP_LEVEL_TAG_STRING))

%.o:%.cpp
	$(CC) $(ELCOR_CXXFLAGS) -c $< -o $*.o 

dust:
	-rm -f *~
	-rm -f */*~
	-rm -f .#*
	-rm -f */.#*
	-rm -f */.#*~

MAKE_CLEAN = $(shell (cd $(dir) ; rm -f *.o; rm -f *.a))

clean:
	-rm -f *.o
	-rm -f *.out
	-rm -f *.a
	$(foreach dir,$(ELCOR_SRC_DIRS) $(ELCOR_LIB_PATH),$(MAKE_CLEAN))

spotless: clean dust

.dependences: 
	@touch .dependences

depend: includedir dep_msg $(ELCOR_MAKE_SOURCES:.cpp=.dep) \
		$(TOOLS_LIB_SOURCES:.cpp=.dep) $(MDES_LIB_SOURCES:.cpp=.dep) \
		$(GRAPH_LIB_SOURCES:.cpp=.dep) $(REBEL_LIB_SOURCES:.cpp=.dep) \
		$(ANALYSIS_LIB_SOURCES:.cpp=.dep) $(OPTI_LIB_SOURCES:.cpp=.dep) \
		$(VECTORIZE_LIB_SOURCES:.cpp=.dep) \
		$(MSCHED_LIB_SOURCES:.cpp=.dep) \
		$(CLUSTER_LIB_SOURCES:.cpp=.dep) \
		$(IMPACT_LIB_SOURCES:.cpp=.dep) \
		$(USCALAR_REG_LIB_SOURCES:.cpp=.dep) \
		$(SCHED_LIB_SOURCES:.cpp=.dep) \
		$(CODEGEN_LIB_SOURCES:.cpp=.dep)

dep_msg :
	@(touch .dependences)
	@(rm .dependences)
	@(touch .dependences)
	@(echo "Reconstructing dependences, please wait ...")

%.dep:%.cpp
	@(echo "Processing $<")
	@($(CC) -M -MT $*.o $(ELCOR_CXXFLAGS) $(GCC_AUXFLAGS) $< >> .dependences)

include .dependences

# DO NOT DELETE THIS LINE -- make depend depends on it.
