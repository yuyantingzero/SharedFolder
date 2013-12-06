# input params

# MAKEFILE is this file
# FILELIST is a file that points to a list of c source files
# SETUP is a string of setup commands
# ARGS is a string of profile arguments
# CHECK is a string of commands that check the profile output
# CLEANUP is a string of cleanup commands

# info for profiling
F2C = -f2c

# Renaming all "gnumake" -> "make" to allow easier installation
# for Trimaran release.
# All instances of the word "make" in this file USED to be "gnumake"
#                                              <lieberm 01/30/2007>

#==========================================================================
default:
	@echo "There is no default make for this makefile"

#==========================================================================
# Standard paths for HP Labs!!

# st_noprof: Standard non-profiling path: generate basic block code with
#               classical opti and memory deps
st_noprof:
	@make -f $(MAKEFILE) chtop
	@make -f $(MAKEFILE) flatten
	@make -f $(MAKEFILE) pprofile_dummy
	@make -f $(MAKEFILE) callgraph
	@make -f $(MAKEFILE) psplit
	@make -f $(MAKEFILE) pinline_dummy
	@make -f $(MAKEFILE) pip
	@make -f $(MAKEFILE) ptoh_sync_C
	@make -f $(MAKEFILE) create_hcode_dir
	@make -f $(MAKEFILE) htol
	@make -f $(MAKEFILE) create_lcode_dir
	@make -f $(MAKEFILE) lopti
	@make -f $(MAKEFILE) lplaydohO
	@make -f $(MAKEFILE) clean_c2h
	@make -f $(MAKEFILE) clean_h2o
	@make -f $(MAKEFILE) clean_o2mc

# st_bb: Standard basic block path: profile, inlining, memory deps, classical opti
#
st_bb:
	@make -f $(MAKEFILE) chtop
	@make -f $(MAKEFILE) flatten
	@make -f $(MAKEFILE) pprofile
	@make -f $(MAKEFILE) callgraph
	@make -f $(MAKEFILE) psplit
	@make -f $(MAKEFILE) pinline
	@make -f $(MAKEFILE) pip
	@make -f $(MAKEFILE) ptoh_sync_C
	@make -f $(MAKEFILE) create_hcode_dir
	@make -f $(MAKEFILE) htol
	@make -f $(MAKEFILE) create_lcode_dir
	@make -f $(MAKEFILE) lopti
	@make -f $(MAKEFILE) lopti_prof
	@make -f $(MAKEFILE) lplaydohOp
	@make -f $(MAKEFILE) clean_c2h
	@make -f $(MAKEFILE) clean_h2o
	@make -f $(MAKEFILE) clean_o2mc

# st_sb: Standard superblock path: profile, inlining, memory deps, classical opti,
#		superblock formation, ILP opti
st_sb:
	@make -f $(MAKEFILE) chtop
	@make -f $(MAKEFILE) flatten
	@make -f $(MAKEFILE) pprofile
	@make -f $(MAKEFILE) callgraph
	@make -f $(MAKEFILE) psplit
	@make -f $(MAKEFILE) pinline
	@make -f $(MAKEFILE) pip
	@make -f $(MAKEFILE) ptoh_sync_C
	@make -f $(MAKEFILE) create_hcode_dir
	@make -f $(MAKEFILE) htol
	@make -f $(MAKEFILE) create_lcode_dir
	@make -f $(MAKEFILE) lopti
	@make -f $(MAKEFILE) lsuper
	@make -f $(MAKEFILE) lsuper_prof
	@make -f $(MAKEFILE) lplaydohSp
	@make -f $(MAKEFILE) clean_c2h
	@make -f $(MAKEFILE) clean_h2o
	@make -f $(MAKEFILE) clean_o2mc

# st_hb: Standard hyperblock path: profile, inlining, classical opti,
#		hyperblock formation, ILP opti
#	 Currently no memory deps are available on this path!!!!!!!!,
#	 This will be fixed.
st_hb:
	@make -f $(MAKEFILE) chtop
	@make -f $(MAKEFILE) flatten
	@make -f $(MAKEFILE) pprofile
	@make -f $(MAKEFILE) callgraph
	@make -f $(MAKEFILE) psplit
	@make -f $(MAKEFILE) pinline
	@make -f $(MAKEFILE) pip
	@make -f $(MAKEFILE) ptoh_sync_C
	@make -f $(MAKEFILE) create_hcode_dir
	@make -f $(MAKEFILE) htol
	@make -f $(MAKEFILE) create_lcode_dir
	@make -f $(MAKEFILE) lopti
	@make -f $(MAKEFILE) lopti_prof
	@make -f $(MAKEFILE) lhyper
	@make -f $(MAKEFILE) lhsuper
	@make -f $(MAKEFILE) lhsuper_prof
	@make -f $(MAKEFILE) lplaydohHSp
	@make -f $(MAKEFILE) clean_c2h
	@make -f $(MAKEFILE) clean_h2o
	@make -f $(MAKEFILE) clean_o2mc

#==========================================================================
# make Pcode steps

chtop:
	@echo ""
	@echo "Preprocess C source code"
	\rm -rf pcode
	mkdir pcode
	gen_ChtoP-hp pcode < $(FILELIST)

flatten:
	@echo ""
	@echo "Flatten pcode"
	ls pcode/*.pc > pc_files
	gen_Psplit pc_files "-Psp_format=None"
	rm -f pc_files
	rm -f impact_mapping
	ls pcode | grep ".pc"$$ > pc_files
	gen_PtoP-flatten pcode < pc_files
	rm -f pc_files

pprofile:
	@echo ""
	@echo "Pcode profiling"
	gen_Pprofiled_hppa pcode -noflatten -ext pcf $(F2C) -setup '$(SETUP)' \
		-args '$(ARGS)' -check '$(CHECK)' -cleanup '$(CLEANUP)'

pprofile_dummy:
	@echo ""
	@echo "DUMMY step for Pcode profiling!!"
	ls pcode | grep ".pcf"$$ > pcf_files
	gen_copy pcode pcode pcf.p < pcf_files
	rm -f pcf_files

callgraph:
	@echo ""
	@echo "Run PIP to get call graph information and merge"
	@echo "Should only be done for C programs!!"
	ls pcode | grep "pcf.p"$$ > pcp_files
	gen_PIP pcode -call_graph < pcp_files
	rm -f pcp_files

psplit:
	@echo ""
	@echo "Split files into files with one function each"
	ls pcode/*.pcg > pcg_files
	gen_Psplit pcg_files '-Psp_dir=pcode'
	rm -f pcg_files

pinline:
	@echo ""
	@echo "Pcode inlining"
	gen_Pinline '-Pil_dir=pcode'
	@echo ""
	rm -f pcode_split/*.temp
	rm -f pcode_split/*.temp.e
	rm -f pcode_split/*.scale

pinline_dummy:
	@echo ""
	@echo "DUMMY step for Pcode inlining"
	ls pcode | grep "pcs"$$ > pcs_files
	gen_copy pcode pcode pci < pcs_files
	rm -f pcs_files

pip:
	@echo ""
	@echo "Run PIP, should only be done for C programs!!"
	ls pcode | grep ".pci"$$ > pci_files
	gen_PIP pcode -full_ip < pci_files
	rm -f pci_files

#==========================================================================
# P to H translation - Only one of the ptoh's should be done!!

ptoh_nosync:
	@echo ""
	@echo "Pcode to Hcode translation"
	ls pcode | grep ".pci"$$ > pcode_files
	gen_PtoH pcode '-Fdd_arith_min_max_exp=yes \
		-Fgenerate_hcode_swp_prags=yes'\
		< pcode_files
#		-noflatten < pcode_files
	rm -f pcode_files

# C assumes IPA is done!!!
ptoh_sync_C:
	@echo ""
	@echo "Pcode to Hcode translation with sync arcs for C"
	ls pcode | grep ".pci"$$ > pcode_files
	gen_PtoH-sync pcode '-Fdd_arith_min_max_exp=yes\
		-Fgenerate_jsr_dependences=no -Fgenerate_hcode_swp_prags=yes \
		-Fdo_dep_anal_for_C=yes -Fdo_loop_conversion_on_C=yes \
		-Fdo_loop_conv_if_unpromoted_iter_var=no \
		-Fannotate_callsites_with_side_effects=yes \
		-Fmerge_interprocedural_data=yes' < pcode_files
#		-Fmerge_interprocedural_data=yes' -noflatten < pcode_files
	rm -f pcode_files

ptoh_sync_F:
	@echo ""
	@echo "Pcode to Hcode translation with sync arcs for Fortran"
	ls pcode | grep ".pci"$$ > pcode_files
	gen_PtoH-sync pcode '-Fdd_equiv_disambig_level=1 \
		-Fdd_arith_min_max_exp=yes\
		-Fgenerate_jsr_dependences=no -Fgenerate_hcode_swp_prags=yes'\
		-Fdo_loop_conv_if_unpromoted_iter_var=no \
		< pcode_files
#		-noflatten < pcode_files
	rm -f pcode_files

create_hcode_dir:
	@echo ""
	@echo "Create hcode directory"
	rm -r -f hcode
	mkdir hcode
	ls pcode | grep ".hc"$$ > hc_files
	gen_move pcode hcode hc < hc_files
	rm -f hc_files
	ls pcode | grep ".hch"$$ > hch_files
	gen_move pcode hcode hch < hch_files
	rm -f hch_files

#==========================================================================
# translate Hcode to Lcode

htol:
	@echo ""
	@echo "Generate Lcode from Hcode"
	ls hcode | grep ".hc"$$ > hc_files
	gen_HtoL hcode hppa PA_7100 '-Fgenerate_sync_arcs=yes' < hc_files
	rm -f hc_files

create_lcode_dir:
	@echo ""
	@echo "Create lcode directory"
	rm -r -f lcode
	mkdir lcode
	ls hcode | grep ".lc"$$ > lc_files
	gen_move hcode lcode lc < lc_files
	rm -f lc_files

#==========================================================================
# Standard optimizations

lopti:
	@echo ""
	@echo "Traditional code optimization"
	ls lcode | grep "lc"$$ > lc_files
	gen_Lopti lcode hppa PA_7100 4 '-Fuse_sync_arcs=yes\
		-Fnon_excepting_ops=yes -Fdo_post_inc_conv=yes' < lc_files
	rm -f lc_files

lopti_prof:
	@echo ""
	@echo "Lcode profile .O files"
	ls lcode | grep "O"$$ > O_files
	gen_profiled_hppa lcode -lcode -loop_iters -setup '$(SETUP)'\
		-args '$(ARGS)' -check '$(CHECK)' -cleanup '$(CLEANUP)' < O_files
	rm -f O_files

#==========================================================================
# Superblock formation/optimization

lsuper:
	@echo ""
	@echo "Superblock formation and ILP optimization"
	ls lcode | grep ".O"$$ > O_files
	gen_Lsuperscalar lcode hppa PA_7100 '-Fuse_sync_arcs=yes\
		-Fnon_excepting_ops=yes' < O_files
	rm -f O_files

lsuper_prof:
	@echo ""
	@echo "Lcode profile .S files"
	ls lcode | grep "S"$$ > S_files
	gen_profiled_hppa lcode -lcode -setup '$(SETUP)'\
		-args '$(ARGS)' -check '$(CHECK)' -cleanup '$(CLEANUP)' < S_files
	rm -f S_files

#==========================================================================
# Hyperblock formation/optimization

lhyper:
	@echo ""
	@echo "Hyperblock formation"
	ls lcode | grep "O.p"$$ > Op_files
	gen_Lhyper lcode impact hp-lcode '-Feliminate_sync_arcs=yes\
		-Fnon_excepting_ops=yes -Flabel_and_reg_access_indep=yes'\
		< Op_files
	rm -f Op_files

lhsuper:
	@echo ""
	@echo "Hyperblock ILP optimization"
	ls lcode | grep ".H"$$ > H_files
	gen_LsuperscalarH lcode hppa PA_7100 '-Fnon_excepting_ops=yes\
		-Flabel_and_reg_access_indep=yes -Fpred_exec_support=yes\
		-Fdo_branch_combining=no' < H_files
	rm -f H_files

lhsuper_prof:
	@echo ""
	@echo "Lcode profile .HS files"
	ls lcode | grep "HS"$$ > HS_files
	gen_profiled_hppa lcode -lcode -setup '$(SETUP)'\
		-args '$(ARGS)' -check '$(CHECK)' -cleanup '$(CLEANUP)' < HS_files
	rm -f HS_files

#==========================================================================
# Playdoh code generation

lplaydohO:
	@echo ""
	@echo "Run Lplaydoh Phase 1 on .O files"
	ls lcode | grep "O"$$ > O_files
	gen_Lplaydoh lcode v1-hp 1 < O_files
	rm -f O_files

lplaydohOp:
	@echo ""
	@echo "Run Lplaydoh Phase 1 on .O files"
	ls lcode | grep "O.p"$$ > Op_files
	gen_Lplaydoh lcode v1-hp 1 < Op_files
	rm -f Op_files

lplaydohSp:
	@echo ""
	@echo "Run Lplaydoh Phase 1 on .S.p files"
	ls lcode | grep "S.p"$$ > Sp_files
	gen_Lplaydoh lcode v1-hp 1 < Sp_files
	rm -f Sp_files

lplaydohHSp:
	@echo ""
	@echo "Run Lplaydoh Phase 1 on .HS.p files"
	ls lcode | grep "HS.p"$$ > HSp_files
	gen_Lplaydoh lcode v1-hp 1 < HSp_files
	rm -f HSp_files

#==========================================================================
# clean up

clean_c2h:
	@echo ""
	@echo "Clean up after compilation from source to Hcode"
	rm -f pcode/*.i
	rm -f pcode/*.c1
	rm -f pcode/*.pc
	rm -f pcode/*.pc.dosuper
	rm -f pcode/*.pcode.stat
	rm -f pcode/*.pc.log
	rm -f pcode/*.pcs
	rm -f pcode/*.pcs.temp
	rm -f pcode/*.pci
	rm -f pcode/*.pch
	rm -f pcode/*.pcf
	rm -f pcode/*.pcf.p
	rm -f pcode/*.pcg
	rm -f pcode/*.hc.stat
	rm -f hcode/*.hc
	rm -f hcode/*.hch
	rm -f hcode/*.hc.stat
	rm -f hcode/*.hc.c
	rm -f hcode/*.hcp
	rm -f hcode/*.hcp.cg
	rm -f impact.ip.cg impact.pc.ip impact.pc.ip1
	rm -f test.hc test.pc.log test.pcode.stat
	rm -f functype.pch impact_mapping Pinline.log
	rmdir pcode

clean_h2o:
	@echo ""
	@echo "Clean up after classic optimization"
	rm -f hcode/*.cg.tar.gz
	rm -f hcode/*.hcp
	rm -f hcode/*.hcp.stat
	rm -f hcode/*.hcp.trace
	rm -f hcode/*.hcp.cg
	rm -f hcode/impact.hcode.profile
	rm -f lcode/*.lc
	rm -f lcode/*.lcf
	rm -f impact1_inline impact1_rename
	rmdir hcode

clean_o2mc:
	@echo ""
	@echo "Clean up after mc file generation"
	rm -f lcode/*.O
	rm -f lcode/*.O.p
	rm -f lcode/*.H
	rm -f lcode/*.S
	rm -f lcode/*.S.p
	rm -f lcode/*.HS
	rm -f lcode/*.HS.p

#==========================================================================
