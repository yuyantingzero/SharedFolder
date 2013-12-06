#!/bin/sh

############################################################################################
# bench_profile.sh
#
#     Compiles a benchmark for the native (host) architecture without optimization
#     to obtain quick profile information using gcc/gprof.
#     
#          - To call, use bench_profile.sh benchmark-name
#          - Writes its output in ./benchmark-name_NATIVE_PROF
#
#     HISTORY:
#          - Initial Version: Steve Lieberman <lieberm 08/09/2005>
############################################################################################

CXX=gcc
CXX_FLAGS="-pg -g -O0"
SCRIPT_NAME="bench_profile.sh"

export HOST_PLATFORM=x86
export RESULT_FILE=result.out
export STDERR_FILE=result.err
export USER_BENCH_PATH1="
${TRIMARAN_ROOT}/benchmarks/simple 
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
${TRIMARAN_ROOT}/benchmarks/specint2000 
${TRIMARAN_ROOT}/benchmarks/specfp2000 
${TRIMARAN_ROOT}/benchmarks/perfect
${TRIMARAN_ROOT}/benchmarks/integer_bench
${TRIMARAN_ROOT}/benchmarks/eecs583
${TRIMARAN_ROOT}/benchmarks/streams
${TRIMARAN_ROOT}/benchmarks/olden
${TRIMARAN_ROOT}/benchmarks/kernels
${TRIMARAN_ROOT}/benchmarks/dvliw_pthreads
"

Usage() {
  echo "usage: $SCRIPT_NAME benchmark_name"
  exit 1;
}

if [ $# -ne 1 ] ; then
  Usage
fi

BENCHMARK=$1
OUTPUT_DIR=${PWD}/${BENCHMARK}_NATIVE_PROF

##### Find the benchmark directory

FIND_BENCH_DIR=${IMPACT_REL_PATH}/scripts/find_bench_dir
if [ ! -e ${FIND_BENCH_DIR} ] ; then
  echo "Error: could not find 'find_bench_dir' in Impact!"
  exit 1;
fi

BENCH_DIR=`${FIND_BENCH_DIR} ${BENCHMARK}`
if [ $? != 0 ] ; then
  echo "> Exiting: Could not find '${BENCHMARK}' using find_bench_dir!"
  echo "> Error message returned by find_bench_dir:"
  echo "$BENCH_DIR";
  exit 1;
fi

##### Load the compile_info from the bench directory

if [ ! -e ${BENCH_DIR}/compile_info ] ; then
  echo "Error: could not find 'compile_info' in bench dir!"
  exit 1;
fi
. $BENCH_DIR/compile_info


##### Create the output dir

if [ ! -d ${OUTPUT_DIR} ] ; then
  mkdir ${OUTPUT_DIR}
fi

if [ ! -d ${OUTPUT_DIR} ] ; then
  echo "Error: could not create '${OUTPUT_DIR}' for output!"
  exit 1;
fi

##### Compile the source with profiling

if [ ! -d ${BENCH_DIR}/src ] ; then
  echo "Error: could not find 'src' in bench dir!"
  exit 1;
fi

SOURCES="${BENCH_DIR}/src/*.c"

# 'compile_info' provides LINKING_PRE/POSTOPTIONS, PREPROCESSING_OPTIONS 
echo ${CXX} -o ${OUTPUT_DIR}/executable ${CXX_FLAGS} ${LINKING_PREOPTIONS} ${PREPROCESSING_OPTIONS}  ${SOURCES} ${LINKING_POSTOPTIONS}
${CXX} -o ${OUTPUT_DIR}/executable ${CXX_FLAGS} ${LINKING_PREOPTIONS} ${PREPROCESSING_OPTIONS}  ${SOURCES} ${LINKING_POSTOPTIONS}

if [ $? != 0 ] ; then
  echo "Error: compilation failed!"
  exit 1;
fi

##### Read exec_info to know how to execute source

# 'compile_info' provides DEFAULT_EVAL
EXEC_INFO="${BENCH_DIR}/exec_info_${DEFAULT_EVAL}"

if [ ! -e ${EXEC_INFO} ] ; then
  echo "Error: could not find '${EXEC_INFO}' specified in 'compile_info'!"
  exit 1;
fi
. $EXEC_INFO

##### Run executable, check results, and cleanup


echo cd ${OUTPUT_DIR}
cd ${OUTPUT_DIR}
echo ${SETUP}
${SETUP}

if [ $? != 0 ] ; then
  echo "Error: setup for executable failed!"
  exit 1;
fi

echo ${PREFIX} executable ${ARGS} 
${PREFIX} executable ${ARGS} > ${RESULT_FILE} 2> ${STDERR_FILE}

if [ $? != 0 ] ; then
  echo "Error: executable failed!"
  exit 1;
fi

echo ${CHECK}
echo ${CHECK} | csh -t

if [ $? != 0 ] ; then
  echo "Error: result check failed!"
  exit 1;
fi

echo ${CLEANUP}
${CLEANUP}

if [ $? != 0 ] ; then
  echo "Error: could not clean up files!"
  exit 1;
fi


##### Profiling output

gprof executable > profile.txt

if [ $? != 0 ] ; then
  echo "Error: error in gprof!"
  exit 1;
fi

echo "Successfully stored profile output in profile.txt (and gmon.out)."



