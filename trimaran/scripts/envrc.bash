export TRIMARAN_HOST_TYPE=x86lin
export TRIMARAN_ROOT=~/trimaran
export TRIMARAN_HOST_COMPILER=gcc
export IMPACT_REL_PATH=${TRIMARAN_ROOT}/openimpact
export IMPACT_ROOT=${IMPACT_REL_PATH}
export IMPACT_HOST_PLATFORM=x86lin
export IMPACT_HOST_COMPILER=gcc
export IMPACT_BUILD_TYPE=${IMPACT_HOST_PLATFORM}
export STD_PARMS_FILE=${IMPACT_REL_PATH}/parms/STD_PARMS.IMPACT
export DEFAULT_PROJECT=full
export CC=gcc
export CXX=g++

export ELCOR_HOME=${TRIMARAN_ROOT}/elcor
export ELCOR_BIN=$ELCOR_HOME/bin
export ELCOR_PARMS_FILE=${ELCOR_HOME}/parms/ELCOR_PARMS
export ELCOR_REL_PATH=${ELCOR_HOME}
export ELCOR_USER=ELCOR

export SIMU_REL_PATH=${TRIMARAN_ROOT}/simu
export SIMU_HOME=${SIMU_REL_PATH}
export SIMU_BIN_PATH=$SIMU_REL_PATH/bin
export SIMU_PARMS_FILE=$SIMU_REL_PATH/parms/SIMULATOR_DEFAULTS
export DINERO_HOME=$SIMU_REL_PATH/dinero

export ARM_REL_PATH=${TRIMARAN_ROOT}/arm

export GUI_REL_PATH=${TRIMARAN_ROOT}/gui
export GUI_BIN=$GUI_REL_PATH/bin

export TAS_HOME=${TRIMARAN_ROOT}/tas
export TAS_BIN=${TAS_HOME}/bin
export TAS_PARMS_FILE=${TAS_HOME}/parms/TAS_PARMS
export TAS_REL_PATH=${TAS_HOME}

export USER_BENCH_PATH1="${TRIMARAN_ROOT}/benchmarks/simple 
                         ${TRIMARAN_ROOT}/benchmarks/utils 
                         ${TRIMARAN_ROOT}/benchmarks/mediabench 
                         ${TRIMARAN_ROOT}/benchmarks/netbench 
                         ${TRIMARAN_ROOT}/benchmarks/encryption 
                         ${TRIMARAN_ROOT}/benchmarks/mibench/automotive 
                         ${TRIMARAN_ROOT}/benchmarks/mibench/network
                         ${TRIMARAN_ROOT}/benchmarks/mibench/telecomm
                         ${TRIMARAN_ROOT}/benchmarks/mibench/security
                         ${TRIMARAN_ROOT}/benchmarks/mibench/consumer
                         ${TRIMARAN_ROOT}/benchmarks/specint92 
                         ${TRIMARAN_ROOT}/benchmarks/specfp92 
                         ${TRIMARAN_ROOT}/benchmarks/specint95 
                         ${TRIMARAN_ROOT}/benchmarks/specfp95 
                         ${TRIMARAN_ROOT}/benchmarks/specint2000 
                         ${TRIMARAN_ROOT}/benchmarks/specfp2000 
                         ${TRIMARAN_ROOT}/benchmarks/specint2006 
                         ${TRIMARAN_ROOT}/benchmarks/specfp2006 
                         ${TRIMARAN_ROOT}/benchmarks/perfect
                         ${TRIMARAN_ROOT}/benchmarks/integer_bench
                         ${TRIMARAN_ROOT}/benchmarks/eecs583
                         ${TRIMARAN_ROOT}/benchmarks/streams
                         ${TRIMARAN_ROOT}/benchmarks/olden
                         ${TRIMARAN_ROOT}/benchmarks/kernels
                         "

export DAVINCIHOME=/usr/share/daVinci_V2.1
export DAVINCIEXE=/usr/share/daVinci_V2.1

# M5elements
export M5_ROOT=$TRIMARAN_ROOT/m5/install
export M5_OUTPUT_DIR=.
export M5_CONFIG_FILE=$M5_ROOT/m5/m5elements/default.py
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$M5_ROOT/m5/build/ALPHA

#benchmark related stuff
export PGPPATH=./

export PATH=${PATH}:${IMPACT_REL_PATH}/bin:${IMPACT_REL_PATH}/scripts:${IMPACT_REL_PATH}/driver:${ELCOR_BIN}:${SIMU_BIN_PATH}:${TRIMARAN_ROOT}/scripts:${GUI_BIN}:${TAS_BIN}:${ARM_REL_PATH}

export SUIFHOME=${TRIMARAN_ROOT}/suif
export MACHINE=x86-redhat-linux
export COMPILER_NAME=gcc
eval `$SUIFHOME/setup_suif -sh 2> /dev/null`
export SUIFPATH=$SUIFPATH:/usr/bin
