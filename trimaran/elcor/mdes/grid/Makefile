
MAKEFILE = $(ELCOR_HOME)/mdes/Makefile

#
# Specify the path of the HMDES2->LMDES2 compiler
#
HC = $(IMPACT_REL_PATH)/scripts/hc

STD_MDES = \
hpl_pd_elcor_std.hmdes2 \
hpl_pd_patterns.hmdes2 \
custom_op_std.hmdes2


MDES_INCLUDES = structure_pristine.hmdes2 
HPL_PD_INCLUDES = \
hpl_pd_ops.hmdes2 \
hpl_pd_pristine.hmdes2 \
hpl_pd_elcor.hmdes2 \
hpl_pd_patterns_std.hmdes2

std_mdes : $(STD_MDES:.hmdes2=.lmdes2)

$(STD_MDES:.hmdes2=.lmdes2) : %.lmdes2 : %.hmdes2 $(MDES_INCLUDES) $(HPL_PD_INCLUDES)
	@echo '*** Processing Mdes $< ***'
	$(HC) $<
	rm -rf $*.hmdes2pp
	@echo '*** Done'

ifmb:
	@echo '*** Processing combinations of IFMB mdes...'
	@gen_lmdes
	rm -rf *.hmdes2pp
	@echo '*** Done'

all: std_mdes ifmb

clean:
	rm -rf core *.hmdes2pp *~

spotless:
	rm -rf core *.hmdes2pp *~ *.lmdes2

%.lmdes2 : %.hmdes2
	@echo '*** Mdes $< ***'
	$(HC) $<
