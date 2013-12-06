LIB_TARGETS = libcodegen.a libmdes.a librebel.a libsched.a \
libanalysis.a libgraph.a libmschedrotreg.a libregwindow.a libtools.a \
libcluster.a libimpactxlate.a liboptimization.a libscalarreg.a libvectorize.a

BIN_TARGETS = elcor 

$(BIN_TARGETS):
	@cd metis-4.0; $(MAKE)
	@cd mdes; $(MAKE)
	@cd src; rm -f .dependences; $(MAKE) depend
	@cd src; $(MAKE) $@
	@cd sumstat; $(MAKE) 
	@echo 'ELCOR complete'

$(LIB_TARGETS): 
	@cd src; rm -f .dependences; $(MAKE) depend
	@cd src; $(MAKE) $@

all: depend $(BIN_TARGETS) 
	@echo "make finished on `date`"
depend: 
	@cd src; $(MAKE) depend

clean:
	@cd mdes; $(MAKE) clean
	@cd src; $(MAKE) clean

spotless:
	@cd mdes; $(MAKE) spotless
	@cd src; $(MAKE) spotless

clean_mdes:
	@cd mdes; $(MAKE) clean

spotless_mdes:
	@cd mdes; $(MAKE) spotless

