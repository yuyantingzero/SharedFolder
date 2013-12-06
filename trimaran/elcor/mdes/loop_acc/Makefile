
# Renaming all "gnumake" -> "make" to allow easier installation
# for Trimaran release.                     <lieberm 01/30/2007>
MAKE = make
MAKEFILE = $(ELCOR_HOME)/mdes/Makefile

#
# Specify the path of the HMDES2->LMDES2 compiler
#
HC = $(IMPACT_REL_PATH)/scripts/hc

STD_MDES = \
loop_elcor_std.hmdes2

LOOP_ACC_INCLUDES = \
loop_ops.hmdes2 \
loop_pristine.hmdes2 \
loop_elcor.hmdes2

std_mdes : $(STD_MDES:.hmdes2=.lmdes2)

$(STD_MDES:.hmdes2=.lmdes2) : %.lmdes2 : %.hmdes2 $(LOOP_ACC_INCLUDES)
	@echo '*** Processing Mdes $< ***'
	$(HC) $<
	rm -rf *.hmdes2pp
	@echo '*** Done'

clean:
	rm -rf *.lmdes2
	rm -rf core *.hmdes2pp *~

spotless:
	rm -rf core *.hmdes2pp *~

%.lmdes2 : %.hmdes2
	@echo '*** Mdes $< ***'
	$(HC) $<
