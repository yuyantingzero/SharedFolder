# Executable name
A_OUT = Sumstat

# Installation paths
SUMSTAT_BIN_PATH = ../bin
SUMSTAT_LIB_PATH = ../lib

INCLUDE_DIRS = $(IMPACT_REL_PATH)/include

INCLUDE_FLAGS = $(foreach dir,$(INCLUDE_DIRS),-I$(dir))
CXXFLAGS = -g -Wall -Wno-unused $(INCLUDE_FLAGS) -m32

CXXOPTS = 
GCC_AUXFLAGS = -DGCC

ifeq ($(IMPACT_REL_PATH),${TRIMARAN_ROOT}/openimpact)
IMPACT_LIBFLAGS = -L$(IMPACT_REL_PATH)/lib
else
IMPACT_LIBFLAGS = -L$(IMPACT_REL_PATH)/lib
endif
ELCOR_LIBFLAGS =

IMPACT_LIBS = -limpact
LIBFLAGS = -m32 $(ELCOR_LIBFLAGS) $(IMPACT_LIBFLAGS) \
	   $(ELCOR_LIBS) $(IMPACT_LIBS) -lstdc++ -lm

SUMSTAT_SOURCES = sumstat.cpp

Sumstat : $(SUMSTAT_BIN_PATH)/Sumstat

$(SUMSTAT_BIN_PATH)/Sumstat : $(SUMSTAT_SOURCES:.cpp=.o)
	$(CC) $(CFLAGS) $(SUMSTAT_SOURCES:.cpp=.o) -o $(A_OUT) $(LIBFLAGS)
	@(echo "Installing $(A_OUT) in $(SUMSTAT_BIN_PATH)")
	-mv -f $(A_OUT) $(SUMSTAT_BIN_PATH)


%.o:%.cpp
	$(CC) $(CXXOPTS) $(CXXFLAGS) $(GCC_AUXFLAGS) -c $< -o $*.o


clean:
	-rm -f *.o
	-rm -f *.a
	-rm -f *.out
	-rm -f $(A_OUT)

includedir :
	@(echo "Linking files into include dir")
	-(cd ../../include; ./make_links)

.dependences:
	@touch .dependences

depend: includedir dep_msg $(SUMSTAT_SOURCES:.cpp=.dep)

dep_msg :
	@(touch .dependences)
	@(rm .dependences)
	@(touch .dependences)
	@(echo "Reconstructing dependences, please wait ...")

%.dep:%.cpp
	@(echo "Processing $<")
ifeq ($(TRIMARAN_HOST_TYPE), x86lin)
	@(echo -n "$(dir $<)" >> .dependences; \
                gcc -M $(CXXOPTS) $(CXXFLAGS) $(GCC_AUXFLAGS) $< >> .dependences)
else
	@(echo "$(dir $<)\c" >> .dependences ; \
                gcc -M $(CXXOPTS) $(CXXFLAGS) $(GCC_AUXFLAGS) $< >> .dependences)
endif

include .dependences

# DO NOT DELETE THIS LINE -- make depend depends on it.

