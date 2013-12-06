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



/*===========================================================================
 *
 *      File:           el_width_propagation.cpp
 *      Author:         Tim Sherwood, Scott Mahlke
 *      Created:        Aug 1999
 *      Description:    Width constraint class, used for width analysis
 *
 *===========================================================================*/
#include "width_utilities.h"
#include "el_width_propagation.h"

//////////////////////////////////////////////////////////////////////////////////////////////////// 
// Macros for predifined widths									  //
////////////////////////////////////////////////////////////////////////////////////////////////////
#define  PREDICATE 1 
#define  BYTE_WIDTH 8 
#define  HALF_WORD_WIDTH 16 
#define  WORD_WIDTH 32 
#define  PC WORD_WIDTH
#define  FLOAT_WIDTH 32
#define  DOUBLE_WIDTH 64
//unch should be bigger than the biggest type
//this will prevent the value from being decreases
#define UNCH 		     100 

//////////////////////////////////////////////////////////////////////////////////////////////////// 
// Shortcut Defines 										  //
////////////////////////////////////////////////////////////////////////////////////////////////////

const int SIGNED   = true;
const int UNSIGNED = false;

#define OP_IGNORE(o)         case o : ( working_constraint->ratchet_up( UNCH, *max_constraint ) ); break;
#define OP_SAME(o)           case o :

#define OP_D(o,d,s,eq)       case o : FORM_D(d,s); FLOW_D(eq); break
#define FORM_D(d,s)          ( assert( destnum==d && num_constraints==s ) )
#define FLOW_D(eq)           ( working_constraint->ratchet_up( (eq), *max_constraint ) )

#define OP_CV_D(o,d,s, eq1, eq2, eq3) \
	case o : FORM_D(d,s); \
	if ( !srcc[0].is_lit && !srcc[1].is_lit) { FLOW_D(eq1); } \
	else if ( srcc[0].is_lit && !srcc[1].is_lit) { FLOW_D(eq2); } \
	else if ( !srcc[0].is_lit && srcc[1].is_lit) { FLOW_D(eq3); } \
	else El_punt("OP_CV_D: cannot handle op with both arguments as literals"); \
	break


#define OP_U(o,d,s,eq1,eq2)  case o : FORM_U(d,s); FLOW_U(eq1,eq2); break
#define FORM_U(d,s)          ( assert( num_destconstraints==d && num_srcconstraints==s ) )
//#define FLOW_U(eq1,eq2)    ( working_constraint->ratchet_up( ((srcnum==1)?(eq1):(eq2)),*max_constraint) )
#define FLOW_U(eq)           ( working_constraint->ratchet_up( (eq), *max_constraint) )
#define NA 		     0

#define UUU		     if(ds==false &&xs==false &&ys==false)
#define UUS		     else if(ds==false &&xs==false &&ys==true )
#define USU		     else if(ds==false &&xs==true  &&ys==false)
#define USS		     else if(ds==false &&xs==true  &&ys==true )
#define SUU		     else if(ds==true  &&xs==false &&ys==false)
#define SUS		     else if(ds==true  &&xs==false &&ys==true )
#define SSU		     else if(ds==true  &&xs==true  &&ys==false)
#define SSS		     else if(ds==true  &&xs==true  &&ys==true )

#define UU		     if(ds==false &&xs==false)
#define US		     else if(ds==false &&xs==true )
#define SU		     else if(ds==true  &&xs==false)
#define SS		     else if(ds==true  &&xs==true )

/* Use these for cut and paste (note: keep in this order)
	int r;
	UU { r = ;}
	US { r = ;}
	SU { r = ;}
	SS { r = ;} 
	return r;

	int r;
	UUU { r = ;}
	UUS { r = ;}
	USU { r = ;}
	USS { r = ;}
	SUU { r = ;}
	SUS { r = ;} 
	SSU { r = ;}
	SSS { r = ;}
	return r;
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
// Ease of use Macros										  //
////////////////////////////////////////////////////////////////////////////////////////////////////
//these are the widths of the src constraints
#define src1 (srcc[0].dt.width())
#define src2 (srcc[1].dt.width())
#define src3 (srcc[2].dt.width())
#define src4 (srcc[3].dt.width())
#define src5 (srcc[4].dt.width())

//these are the widths of the dst constraints (up only)
#define dst1 (destc[0].dt.width())
#define dst2 (destc[1].dt.width())
#define dst3 (destc[2].dt.width())

//these are the values of the src constraints (or zero if not a lit)
#define val1 (srcc[0].val)
#define val2 (srcc[1].val)
#define val3 (srcc[2].val)
#define val4 (srcc[3].val)
#define val5 (srcc[4].val)

//these are the true if the src is an int literal
#define islit1 (srcc[0].is_lit)
#define islit2 (srcc[1].is_lit)
#define islit3 (srcc[2].is_lit)
#define islit4 (srcc[3].is_lit)
#define islit5 (srcc[4].is_lit)

//these are the values of the src constraints (or zero if not a lit)
#define sign1 (srcc[0].dt.is_signed())
#define sign2 (srcc[1].dt.is_signed())
#define sign3 (srcc[2].dt.is_signed())
#define sign4 (srcc[3].dt.is_signed())
#define sign5 (srcc[4].dt.is_signed())

//these are the widths of the dst constraints (up only)
#define dstsign1 (destc[0].dt.is_signed())
#define dstsign2 (destc[1].dt.is_signed())
#define dstsign3 (destc[2].dt.is_signed())


//////////////////////////////////////////////////////////////////////////////////////////////////// 
// Macros and functions for common use 								  //
////////////////////////////////////////////////////////////////////////////////////////////////////
//-----DOWN---------------------------------------------------------------------------------------//
int down_plus(bool ds, int x, bool xs, int y, bool ys) {
	int r = 0;
	UUU { r = WIDTH_MAX(x,y) + 1 ; }
	UUS { r = UNCH; }
	USU { r = UNCH; }
	USS { r = UNCH; }
	SUU { r = WIDTH_MAX(x,y)   + 2 ; }
	SUS { r = WIDTH_MAX(x+1,y) + 1 ; } 
	SSU { r = WIDTH_MAX(x,y+1) + 1 ; }
	SSS { r = WIDTH_MAX(x,y)   + 1 ; }
	return r;
} 

int down_and(bool ds, int x, bool xs, int y, bool ys) {
	int r = 0;
	UUU { r = WIDTH_MIN(x,y); }
	UUS { r = x; }
	USU { r = y; }
	USS { r = UNCH; }
	SUU { r = WIDTH_MIN(x,y)+1; }
	SUS { r = x+1; } 
	SSU { r = y+1; }
	SSS { r = WIDTH_MAX(x,y); }
	return r;
} 

int down_or(bool ds, int x, bool xs, int y, bool ys) {
	int r = 0;
	UUU { r = WIDTH_MAX(x,y); }
	UUS { r = UNCH; }
	USU { r = UNCH; }
	USS { r = UNCH; }
	SUU { r = WIDTH_MAX(x,y)+1; }
	SUS { r = WIDTH_MAX(x+1,y); } 
	SSU { r = WIDTH_MAX(x,y+1); }
	SSS { r = WIDTH_MAX(x,y); }
	return r;
} 

/*
 *	SAM 8-2000, added.  Should this be the same as OR_W?  I cannot
 *	see why it would be different right now.  But may need a future fix
 *	that I am too stupid to see right now.
 */
int down_xor(bool ds, int x, bool xs, int y, bool ys) {
	int r = 0;
        UUU { r = WIDTH_MAX(x,y); }
        UUS { r = UNCH; }
        USU { r = UNCH; }
        USS { r = UNCH; }
        SUU { r = WIDTH_MAX(x,y)+1; }
        SUS { r = WIDTH_MAX(x+1,y); }
        SSU { r = WIDTH_MAX(x,y+1); }
        SSS { r = WIDTH_MAX(x,y); }
        return r;
}

// SAM 8-2000, added
// y is the actual shift val not the width of the shift val!!
int down_shiftl_const(bool ds, int x, bool xs, int y, bool ys) {
	int r = 0;
        UUU { r = x+y; }
        UUS { r = x+y; }
        USU { r = UNCH; }
        USS { r = UNCH; }
        SUU { r = x+y+1; }
        SUS { r = x+y+1; }
        SSU { r = x+y; }
        SSS { r = x+y; }
        return r;
}

int down_shiftl(bool ds, int x, bool xs, int y, bool ys) {
	int r = 0;

	int max_shift_val = get_positive_max_value(y, ys);

	if (((unsigned int) max_shift_val) > 32) {
	    r = UNCH;
	}
	else {
            UUU { r = x+max_shift_val; }
            UUS { r = x+max_shift_val; }
            USU { r = UNCH; }
            USS { r = UNCH; }
            SUU { r = x+max_shift_val+1; }
            SUS { r = x+max_shift_val+1; }
            SSU { r = x+max_shift_val; }
            SSS { r = x+max_shift_val; }
	}
        return r;
}

int down_move(bool ds, int x, bool xs ) {
	int r = 0;
	UU { r = x; }
	US { r = UNCH; }
	SU { r = x+1; }
	SS { r = x;}
	return r;
} 
int down_saturate_plus(bool ds, int x, bool xs, int y, bool ys,Opcode opc) {
	int r = 0;
        int sp = extract_saturate_point(opc);
	UUU { r = WIDTH_MAX(x,y) + 1 ; }
	UUS { r = UNCH; }
	USU { r = UNCH; }
	USS { r = UNCH; }
	SUU { r = WIDTH_MAX(x,y)   + 2 ; }
	SUS { r = WIDTH_MAX(x+1,y) + 1 ; } 
	SSU { r = WIDTH_MAX(x,y+1) + 1 ; }
	SSS { r = WIDTH_MAX(x,y)   + 1 ; }
	return WIDTH_MIN(r,sp);
}

int down_saturate_mpy(bool ds, int x, bool xs, int y, bool ys,Opcode opc) {
	int safe1, safe2;

        int sp = extract_saturate_point(opc);
	//convert both srcs to signed
	safe1 = down_move(SIGNED,x,xs); 
	safe2 = down_move(SIGNED,y,ys);
	//widths add and then convert to final sign from signed
	int r =  down_move(ds,safe1+safe2,SIGNED) ;
	return WIDTH_MIN(r,sp);
}
int down_saturate_mpyl(bool ds, int x, bool xs, int y, bool ys,Opcode opc) {
	int safe1, safe2;

        int sp = extract_saturate_point(opc);
	//convert both srcs to signed
	safe1 = down_move(UNSIGNED,x,xs); 
	safe2 = down_move(UNSIGNED,y,ys);
	//widths add and then convert to final sign from signed
	int r =  down_move(ds,safe1+safe2,UNSIGNED) ;
	return WIDTH_MIN(r,sp);
}

int down_saturate_mpyadd(bool ds, int x, bool xs, int y, bool ys,int z, bool zs, Opcode opc) {
	int safe1, safe2,safe3;

        int sp = extract_saturate_point(opc);
	//convert both srcs to signed
	safe1 = down_move(SIGNED,x,xs); 
	safe2 = down_move(SIGNED,y,ys);
	safe3 = down_move(SIGNED,z,zs);
	//widths add and then convert to final sign from signed
	int r = down_move(ds,(WIDTH_MAX((safe1+safe2), safe3)+1),SIGNED);

	return WIDTH_MIN(r,sp);
}
int up_saturate_plus(bool ds, int x, bool xs, int y, bool ys,Opcode opc) {
        int sp = extract_saturate_point(opc);
	return sp;
}
int up_saturate_mpyadd(bool ds, int x, bool xs, int y, bool ys,int z, bool zs, Opcode opc) {
        int sp = extract_saturate_point(opc);
	return sp;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Down Propagation										  //
////////////////////////////////////////////////////////////////////////////////////////////////////

void compute_width_propagation_down( 
	WConstraint* working_constraint, 
	WConstraint* max_constraint, 
	int destnum, 
	Opcode opc, 
	WConstraint srcc[],
	int num_constraints,
        int debug_flag, 
	int local_differ )
{
	int i;
	int safe1, safe2, safe3;
	WConstraint old_constraint = *working_constraint;
	bool destsign = working_constraint->dt.is_signed();
	int srccvt1,  srccvt2;


	//////////////////////////////////////////////////////////////////////////////////////////
	/*** BULK HANDLING DOWN *****************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////

	//----CMPR------------------------------------------------------------------------------//
	if ( is_cmpr(opc) )
	{
		FORM_D( 1,2 );
		//result is either zero or 1 bit unsigned
		FLOW_D( down_move(destsign,1,UNSIGNED) );
	}

	//----CMPP------------------------------------------------------------------------------//
	else if ( is_cmpp(opc) )
	{
		FLOW_D( UNCH ); //ignore
	}
	//----PSEUDO_OPS-----------------------------------------------------------------------//
	else if ( is_pseudo(opc) )
	{
		FLOW_D( UNCH ); //ignore
	}
	//----BRANCHES-------------------------------------------------------------------------//
	else if ( is_branch(opc) )
	{
		FLOW_D( UNCH ); //ignore
	}
	else if ( is_brl(opc) )
	{
		FLOW_D( UNCH ); //ignore
	}

	//----LOADS----------------------------------------------------------------------------//
	else if ( is_load(opc) )
	{
		if (destnum==1) {
			//sign extender case needs to go here?
			if ( opc & IR_BYTE )           FLOW_D( down_move(destsign,BYTE_WIDTH,UNSIGNED) );
			else if ( opc & IR_HALFWORD )  FLOW_D( down_move(destsign,HALF_WORD_WIDTH,UNSIGNED) );
			else                           FLOW_D( down_move(destsign,UNCH,UNSIGNED) );
		} else if (destnum==2) {
			//only post inc loads should have two dests
			assert( opc & IR_INCREMENT );
			assert(num_constraints==2);
			//increment the dest by one bit unsigned
			FLOW_D( down_plus(destsign,src1,sign1,1,UNSIGNED) );
		} else {
			El_punt("el_width_propagation: Load opcode with more dests than thought possible");
		}
	}
	//----STORES---------------------------------------------------------------------------//
	else if ( is_store(opc) )
	{
		if ( opc & IR_INCREMENT ) {
			assert( num_constraints == 3 );
			FLOW_D( down_plus(destsign,src1,sign1,src3,sign3) );
		} else {
			El_punt("el_width_propagation: There should not be a non-inc store with a dest");
		}
	}
	//----FLOAT-COMP-----------------------------------------------------------------------//
	else if ( is_flt_computation_op(opc) )
	{
		FLOW_D( UNCH ); // ignore
	}
	//----DOUBLE-COMP----------------------------------------------------------------------//
	else if ( is_dbl_computation_op(opc) )
	{
		FLOW_D( UNCH ); // ignore
	}
	//----CONVERSION but not int extraction------------------------------------------------//
	else if ( is_int_fltdbl_conversion_op(opc) )
	{
		FLOW_D( UNCH ); // ignore
	}


	//////////////////////////////////////////////////////////////////////////////////////////
	/*** PER-OPCODE HANDLING DOWN ***********************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////
        else switch ( opc )
        {


	//////////////////////////////////////////////////////////////////////////////////////////
	//  ARITHMETIC										//
	//////////////////////////////////////////////////////////////////////////////////////////
	//----MOVE------------------------------------------------------------------------------//
	case (MOVE):
		FORM_D( 1,1 );
		FLOW_D( down_move(destsign,src1,sign1) );
		break;
	//----FLOAT/DOUBLE_MOVE-----------------------------------------------------------------//
	case (MOVEF_S): case (MOVEF_D):
	  FLOW_D( UNCH ); // ignore
		break;
	//----ADD/SUB---------------------------------------------------------------------------//
	case (ADD_W): case (ADDL_W): case (SUB_W): case (SUBL_W):
		FORM_D( 1,2 );
		FLOW_D( down_plus(destsign,src1,sign1,src2,sign2) );
		break;


	//----AND-------------------------------------------------------------------------------//
	case (AND_W):
        case (NAND_W):  // mchu 10-2005 NAND should be same as AND
		FORM_D( 1,2 );
		//if the source is a signed positive literal, cast to unsigned lit for this op
		if      ( islit1 && sign1==SIGNED && val1>0 )  {
			 srccvt1 = down_move(UNSIGNED, src1, sign1);
			 FLOW_D( down_and(destsign,srccvt1,UNSIGNED,src2,sign2) );
		}
		else if ( islit2 && sign2==SIGNED && val2>0 ) {
			 srccvt2 = down_move(UNSIGNED, src2, sign2);
			 FLOW_D( down_and(destsign,src1,sign1,srccvt2,UNSIGNED) );
		}
		else {
			 FLOW_D( down_and(destsign,src1,sign1,src2,sign2) );
		}
		break;
	//----OR--------------------------------------------------------------------------------//
	case (OR_W):
        case (NOR_W):  // mchu 10-2005 NOR should be same as OR
		FORM_D( 1,2 );
		FLOW_D( down_or(destsign,src1,sign1,src2,sign2) );
		break;
	//----XOR-------------------------------------------------------------------------------//
	case (XOR_W):
		FORM_D( 1,2 );
		FLOW_D( down_xor(destsign,src1,sign1,src2,sign2) );
		break;
	//----SIGNED_MULTIPLY-------------------------------------------------------------------//
	case (MPY_W):
		FORM_D( 1,2 );
		//convert both srcs to signed
		safe1 = down_move(SIGNED,src1,sign1); 
		safe2 = down_move(SIGNED,src2,sign2);
		//widths add and then convert to final sign from signed
		FLOW_D( down_move(destsign,safe1+safe2,SIGNED) );
		break;
	//----UNSIGNED_MULTIPLY-----------------------------------------------------------------//
	case (MPYL_W):
		FORM_D( 1,2 );
		//convert both srcs to signed
		safe1 = down_move(UNSIGNED,src1,sign1); 
		safe2 = down_move(UNSIGNED,src2,sign2);
		//widths add and then convert to final sign from unsigned
		FLOW_D( down_move(destsign,safe1+safe2,UNSIGNED) );
		break;
	//----SIGNED_MULTIPLY ADD--------------------------------------------------------------//
	case (MPYADD_W): case (MPYADDN_W): case (MPYRSUB_W): case (MPYSUB_W):
		FORM_D( 1,3 );
		//convert all srcs to signed
		safe1 = down_move(SIGNED,src1,sign1); 
		safe2 = down_move(SIGNED,src2,sign2);
		safe3 = down_move(SIGNED,src3,sign3);
		//widths add and then convert to final sign from signed
		FLOW_D( down_move(destsign,(WIDTH_MAX((safe1+safe2), safe3)+1),SIGNED) );
		break;
	//----SIGNED_DIVIDE---------------------------------------------------------------------//
	case (DIV_W):
		FORM_D( 1,2 );
		//convert both srcs to signed
		safe1 = down_move(SIGNED,src1,sign1); 
		//dest no wider than src1 since src2 can always be 1
		FLOW_D( down_move(destsign,safe1,SIGNED) );
		break;
	//----UNSIGNED_DIVIDE-------------------------------------------------------------------//
	case (DIVL_W):
		FORM_D( 1,2 );
		//convert both srcs to signed
		safe1 = down_move(UNSIGNED,src1,sign1); 
		//dest no wider than src1 since src2 can always be 1
		FLOW_D( down_move(destsign,safe1,UNSIGNED) );
		break;
	//----SIGNED_REMAINDER------------------------------------------------------------------//
	case (REM_W):
		FORM_D( 1,2 );
		//convert both srcs to signed
		safe2 = down_move(SIGNED,src2,sign2); 
		//dest no wider than src2
		FLOW_D( down_move(destsign,safe2,SIGNED) );
		break;
	//----UNSIGNED_REMAINDER----------------------------------------------------------------//
	case (REML_W):
		FORM_D( 1,2 );
		//convert both srcs to signed
		safe2 = down_move(UNSIGNED,src2,sign2); 
		//dest no wider than src2
		FLOW_D( down_move(destsign,safe2,UNSIGNED) );
		break;
	//----SHIFT_LEFT------------------------------------------------------------------------//
	case (SHL_W): case (SHLA_W):
		FORM_D( 1,2 );
		if( islit2 ) FLOW_D( down_shiftl_const(destsign,src1,sign1,val2,sign2) );
		else	     FLOW_D( down_shiftl(destsign,src1,sign1,src2,sign2 ) );
		break;
	//----SHIFT_RIGHT_ARITHMETIC------------------------------------------------------------//
	case (SHRA_W):
		FORM_D( 1,2 );
		if( islit2 ) FLOW_D( down_move(destsign,src1-val2,SIGNED) );
		else	     FLOW_D( down_move(destsign,src1,SIGNED) );
		break;
	//----SHIFT_RIGHT_LOGICAL---------------------------------------------------------------//
	case (SHR_W):
		FORM_D( 1,2 );
		if( islit2 ) FLOW_D( down_move(destsign,src1-val2,UNSIGNED) );
		else	     FLOW_D( down_move(destsign,src1,UNSIGNED) );
		break;
	case (SH1ADDL_W): 
		FORM_D( 1,2 );
		safe1 = ( down_shiftl_const(sign1,src1,sign1,1,sign1 ) );
		FLOW_D( down_plus(destsign,safe1,sign1,src2,sign2) );
		break;
	case (SH2ADDL_W): 
		FORM_D( 1,2 );
		safe1 = ( down_shiftl_const(sign1,src1,sign1,2,sign1 ) );
		FLOW_D( down_plus(destsign,safe1,sign1,src2,sign2) );
		break;
	case (SH3ADDL_W): 
		FORM_D( 1,2 );
		safe1 = ( down_shiftl_const(sign1,src1,sign1,3,sign1 ) );
		FLOW_D( down_plus(destsign,safe1,sign1,src2,sign2) );
		break;

	//////////////////////////////////////////////////////////////////////////////////////////
	//  PREPARE BRANCH INSTRUCIONS								//
	//////////////////////////////////////////////////////////////////////////////////////////
	//----PREPARE_TO_BRANCH--------------------------------------------------------//
	case (PBR):
		FORM_D( 1,2 );
		FLOW_D( UNCH );
		break;

	// Saturated Operations

	case (SADD1_W):  case (SADD2_W):  case (SADD3_W):  case (SADD4_W): 
        case (SADD5_W):  case (SADD6_W):  case (SADD7_W):  case (SADD8_W): 
        case (SADD9_W):  case (SADD10_W): case (SADD11_W): case (SADD12_W): 
        case (SADD13_W): case (SADD14_W): case (SADD15_W): case (SADD16_W): 
        case (SADD17_W): case (SADD18_W): case (SADD19_W): case (SADD20_W): 
	case (SADD21_W): case (SADD22_W): case (SADD23_W): case (SADD24_W): 
        case (SADD25_W): case (SADD26_W): case (SADD27_W): case (SADD28_W): 
        case (SADD29_W): case (SADD30_W): case (SADD31_W): case (SADD32_W): 

	case (SADDL1_W):  case (SADDL2_W):  case (SADDL3_W):  case (SADDL4_W): 
        case (SADDL5_W):  case (SADDL6_W):  case (SADDL7_W):  case (SADDL8_W): 
        case (SADDL9_W):  case (SADDL10_W): case (SADDL11_W): case (SADDL12_W): 
        case (SADDL13_W): case (SADDL14_W): case (SADDL15_W): case (SADDL16_W): 
        case (SADDL17_W): case (SADDL18_W): case (SADDL19_W): case (SADDL20_W): 
	case (SADDL21_W): case (SADDL22_W): case (SADDL23_W): case (SADDL24_W): 
        case (SADDL25_W): case (SADDL26_W): case (SADDL27_W): case (SADDL28_W): 
        case (SADDL29_W): case (SADDL30_W): case (SADDL31_W): case (SADDL32_W): 

	case (SSUB1_W):  case (SSUB2_W):  case (SSUB3_W):  case (SSUB4_W): 
        case (SSUB5_W):  case (SSUB6_W):  case (SSUB7_W):  case (SSUB8_W): 
        case (SSUB9_W):  case (SSUB10_W): case (SSUB11_W): case (SSUB12_W): 
        case (SSUB13_W): case (SSUB14_W): case (SSUB15_W): case (SSUB16_W): 
        case (SSUB17_W): case (SSUB18_W): case (SSUB19_W): case (SSUB20_W): 
	case (SSUB21_W): case (SSUB22_W): case (SSUB23_W): case (SSUB24_W): 
        case (SSUB25_W): case (SSUB26_W): case (SSUB27_W): case (SSUB28_W): 
        case (SSUB29_W): case (SSUB30_W): case (SSUB31_W): case (SSUB32_W): 

	case (SSUBL1_W):  case (SSUBL2_W):  case (SSUBL3_W):  case (SSUBL4_W): 
        case (SSUBL5_W):  case (SSUBL6_W):  case (SSUBL7_W):  case (SSUBL8_W): 
        case (SSUBL9_W):  case (SSUBL10_W): case (SSUBL11_W): case (SSUBL12_W): 
        case (SSUBL13_W): case (SSUBL14_W): case (SSUBL15_W): case (SSUBL16_W): 
        case (SSUBL17_W): case (SSUBL18_W): case (SSUBL19_W): case (SSUBL20_W): 
	case (SSUBL21_W): case (SSUBL22_W): case (SSUBL23_W): case (SSUBL24_W): 
        case (SSUBL25_W): case (SSUBL26_W): case (SSUBL27_W): case (SSUBL28_W): 
        case (SSUBL29_W): case (SSUBL30_W): case (SSUBL31_W): case (SSUBL32_W): 
	{
		FORM_D( 1,2 );
		FLOW_D( down_saturate_plus(destsign,src1,sign1,src2,sign2,opc) );
		break;
	}
	case (SMPY1_W):  case (SMPY2_W):  case (SMPY3_W):  case (SMPY4_W): 
        case (SMPY5_W):  case (SMPY6_W):  case (SMPY7_W):  case (SMPY8_W): 
        case (SMPY9_W):  case (SMPY10_W): case (SMPY11_W): case (SMPY12_W): 
        case (SMPY13_W): case (SMPY14_W): case (SMPY15_W): case (SMPY16_W): 
        case (SMPY17_W): case (SMPY18_W): case (SMPY19_W): case (SMPY20_W): 
	case (SMPY21_W): case (SMPY22_W): case (SMPY23_W): case (SMPY24_W): 
        case (SMPY25_W): case (SMPY26_W): case (SMPY27_W): case (SMPY28_W): 
        case (SMPY29_W): case (SMPY30_W): case (SMPY31_W): case (SMPY32_W): 
        {
		FORM_D( 1,2 );
		FLOW_D( down_saturate_mpy(destsign,src1,sign1,src2,sign2,opc) );
		break;
        }
	case (SMPYL1_W):  case (SMPYL2_W):  case (SMPYL3_W):  case (SMPYL4_W): 
        case (SMPYL5_W):  case (SMPYL6_W):  case (SMPYL7_W):  case (SMPYL8_W): 
        case (SMPYL9_W):  case (SMPYL10_W): case (SMPYL11_W): case (SMPYL12_W): 
        case (SMPYL13_W): case (SMPYL14_W): case (SMPYL15_W): case (SMPYL16_W): 
        case (SMPYL17_W): case (SMPYL18_W): case (SMPYL19_W): case (SMPYL20_W): 
	case (SMPYL21_W): case (SMPYL22_W): case (SMPYL23_W): case (SMPYL24_W): 
        case (SMPYL25_W): case (SMPYL26_W): case (SMPYL27_W): case (SMPYL28_W): 
        case (SMPYL29_W): case (SMPYL30_W): case (SMPYL31_W): case (SMPYL32_W): 
        {
		FORM_D( 1,2 );
		FLOW_D( down_saturate_mpyl(destsign,src1,sign1,src2,sign2,opc) );
		break;
        }

	case (SMPYADD1_W):  case (SMPYADD2_W):  case (SMPYADD3_W):  case (SMPYADD4_W): 
        case (SMPYADD5_W):  case (SMPYADD6_W):  case (SMPYADD7_W):  case (SMPYADD8_W): 
        case (SMPYADD9_W):  case (SMPYADD10_W): case (SMPYADD11_W): case (SMPYADD12_W): 
        case (SMPYADD13_W): case (SMPYADD14_W): case (SMPYADD15_W): case (SMPYADD16_W): 
        case (SMPYADD17_W): case (SMPYADD18_W): case (SMPYADD19_W): case (SMPYADD20_W): 
	case (SMPYADD21_W): case (SMPYADD22_W): case (SMPYADD23_W): case (SMPYADD24_W): 
        case (SMPYADD25_W): case (SMPYADD26_W): case (SMPYADD27_W): case (SMPYADD28_W): 
        case (SMPYADD29_W): case (SMPYADD30_W): case (SMPYADD31_W): case (SMPYADD32_W): 
        {
		FORM_D( 1,3 );
		FLOW_D( down_saturate_mpyadd(destsign,src1,sign1,src2,sign2,src3,sign3,opc) );
		break;
        }
	case (SMPYADDN1_W):  case (SMPYADDN2_W):  case (SMPYADDN3_W):  case (SMPYADDN4_W): 
        case (SMPYADDN5_W):  case (SMPYADDN6_W):  case (SMPYADDN7_W):  case (SMPYADDN8_W): 
        case (SMPYADDN9_W):  case (SMPYADDN10_W): case (SMPYADDN11_W): case (SMPYADDN12_W): 
        case (SMPYADDN13_W): case (SMPYADDN14_W): case (SMPYADDN15_W): case (SMPYADDN16_W): 
        case (SMPYADDN17_W): case (SMPYADDN18_W): case (SMPYADDN19_W): case (SMPYADDN20_W): 
	case (SMPYADDN21_W): case (SMPYADDN22_W): case (SMPYADDN23_W): case (SMPYADDN24_W): 
        case (SMPYADDN25_W): case (SMPYADDN26_W): case (SMPYADDN27_W): case (SMPYADDN28_W): 
        case (SMPYADDN29_W): case (SMPYADDN30_W): case (SMPYADDN31_W): case (SMPYADDN32_W): 
        {
		FORM_D( 1,3 );
		FLOW_D( down_saturate_mpyadd(destsign,src1,sign1,src2,sign2,src3,sign3,opc) );
		break;
        }
	case (SMPYSUB1_W):  case (SMPYSUB2_W):  case (SMPYSUB3_W):  case (SMPYSUB4_W): 
        case (SMPYSUB5_W):  case (SMPYSUB6_W):  case (SMPYSUB7_W):  case (SMPYSUB8_W): 
        case (SMPYSUB9_W):  case (SMPYSUB10_W): case (SMPYSUB11_W): case (SMPYSUB12_W): 
        case (SMPYSUB13_W): case (SMPYSUB14_W): case (SMPYSUB15_W): case (SMPYSUB16_W): 
        case (SMPYSUB17_W): case (SMPYSUB18_W): case (SMPYSUB19_W): case (SMPYSUB20_W): 
	case (SMPYSUB21_W): case (SMPYSUB22_W): case (SMPYSUB23_W): case (SMPYSUB24_W): 
        case (SMPYSUB25_W): case (SMPYSUB26_W): case (SMPYSUB27_W): case (SMPYSUB28_W): 
        case (SMPYSUB29_W): case (SMPYSUB30_W): case (SMPYSUB31_W): case (SMPYSUB32_W): 
        {
		FORM_D( 1,3 );
		FLOW_D( down_saturate_mpyadd(destsign,src1,sign1,src2,sign2,src3,sign3,opc) );
		break;
        }
	case (SMPYRSUB1_W):  case (SMPYRSUB2_W):  case (SMPYRSUB3_W):  case (SMPYRSUB4_W): 
        case (SMPYRSUB5_W):  case (SMPYRSUB6_W):  case (SMPYRSUB7_W):  case (SMPYRSUB8_W): 
        case (SMPYRSUB9_W):  case (SMPYRSUB10_W): case (SMPYRSUB11_W): case (SMPYRSUB12_W): 
        case (SMPYRSUB13_W): case (SMPYRSUB14_W): case (SMPYRSUB15_W): case (SMPYRSUB16_W): 
        case (SMPYRSUB17_W): case (SMPYRSUB18_W): case (SMPYRSUB19_W): case (SMPYRSUB20_W): 
	case (SMPYRSUB21_W): case (SMPYRSUB22_W): case (SMPYRSUB23_W): case (SMPYRSUB24_W): 
        case (SMPYRSUB25_W): case (SMPYRSUB26_W): case (SMPYRSUB27_W): case (SMPYRSUB28_W): 
        case (SMPYRSUB29_W): case (SMPYRSUB30_W): case (SMPYRSUB31_W): case (SMPYRSUB32_W): 
        {
		FORM_D( 1,3 );
		FLOW_D( down_saturate_mpyadd(destsign,src1,sign1,src2,sign2,src3,sign3,opc) );
		break;
        }

	case (EXTS_B): 
	  {
	    FORM_D ( 1,1 );
	    FLOW_D ( down_move(destsign,EL_DEFAULT_CHAR_WIDTH,SIGNED) );
	    break;
	  }
	case (EXTZ_B):
	  {
	    FORM_D ( 1,1 );
	    FLOW_D ( down_move(destsign,EL_DEFAULT_CHAR_WIDTH,UNSIGNED) );
	    break;
	  }
	case (EXTS_H): 
	  {
	    FORM_D ( 1,1 );
	    FLOW_D ( down_move(destsign,EL_DEFAULT_SHORT_WIDTH,SIGNED) );
	    break;
	  }
	case (EXTZ_H):
	  {
	    FORM_D ( 1,1 );
	    FLOW_D ( down_move(destsign,EL_DEFAULT_SHORT_WIDTH,UNSIGNED) );
	    break;
	  }

        // ignore pred_clear and pred_set. -KF 1/2005
        case (PRED_CLEAR): case (PRED_SET):
          {
            FLOW_D( UNCH );
            break;
          }

	default:
		cerr << "\n*** compute_width_propagation_down: warning unknown opcode:\n ";
		cerr << opc << endl; 
                FLOW_D( UNCH );
		break;
	}

	if ( debug_flag ) {

	  if ( old_constraint.dt.width() != working_constraint->dt.width() ) cerr<<"D";
	  else cerr<<" ";
	  if ( local_differ ) cerr<<"S ";
	  else  cerr<<"  ";
	  

	  cerr << "DOWN: " << opc << " d" << destnum; 
	  cerr << " Dest=[" << (old_constraint) << "]" ;
	  cerr << " Src=["; 
	  for( i=0; i<num_constraints; i++) cerr << srcc[i] << ","; 
	  cerr << "] -> ";

	  cerr << " Dest=[" << (*working_constraint) << "]" ;
	  cerr << " Src=["; 
	  for( i=0; i<num_constraints; i++) cerr << srcc[i] << ","; 
	  cerr << "]\n";
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////// 
// Macros and functions for common use 								  //
////////////////////////////////////////////////////////////////////////////////////////////////////
 #define HANDLE_S1 if(srcnum==1)
 #define HANDLE_S2 if(srcnum==2)
 #define HANDLE_S3 if(srcnum==3)

//-----UP-----------------------------------------------------------------------------------------//
int up_move(int d, bool ds, bool xs ) {
	int r = 0;
	UU { r = d; }
	US { r = d+1; }
	SU { r = UNCH; }
	SS { r = d;}
	return r;
} 

int up_plus(int d, bool ds, bool xs, bool ys ) {
	int r;
	//C symantic add is signed if either source is signed
	bool add_sign = xs || ys;
	int dstcvt;

	if ( add_sign ) {
	   dstcvt = up_move( d, ds, SIGNED );
	   r = up_move( dstcvt, SIGNED, xs );
	} else {
	   dstcvt = up_move( d, ds, UNSIGNED );
	   r = up_move( dstcvt, UNSIGNED, xs );
	}
	return r;
} 

int up_and(int d, bool ds, bool xs ) {
	int r, dstcvt;
	dstcvt = up_move( d, ds, UNSIGNED );
	r = up_move( dstcvt, UNSIGNED, xs );
	return r;
} 

int up_or(int d, bool ds, bool xs ) {
	int r, dstcvt;
	dstcvt = up_move( d, ds, UNSIGNED );
	r = up_move( dstcvt, UNSIGNED, xs );
	return r;
} 

int up_xor(int d, bool ds, bool xs ) {
	int r, dstcvt;
	dstcvt = up_move( d, ds, UNSIGNED );
	r = up_move( dstcvt, UNSIGNED, xs );
	return r;
} 

int up_mpyl(int d, bool ds, bool xs ) {
	int r, dstcvt;
	dstcvt = up_move( d, ds, UNSIGNED );
	r = up_move( dstcvt, UNSIGNED, xs );
	return r;
} 

int up_mpy(int d, bool ds, bool xs ) {
	int r, dstcvt;
	dstcvt = up_move( d, ds, SIGNED );
	r = up_move( dstcvt, SIGNED, xs );
	return r;
} 

// Only have signed version of this opcode in Playdoh
int up_mpyadd(int d, bool ds, bool xs ) {
        int r, dstcvt;
        dstcvt = up_move( d, ds, SIGNED );
        r = up_move( dstcvt, SIGNED, xs );
        return r;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Up Propagation										  //
////////////////////////////////////////////////////////////////////////////////////////////////////

void compute_width_propagation_up( 
	WConstraint* working_constraint, 
	WConstraint* max_constraint, 
	int srcnum, 
	Opcode opc, 
	WConstraint destc[],
	int num_destconstraints,
	WConstraint srcc[],
	int num_srcconstraints,
        int debug_flag, 
	int local_differ,
	int op_id)
{
	int i;
	WConstraint old_constraint = *working_constraint;
	bool srcsign = working_constraint->dt.is_signed();
	int dstcvt1; //temps used in the switch statment


	//////////////////////////////////////////////////////////////////////////////////////////
	/*** BULK HANDLING UP *******************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////

	//----CMPR------------------------------------------------------------------------------//
	if ( is_cmpr(opc) )
	{
		FORM_U( 1,2 );
		//nothing can be determined for compares
		FLOW_U( UNCH );
	}

	//----CMPP------------------------------------------------------------------------------//
	else if ( is_cmpp(opc) )
	{
		FLOW_U( UNCH ); //ignore
	}
	//----PSEUDO_OPS-----------------------------------------------------------------------//
	else if ( is_pseudo(opc) )
	{
		FLOW_U( UNCH ); //ignore
	}
	//----BRANCHES-------------------------------------------------------------------------//
	else if ( is_branch(opc) )
	{
		FLOW_U( UNCH ); //ignore
	}
	else if ( is_brl(opc) )
	{
		FLOW_U( UNCH ); //ignore
	}

	//----LOADS----------------------------------------------------------------------------//
	else if ( is_load(opc) )
	{
		//cannot reduce loads this way because
		//src1 is used as the memory address
		FLOW_U( UNCH ); //ignore
	}
	//----STORES---------------------------------------------------------------------------//
	else if ( is_store(opc) )
	{
	    // SAM 9-03, we cannot change the address, but the store
	    // value can be limited by the opcode itself, e.g., byte, halfword
	    if (opc & IR_BYTE) {
	        if (is_base_plus_offset_store(opc)) {
		    FORM_U(0,3);
		    HANDLE_S1      { FLOW_U( UNCH ); }
		    else HANDLE_S2 { FLOW_U( UNCH ); }
		    else HANDLE_S3 { FLOW_U( EL_DEFAULT_CHAR_WIDTH ); }
	        }
	        else {
		    FORM_U(0,2);
		    HANDLE_S1      { FLOW_U( UNCH ); }
		    else HANDLE_S2 { FLOW_U( EL_DEFAULT_CHAR_WIDTH ); }
	        }
	    }
	    else if (opc & IR_HALFWORD) {
	        if (is_base_plus_offset_store(opc)) {
		    FORM_U(0,3);
		    HANDLE_S1      { FLOW_U( UNCH ); }
		    else HANDLE_S2 { FLOW_U( UNCH ); }
		    else HANDLE_S3 { FLOW_U( EL_DEFAULT_SHORT_WIDTH ); }
	        }
	        else {
		    FORM_U(0,2);
		    HANDLE_S1      { FLOW_U( UNCH ); }
		    else HANDLE_S2 { FLOW_U( EL_DEFAULT_SHORT_WIDTH ); }
	        }
	    }
	    else {
		//cannot reduce stores this way because
		//src1 is used as the memory address
		FLOW_U( UNCH ); //ignore
	    }
	}
	//----FLOAT-COMP-----------------------------------------------------------------------//
	else if ( is_flt_computation_op(opc) )
	{
		FLOW_U( UNCH ); // ignore
	}
	//----DOUBLE-COMP----------------------------------------------------------------------//
	else if ( is_dbl_computation_op(opc) )
	{
		FLOW_U( UNCH ); // ignore
	}
	//----CONVERSION but not int extracts--------------------------------------------------//
	else if ( is_int_fltdbl_conversion_op(opc) )
	{
		FLOW_U( UNCH ); // ignore
	}


	//////////////////////////////////////////////////////////////////////////////////////////
	/*** PER-OPCODE HANDLING UP   ***********************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////
        else switch ( opc )
        {


	//////////////////////////////////////////////////////////////////////////////////////////
	//  ARITHMATIC										//
	//////////////////////////////////////////////////////////////////////////////////////////
	//----MOVE------------------------------------------------------------------------------//
	case (MOVE):
		FORM_U(1,1);
		FLOW_U( up_move(dst1,dstsign1,srcsign) );
		break;
	//----FLOAT/DOUBLE_MOVE-----------------------------------------------------------------//
	case (MOVEF_S): case (MOVEF_D):
	  FLOW_U( UNCH ); // ignore
		break;
	//----ADDER-----------------------------------------------------------------------------//
	case (ADD_W): case (ADDL_W): case (SUB_W): case (SUBL_W): 
		FORM_U(1,2);
		HANDLE_S1      { FLOW_U( up_plus(dst1,dstsign1,sign1,sign2) ); }
		else HANDLE_S2 { FLOW_U( up_plus(dst1,dstsign1,sign2,sign1) ); }
		break;
	//----AND-------------------------------------------------------------------------------//
	case (AND_W):
		FORM_U(1,2);
		HANDLE_S1      { FLOW_U( up_and(dst1,dstsign1,sign1) ); }
		else HANDLE_S2 { FLOW_U( up_and(dst1,dstsign1,sign2) ); }
		break;
	//----OR--------------------------------------------------------------------------------//
	case (OR_W):
		FORM_U(1,2);
		HANDLE_S1      { FLOW_U( up_or(dst1,dstsign1,sign1) ); }
		else HANDLE_S2 { FLOW_U( up_or(dst1,dstsign1,sign2) ); }
		break;
	//----XOR--------------------------------------------------------------------------------//
	case (XOR_W):
		FORM_U(1,2);
		HANDLE_S1      { FLOW_U( up_xor(dst1,dstsign1,sign1) ); }
		else HANDLE_S2 { FLOW_U( up_xor(dst1,dstsign1,sign2) ); }
		break;
	//----SIGNED_MULTIPLY-------------------------------------------------------------------//
	case (MPY_W):
		FORM_U(1,2);
		HANDLE_S1      { FLOW_U( up_mpy(dst1,dstsign1,sign1) ); }
		else HANDLE_S2 { FLOW_U( up_mpy(dst1,dstsign1,sign2) ); }
		break;
	//----UNSIGNED_MULTIPLY-----------------------------------------------------------------//
	case (MPYL_W):
		FORM_U(1,2);
		HANDLE_S1      { FLOW_U( up_mpyl(dst1,dstsign1,sign1) ); }
		else HANDLE_S2 { FLOW_U( up_mpyl(dst1,dstsign1,sign2) ); }
		break;
	//----SIGNED_MULTIPLY ADD --------------------------------------------------------------//
        case (MPYADD_W): case (MPYADDN_W): case (MPYRSUB_W): case (MPYSUB_W): 
                FORM_U(1,3);
                HANDLE_S1      { FLOW_U( up_mpyadd(dst1,dstsign1,sign1) ); }
                else HANDLE_S2 { FLOW_U( up_mpyadd(dst1,dstsign1,sign2) ); }
                else HANDLE_S3 { FLOW_U( up_mpyadd(dst1,dstsign1,sign3) ); }
                break;
	//----SIGNED_DIVIDE---------------------------------------------------------------------//
	case (DIV_W):
		FORM_U(1,2);
		HANDLE_S1      { FLOW_U( UNCH ); }
		else HANDLE_S2 { FLOW_U( UNCH ); }
		break;
	//----UNSIGNED_DIVIDE-------------------------------------------------------------------//
	case (DIVL_W):
		FORM_U(1,2);
		HANDLE_S1      { FLOW_U( UNCH ); }
		else HANDLE_S2 { FLOW_U( UNCH ); }
		break;
	//----SIGNED_REMAINDER------------------------------------------------------------------//
	case (REM_W):
		FORM_U(1,2);
		HANDLE_S1      { FLOW_U( UNCH ); }
		else HANDLE_S2 { FLOW_U( UNCH ); }
		break;
	//----UNSIGNED_REMAINDER-----------------------------------------------------------------//
	case (REML_W):
		FORM_U(1,2);
		HANDLE_S1      { FLOW_U( UNCH ); }
		else HANDLE_S2 { FLOW_U( UNCH ); }
		break;
	//----SHIFT_LEFT------------------------------------------------------------------------//
	case (SHL_W): case (SHLA_W):
		FORM_U(1,2);
                HANDLE_S1      { FLOW_U( up_move(dst1,dstsign1,sign1) ); }
                else HANDLE_S2 { FLOW_U( UNCH ); }
		break;
	//----SHIFT_RIGHT_ARITHMETIC------------------------------------------------------------//
	case (SHRA_W):
		FORM_U(1,2);
                HANDLE_S1      {
			if ( islit2 ) {
			    dstcvt1 = up_move(dst1,dstsign1,SIGNED);
			    FLOW_U( up_move(dstcvt1+val2,SIGNED,sign1) ); 
			} else {
			    FLOW_U( UNCH );
			}
                } else HANDLE_S2 { FLOW_U( UNCH ); }
		break;
	//----SHIFT_RIGHT_LOGICAL---------------------------------------------------------------//
	case (SHR_W):
		FORM_U(1,2);
                HANDLE_S1      {
			if ( islit2 ) {
			    dstcvt1 = up_move(dst1,dstsign1,UNSIGNED);
			    FLOW_U( up_move(dstcvt1+val2,UNSIGNED,sign1) ); 
			} else {
			    FLOW_U( UNCH );
			}
                } else HANDLE_S2 { FLOW_U( UNCH ); }
		break;
	case (SH1ADDL_W): case (SH2ADDL_W): case (SH3ADDL_W): 
		FORM_U(1,2);
		HANDLE_S1      { FLOW_U( up_plus(dst1,dstsign1,sign1,sign2) ); }
		else HANDLE_S2 { FLOW_U( up_plus(dst1,dstsign1,sign2,sign1) ); }
		break;
	//////////////////////////////////////////////////////////////////////////////////////////
	//  PREPARE BRANCH INSTRUCIONS								//
	//////////////////////////////////////////////////////////////////////////////////////////
	//----PREPARE_TO_BRANCH--------------------------------------------------------//
	case (PBR):
		FORM_U(1,2);
		//this could be better if we knew the PC of the instruction
                HANDLE_S1      { FLOW_U( up_move(dst1,dstsign1,sign1) ); }
                else HANDLE_S2 { FLOW_U( UNCH ); }
		break;
	// Saturating Opeations

	case (SADD1_W):  case (SADD2_W):  case (SADD3_W):  case (SADD4_W): 
        case (SADD5_W):  case (SADD6_W):  case (SADD7_W):  case (SADD8_W): 
        case (SADD9_W):  case (SADD10_W): case (SADD11_W): case (SADD12_W): 
        case (SADD13_W): case (SADD14_W): case (SADD15_W): case (SADD16_W): 
        case (SADD17_W): case (SADD18_W): case (SADD19_W): case (SADD20_W): 
	case (SADD21_W): case (SADD22_W): case (SADD23_W): case (SADD24_W): 
        case (SADD25_W): case (SADD26_W): case (SADD27_W): case (SADD28_W): 
        case (SADD29_W): case (SADD30_W): case (SADD31_W): case (SADD32_W): 

	case (SADDL1_W):  case (SADDL2_W):  case (SADDL3_W):  case (SADDL4_W): 
        case (SADDL5_W):  case (SADDL6_W):  case (SADDL7_W):  case (SADDL8_W): 
        case (SADDL9_W):  case (SADDL10_W): case (SADDL11_W): case (SADDL12_W): 
        case (SADDL13_W): case (SADDL14_W): case (SADDL15_W): case (SADDL16_W): 
        case (SADDL17_W): case (SADDL18_W): case (SADDL19_W): case (SADDL20_W): 
	case (SADDL21_W): case (SADDL22_W): case (SADDL23_W): case (SADDL24_W): 
        case (SADDL25_W): case (SADDL26_W): case (SADDL27_W): case (SADDL28_W): 
        case (SADDL29_W): case (SADDL30_W): case (SADDL31_W): case (SADDL32_W): 

	case (SSUB1_W):  case (SSUB2_W):  case (SSUB3_W):  case (SSUB4_W): 
        case (SSUB5_W):  case (SSUB6_W):  case (SSUB7_W):  case (SSUB8_W): 
        case (SSUB9_W):  case (SSUB10_W): case (SSUB11_W): case (SSUB12_W): 
        case (SSUB13_W): case (SSUB14_W): case (SSUB15_W): case (SSUB16_W): 
        case (SSUB17_W): case (SSUB18_W): case (SSUB19_W): case (SSUB20_W): 
	case (SSUB21_W): case (SSUB22_W): case (SSUB23_W): case (SSUB24_W): 
        case (SSUB25_W): case (SSUB26_W): case (SSUB27_W): case (SSUB28_W): 
        case (SSUB29_W): case (SSUB30_W): case (SSUB31_W): case (SSUB32_W): 

	case (SSUBL1_W):  case (SSUBL2_W):  case (SSUBL3_W):  case (SSUBL4_W): 
        case (SSUBL5_W):  case (SSUBL6_W):  case (SSUBL7_W):  case (SSUBL8_W): 
        case (SSUBL9_W):  case (SSUBL10_W): case (SSUBL11_W): case (SSUBL12_W): 
        case (SSUBL13_W): case (SSUBL14_W): case (SSUBL15_W): case (SSUBL16_W): 
        case (SSUBL17_W): case (SSUBL18_W): case (SSUBL19_W): case (SSUBL20_W): 
	case (SSUBL21_W): case (SSUBL22_W): case (SSUBL23_W): case (SSUBL24_W): 
        case (SSUBL25_W): case (SSUBL26_W): case (SSUBL27_W): case (SSUBL28_W): 
        case (SSUBL29_W): case (SSUBL30_W): case (SSUBL31_W): case (SSUBL32_W): 
	{
		FORM_U( 1,2 );
		FLOW_U( up_saturate_plus(srcsign,src1,sign1,src2,sign2,opc) );
		break;
	}
	//----SATURATING MULTIPLY---------------------------------------------------------------//
	case (SMPY1_W):  case (SMPY2_W):  case (SMPY3_W):  case (SMPY4_W): 
        case (SMPY5_W):  case (SMPY6_W):  case (SMPY7_W):  case (SMPY8_W): 
        case (SMPY9_W):  case (SMPY10_W): case (SMPY11_W): case (SMPY12_W): 
        case (SMPY13_W): case (SMPY14_W): case (SMPY15_W): case (SMPY16_W): 
        case (SMPY17_W): case (SMPY18_W): case (SMPY19_W): case (SMPY20_W): 
	case (SMPY21_W): case (SMPY22_W): case (SMPY23_W): case (SMPY24_W): 
        case (SMPY25_W): case (SMPY26_W): case (SMPY27_W): case (SMPY28_W): 
        case (SMPY29_W): case (SMPY30_W): case (SMPY31_W): case (SMPY32_W): 
        {
		FORM_U( 1,2 );
		FLOW_U( up_saturate_plus(srcsign,src1,sign1,src2,sign2,opc) );
		break;
        }
	//----UNSIGNED_MULTIPLY-----------------------------------------------------------------//
	case (SMPYL1_W):  case (SMPYL2_W):  case (SMPYL3_W):  case (SMPYL4_W): 
        case (SMPYL5_W):  case (SMPYL6_W):  case (SMPYL7_W):  case (SMPYL8_W): 
        case (SMPYL9_W):  case (SMPYL10_W): case (SMPYL11_W): case (SMPYL12_W): 
        case (SMPYL13_W): case (SMPYL14_W): case (SMPYL15_W): case (SMPYL16_W): 
        case (SMPYL17_W): case (SMPYL18_W): case (SMPYL19_W): case (SMPYL20_W): 
	case (SMPYL21_W): case (SMPYL22_W): case (SMPYL23_W): case (SMPYL24_W): 
        case (SMPYL25_W): case (SMPYL26_W): case (SMPYL27_W): case (SMPYL28_W): 
        case (SMPYL29_W): case (SMPYL30_W): case (SMPYL31_W): case (SMPYL32_W): 
        {
		FORM_U( 1,2 );
		FLOW_U( up_saturate_plus(srcsign,src1,sign1,src2,sign2,opc) );
		break;
        }
	case (SMPYADD1_W):  case (SMPYADD2_W):  case (SMPYADD3_W):  case (SMPYADD4_W): 
        case (SMPYADD5_W):  case (SMPYADD6_W):  case (SMPYADD7_W):  case (SMPYADD8_W): 
        case (SMPYADD9_W):  case (SMPYADD10_W): case (SMPYADD11_W): case (SMPYADD12_W): 
        case (SMPYADD13_W): case (SMPYADD14_W): case (SMPYADD15_W): case (SMPYADD16_W): 
        case (SMPYADD17_W): case (SMPYADD18_W): case (SMPYADD19_W): case (SMPYADD20_W): 
	case (SMPYADD21_W): case (SMPYADD22_W): case (SMPYADD23_W): case (SMPYADD24_W): 
        case (SMPYADD25_W): case (SMPYADD26_W): case (SMPYADD27_W): case (SMPYADD28_W): 
        case (SMPYADD29_W): case (SMPYADD30_W): case (SMPYADD31_W): case (SMPYADD32_W): 

	case (SMPYADDN1_W):  case (SMPYADDN2_W):  case (SMPYADDN3_W):  case (SMPYADDN4_W): 
        case (SMPYADDN5_W):  case (SMPYADDN6_W):  case (SMPYADDN7_W):  case (SMPYADDN8_W): 
        case (SMPYADDN9_W):  case (SMPYADDN10_W): case (SMPYADDN11_W): case (SMPYADDN12_W): 
        case (SMPYADDN13_W): case (SMPYADDN14_W): case (SMPYADDN15_W): case (SMPYADDN16_W): 
        case (SMPYADDN17_W): case (SMPYADDN18_W): case (SMPYADDN19_W): case (SMPYADDN20_W): 
	case (SMPYADDN21_W): case (SMPYADDN22_W): case (SMPYADDN23_W): case (SMPYADDN24_W): 
        case (SMPYADDN25_W): case (SMPYADDN26_W): case (SMPYADDN27_W): case (SMPYADDN28_W): 
        case (SMPYADDN29_W): case (SMPYADDN30_W): case (SMPYADDN31_W): case (SMPYADDN32_W): 

	case (SMPYSUB1_W):  case (SMPYSUB2_W):  case (SMPYSUB3_W):  case (SMPYSUB4_W): 
        case (SMPYSUB5_W):  case (SMPYSUB6_W):  case (SMPYSUB7_W):  case (SMPYSUB8_W): 
        case (SMPYSUB9_W):  case (SMPYSUB10_W): case (SMPYSUB11_W): case (SMPYSUB12_W): 
        case (SMPYSUB13_W): case (SMPYSUB14_W): case (SMPYSUB15_W): case (SMPYSUB16_W): 
        case (SMPYSUB17_W): case (SMPYSUB18_W): case (SMPYSUB19_W): case (SMPYSUB20_W): 
	case (SMPYSUB21_W): case (SMPYSUB22_W): case (SMPYSUB23_W): case (SMPYSUB24_W): 
        case (SMPYSUB25_W): case (SMPYSUB26_W): case (SMPYSUB27_W): case (SMPYSUB28_W): 
        case (SMPYSUB29_W): case (SMPYSUB30_W): case (SMPYSUB31_W): case (SMPYSUB32_W): 

	case (SMPYRSUB1_W):  case (SMPYRSUB2_W):  case (SMPYRSUB3_W):  case (SMPYRSUB4_W): 
        case (SMPYRSUB5_W):  case (SMPYRSUB6_W):  case (SMPYRSUB7_W):  case (SMPYRSUB8_W): 
        case (SMPYRSUB9_W):  case (SMPYRSUB10_W): case (SMPYRSUB11_W): case (SMPYRSUB12_W): 
        case (SMPYRSUB13_W): case (SMPYRSUB14_W): case (SMPYRSUB15_W): case (SMPYRSUB16_W): 
        case (SMPYRSUB17_W): case (SMPYRSUB18_W): case (SMPYRSUB19_W): case (SMPYRSUB20_W): 
	case (SMPYRSUB21_W): case (SMPYRSUB22_W): case (SMPYRSUB23_W): case (SMPYRSUB24_W): 
        case (SMPYRSUB25_W): case (SMPYRSUB26_W): case (SMPYRSUB27_W): case (SMPYRSUB28_W): 
        case (SMPYRSUB29_W): case (SMPYRSUB30_W): case (SMPYRSUB31_W): case (SMPYRSUB32_W): 
        {
		FORM_U( 1,3 );
		FLOW_U( up_saturate_mpyadd(srcsign,src1,sign1,src2,sign2,src3,sign3,opc) );
		break;
	}

	case (EXTS_B): 	case (EXTZ_B): 	case (EXTS_H): 	case (EXTZ_H):
	  {
	    FORM_U(1,1);
	    FLOW_U( UNCH );
	    break;
	  }

	default:
		cerr << "\n*** compute_width_propagation_up: warning unknown opcode:\n ";
		cerr << opc << endl; 
                FLOW_U( UNCH );
		break;
	}
	
	if ( debug_flag ) {

	  if ( old_constraint.dt.width() != working_constraint->dt.width() ) cerr<<"D";
	  else cerr<<" ";
	  if ( local_differ ) cerr<<"S ";
	  else  cerr<<"  ";

	  cerr << "UP: " << op_id << " " << opc << " s" << srcnum << " Dst=["; 
	  for( i=0; i<num_destconstraints; i++) cerr << destc[i] << ","; 
	  cerr << "] ";

	  cerr << " Src=["; 
	  for( i=0; i<num_srcconstraints; i++) cerr << srcc[i] << ","; 
	  cerr << "] -> ";

	  cerr << " Dst=["; 
	  for( i=0; i<num_destconstraints; i++) cerr << destc[i] << ","; 
	  cerr << "] ";

	  cerr << " Src=["; 
	  for( i=0; i<num_srcconstraints; i++) {
		if ( i+1 == srcnum ) cerr << (* working_constraint ) << ",";
		else cerr << srcc[i] << ","; 
	  }
	  cerr << "]\n";
	}
}

#undef PC
#undef OP_IGNORE
#undef OP_SAME           
#undef OP_D       
#undef OP_U  
#undef PLUS     	    
#undef FORM            
#undef FLOW_D           
#undef FLOW_U           
#undef NA 		    
#undef UNCH
