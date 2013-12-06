#!/bin/sh

SCRIPT="m5e_patch.sh"
M5_DIRNAME="m5_1.1"            # dir name that the tarball normally creates 
M5_TARBALL="m5_1.1.tar.gz"     # name of M5 tarball

PATCH_DIR=$1
PATCH_FILES_DIR="${PATCH_DIR}/patch_files"
INSTALL_DIR=$2
INITIAL_DIR=$PWD

# files to copy to m5 dir, some are new, some are overwriting m5 files
# ...(directories will be automatically created by this script)
PATCH_FILES=`cat ${PATCH_DIR}/patch_files_list~`

#############################################################################
# BEGIN FUNCTIONS
#############################################################################

Error() {
  echo ""
  echo "ERROR: $1"
  exit 1;
}

ErrorIfFailed() {
  if [ "$?" -ne 0 ] ; then
    Error "$1"
  fi
}

Usage() {
  echo "Usage: ${SCRIPT} patch_dir install_dir"
  exit 1;
}

#############################################################################
# END FUNCTIONS
#############################################################################



if [ $# -ne 2 ] ; then     # check number of command line arguments
 Usage
fi

if [ ! -f $M5_TARBALL ] ; then
  Error "could not locate M5 source: need \"${M5_TARBALL}\""
fi

if [ ! -d $PATCH_FILES_DIR ] ; then
  Error "could not find patch dir: \"${PATCH_FILES_DIR}\""
fi

# if INSTALL_DIR doesn't exist, then the tarball was never extracted,
# so extract it and move its contents to INSTALL_DIR
if [ ! -d $INSTALL_DIR ] ; then
  # create INSTALL_DIR
  mkdir -p $INSTALL_DIR > /dev/null 2>&1
  ErrorIfFailed "could not create install dir: ${INSTALL_DIR}" 
  if [ ! -d $INSTALL_DIR ] ; then
    Error "install dir does not exist after creation attempt: $INSTALL_DIR"
  fi

  cd $INSTALL_DIR

  # extract tarball
  ABS_M5_TARBALL=${INITIAL_DIR}/${M5_TARBALL}
  echo -n "Extracting ${M5_TARBALL}............"
  tar xzvf $ABS_M5_TARBALL > /dev/null 2>&1
  ErrorIfFailed "could not extract from: \"${ABS_M5_TARBALL}\""
  echo "done."

  if [ ! -d $M5_DIRNAME ] ; then
    Error "extracting M5 did not create the expected dir: \"${M5_DIRNAME}\""
  fi

  # move tarball contents to INSTALL_DIR
  echo -n "Renaming............................"
  mv -f ${M5_DIRNAME}/* . > /dev/null 2>&1
  ErrorIfFailed "could not move: \"${M5_DIRNAME}\""
  rmdir $M5_DIRNAME > /dev/null 2>&1
  ErrorIfFailed "could not remove: \"${M5_DIRNAME}\""
  echo "done."
else

  # INSTALL_DIR already exists, just use it
  cd $INSTALL_DIR
  ErrorIfFailed "could not chdir to $INSTALL_DIR"
fi


ABS_PATCH_FILES_DIR=${INITIAL_DIR}/${PATCH_FILES_DIR}
if [ ! -d  $ABS_PATCH_FILES_DIR ] ; then
  Error "could not find patch dir: ${ABS_PATCH_FILES_DIR}"
fi

echo -n "Patching files......................"
for patch_file in $PATCH_FILES ; do
  dest_dir_rel=`echo $patch_file | sed -e 's/[^/]*$//'`
  dest_dir=$PWD/$dest_dir_rel
  source_file=${ABS_PATCH_FILES_DIR}/${patch_file}
  dest_file=${PWD}/${patch_file}
  if [ ! -e $source_file ] ; then
    echo ""
    echo "source file: $source_file"
    Error "could not find original patch file $patch_file"
  fi
  if [ ! -d $dest_dir ] ; then
    mkdir -p ${dest_dir} > /dev/null 2>&1
    ErrorIfFailed "could not create dir $dest_dir for $patch_file"
  fi
  if [ ! -d $dest_dir ] ; then
    Error "dir $dest_dir_rel does not exist after creation attempt"
  fi
  if [ ! -L $dest_file ] ; then
    # the symbolic link doesn't exist yet

    if [ -f $dest_file ] ; then
      # there's a file where we want to put this symbolic link, probably
      # a file from the original tarball that we're patching

      if [ -f ${dest_file}.orig ] ; then
	echo "while working with: $patch_file"
	Error "failed trying to patch file that was incorrectly patched before"
      else
	# we're okay, let's move the original file to .orig and then create
	# a symbolic link to the patch in its place
	mv -f $dest_file ${dest_file}.orig > /dev/null 2>&1
	ErrorIfFailed "couldn't move away original file $dest_file"

	if [ -f ${dest_file} ] ; then
	  Error "file $dest_file exists after we tried to move it away"
	fi
	if [ ! -f ${dest_file}.orig ] ; then
	  Error "backup file ${dest_file}.orig wasn't created properly"
	fi

      fi
    fi

    # the symlink is actually created here
    ln -s $source_file $dest_dir > /dev/null 2>&1
    ErrorIfFailed "could not symlink to $source_file in dir $dest_dir"

    if [ ! -L $dest_file ] ; then
      Error "symlink $dest_file is not a symlink after creation attempt"
    fi
  fi
  # else, the symlink already exists, assume it points to the correct
  #       place -- do nothing
      
done

touch patch_complete > /dev/null 2>&1
ErrorIfFailed "could not create patch_complete file."
echo "done."

