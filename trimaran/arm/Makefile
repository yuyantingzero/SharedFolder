CC = g++
CXXFLAGS = -m32 -g -gstabs+ -fno-for-scope -Wno-unused -Wno-deprecated -static
#CXXFLAGS = -m32 -O3 -fomit-frame-pointer -march=pentium4 -mfpmath=sse -ffast-math -fno-for-scope -Wno-unused -Wno-deprecated -static

ARM_SOURCES = main.cpp process_rebel.cpp util.cpp templates.cpp data_segment.cpp \
	peephole_opti.cpp process_custom_ops.cpp

OBJS = $(ARM_SOURCES:.cpp=.o)

LIB = analysis rebel mdes graph tools lcode mspec impact stdc++

LIBDIRS = $(ELCOR_REL_PATH)/lib $(IMPACT_REL_PATH)/lib 
LIBFLAGS = $(foreach dir,$(LIBDIRS),-L$(dir)) $(foreach lib,$(LIB),-l$(lib))

INC = $(IMPACT_REL_PATH)/include $(ELCOR_REL_PATH)/include $(CXXINCLD)
INCFLAGS = $(foreach dir,$(INC),-I$(dir))

.SUFFIXES:



arm-trans: $(OBJS) 
	$(CC) -o arm-trans $(OBJS) $(LIBFLAGS) $(CXXFLAGS)
	@(echo "arm-trans created successfully")
%.o: %.cpp 
	$(CC) $(INCFLAGS) -c $(CXXFLAGS) $< 

clean:
	rm -f *.o arm-trans

.dependences:
	@touch .dependences

depend: dep_msg $(ARM_SOURCES:.cpp=.dep)

dep_msg:
	@(touch .dependences)
	@(rm .dependences)
	@(echo "Reconstructing dependences, please wait ...")

%.dep:%.cpp
	@(echo "Processing $<")
ifeq ($(TRIMARAN_HOST_TYPE), x86lin)
	@(echo -n "$(dir $<)" >> .dependences ; \
		$(CC) -M $(CXXFLAGS) $(INCFLAGS) $< >> .dependences) 
else
	@(echo "$(dir $<)\c" >> .dependences ; \
		$(CC) -M $(CXXFLAGS) $(INCFLAGS) $< >> .dependences) 
endif

include .dependences



