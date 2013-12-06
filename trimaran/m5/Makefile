PATCH_DIR=patch
INSTALL_DIR=install

INSTALL_DIR_ABS=$(PWD)/install

M5_BUILD_DIR=$(INSTALL_DIR)/m5/build
M5E_LIB=$(M5_BUILD_DIR)/ALPHA/libm5shared.so

# files to copy to m5 dir, some are new, some are overwriting m5 files
# ...(directories will be automatically created by this script)
PATCH_FILES=m5/build/Makefile \
            m5/m5elements/M5E_Cluster.py \
	    m5/m5elements/cache.cc \
	    m5/m5elements/cache.hh \
	    m5/m5elements/m5e_cluster.cc \
	    m5/m5elements/m5e_cluster.hh \
	    m5/m5elements/default.py \
	    m5/m5elements/mem_types.hh \
	    m5/m5elements/memtest_run.py \
	    m5/m5elements/req_by_cluster.cc \
	    m5/m5elements/req_by_cluster.hh \
	    m5/m5elements/simple_test.c \
	    m5/m5elements/speed_tester.cpp \
	    m5/m5elements/tester.cpp \
	    m5/sim/sim_exit.hh \
	    m5/sim/sim_exit.cc \
	    m5/sim/syscall_emul.hh \
	    m5/mem/mem_req.hh \
	    m5/SConscript

PATCH_FILES_REL=$(addprefix $(PATCH_DIR)/patch_files/,$(PATCH_FILES))
PATCHED_SRC=$(INSTALL_DIR)/patch_complete


####################################################################
# default build:
####################################################################

all: $(PATCHED_SRC) $(M5E_LIB)

####################################################################
# installing m5 source, and our patch files:
####################################################################

$(PATCHED_SRC): m5_1.1.tar.gz $(PATCH_DIR)/patch_files_list~
	$(PATCH_DIR)/m5e_patch.sh $(PATCH_DIR) $(INSTALL_DIR_ABS)

$(PATCH_DIR)/patch_files_list~: Makefile
	@echo $(PATCH_FILES) > $@

m5_1.1.tar.gz:
	wget http://umn.dl.sourceforge.net/sourceforge/m5sim/m5_1.1.tar.gz
	#wget http://voxel.dl.sourceforge.net/sourceforge/m5sim/m5_1.1.tar.gz
	#wget http://internap.dl.sourceforge.net/sourceforge/m5sim/m5_1.1.tar.gz

####################################################################
# compile the newly patched m5:
####################################################################

$(M5E_LIB): $(PATCHED_SRC) $(PATCH_FILES_REL) $(M5_BUILD_DIR)/m5
	cd $(M5_BUILD_DIR) ; scons ALPHA/libm5shared.so USE_MYSQL=false
	touch $(M5E_LIB) # since the scons call ensures M5E_LIB is up-to-date

$(M5_BUILD_DIR)/m5:
	cd $(M5_BUILD_DIR) ; ln -s .. m5

####################################################################
# cleanup code:
####################################################################

clean:
	cd $(M5_BUILD_DIR) ; $(MAKE) clean

spotless:
	cd $(M5_BUILD_DIR) ; $(MAKE) spotless
	rm $(PATCH_DIR)/patch_files_list~
	

####################################################################
# riskier cleanup code:
####################################################################

delete_m5_install:
	-rm -rf $(INSTALL_DIR)

delete_m5_tarball:
	-rm -rf m5_1.1.tar.gz

true_spotless: delete_m5_install delete_m5_tarball

####################################################################

