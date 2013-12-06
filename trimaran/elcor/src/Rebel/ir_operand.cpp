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
//      File:           ir_operand.cpp
//      Authors:        Greg Snider, Scott A. Mahlke, Sadun Anik
//      Created:        August 1995
//      Description:    Elcor Text IR reader-writer for class Operand
//
///////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "ir_operand.h"
#include "IR_symbol.h"

//-------------------------- Writer -------------------------------

IR_outstream &operator<<(IR_outstream &out, const Reg_file &reg_file);

IR_outstream &operator<<(IR_outstream &out, const Macro_name &name);

IR_outstream &operator<<(IR_outstream &out, const Operand &op)
{
    // Write out an operand;

    const Base_operand& operand = *op.sptr->optr ;
    if (operand.is_int()) {
	out << INT_OPERAND_STRING << COLON << operand.dtype() << L_ANGLE
	    << ((const Int_lit&) operand).value() ;
	if (operand.assigned_to_physical_file())
	   out << (char*)operand.physical_file_type() << R_ANGLE;
	else
	   out << R_ANGLE;
    }
    else if (operand.is_predicate())
	out << PREDICATE_OPERAND_STRING << COLON << operand.dtype() << L_ANGLE
	    << (((const Pred_lit&) operand).value() ? 't' : 'f') << R_ANGLE;
    else if (operand.is_float()) {
	out << FLOAT_OPERAND_STRING << COLON << operand.dtype() << L_ANGLE
	    << ((const Float_lit&) operand).value();
	if (operand.assigned_to_physical_file())
	   out << (char*)operand.physical_file_type() << R_ANGLE;
	else
	   out << R_ANGLE;
    }

    else if (operand.is_double()) {
	out << DOUBLE_OPERAND_STRING << COLON << operand.dtype() << L_ANGLE
	    << ((const Double_lit&) operand).value();
	if (operand.assigned_to_physical_file())
	   out << (char*)operand.physical_file_type() << R_ANGLE;
	else
	   out << R_ANGLE;
    }

    else if (operand.is_string()) {
	out << STRING_OPERAND_STRING << COLON << operand.dtype() << L_ANGLE
	    << (char*)((const String_lit&)operand).value();
	if (operand.assigned_to_physical_file())
	   out << (char*)operand.physical_file_type() << R_ANGLE;
	else
	   out << R_ANGLE;
    }

    else if (operand.is_label()) {
	out << LABEL_OPERAND_STRING << COLON << operand.dtype() << L_ANGLE
	    << (char*)((const Label_lit&)operand).value();
	if (operand.assigned_to_physical_file())
	   out << (char*)operand.physical_file_type() << R_ANGLE;
	else
	   out << R_ANGLE;
    }

    else if (operand.is_cb()) {
	out << CB_OPERAND_STRING << COLON << operand.dtype() << L_ANGLE
	    << ((const Cb_operand&) operand).id();
	if (operand.assigned_to_physical_file())
	   out << (char*)operand.physical_file_type() << R_ANGLE;
	else
	   out << R_ANGLE;
    }

    else if (operand.is_undefined())
	out << UNDEFINED_OPERAND_STRING  << L_ANGLE << R_ANGLE;
    else if (operand.is_macro_reg()) {
	const Macro_reg &reg = (const Macro_reg&) operand;
	out << MACRO_OPERAND_STRING  << COLON << reg.dtype() << L_ANGLE << reg.name();
	if (reg.assigned_to_file()) {
	   out << reg.file_type();
	   if (reg.assigned_to_physical_file()) {
	      out << COLON << (char*) reg.physical_file_type();
              if (reg.allocated())
                out << reg.mc_num();
           }
	}
	out << R_ANGLE;
    } else if (operand.is_reg()) {
	const Reg &reg = (const Reg&) operand;
	if (reg.assigned_to_file() && reg.allocated())
	    out << BOUND_REG_OPERAND_STRING << COLON << reg.dtype() << L_ANGLE
		<< reg.vr_num();	// fully bound register;
	else if (reg.assigned_to_file() && !reg.allocated())
	    out << FILEBOUND_REG_OPERAND_STRING << COLON << reg.dtype() << L_ANGLE
		<< reg.vr_num();	// file bound register;
	else
	    out << UNBOUND_REG_OPERAND_STRING << COLON << reg.dtype() << L_ANGLE
		<< reg.vr_num();	// unbound register;
	if (reg.omega() != 0)
	    out << L_BRACKET << reg.omega() << R_BRACKET;
	if (reg.assigned_to_file() && reg.allocated()) {
	    out << reg.file_type();
	    if (reg.assigned_to_physical_file())
	       out << COLON << (char*) reg.physical_file_type();
	    if (reg.is_rotating())
                out << ROTATING_OPERAND_STRING;
	    out << reg.mc_num();
	    out << R_ANGLE;
	}
	else if (reg.assigned_to_file() && !reg.allocated()) {
	    out << reg.file_type();
	    if (reg.assigned_to_physical_file())
	       out << COLON << (char*) reg.physical_file_type();
	    if (reg.is_rotating())
		out << ROTATING_OPERAND_STRING;
	    out << R_ANGLE;
	}
	else
	    out << R_ANGLE;
    } else if (operand.is_mem_vr()) {
	//note that this does not conform to the "operand:dtype" convention
	const Mem_vr &reg = (const Mem_vr&) operand;
	out << MEMVR_OPERAND_STRING << L_ANGLE << reg.vr_num();
	if (reg.omega() != 0)  out << L_BRACKET << reg.omega() << R_BRACKET;
	out << R_ANGLE;
    } else if (operand.is_vr_name()) {
	const VR_name &reg = (const VR_name&) operand;
	out << VR_NAME_OPERAND_STRING << COLON << reg.dtype() << L_ANGLE << reg.vr_num() << R_ANGLE;
    } else {
	cerr << "unknown operand type" << endl;
	exit(1);
    }
    return out;
}



//----------------- support operators ---------------------------


IR_outstream &operator<<(IR_outstream &out, const Reg_file &reg_file)
{
   out << vregfile_to_mdesname(reg_file);
   return out;
}


IR_outstream &operator<<(IR_outstream &out, const Macro_name &name)
{
    // Write out a macro name;

    switch (name) {
      case UNDEFINED:	out << "undef";  break;
      case LOCAL:	out << "local";  break;
      case PARAM:	out << "param";  break;
      case SWAP:	out << "swap";  break;
      case INT_RETURN_TYPE: out << "int_ret_type";  break;
      case FLT_RETURN_TYPE: out << "flt_ret_type";  break;
      case DBL_RETURN_TYPE: out << "dbl_ret_type";  break;
      case INT_RETURN:	out << "int_ret";  break;
      case INT_PARAM_1:	out << "int_p1";  break;
      case INT_PARAM_2:	out << "int_p2";  break;
      case INT_PARAM_3:	out << "int_p3";  break;
      case INT_PARAM_4:	out << "int_p4";  break;
      case FLT_RETURN:  out << "flt_ret";  break;
      case FLT_PARAM_1: out << "flt_p1";  break;
      case FLT_PARAM_2:	out << "flt_p2";  break;
      case FLT_PARAM_3:	out << "flt_p3";  break;
      case FLT_PARAM_4:	out << "flt_p4";  break;
      case DBL_RETURN:  out << "dbl_ret";  break;
      case DBL_PARAM_1: out << "dbl_p1";  break;
      case DBL_PARAM_2:	out << "dbl_p2";  break;
      case DBL_PARAM_3: out << "dbl_p3";  break;
      case DBL_PARAM_4:	out << "dbl_p4";  break;
      case INT_TM_TYPE:	out << "int_tm";  break;
      case FLT_TM_TYPE:	out << "flt_tm";  break;
      case DBL_TM_TYPE:	out << "dbl_tm";  break;
      case SP_REG:	out << "sp";  break;
      case FP_REG:	out << "fp";  break;
      case IP_REG:	out << "ip";  break;
      case OP_REG:	out << "op";  break;
      case RGS_REG:	out << "rgs";  break;
      case LV_REG:	out << "lv";  break;
      case LC_REG:	out << "lc";  break;
      case ESC_REG:	out << "esc";  break;
      case ALL_PRED:	out << "all_pred";  break;
      case ALL_ROT_PRED: out << "all_rot_pred";  break;
      case ALL_STATIC_PRED: out << "all_static_pred";  break;
      case RRB:		out << "rrb";  break;
      case RETURN_ADDR:	out << "ret_addr";  break;
      case FLT_ZERO:	out << "flt_zero";  break;
      case FLT_ONE:	out << "flt_one";  break;
      case DBL_ZERO:	out << "dbl_zero";  break;
      case DBL_ONE:	out << "dbl_one";  break;
      case INT_ZERO:	out << "int_zero";  break;
      case PRED_FALSE:	out << "pred_f";  break;
      case PRED_TRUE:	out << "pred_t";  break;
	/* RMR { */ 
	/* add default all true vector mask */
      case VEC_MASK_TRUE: out << "vm_t"; break;
      case VEC_SCRATCH_PAD: out << "vsp"; break;
	/* } RMR */
      case SPILL_TEMPREG: out << "temp_reg";  break;
      case ICMOVE_REG: out << "icmove_reg";  break;
      case PV_0: out << "pv_0";  break;
      case PV_1: out << "pv_1";  break;
      case PV_2: out << "pv_2";  break;
      case PV_3: out << "pv_3";  break;
      case PV_4: out << "pv_4";  break;
      case PV_5: out << "pv_5";  break;
      case PV_6: out << "pv_6";  break;
      case PV_7: out << "pv_7";  break;
      default:
	cerr << "unknown macro name" << endl;
	exit(1);
	break;
    }
    return out;
}
	

	

	
//-------------------------- Reader -------------------------------

Macro_reg *macro_reg(IR_instream &in);
Reg *reg(IR_instream &in);
Mem_vr *mem_vr(IR_instream &in);
VR_name *vr_name(IR_instream &in);
Macro_name macro_name(IR_instream &in);
DType data_type(IR_instream &in);
Reg_file reg_file(IR_instream &in);
Reg_sr reg_sr(IR_instream &in);
eString phys_file(IR_instream &in);

Operand *read_operand(IR_instream &in)
{
    // Return an operand from the input stream if there is one, otherwise
    // return NULL;

    IR_token token, token2;
    int i;  double d;  float f;  char *s;
    int width;
    DType dt;
    
    in >> token;

    if (token == INT_OPERAND_STRING) {
        in >> COLON;
        dt = data_type(in);
        width = dt.width();
	in >> L_ANGLE >> i;
	Operand* operand;

	if ( dt.info() == EL_DT_INFO_INT && dt.is_signed() ) 
		operand = new Operand(new Int_lit(i, width, true));
	else if ( dt.info() == EL_DT_INFO_INT && !dt.is_signed() ) 
		operand = new Operand(new Int_lit(i, width, false));
	else if ( dt.info() == EL_DT_INFO_POINTER ) {
		Int_lit *templit = new Int_lit( i, width, false);
		templit->dtype().set_info( EL_DT_INFO_POINTER );
		operand = new Operand(templit);
	} else { 
		in.syntax_error(token2);
		return NULL;
	}
	in >> token;
	if (token != R_ANGLE) {
	   operand->bind_physical_file(eString((char*)token));
	   in >> R_ANGLE;
	}
	return operand;
    } else if (token == FLOAT_OPERAND_STRING) {
	in >> COLON >> EL_STANDARD_STRING >> L_ANGLE >> f;
	Operand* operand = new Operand(new Float_lit(f));
	in >> token;
	if (token != R_ANGLE) {
	   operand->bind_physical_file(eString((char*)token));
	   in >> R_ANGLE;
	}
	return operand;
    } else if (token == DOUBLE_OPERAND_STRING) {
	in >> COLON >> EL_STANDARD_DOUBLE >> L_ANGLE >> d;
	Operand* operand = new Operand(new Double_lit(d));
	in >> token;
	if (token != R_ANGLE) {
	   operand->bind_physical_file(eString((char*)token));
	   in >> R_ANGLE;
	}
	return operand;
    } else if (token == CB_OPERAND_STRING) {
	in >> COLON >> EL_STANDARD_BRANCH >> L_ANGLE >> i;
	Operand* operand = new Operand(new Cb_operand(i));
	in >> token;
	if (token != R_ANGLE) {
	   operand->bind_physical_file(eString((char*)token));
	   in >> R_ANGLE;
	}
	return operand;
    } else if (token == PREDICATE_OPERAND_STRING) {
	in >> COLON >> EL_STANDARD_PREDICATE >> L_ANGLE >> s >> R_ANGLE;
	bool pred = (strcmp(s, "t") == 0) ? true : false;
	return new Operand(new Pred_lit(pred));
    } else if (token == LABEL_OPERAND_STRING) {
	in >> COLON >> dt >> L_ANGLE >> s;
	if ( !(    (dt==EL_STANDARD_LOCAL_ABS ) 
		|| (dt==EL_STANDARD_LOCAL_GP  ) 
		|| (dt==EL_STANDARD_GLOBAL_ABS) 
		|| (dt==EL_STANDARD_GLOBAL_GP ) ) ) {
	   El_punt("read_operand: Unknown label data type");
	   return NULL;
	}
	Operand* operand = new Operand(new Label_lit(eString(s), dt.info() ));
	in >> token;
	if (token != R_ANGLE) {
	   operand->bind_physical_file(eString((char*)token));
	   in >> R_ANGLE;
	}
	return operand;
    } else if (token == STRING_OPERAND_STRING) {

	in >> COLON >> dt;
	eString string(DOUBLE_QUOTE);
	in >> L_ANGLE >> s;
	string |= s;
	string |= DOUBLE_QUOTE;
        if ( !(    (dt==EL_STANDARD_LOCAL_ABS )
                || (dt==EL_STANDARD_LOCAL_GP  )
                || (dt==EL_STANDARD_GLOBAL_ABS)
                || (dt==EL_STANDARD_GLOBAL_GP ) ) ) {
           El_punt("read_operand: Unknown string data type");
           return NULL;
        }
	Operand* operand = new Operand(new String_lit(string, dt.info() ));
	in >> token;
	if (token != R_ANGLE) {
	   operand->bind_physical_file(eString((char*)token));
	   in >> R_ANGLE;
	}
	return operand;
    } else if (token == MACRO_OPERAND_STRING) {
	in.unget_token();
	return new Operand(macro_reg(in));
    } else if ((token == UNBOUND_REG_OPERAND_STRING) ||
	       (token == FILEBOUND_REG_OPERAND_STRING) ||
	       (token == BOUND_REG_OPERAND_STRING)) {
	in.unget_token();
	return new Operand(reg(in));
    } else if (token == MEMVR_OPERAND_STRING) {
	in.unget_token();
	return new Operand(mem_vr(in));
    } else if (token == VR_NAME_OPERAND_STRING) {
	in.unget_token();
	return new Operand(vr_name(in));
    } else if (token == UNDEFINED_OPERAND_STRING) {
	in  >> L_ANGLE >> R_ANGLE;
	return new Operand(new Undefined());
    } else {
	in.unget_token();
	return NULL;
    }

    El_punt("Operand *read_operand(IR_instream &in): should never read here!");
    return NULL;
}


Macro_reg *macro_reg(IR_instream &in)
{
    // Return a macro register operand from the input stream if there is one,
    // otherwise return NULL;

    IR_token token;
    IR_token typestring;
    eString pf;
    Reg_file file;

    in >> token;
    if (token == MACRO_OPERAND_STRING) {
	in >> COLON;
	DType dtype = data_type(in); 
	in >> L_ANGLE;
	Macro_name name = macro_name(in);

	Macro_reg* retreg = new Macro_reg(name);
        retreg->set_dtype(dtype);

//      Rajiv: why do we punt here? shouldn't we be setting the dtype instead just as in reg
//	if ( retreg->dtype() != dtype ) 
//		El_punt("macro_reg(IR_instream&): macro reg implicit type edoes not equal specified type");

	in >> token;

	if (token == R_ANGLE)
	   return( retreg );

	else {
	   in.unget_token();
	   file = reg_file(in);
	   pf = phys_file(in);
	   retreg->bind_physical_file(pf);
           retreg->bind_file(file);

           in >> token;
           
           if (token == R_ANGLE)
             return retreg;
           else {
             retreg->bind_reg((int)token);
             in >> R_ANGLE;
             return retreg;
           }
	}
	//	in >> R_ANGLE;
	//        return( retreg );
    } else {
	in.unget_token();
	return NULL;
    }
}


Reg *reg(IR_instream &in)
{
    // Return a register operand from the input stream if there is one,
    // otherwise return NULL;

    IR_token token;
    in >> token;
    enum VR_type {BR, FR, VR} vr_type;
    if (token == BOUND_REG_OPERAND_STRING)
	vr_type = BR;
    else if (token == FILEBOUND_REG_OPERAND_STRING)
	vr_type = FR;
    else if (token == UNBOUND_REG_OPERAND_STRING)
	vr_type = VR;
    else {
	in.unget_token();
	return NULL;
    }
    int vr_rep;
    in >> COLON;
    DType dtype = data_type(in);
    in >> L_ANGLE >> vr_rep;

    // omega;
    int omega = 0;  // default value;
    in >> token;
    if (token == L_BRACKET)
	in >> omega >> R_BRACKET;
    else
	in.unget_token();
    Reg *reg = new Reg(dtype, vr_rep, omega);
    Reg_file file;
    Reg_sr sr;
    eString pf;
    switch (vr_type) {
      case BR:
	file = reg_file(in);
	pf = phys_file(in);
	sr = reg_sr(in);
	reg->bind_file(file, sr);
	reg->bind_physical_file(pf);
	int mc_rep;
	in >> mc_rep >> R_ANGLE;
	reg->bind_reg(mc_rep);
	break;
      case FR:
	file = reg_file(in);
	pf = phys_file(in);
	sr = reg_sr(in);
	reg->bind_file(file, sr);
	reg->bind_physical_file(pf);
	in >> R_ANGLE;
	break;
      case VR:
	in >> R_ANGLE;
	break;
    }
    return reg;
}


Mem_vr *mem_vr(IR_instream &in)
{
    // Return a mem_vr operand from the input stream if there is one,
    // otherwise return NULL;
    
    IR_token token;
    in >> token;
    if (token == MEMVR_OPERAND_STRING) {
	int vr_rep;
	in >> L_ANGLE >> vr_rep;
	int omega = 0;  // default value;
	in >> token;
	if (token == L_BRACKET)
	    in >> omega >> R_BRACKET;
	else
	    in.unget_token();
	in >> R_ANGLE;
	return new Mem_vr(vr_rep, omega);
    } else {
	in.unget_token();
	return NULL;
    }
}

	    
VR_name *vr_name(IR_instream &in)
{
    // Return a VR_name operand from the input stream if there is one,
    // otherwise return NULL;
    
    IR_token token;
    in >> token;
    if (token == VR_NAME_OPERAND_STRING) {
	int vr_rep;
	in >> COLON;
	DType dtype = data_type(in);
	in >> L_ANGLE >> vr_rep;
	in >> R_ANGLE;
	return new VR_name(dtype, vr_rep);
    } else {
	in.unget_token();
	return NULL;
    }
}


Macro_name macro_name(IR_instream &in)
{
    // Decode a string token to a macro name.

    IR_token token;
    in >> token;
    if (token == (const char*)"undef") return UNDEFINED;
    else if (token == (const char*)"local") return LOCAL;
    else if (token == (const char*)"param") return PARAM;
    else if (token == (const char*)"swap") return SWAP;
    else if (token == (const char*)"int_ret_type") return INT_RETURN_TYPE;
    else if (token == (const char*)"flt_ret_type") return FLT_RETURN_TYPE;
    else if (token == (const char*)"dbl_ret_type") return DBL_RETURN_TYPE;
    else if (token == (const char*)"int_ret") return INT_RETURN;
    else if (token == (const char*)"int_p1") return INT_PARAM_1;
    else if (token == (const char*)"int_p2") return INT_PARAM_2;
    else if (token == (const char*)"int_p3") return INT_PARAM_3;
    else if (token == (const char*)"int_p4") return INT_PARAM_4;
    else if (token == (const char*)"flt_ret") return FLT_RETURN;
    else if (token == (const char*)"flt_p1") return FLT_PARAM_1;
    else if (token == (const char*)"flt_p2") return FLT_PARAM_2;
    else if (token == (const char*)"flt_p3") return FLT_PARAM_3;
    else if (token == (const char*)"flt_p4") return FLT_PARAM_4;
    else if (token == (const char*)"dbl_ret") return DBL_RETURN;
    else if (token == (const char*)"dbl_p1") return DBL_PARAM_1;
    else if (token == (const char*)"dbl_p2") return DBL_PARAM_2;
    else if (token == (const char*)"dbl_p3") return DBL_PARAM_3;
    else if (token == (const char*)"dbl_p4") return DBL_PARAM_4;
    else if (token == (const char*)"int_tm") return INT_TM_TYPE;
    else if (token == (const char*)"flt_tm") return FLT_TM_TYPE;
    else if (token == (const char*)"dbl_tm") return DBL_TM_TYPE;
    else if (token == (const char*)"sp") return SP_REG;
    else if (token == (const char*)"fp") return FP_REG;
    else if (token == (const char*)"ip") return IP_REG;
    else if (token == (const char*)"rgs") return RGS_REG;
    else if (token == (const char*)"op") return OP_REG;
    else if (token == (const char*)"lv") return LV_REG;
    else if (token == (const char*)"lc") return LC_REG;
    else if (token == (const char*)"esc") return ESC_REG;
    else if (token == (const char*)"all_pred") return ALL_PRED;
    else if (token == (const char*)"all_rot_pred") return ALL_ROT_PRED;
    else if (token == (const char*)"all_static_pred") return ALL_STATIC_PRED;
    else if (token == (const char*)"rrb") return RRB;
    else if (token == (const char*)"ret_addr") return RETURN_ADDR;
    else if (token == (const char*)"flt_zero") return FLT_ZERO;
    else if (token == (const char*)"flt_one") return FLT_ONE;
    else if (token == (const char*)"dbl_zero") return DBL_ZERO;
    else if (token == (const char*)"dbl_one") return DBL_ONE;
    else if (token == (const char*)"int_zero") return INT_ZERO;
    else if (token == (const char*)"pred_f") return PRED_FALSE;
    else if (token == (const char*)"pred_t") return PRED_TRUE;
    /* RMR { */
    /* add default all true vector mask */
    else if (token == (const char*)"vm_t") return VEC_MASK_TRUE;
    else if (token == (const char*)"vsp") return VEC_SCRATCH_PAD;
    /* } RMR */
    else if (token == (const char*)"temp_reg") return SPILL_TEMPREG;
    else if (token == (const char*)"icmove_reg") return ICMOVE_REG;
    else if (token == (const char*)"pv_0") return PV_0;
    else if (token == (const char*)"pv_1") return PV_1;
    else if (token == (const char*)"pv_2") return PV_2;
    else if (token == (const char*)"pv_3") return PV_3;
    else if (token == (const char*)"pv_4") return PV_4;
    else if (token == (const char*)"pv_5") return PV_5;
    else if (token == (const char*)"pv_6") return PV_6;
    else if (token == (const char*)"pv_7") return PV_7;
    else {
	in.syntax_error(token);
	return UNDEFINED;
    }
}
    

DType data_type(IR_instream &in)
{
    // Decode a string token to a data type;

    DType dtype;
    in >> dtype;
    return dtype;
}


Reg_file reg_file(IR_instream &in)
{
    // Decode a string token to a register file type;

   IR_token token;
   in >> token;
   return mdesname_to_vregfile(*(char*)token);
}


Reg_sr reg_sr(IR_instream &in)
{
    // Decode a string to a Reg_sr type;

    IR_token token;
    in >> token;
    if (token == ROTATING_OPERAND_STRING) {
	return ROTATING;
    }
    // Default is static
    else {
	in.unget_token();
	return STATIC_REG;
    }
}

eString phys_file(IR_instream &in)
{
   // Decode an optional physical file string
   IR_token token;

   in >> token;
   if (token == COLON) {
      in >> token;
      eString pf((char*)token);
      return pf;
   }
   else {
      in.unget_token();
      eString pf;
      return pf;
   }
}
