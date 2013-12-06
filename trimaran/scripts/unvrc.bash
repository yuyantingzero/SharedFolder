
[ "$TRIMARAN_ROOT" != "" ] || return

export PATH=`perl -e '$pattern = "$ENV{TRIMARAN_ROOT}" . "[^:]+:?"; $p = $ENV{PATH}; $p =~ s/$pattern//g; $p =~ s/:$//g; print "$p"'`

unset TRIMARAN_ROOT
unset TRIMARAN_HOST_TYPE
unset TRIMARAN_HOST_COMPILER

unset IMPACT_REL_PATH
unset IMPACT_ROOT
unset IMPACT_HOST_PLATFORM
unset IMPACT_HOST_COMPILER
unset IMPACT_BUILD_TYPE
unset STD_PARMS_FILE
unset DEFAULT_PROJECT

unset ELCOR_HOME
unset ELCOR_BIN 
unset ELCOR_PARMS_FILE
unset ELCOR_REL_PATH  
unset ELCOR_USER      

unset LEPROC_HOME     
unset LEPROC_BIN      
unset LEPROC_PARMS_FILE
unset LEPROC_REL_PATH  

unset TOOLBOX_HOME	

unset SIMU_REL_PATH  
unset SIMU_HOME      
unset SIMU_BIN_PATH  
unset SIMU_PARMS_FILE

unset ARM_REL_PATH	

unset WIMS_REL_PATH	
unset WIMS_BIN_PATH	
unset WIMS_XLATOR_PARMS_FILE

unset GUI_REL_PATH		
unset GUI_BIN		

unset TAS_HOME		
unset TAS_BIN		
unset TAS_PARMS_FILE		
unset TLD_PARMS_FILE		
unset TAS_REL_PATH		

unset DAVINCIHOME		
unset DAVINCIEXE		

unset ADL_HOME
unset ADL_PARMS_FILE
unset DC_SCRIPTS_PATH

unset M5_ROOT
unset M5_OUTPUT_DIR
unset M5_CONFIG_FILE
unset LD_LIBRARY_PATH


#benchmark related stuff
unset PGPPATH 


