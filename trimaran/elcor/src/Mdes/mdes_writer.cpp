/******************************************************************************

                    SOFTWARE LICENSE AGREEMENT NOTICE
                   -----------------------------------

IT IS A BREACH OF THIS LICENSE AGREEMENT TO REMOVE THIS NOTICE FROM THE FILE
OR SOFTWARE, OR ANY MODIFIED VERSIONS OF THIS FILE OR SOFTWARE OR DERIVATIVE
WORKS.
___________________________________________________

Copyright Notices/Identification of Licensor(s) of Original Software
in the File


All rights reserved by the foregoing, respectively.
___________________________________________________

Copyright Notices/Identification of Subsequent Licensor(s)/Contributors of
Derivative Works

Copyright 1994 Hewlett-Packard Company

All rights reserved by the foregoing, respectively.
___________________________________________________

The code contained in this file, including both binary and source [if released
by the owner(s)] (hereafter, Software) is subject to copyright by the
respective Licensor(s) and ownership remains with such Licensor(s).  The
Licensor(s) of the original Software remain free to license their respective
proprietary Software for other purposes that are independent and separate from
this file, without obligation to any party.

Licensor(s) grant(s) you (hereafter, Licensee) a license to use the Software
for academic, research and internal business purposes only, without a fee.
"Internal business purposes" means that Licensee may install, use and execute
the Software for the purpose of designing and evaluating products.  Licensee
may submit proposals for research support, and receive funding from private
and Government sponsors for continued development, support and maintenance of
the Software for the purposes permitted herein.

Licensee may also disclose results obtained by executing the Software, as well
as algorithms embodied therein.  Licensee may redistribute the Software to
third parties provided that the copyright notices and this License Agreement
Notice statement are reproduced on all copies and that no charge is associated
with such copies. No patent or other intellectual property license is granted
or implied by this Agreement, and this Agreement does not license any acts
except those expressly recited.

Licensee may modify the Software to make derivative works (as defined in
Section 101 of Title 17, U.S. Code) (hereafter, Derivative Works), as
necessary for its own academic, research and internal business purposes.
Title to copyrights and other proprietary rights in Derivative Works created
by Licensee shall be owned by Licensee subject, however, to the underlying
ownership interest(s) of the Licensor(s) in the copyrights and other
proprietary rights in the original Software.  All the same rights and licenses
granted herein and all other terms and conditions contained in this Agreement
pertaining to the Software shall continue to apply to any parts of the
Software included in Derivative Works.  Licensee's Derivative Work should
clearly notify users that it is a modified version and not the original
Software distributed by the Licensor(s).

If Licensee wants to make its Derivative Works available to other parties,
such distribution will be governed by the terms and conditions of this License
Agreement.  Licensee shall not modify this License Agreement, except that
Licensee shall clearly identify the contribution of its Derivative Work to
this file by adding an additional copyright notice to the other copyright
notices listed above, to be added below the line "Copyright
Notices/Identification of Subsequent Licensor(s)/Contributors of Derivative
Works."  A party who is not an owner of such Derivative Work within the
meaning of U.S. Copyright Law (i.e., the original author, or the employer of
the author if "work of hire") shall not modify this License Agreement or add
such party's name to the copyright notices above.

Each party who contributes Software or makes a Derivative Work to this file
(hereafter, Contributed Code) represents to each Licensor and to other
Licensees for its own Contributed Code that:

(a)  Such Contributed Code does not violate (or cause the Software to
violate) the laws of the United States, including the export control laws of
the United States, or the laws of any other jurisdiction.

(b)  The contributing party has all legal right and authority to make such
Contributed Code available and to grant the rights and licenses contained in
this License Agreement without violation or conflict with any law.

(c)  To the best of the contributing party's knowledge and belief, the
Contributed Code does not infringe upon any proprietary rights or intellectual
property rights of any third party.

LICENSOR(S) MAKE(S) NO REPRESENTATIONS ABOUT THE SUITABILITY OF THE SOFTWARE
OR DERIVATIVE WORKS FOR ANY PURPOSE.  IT IS PROVIDED "AS IS"    WITHOUT
EXPRESS OR IMPLIED WARRANTY, INCLUDING BUT NOT LIMITED TO THE MERCHANTABILITY,
USE OR FITNESS FOR ANY PARTICULAR PURPOSE AND ANY WARRANTY AGAINST
INFRINGEMENT OF ANY INTELLECTUAL PROPERTY RIGHTS.  LICENSOR(S) SHALL NOT BE
LIABLE FOR ANY DAMAGES SUFFERED BY THE USERS OF THE SOFTWARE OR DERIVATIVE
WORKS.

Any Licensee wishing to make commercial use of the Software or Derivative
Works should contact each and every Licensor to negotiate an appropriate
license for such commercial use, and written permission of all Licensors will
be required for such a commercial license.  Commercial use includes (1)
integration of all or part of the source code into a product for sale by or on
behalf of Licensee to third parties, or (2) distribution of the Software or
Derivative Works to third parties that need it to utilize a commercial product
sold or licensed by or on behalf of Licensee.

By using or copying this Contributed Code, Licensee agrees to abide by the
copyright law and all other applicable laws of the U.S., and the terms of this
License Agreement.  Any individual Licensor shall have the right to terminate
this license immediately by written notice upon Licensee's breach of, or
non-compliance with, any of its terms.  Licensee may be held legally
responsible for any copyright infringement that is caused or encouraged by
Licensee's failure to abide by the terms of this License Agreement.

******************************************************************************/




/////////////////////////////////////////////////////////////////////////////
//
//      File:           mdes_writer.cpp
//      Authors:        Vinod Kathail
//      Created:        March 1998
//      Description:    Mdes writer
//
/////////////////////////////////////////////////////////////////////////////

#include "mdes_writer.h"
#include "bit_vector.h"
#include "intf.h"
#include "hash_functions.h"

Mdes_writer::Mdes_writer(MDES* mdes_in, char* file_name) 
    :include_path(""),
     spec_str("SPECULATIVE"),
     non_spec_str("NONSPECULATIVE"),
     is_pred_str("is_pred"),
     is_leop_str("is_leop"),
     is_advload_str("is_advload"),
     is_assoc_str("is_assoc"),
     is_comm_str("is_comm"),
     is_two_operand_str("is_two_operand"),
     is_unsupported_str("is_unsupported"),
     opclass_str(hash_opclass, 10), resclass_str(hash_resclass, 10),
     mdes(mdes_in), out(file_name, 80, 2), 
     io_descriptors(hash_Io_descr_ptr, 50), 
     resource_usages(hash_res_use_descr_ptr, 50),
     reservation_tables(hash_res_use_descr_ptr, 50),
     latency_descriptors(hash_lat_descr_ptr, 50),
     scheduling_alternatives(hash_triple_io_res_use_lat_descrs_ptrs, 50),
     operand_latencies(hash_int, 50)
{
    init_opclass_map();
    init_resclass_map();
}

Mdes_writer::~Mdes_writer() {}

void Mdes_writer::set_include_path(char* path)
{
  include_path = path;
}

void Mdes_writer::init_opclass_map() 
{
    opclass_str.bind(NULLOP, "NULLOP");
    opclass_str.bind(LOADOP, "LOADOP");
    opclass_str.bind(STOREOP, "STOREOP");
    opclass_str.bind(BRANCHOP, "BRANCHOP");
    opclass_str.bind(ARITHOP, "ARITHOP");
}

void Mdes_writer::init_resclass_map() 
{
    resclass_str.bind(RES_NULL, "NULL");
    resclass_str.bind(RES_INT,  "INT");
    resclass_str.bind(RES_FLOAT, "FLOAT");
    resclass_str.bind(RES_MEM, "MEMORY");
    resclass_str.bind(RES_BRANCH, "BRANCH");
    resclass_str.bind(RES_LMEM, "LOCALMEMORY");
}


void Mdes_writer::print_mdes()
{
  collect_sa_info();
  collect_resource_info();
  collect_operand_latencies();
  print_include();
  print_misc();
  print_registers();
  print_constant_sets();
  print_register_files();
  print_field_types();
  print_io_descriptors();
  print_resources();
  print_resource_usages();
  print_reservation_tables();
  print_operand_latencies();
  print_latency_descrs();
  print_sas();
  print_arch_ops();
  print_elcor_flags();
  print_elcor_ops();

  //print_operand_hookup();
//   print_primitive_op();
//   print_customop_semantics();
//   print_custom_opcode();
}


void Mdes_writer::collect_sa_info()
{		
  int io_descr_num = 0;
  int reservation_table_num = 0;
  int lat_descr_num = 0;
  int scheduling_alternative_num = 0;

  MdesHashtable* hash_table = mdes->get_MDES_hashtable();
  hash_table->init_next_op();
  Op_descr* op_descr;

  while ((op_descr = hash_table->get_next_op()) != NULL) {
    if (op_descr->get_binding() == UNITOP) {
      op_descr->io_list_init();
      Io_list* io_list;

      while ((io_list = op_descr->get_next_io()) != NULL) {
	Io_descr* io_descr = io_list->get_iod();
	assert(io_descr);
	if (!io_descriptors.is_bound(io_descr)) {
	  io_descriptors.bind(io_descr, io_descr_num);
	  io_descr_num++;
	}

	io_list->init_alt();
	Alt_descr* alt_descr; 
	while((alt_descr = io_list->get_next_alt()) != NULL) {
	  Lat_descr* lat_descr = alt_descr->get_lat();
	  assert(lat_descr);
	  if (!latency_descriptors.is_bound(lat_descr)) {
	    latency_descriptors.bind(lat_descr, lat_descr_num);
	    lat_descr_num++;
	  }

	  Res_use_descr* res_table = alt_descr->get_res_use();
	  if (!reservation_tables.is_bound(res_table)) {
	    reservation_tables.bind(res_table, reservation_table_num);
	    reservation_table_num++;
	  }

	  Triple<Io_descr*, Res_use_descr*, Lat_descr*> 
	     sa(io_descr, res_table,  lat_descr);
	  if (!scheduling_alternatives.is_bound(sa)) {
	    scheduling_alternatives.bind(sa, scheduling_alternative_num);
	    scheduling_alternative_num++;
	  }
	}
      }
    }
  }
}
	
	
void Mdes_writer::collect_resource_info()
{
  int  resource_usage_num = 0;
  
  for(Hash_map_domain_iterator<Res_use_descr*, int> 
	  res_tables(reservation_tables); res_tables != 0; res_tables++) {
    Res_use_descr* res_use = *res_tables;
    while (res_use != NULL) {
      if (!resource_usages.is_bound(res_use)) {
	resource_usages.bind(res_use, resource_usage_num);
	resource_usage_num++;
      }
      res_use = res_use->get_next();
    }
  }
}

void Mdes_writer::collect_operand_latencies()
{
  int i;
  for(Hash_map_domain_iterator<Lat_descr*, int> lats(latency_descriptors); 
      lats != 0; lats++) {
    Lat_descr* lat_descr = *lats;

    // Destination times
    for (i = 1; i < MDES_maxout; i++) {
      operand_latencies += lat_descr->get_flow_lat(DATA_OUT, i);
    }
    
    // Source times
    for (i = 1; i < MDES_maxin; i++) {
      operand_latencies += lat_descr->get_flow_lat(DATA_IN, i);
    }
    // Predicate time 
    operand_latencies +=  lat_descr->get_flow_lat(DATA_IN, 0);

    // Times for sync destinations
    for (i = 0; i < MDES_maxsynchout; i++) {
      operand_latencies += lat_descr->get_flow_lat(SYNC_OUT, i);
    }

    // Times for sync sources
    for (i = 0; i < MDES_maxsynchin; i++) {
      operand_latencies += lat_descr->get_flow_lat(SYNC_IN, i);
    }
    // Exception time 
    operand_latencies += lat_descr->get_anti_lat(DATA_IN, 0);
    
    // Times output registers should be allocated
    for (i = 1; i < MDES_maxout; i++) {
      operand_latencies += lat_descr->get_anti_lat(DATA_OUT, i); 
    }
  }
}

void Mdes_writer::print_include() 
{
  out << "$include \"" << include_path << "structure_pristine.hmdes2\" "
      << endline << endline ;
}

// Print Register section
void Mdes_writer::print_registers()
{
  // Print section header "SECTION Register { "
  out << "SECTION Register" << endline << LEFT_BRACE << endline;

  int total_files = mdes->get_MDES_num_reg_types();

  // reverse map for macros
  Hash_map<eString, Hash_set<eString> > reg_to_macro_map(hash_estring, 32);

  // followed by register information
  for (int file_index = 0; file_index < total_files; file_index++) {
      Reg_descr* reg_descr = mdes->get_MDES_reg_descr(file_index);
      char* file_name = reg_descr->get_aname();

      // Print static registers in the form "GPR_30  ();"
      if (reg_descr->get_scap() > 0) {
	for (int i = 0; i < reg_descr->get_scap(); i++) {
          out << file_name << "_" << i << indent;
	  out << "();" << indent << indent << endfield;
	}
	out << endline;
      }

      // Print rotating registers in the form " "GPR[30]"  ();"
      if (reg_descr->get_rcap() > 0) {
	for (int j = 0;j < reg_descr->get_rcap(); j++) {
	  out << "\'" << file_name << "[" << j << "]\'";
	  out << indent << "();" << indent << indent << endfield;
	}
	out << endline;
      }

      // enter the reverse macro mapping so we can output it later
      Hash_map<eString, Hash_set<eString> >& macro_to_reg_map = reg_descr->get_physical_reg_map();
      for (Hash_map_iterator<eString, Hash_set<eString> > iter(macro_to_reg_map);
           iter != 0; iter++) {
         eString macro = (*iter).first;
         Hash_set<eString> regs = (*iter).second;
         for (Hash_set_iterator<eString> iter2(regs); iter2 != 0; iter2++) {
            eString reg = *iter2;
            if (!reg_to_macro_map.is_bound(reg)) {
               Hash_set<eString> macroset(hash_estring, 4);
               macroset += macro;
               reg_to_macro_map.bind(reg, macroset);
            } else {
               Hash_set<eString> macroset = reg_to_macro_map.value(reg);
               macroset += macro;
               reg_to_macro_map.bind(reg, macroset);
            }
         }
      }
  }

  // macro attributes.
  for (Hash_map_iterator<eString, Hash_set<eString> > i(reg_to_macro_map);
       i != 0; i++) {
     eString reg = (*i).first;
     out << "'" << reg << "'" << indent;
     out << LEFT_PAREN << "macro ||" << LEFT_PAREN;
     for (Hash_set_iterator<eString> ii((*i).second); ii != 0; ii++) {
        eString macro = *ii;
        out << macro << indent;
     }
     out << RIGHT_PAREN << RIGHT_PAREN << ";" << endfield;
     out << endline;
  }

  // followed by }
  out << RIGHT_BRACE << endline;

}
/*
//Print Operand_hookup section
void Mdes_writer::print_operand_hookup()
{
  //Print section header "SECTION Operand_Hookup{ "
    out << "SECTION Operand_Hookup" << endline << LEFT_BRACE << endline;

    Operand_hookup* op_hookup;

    op_hookup = mdes->get_MDES_operand_hookup();
    int hookupsize = op_hookup->get_hookup_size();
    
    for(int i = 0; i< hookupsize; i++)
    {
        char* operand_name = op_hookup->get_hookup_name(i);
        out << operand_name << LEFT_PAREN << RIGHT_PAREN << ";" << endline;
    }

    out << RIGHT_BRACE << endline;
}
*/
void Mdes_writer::print_custom_opcode()
{
  // FIX: The format changed as of 10/17/05

//     //Print section header "SECTION Custom_Opcode{ "
//     out << "SECTION Custom_Opcode" << endline << LEFT_BRACE << endline;

//     out << "CUSTOM" << LEFT_PAREN << "op" << LEFT_PAREN;

//     Customop_semantics* customop;
//     int custom_size;

//     custom_size = mdes->customop_ptr_tab.dim();
    
//     for(int i = 0; i<custom_size; i++)
//     {
//         customop = mdes->get_MDES_customop_semantics(i);
//         char* customop_name = customop->get_customop();
//         out << customop_name << " ";
//     }

//     out << RIGHT_PAREN << RIGHT_PAREN << ";" << endline;
//     out << RIGHT_BRACE << endline;

}

void Mdes_writer::print_customop_semantics()
{
  //Print section header "SECTION Customop_Semantics{ "
//   out << "SECTION Customop_Semantics" << endline << LEFT_BRACE << endline;

//   Customop_semantics* customop;

//   int custom_size;
//   custom_size = mdes->customop_ptr_tab.dim();

//   for(int i = 0;i<custom_size; i++)
//     {
//       char* custom_name;

//       customop = mdes->get_MDES_customop_semantics(i);

//       custom_name = customop->get_customop();
        
//       out << custom_name << LEFT_PAREN << "op" << LEFT_PAREN;

//       for(int j = 0; j < (customop->get_unitop_size()); j++)
//         {
// 	  char* primitive_name;
// 	  primitive_name = customop->get_primitiveop_ptr(j)->get_name();
// 	  //primitive_name = customop->get_name(j);
            
// 	  out << primitive_name << " ";
//         }

//       out << RIGHT_PAREN << RIGHT_PAREN << ";" << endline;
//     }

//   out << RIGHT_BRACE << endline;
}
    
void Mdes_writer::print_primitive_op()
{
  //Print section header "SECTION Primitive_Op{ "
//   out << "SECTION Primitive_Op" << endline << LEFT_BRACE << endline;

//   Customop_semantics* customop;

//   Primitive_op* primitive_op;

//   int custom_size;
//   custom_size = mdes->customop_ptr_tab.dim();
    
//   for(int i = 0; i<custom_size; i++) {
//     customop=mdes->get_MDES_customop_semantics(i);
    
//     int size;

//     size = customop->get_unitop_size();

//     for(int j =0; j <size; j++) {
//       char* primitive_name;
//       char* op_name;
            
//       primitive_op = customop->get_primitiveop_ptr(j);
//       primitive_name = primitive_op->get_name();
//       op_name = primitive_op->get_name();

//       out << primitive_name << LEFT_PAREN << "op" << LEFT_PAREN << op_name << RIGHT_PAREN << " ";

//       out << "pre" << LEFT_PAREN;
//       char* predicate_name = primitive_op->get_pred()->get_name();
//       out << predicate_name << " ";
	    
//       out << RIGHT_PAREN << " ";

//       out << "src" << LEFT_PAREN;
//       for(int m = 0; m < (primitive_op->get_src_size()); m++) {
// 	char* source_name = primitive_op->get_srcs()[m]->get_name();

// 	out << source_name << " ";
//       }

//       out << RIGHT_PAREN << " ";

//       out << "dest" << LEFT_PAREN;
//       for(int n = 0; n < (primitive_op->get_dest_size()); n++) {
// 	char* destination_name = primitive_op->get_dests()[n]->get_name();

// 	out << destination_name << " ";
//       }

//       out << RIGHT_PAREN << RIGHT_PAREN << ";" << endline;
//     }
//   }

//   out << RIGHT_BRACE << endline;
}   

void Mdes_writer::print_constant_sets()
{

  // Print section header "SECTION Constant_Range { "
  out << "SECTION Constant_Range" << endline << LEFT_BRACE << endline;
  int j = 0;
  for(Hash_map_iterator<eString, Const_set_descr*> hmi(mdes->name_to_const_desc_map);
      hmi != 0; hmi++, j++) {
    eString set_name = (*hmi).first;
    Const_set_descr* desc = (*hmi).second;
    for(unsigned i = 0; i < desc->mins.size(); i++) {
      out << indent << "Set_" << j << "_range_" << i << LEFT_PAREN
	  << " min" << LEFT_PAREN << desc->mins[i] << RIGHT_PAREN
	  << endfield << "  max" << LEFT_PAREN << desc->maxes[i] << RIGHT_PAREN
	  << endfield << "  step" << LEFT_PAREN << desc->steps[i] << RIGHT_PAREN
	  << endfield << RIGHT_PAREN << ";" << endline;
    }
  }
  out << RIGHT_BRACE << endline;

  out << "SECTION Constant_Set" << endline << LEFT_BRACE << endline;
  j = 0;
  for(Hash_map_iterator<eString, Const_set_descr*> hmi(mdes->name_to_const_desc_map);
      hmi != 0; hmi++, j++) {
    eString set_name = (*hmi).first;    
    Const_set_descr* desc = (*hmi).second;

    out << indent << set_name << LEFT_PAREN << "ranges" << LEFT_PAREN;
    for(unsigned i = 0; i < desc->mins.size(); i++) {
      out << "Set_" << j << "_range_" << i << " ";
    }
    out << RIGHT_PAREN << RIGHT_PAREN << ";" << endline;
  }
  out << RIGHT_BRACE << endline;

}

// Print Register_file section
void Mdes_writer::print_register_files()
{
  // Print section header "SECTION Register_File { "
  out << "SECTION Register_File" << endline << LEFT_BRACE << endline; 

  int total_files = mdes->get_MDES_num_reg_types();
  // followed by register information
  for (int file_index = 0; file_index < total_files; file_index++) {
      Reg_descr* reg_descr = mdes->get_MDES_reg_descr(file_index);
      char* file_name = reg_descr->get_aname();
      int i;
      
      out << file_name;
      out << indent << indent << LEFT_PAREN;
      out << "width" << LEFT_PAREN << reg_descr->get_width()
	  << RIGHT_PAREN << endline;
      
      out << "static" << LEFT_PAREN;
      for (i = 0; i < reg_descr->get_scap(); i++) {
        out << file_name << "_" << i << indent << endfield;
      }  
      out << RIGHT_PAREN << endline;
      
      out << "rotating" << LEFT_PAREN;
      for (i = 0; i < reg_descr->get_rcap(); i++) {
	out << "\'" << file_name << "[" << i << "]\'" << indent << endfield;
      }  
      out << RIGHT_PAREN << endline;
      
      out << "speculative" << LEFT_PAREN << (reg_descr->get_tag() ? 1 : 0) 
	  << RIGHT_PAREN << endline;
      out << "virtual" << LEFT_PAREN << "\"" << reg_descr->get_vname() 
	  << "\"" << RIGHT_PAREN << endfield;

      switch (reg_descr->get_regkind()) {
      case REG_LITINT:
      case REG_LITINTRANGE: {
	out << endline << "consts" << LEFT_PAREN;
	out << reg_descr->ilits->get_name() << endfield;
	out << RIGHT_PAREN << endfield;
	break;
      }
      default:
	break;
      }

      out << RIGHT_PAREN << ";" << endline;
  }

  // followed by }
  out << RIGHT_BRACE << endline;
}


// Print field types
void Mdes_writer::print_field_types()
{
  // Print section header "SECTION Field_Type { "
  out << "SECTION Field_Type" << endline << LEFT_BRACE << endline;     
  
  // Npw print all the field types
  for (int i = 0; i < mdes->get_MDES_num_field_types(); i++) {
      Ft_descr* ft_descr = mdes->get_MDES_ft_descr(i);

      const Bitvector& bv = ft_descr->get_iocode();
      
      out << ft_descr->get_name() << indent << indent << indent 
	  << indent << indent << LEFT_PAREN;
      // If the Iocode has only one bit set, then it is an elementary 
      // field type defined in terms of a register file.
      if (bv.ones_count() == 1) {
	  out << "regfile " << LEFT_PAREN
	      << mdes->find_MDES_rf_name(bv, IO_PHYSICAL)
	      << RIGHT_PAREN;
      }
      // If not, then it is a "compound" field type defined in terms of 
      // other field types.
      else {
	  Bit_iterator bi(bv);
	  out << "compatible_with " << LEFT_PAREN;
	  for (; bi != 0; bi++) {
            Bitvector bv1(IOCODE_SIZE);
            bv1.set_bit(*bi);
            out << indent << mdes->find_MDES_rf_name(bv1, IO_VIRTUAL);
	  }
	  out << RIGHT_PAREN;
      }
      out << RIGHT_PAREN << ";" << endline;
  }
  out << RIGHT_BRACE << endline;
}

void Mdes_writer::print_io_descriptors()
{
  // Print section header "SECTION Operation_Format { "
  out << "SECTION Operation_Format" << endline << LEFT_BRACE << endline; 

  int i;
  for(Hash_map_domain_iterator<Io_descr*, int> iods(io_descriptors); 
      iods != 0; iods++) {
    Io_descr* io_descr = *iods;
    
    out << "OF_" << io_descriptors.value(io_descr) << indent << indent 
	<< endfield;
    out << LEFT_PAREN;

    if (io_descr->is_predicated()) {
      out << "pred" << LEFT_PAREN 
	  << mdes->find_MDES_rf_name(io_descr->get_pred(), IO_VIRTUAL)
	  << RIGHT_PAREN << endline;
    }

    out << "src" << LEFT_PAREN;
    for (i = 0; i < io_descr->get_num_in(); i++) {
      out << mdes->find_MDES_rf_name(io_descr->get_in_reg(i), IO_VIRTUAL)
	  << indent << endfield;
    } 
    out << RIGHT_PAREN << endline;

    out << "dest" << LEFT_PAREN;
    for (i = 0; i < io_descr->get_num_out(); i++) {
      out << mdes->find_MDES_rf_name(io_descr->get_out_reg(i), IO_VIRTUAL)
	  << indent << endfield;
    } 
    out << RIGHT_PAREN;
    out << RIGHT_PAREN << " ;" << endline;
  }

  // followed by }
  out << RIGHT_BRACE << endline;
}

      
void Mdes_writer::print_resources()
{

  // Print section header "SECTION Resource { "
  out << "SECTION Resource" << endline << LEFT_BRACE << endline; 
  
  int res_num = mdes->get_MDES_num_resources();
  for (int i = 0; i < res_num; i++) {
    Res_descr* resource = mdes->get_MDES_res_descr(i);
    out << resource->get_name() << endfield 
	<< indent << indent << LEFT_PAREN;

    if (resource->get_count()!=1)
      out << endfield << "count" << LEFT_PAREN 
	  << resource->get_count() << RIGHT_PAREN;

    if (resource->get_width() != 0) {
      out << endfield << "width" << LEFT_PAREN 
	  << (int)resource->get_width() << RIGHT_PAREN;
    }
    out << RIGHT_PAREN << " ;" << endline;
      }
  // followed by }
  out << RIGHT_BRACE << endline;
}

void Mdes_writer::print_resource_usages()
{
  // Print section header "SECTION Resource_Usage { "
  out << "SECTION Resource_Usage" << endline << LEFT_BRACE << endline; 

  for(Hash_map_domain_iterator<Res_use_descr*, int> rus(resource_usages); 
      rus != 0; rus++) {
    Res_use_descr* res_use = *rus;
    int res_index = res_use->get_res();
    int time = res_use->get_time();
    int count = res_use->get_count();
    out << "RU_" << resource_usages.value(res_use) << endfield
	<< indent << indent << LEFT_PAREN 
        << "use" << LEFT_PAREN 
	<< (mdes->get_MDES_res_descr(res_index))->get_name()
	<< RIGHT_PAREN << indent << endfield 
        << "time" << LEFT_PAREN << time << RIGHT_PAREN << endfield;
    if (count!=1)
      out << "count" << LEFT_PAREN << count << RIGHT_PAREN << endfield;
    out << RIGHT_PAREN << " ;" << endline;
  }
  // followed by }
  out << RIGHT_BRACE << endline;
}

void Mdes_writer::print_reservation_tables()
{
  // Print section header "SECTION Reservation_Table { "
  out << "SECTION Reservation_Table" << endline << LEFT_BRACE << endline; 

  for(Hash_map_domain_iterator<Res_use_descr*, int> 
	  res_tables(reservation_tables); res_tables != 0; res_tables++) {
    Res_use_descr* res_use = *res_tables;
    out << "RT_" << reservation_tables.value(res_use) << indent << indent
        << endfield
        << LEFT_PAREN << "use" << LEFT_PAREN;
    while (res_use != NULL) {
      out << "RU_" << resource_usages.value(res_use) << indent << endfield;
	res_use = res_use->get_next();
    }
    out << RIGHT_PAREN << RIGHT_PAREN << " ;" << endline;
  }
  // followed by }
  out << RIGHT_BRACE << endline;
}

void Mdes_writer::print_operand_latencies()
{
  // Print section header "SECTION Operand_Latency { "
  out << "SECTION Operand_Latency" << endline << LEFT_BRACE << endline; 
  for (Hash_set_iterator<int> elems(operand_latencies); elems != 0; elems++) {
    int latency = *elems;
    out << "TIME_" << latency << indent << indent 
	<< LEFT_PAREN << "time" << LEFT_PAREN << latency 
	<< RIGHT_PAREN << RIGHT_PAREN << ";" << endline;
  }
  // followed by }
  out << RIGHT_BRACE << endline;
}

void Mdes_writer::print_latency_descrs()
{
  // Print section header "SECTION Operation_Latency { "
  out << "SECTION Operation_Latency" << endline << LEFT_BRACE << endline; 
  
  int i;
  for(Hash_map_domain_iterator<Lat_descr*, int> lats(latency_descriptors); 
      lats != 0; lats++) {
    Lat_descr* lat_descr = *lats;
    out << "OL_" << latency_descriptors.value(lat_descr) 
	<< indent << indent << endfield << LEFT_PAREN;

    // Print the times destinations are available
    out << "dest" << LEFT_PAREN;
    for (i = 1; i < MDES_maxout; i++) {
      out << "TIME_"
	  <<lat_descr->get_flow_lat(DATA_OUT, i) << indent << endfield;
    }
    out << RIGHT_PAREN << endline;

    // Print the times real sources are sampled
    out << "src" << LEFT_PAREN;
    for (i = 1; i < MDES_maxin; i++) {
      out << "TIME_"
	  <<lat_descr->get_flow_lat(DATA_IN, i) << indent << endfield;
    }
    out << RIGHT_PAREN << endline;

    // Print the time predicate input is sampled
    out << "pred" << LEFT_PAREN;
    out <<  "TIME_" 
	<< lat_descr->get_flow_lat(DATA_IN, 0) << indent << endfield;
    out << RIGHT_PAREN << endline;

    // Print the times sync destinations are available
    out << "sync_dest" << LEFT_PAREN;
    for (i = 0; i < MDES_maxsynchout; i++) {
      out <<  "TIME_" 
	  << lat_descr->get_flow_lat(SYNC_OUT, i) << indent << endfield;
    }
    out << RIGHT_PAREN << endline;

    // Print the times sync sources are sampled
    out << "sync_src" << LEFT_PAREN;
    for (i = 0; i < MDES_maxsynchin; i++) {
      out <<   "TIME_" 
	  << lat_descr->get_flow_lat(SYNC_IN, i) << indent << endfield;
    }
    out << RIGHT_PAREN << endline;

    // Print the exception time (or deallocation time)
    out << "exc" << LEFT_PAREN;
    out <<   "TIME_" 
	<< lat_descr->get_anti_lat(DATA_IN, 0) << indent << endfield;
    out << RIGHT_PAREN << endline;
    
    // Print the times output registers should be allocated
    out << "rsv" << LEFT_PAREN;
    for (i = 1; i < MDES_maxout; i++) {
      out <<   "TIME_" 
	  << lat_descr->get_anti_lat(DATA_OUT, i) << indent << endfield;
    }
    out << RIGHT_PAREN << RIGHT_PAREN << ";" << endline;
  }
  // followed by }
  out << RIGHT_BRACE << endline;
}
    
void Mdes_writer::print_sas()
{
  // Print section header "SECTION Scheduling_Alternative { "
  out << "SECTION Scheduling_Alternative" << endline << LEFT_BRACE 
      << endline; 

  for(Hash_map_domain_iterator
	  <Triple<Io_descr*, Res_use_descr*, Lat_descr*>, int>
	  sas(scheduling_alternatives); sas != 0; sas++) {

    Triple<Io_descr*, Res_use_descr*, Lat_descr*> sched  = *sas;
    out << "SA_" << scheduling_alternatives.value(sched) << indent 
	<< indent << endfield;
    out << LEFT_PAREN;
    out << "format" << LEFT_PAREN << "OF_" 
	<< io_descriptors.value(sched.first) << RIGHT_PAREN << endline;
    out << "resv" << LEFT_PAREN << "RT_" 
      	<< reservation_tables.value(sched.second) << RIGHT_PAREN << endline;
    out << "latency" << LEFT_PAREN << "OL_" 
      	<< latency_descriptors.value(sched.third) << RIGHT_PAREN << endfield;
    out << RIGHT_PAREN << " ;" << endline;
      
  }
  // followed by }
  out << RIGHT_BRACE << endline;
}

void Mdes_writer::print_arch_ops()
{
  // Print section header "SECTION Operation { "
  out << "SECTION Operation" << endline << LEFT_BRACE 
      << endline; 

  MdesHashtable* hash_table = mdes->get_MDES_hashtable();
  hash_table->init_next_op();
  Op_descr* op_descr;
  while ((op_descr = hash_table->get_next_op()) != NULL) {
    if (op_descr->get_binding() == UNITOP) {

      // Print opcode 
      int len = strlen (op_descr->get_aname_ptr());
      out << "'" << op_descr->get_aname_ptr() << "'";
      print_spaces(len, 22);
      out << indent << endfield;
      out << LEFT_PAREN << "alt" << LEFT_PAREN;

      op_descr->io_list_init();
      Io_list* io_list;
      while ((io_list = op_descr->get_next_io()) != NULL) {
	Io_descr* io_descr = io_list->get_iod();

	io_list->init_alt();
	Alt_descr* alt_descr; 
	while((alt_descr = io_list->get_next_alt()) != NULL) {

	  Lat_descr* lat_descr = alt_descr->get_lat();
	  Res_use_descr* res_table = alt_descr->get_res_use();

	  Triple<Io_descr*, Res_use_descr*, Lat_descr*> 
	     sa(io_descr, res_table,  lat_descr);
	  out << "SA_" << scheduling_alternatives.value(sa) << " "
	      << endfield;
	}
      }
      out << RIGHT_PAREN << RIGHT_PAREN << " ;" << endline ; 
    }
  }
  // followed by }
  out << RIGHT_BRACE << endline;

}

void Mdes_writer::print_elcor_flags ()
{
  // Print section header "SECTION Elcor_Operation_Flag { "
  out << "SECTION Elcor_Operation_Flag" << endline << LEFT_BRACE 
      << endline; 
  // Print op class 

  eString& op1 = opclass_str.value(NULLOP);
  out << op1;
  print_spaces(op1.length(), 16);
  out << "() ;" << endline;

  eString& op2 = opclass_str.value(LOADOP);
  out << op2;
  print_spaces(op2.length(), 16);
  out << "() ;" << endline;

  eString& op3 = opclass_str.value(STOREOP);
  out << op3;
  print_spaces(op3.length(), 16);
  out << "() ;" << endline;

  eString& op4 = opclass_str.value(BRANCHOP);
  out << op4;
  print_spaces(op4.length(), 16);
  out << "() ;" << endline;

  eString& op5 = opclass_str.value(ARITHOP);
  out << op5;
  print_spaces(op5.length(), 16);
  out << "() ;" << endline;

  // Print resource class

  eString& res1 = resclass_str.value(RES_NULL);
  out << res1;
  print_spaces(res1.length(), 16);
  out << "() ;" << endline;

  eString& res2 = resclass_str.value(RES_INT);
  out << res2;
  print_spaces(res2.length(), 16);
  out << "() ;" << endline;

  eString& res3 = resclass_str.value(RES_FLOAT);
  out << res3;
  print_spaces(res3.length(), 16);
  out << "() ;" << endline;

  eString& res4 = resclass_str.value(RES_MEM);
  out << res4;
  print_spaces(res4.length(), 16);
  out << "() ;" << endline;

  eString& res5 = resclass_str.value(RES_BRANCH);
  out << res5;
  print_spaces(res5.length(), 16);
  out << "() ;" << endline;

  eString& res6 = resclass_str.value(RES_LMEM);
  out << res6;
  print_spaces(res6.length(), 16);
  out << "() ;" << endline;
  
  // Print other flags
  out << non_spec_str;
  print_spaces(non_spec_str.length(), 16);
  out << "() ;" << endline;

  out << spec_str;
  print_spaces(spec_str.length(), 16);
  out << "() ;" << endline;

  out << is_pred_str;
  print_spaces(is_pred_str.length(), 16);
  out << "() ;" << endline;

  out << is_leop_str;
  print_spaces(is_leop_str.length(), 16);
  out << "() ;" << endline;

  out << is_advload_str;
  print_spaces(is_advload_str.length(), 16);
  out << "() ;" << endline;

  out << is_assoc_str;
  print_spaces(is_assoc_str.length(), 16);
  out << "() ;" << endline;

  out << is_comm_str;
  print_spaces(is_comm_str.length(), 16);
  out << "() ;" << endline;

  out << is_two_operand_str;
  print_spaces(is_two_operand_str.length(), 16);
  out << "() ;" << endline;

  out << is_unsupported_str;
  print_spaces(is_unsupported_str.length(), 16);
  out << "() ;" << endline;

  // followed by }
  out << RIGHT_BRACE << endline;
}


void Mdes_writer::print_elcor_ops ()
{
  // Print section header "SECTION Elcor_Operation { "
  out << "SECTION Elcor_Operation" << endline << LEFT_BRACE 
      << endline; 

  MdesHashtable* hash_table = mdes->get_MDES_hashtable();

  hash_table->init_next_op();
  Op_descr* op_descr;
  while ((op_descr = hash_table->get_next_op()) != NULL) {
    if (op_descr->get_binding() == CLSTROP) {

      // Print opcode 
      int len = strlen (op_descr->get_aname_ptr());
      out << op_descr->get_aname_ptr();
      print_spaces(len, 22);
      out << indent << endfield;
      out << LEFT_PAREN << "op" << LEFT_PAREN;

      op_descr->io_list_init();
      Io_list* io_list;
      while ((io_list = op_descr->get_next_io()) != NULL) {

	io_list->init_alt();
	Alt_descr* alt_descr; 
	while((alt_descr = io_list->get_next_alt()) != NULL) {
	  Op_descr* aop = alt_descr->get_op();
	  out << "'" << aop->get_aname_ptr() << "' "
	      << endfield;
	}
      }

      out << RIGHT_PAREN << endline;
      out << "flags" << LEFT_PAREN;
      out << opclass_str.value(op_descr->get_opclass()) << " " << endfield
	  << resclass_str.value(op_descr->get_resclass()) << " " << endfield
	  << (op_descr->has_spec() ? spec_str : non_spec_str);
      out << RIGHT_PAREN;
      out << RIGHT_PAREN << " ;" << endline ; 
    }
  }
  // followed by }
  out << RIGHT_BRACE << endline;
}

// This part should probably be printed based on the actual MDES.  But it
// doesn't change often so just hard code it for now.  -KF 2/2005
void Mdes_writer::print_misc()
{
  out << "$def !num_int_param_reg 4" << endline;
  out << "$def !num_flt_param_reg 4" << endline;
  out << "$def !num_dbl_param_reg 4" << endline;
  out << "$def !num_pv_reg        8" << endline;
  out << endline;
  out << "SECTION Register_Class" << endline;
  out << "{" << endline;
  out << "  CALLER();" << endline;
  out << "  CALLEE();" << endline;
  out << "}" << endline;
  out << endline;
  out << "SECTION Stack_Descr" << endline;
  out << "{" << endline;
  out << "  STACK_DESCR(" << endline;
  out << "    Dir(\"HIGH_TO_LOW\")          // HIGH_TO_LOW or LOW_TO_HIGH" << endline;
  out << "    RetAddrThruStack(\"true\")    // true or false" << endline;
  out << "    RetAddrSize(4) " << endline;
  out << "    Alignment(8)" << endline;
  out << "  );" << endline;
  out << "}" << endline;
  out << endline;
  out << "SECTION Macro_Class {" << endline;
  out << "  READ_ONLY();" << endline;
  out << "  PSEUDO();" << endline;
  out << "  ALIAS();" << endline;
  out << "  MULTI_ALIAS();" << endline;
  out << "  COHERENT();" << endline;
  out << "}" << endline;
  out << endline;
  out << "SECTION Macro" << endline;
  out << "{" << endline;
  out << "  LOCAL(class(PSEUDO));" << endline;
  out << "  PARAM(class(PSEUDO));" << endline;
  out << "  SWAP(class(PSEUDO));" << endline;
  out << "  INT_RETURN_TYPE(class(PSEUDO));" << endline;
  out << "  FLT_RETURN_TYPE(class(PSEUDO));" << endline;
  out << "  DBL_RETURN_TYPE(class(PSEUDO));" << endline;
  out << "  INT_RETURN();" << endline;
  out << "  $for (N in $1..(num_int_param_reg)) { \"INT_PARAM_${N}\"(); }" << endline;
  out << "  FLT_RETURN();" << endline;
  out << "  $for (N in $1..(num_flt_param_reg)) { \"FLT_PARAM_${N}\"(); }  " << endline;
  out << "  DBL_RETURN();" << endline;
  out << "  $for (N in $1..(num_dbl_param_reg)) { \"DBL_PARAM_${N}\"(); }  " << endline;
  out << "  INT_TM_TYPE(class(PSEUDO));" << endline;
  out << "  FLT_TM_TYPE(class(PSEUDO));" << endline;
  out << "  DBL_TM_TYPE(class(PSEUDO));" << endline;
  out << "  SP_REG();				// Stack pointer" << endline;
  out << "  FP_REG();				// Frame pointer" << endline;
  out << "  IP_REG(alias(SP_REG));		// Incoming parameter space" << endline;
  out << "  OP_REG(alias(SP_REG));		// Outgoing parameter space" << endline;
  out << "  LV_REG(alias(SP_REG));		// Local variable space" << endline;
  out << "  RGS_REG(alias(SP_REG));		// Register swap space" << endline;
  out << "  LC_REG();				// Loop Counter" << endline;
  out << "  ESC_REG();				// Epilogue stage counter" << endline;
  out << "  ALL_ROT_PRED(class(MULTI_ALIAS));	// All the rotating predicate registers" << endline;
  out << "  ALL_PRED(class(MULTI_ALIAS));	        // The entire predicate register file" << endline;
  out << "  ALL_STATIC_PRED(class(MULTI_ALIAS));	" << endline;
  out << "  RRB();	" << endline;
  out << "  RETURN_ADDR();" << endline;
  out << "  FLT_ZERO(class(READ_ONLY));" << endline;
  out << "  FLT_ONE(class(READ_ONLY));" << endline;
  out << "  DBL_ZERO(class(READ_ONLY));" << endline;
  out << "  DBL_ONE(class(READ_ONLY));" << endline;
  out << "  INT_ZERO(class(READ_ONLY));" << endline;
  out << "  PRED_FALSE(class(READ_ONLY));" << endline;
  out << "  PRED_TRUE(class(READ_ONLY));" << endline;
  out << "  ICMOVE_REG();" << endline;
  out << "  $for (N in $0..(num_pv_reg-1)) { \"PV_${N}\"(class(MULTI_ALIAS)); }  " << endline;
  out << "}" << endline;
  out << endline;
}

void Mdes_writer::print_spaces(int field_length, int fixed_width)
{
    int spaces_to_print;
    if (field_length >= fixed_width) {
	out << endline;
	spaces_to_print = fixed_width;
      }
      else {
	spaces_to_print = fixed_width - field_length;
      }
      for (int i = 0; i < spaces_to_print; i++) {
	out << " ";
      }
}

unsigned hash_res_use_descr_ptr(Res_use_descr*& v) 
{
  return (ELCOR_INT_HASH(v)) ;
}

unsigned hash_lat_descr_ptr(Lat_descr*& v)
{
  return (ELCOR_INT_HASH(v)) ;
}

unsigned hash_triple_io_res_use_lat_descrs_ptrs(Triple<Io_descr*,
						Res_use_descr*, Lat_descr*>& 
						v)
{
  Io_descr* io = v.first;
  return (ELCOR_INT_HASH(io)) ;
}


unsigned hash_opclass(OpClass& v)
{
  return (ELCOR_INT_HASH(v)) ;
}

unsigned hash_resclass(ResClass& v)
{
  return (ELCOR_INT_HASH(v)) ;
}
