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


SOURCES = simu_main.cpp \
          simu_init.cpp \
          simu_el_code_processor.cpp \
          simu_el_data_processor.cpp \
          packager.cpp \
          vectorizer.cpp \
          annotations.cpp \
          emitter.cpp \
          asm_emitter.cpp \
          utils.cpp \
          streamBuffer.cpp \
          error.cpp \
          simu_timer.cpp \
          template_simu.cpp
LIB_SOURCES = 


TARGET     = codegen
SOURCE_DIR = ../src/codegen
TARGET_DIR = ../../bin

LIBS    = rebel graph mdes tools m stdc++
LIBDIRS = $(ELCOR_HOME)/lib 
INCLUDE = $(IMPACT_REL_PATH)/include $(ELCOR_HOME)/include $(SIMU_HOME)/include \
          $(IMPACT_REL_PATH)/platform/${TRIMARAN_HOST_TYPE}_${CC}/include

# Recommended GCC Flags
CXXFLAGS_R  = -DGCC -Wall -static
CXXFLAGS_O  = -m32 -DGCC -Wall -static -O3
CXXFLAGS_D  = -m32 -DGCC -Wall -static -g

CXXFLAGS = $(CXXFLAGS_O)

# DO NOT NEED TO CHANGE ANYTHING BELOW THIS LINE

ifeq ($(TRIMARAN_HOST_TYPE), x86lin)
	CXXFLAGS += -DLINUX
else
	ifeq ($(TRIMARAN_HOST_TYPE), x86win)
		CXXFLAGS += -DWINUX
	else
		ifeq ($(TRIMARAN_HOST_TYPE), sunsol)
			CXXFLAGS += -DSOLARIS
		else 
			ifeq ($(TRIMARAN_HOST_TYPE), hp)
				CXXFLAGS  += -DHPUX
			endif
		endif
	endif
endif

OBJS = $(SOURCES:.cpp=.o) $(LIB_SOURCES:.cpp=.o)

LIBFLAGS = $(foreach dir,$(LIBDIRS),-L$(dir)) $(foreach lib,$(LIBS),-l$(lib))
INCFLAGS = $(foreach dir,$(INCLUDE),-I$(dir))

.SUFFIXES:

all: $(TARGET_DIR)/$(TARGET)

$(TARGET_DIR)/$(TARGET): $(TARGET)
	ln -sf $(SOURCE_DIR)/$(TARGET)  $(TARGET_DIR)

$(TARGET): $(OBJS) $(LIBDEPS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBFLAGS)

%.o: %.cpp Makefile
	$(CXX) $(INCFLAGS) $(CXXFLAGS) -c $*.cpp

clean : 
	rm -f *.o
	rm -f $(TARGET)
	rm -f $(TARGET_DIR)/$(TARGET)

spotless: clean
	rm -f *~ .dependences


.dependences:
	@touch .dependences

depend: dep_msg $(SOURCES:.cpp=.dep) $(LIB_SOURCES:.cpp=.dep)

dep_msg:
	@(touch .dependences)
	@(rm .dependences)
	@(echo "Reconstructing dependences, please wait ...")


%.dep:%.cpp
	@(echo "Processing $<")
	@(echo -n "$(dir $<)" >> .dependences ; \
		$(CXX) -M $(CXXFLAGS) $(INCFLAGS) $< >> .dependences) 

include .dependences

# DO NOT DELETE THIS LINE -- make depend depends on it.
