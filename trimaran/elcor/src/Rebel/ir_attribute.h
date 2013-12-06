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




///////////////////////////////////////////////////////////////////////////
//
//      File:           ir_attribute.h
//      Authors:        Greg Snider, Scott A. Mahlke
//      Created:        August 1995
//      Description:    Attribute reader/writer functions
//
///////////////////////////////////////////////////////////////////////////

#ifndef _IR_ATTRIBUTE_INCLUDED_
#define _IR_ATTRIBUTE_INCLUDED_

#include "attribute_types.h"
#include "region_attributes.h"
#include "op_attributes.h"
#include "edge_attributes.h"
#include "IR_outstream.h"
#include "IR_instream.h"

// global dispatch functions

IR_outstream &operator<<(IR_outstream &out, Base_attribute *attr);
IR_outstream &operator<<(IR_outstream &out, Base_attribute &attr);

bool scan_attribute_dictionary(IR_instream &in);
Base_attribute* scan_attribute_ptr(IR_instream &in);

// individual attribute functions

// The attribute print functions either print a pointer or a
// dictionary to the output stream. Inlined attributes print directly
// instead of a separate dictionary.

// The attribute pointer scan function should read and assign the
// attribute to the keeper object, while dictionary scan function
// should read in the dictionary. The return value is true if a
// successful scan was performed. E.g.
// bool scan_<attribute>_dictionary(IR_instream &in);
// <attribute_class>* scan_<attribute>_ptr(IR_instream &in); 
// An inlined attribute only has a direct reading function.
// <attribute_class>* scan_<attribute>(IR_instream &in); 

///////////////////////////////////////////////////////////////////////////
// Lcode_attribute_map_attrib

IR_outstream &operator<<(IR_outstream &out, Lcode_attribute_map_attrib *ptr);
IR_outstream &operator<<(IR_outstream &out, Lcode_attribute_map_attrib &attr);
bool scan_lcode_attribute_map_dictionary(IR_instream &in);
Lcode_attribute_map_attrib* scan_lcode_attribute_map_ptr(IR_instream &in);

///////////////////////////////////////////////////////////////////////////
// Reg_to_liveness_map_attrib

IR_outstream &operator<<(IR_outstream &out, Reg_to_liveness_map_attrib *ptr);
IR_outstream &operator<<(IR_outstream &out, Reg_to_liveness_map_attrib &attr);
bool scan_reg_to_liveness_map_dictionary(IR_instream &in);
Reg_to_liveness_map_attrib* scan_reg_to_liveness_map_ptr(IR_instream &in);

///////////////////////////////////////////////////////////////////////////
// Loop_count_attrib

IR_outstream &operator<<(IR_outstream &out, Loop_count_attrib &attr);
Loop_count_attrib* scan_loop_count(IR_instream &in);

///////////////////////////////////////////////////////////////////////////
// Merge_op_BB_id

IR_outstream &operator<<(IR_outstream &out, Merge_op_BB_id &attr);
Merge_op_BB_id* scan_merge_op_bb_id(IR_instream &in);

///////////////////////////////////////////////////////////////////////////
// Reference_dtype_attr

IR_outstream &operator<<(IR_outstream &out, Reference_dtype_attr *ptr);
IR_outstream &operator<<(IR_outstream &out, Reference_dtype_attr &attr);
bool scan_reference_dtype_attr_dictionary(IR_instream &in);
Reference_dtype_attr* scan_reference_dtype_attr_ptr(IR_instream &in);

///////////////////////////////////////////////////////////////////////////
// Physfile_binding_attr

IR_outstream &operator<<(IR_outstream &out, Physfile_binding_attr *ptr);
IR_outstream &operator<<(IR_outstream &out, Physfile_binding_attr &attr);
bool scan_prf_bind_attr_dictionary(IR_instream &in);
Physfile_binding_attr* scan_prf_bind_attr_ptr(IR_instream &in);

///////////////////////////////////////////////////////////////////////////
// Spatial_ref_attr

IR_outstream &operator<<(IR_outstream &out, Spatial_ref_attr *ptr);
IR_outstream &operator<<(IR_outstream &out, Spatial_ref_attr &attr);
bool scan_spatial_ref_attr_dictionary(IR_instream &in);
Spatial_ref_attr* scan_spatial_ref_attr_ptr(IR_instream &in);

///////////////////////////////////////////////////////////////////////////
// Jump_table_name

IR_outstream &operator<<(IR_outstream &out, Jump_table_name &attr);
Jump_table_name* scan_jump_table_name(IR_instream &in);

///////////////////////////////////////////////////////////////////////////
// Op_chaining_attr

IR_outstream &operator<<(IR_outstream &out, Op_chaining_attr *ptr);
IR_outstream &operator<<(IR_outstream &out, Op_chaining_attr &attr);
bool scan_op_chaining_attr_dictionary(IR_instream &in);
Op_chaining_attr* scan_op_chaining_attr_ptr(IR_instream &in);

///////////////////////////////////////////////////////////////////////////
// nametag_attr

IR_outstream &operator<<(IR_outstream &out, const Nametag_attr& attr);
Nametag_attr* scan_nametag_attr(IR_instream &in);

///////////////////////////////////////////////////////////////////////////
// Liveness_info_attrib

IR_outstream &operator<<(IR_outstream &out, Liveness_info_attrib *ptr);
IR_outstream &operator<<(IR_outstream &out, Liveness_info_attrib &attr);
bool scan_liveness_info_dictionary(IR_instream &in);
Liveness_info_attrib* scan_liveness_info_ptr(IR_instream &in);

///////////////////////////////////////////////////////////////////////////
// Downward_use_attrib

IR_outstream &operator<<(IR_outstream &out, Downward_use_attrib *ptr);
IR_outstream &operator<<(IR_outstream &out, Downward_use_attrib &attr);
bool scan_downward_use_dictionary(IR_instream &in);
Downward_use_attrib* scan_downward_use_ptr(IR_instream &in);

///////////////////////////////////////////////////////////////////////////
// Upward_def_attrib

IR_outstream &operator<<(IR_outstream &out, Upward_def_attrib *ptr);
IR_outstream &operator<<(IR_outstream &out, Upward_def_attrib &attr);
bool scan_upward_def_dictionary(IR_instream &in);
Upward_def_attrib* scan_upward_def_ptr(IR_instream &in);

///////////////////////////////////////////////////////////////////////////
// Downward_def_attrib

IR_outstream &operator<<(IR_outstream &out, Downward_def_attrib *ptr);
IR_outstream &operator<<(IR_outstream &out, Downward_def_attrib &attr);
bool scan_downward_def_dictionary(IR_instream &in);
Downward_def_attrib* scan_downward_def_ptr(IR_instream &in);

///////////////////////////////////////////////////////////////////////////
// Control_flow_freq

IR_outstream &operator<<(IR_outstream &out, Control_flow_freq &attr);
Control_flow_freq* scan_control_flow_freq(IR_instream &in);

///////////////////////////////////////////////////////////////////////////
// Exsym_table_attrib

IR_outstream &operator<<(IR_outstream &out, Exsym_table_attrib *ptr);
IR_outstream &operator<<(IR_outstream &out, Exsym_table_attrib &attr);
bool scan_exsym_table_dictionary(IR_instream &in);
Exsym_table_attrib* scan_exsym_table_ptr(IR_instream &in);

///////////////////////////////////////////////////////////////////////////
// Cluster_id_attr

IR_outstream &operator<<(IR_outstream &out, Cluster_id_attr &attr);
Cluster_id_attr* scan_cluster_id_attr(IR_instream &in);

///////////////////////////////////////////////////////////////////////////
// Usched_time_attr

IR_outstream &operator<<(IR_outstream &out, Usched_time_attr &attr);
Usched_time_attr* scan_usched_time_attr(IR_instream &in);

///////////////////////////////////////////////////////////////////////////
// Msched_attr

IR_outstream &operator<<(IR_outstream &out, Msched_attr *ptr);
IR_outstream &operator<<(IR_outstream &out, Msched_attr &attr);
bool scan_msched_attr_dictionary(IR_instream &in);
Msched_attr* scan_msched_attr_ptr(IR_instream &in);

///////////////////////////////////////////////////////////////////////////
// Branch_target_attr

IR_outstream &operator<<(IR_outstream &out, Branch_target_attr &attr);
Branch_target_attr* scan_branch_target_attr(IR_instream &in);

///////////////////////////////////////////////////////////////////////////
// AccSpecAttr

IR_outstream &operator<<(IR_outstream &out, AccSpecAttr &attr);
AccSpecAttr* scan_accspec_attr(IR_instream &in);

#endif
