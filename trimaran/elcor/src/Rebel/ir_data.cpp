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
//      File:           ir_data.cpp
//      Authors:        Scott A. Mahlke
//      Created:        July 1996
//      Description:    Elcor Text IR reader-writer for Data 
//
///////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "IR_symbol.h"
#include "el_expression.h"
#include "el_data.h"
#include "ir_data.h"
#include "list.h"


//-------------------------- Writer -------------------------------

/*
 *	El_expression output routines
 */

IR_outstream &operator<<(IR_outstream &out, El_expression_opcode &opc)
{
    switch (opc) {
	case ELEX_ADD:
	    out << "add";
	    break;
	case ELEX_SUB:
	    out << "sub";
	    break;
	case ELEX_MUL:
	    out << "mul";
	    break;
	case ELEX_DIV:
	    out << "div";
	    break;
	case ELEX_NEG:
	    out << "neg";
	    break;
	case ELEX_COM:
	    out << "com";
	    break;
	default:
	    El_punt("<< El_expression_opcode: unknown opcode!");
    }

    return (out);
}

IR_outstream &operator<<(IR_outstream &out, El_expression *expr)
{
    if (expr == NULL)
	return (out);

    if (expr->is_int_expr()) {
	int val;
	val = ((El_value_expression *) expr)->value.i;
	out << L_PAREN << "i" << val << R_PAREN;
    }
    else if (expr->is_predicate_expr()) {
	bool val;
	val = ((El_value_expression *) expr)->value.p;
	out << L_PAREN << "p" << val << R_PAREN;
    }
    else if (expr->is_float_expr()) {
	float val;
	val = ((El_value_expression *) expr)->value.f;
	out << L_PAREN << "f" << val << R_PAREN;
    }
    else if (expr->is_double_expr()) {
	double val;
	val = ((El_value_expression *) expr)->value.f2;
	out << L_PAREN << "f2" << val << R_PAREN;
    }
    else if (expr->is_symbol_expr()) {
	char *val;
	val = (char *) ((El_symbol_expression *) expr)->value;
	out << L_PAREN << "l" << val << R_PAREN;
    }
    else if (expr->is_string_expr()) {
	char *val;
	val = (char *) ((El_string_expression *) expr)->value;
	out << L_PAREN << "s" << val << R_PAREN;
    }
    else if (expr->is_op_expr()) {
	El_expression_opcode val;
	val = ((El_op_expression *) expr)->opcode;
	out << val;
    }
    else if (expr->is_compound_expr()) {
	int i, size;
	Vector<El_expression *> &args = ((El_compound_expression *) expr)->args;

 	size = args.size();

	out << L_PAREN;
	if (size > 0)
	    out << args[0] << SPACE;
	for (i=1; i<size; i++) {
	    out << args[i];
	}

	out << R_PAREN;
    }

    return (out);  
}

void print_symbol_expr_value(IR_outstream &out, El_expression *expr)
{
    char *val;

    if (expr == NULL)
	El_punt("print_symbol_expr_value: expr is NULL");
    if (! expr->is_symbol_expr())
	El_punt("print_symbol_expr_wo_parens: expr must be a symbol expr");

    val = (char *) ((El_symbol_expression *) expr)->value;
    out << val;
}

void print_string_expr_value(IR_outstream &out, El_expression *expr)
{
    char *val;

    if (expr == NULL)
	El_punt("print_string_expr_value: expr is NULL");
    if (! expr->is_string_expr())
        El_punt("print_string_expr_wo_parens: expr must be a symbol expr");

    val = (char *) ((El_string_expression *) expr)->value;
    out << val;
}

/*
 *	El_data output routines
 */

IR_outstream &operator<<(IR_outstream &out, El_data_memory_segment_type &ms_type)
{
    switch (ms_type) {
	case EL_DATA_MS_TEXT:
	    out << "text";
	    break;
	case EL_DATA_MS_DATA:
	    out << "data";
	    break;
	case EL_DATA_MS_DATA1:
	    out << "data1";
	    break;
	case EL_DATA_MS_DATA2:
	    out << "data2";
	    break;
	case EL_DATA_MS_BSS:
	    out << "bss";
	    break;
	case EL_DATA_MS_SYNC:
	    out << "sync";
	    break;
	default:
	    El_punt("<< El_memory_segment_type: illegal ms_type");
    }

    return (out);
}

void print_data_stype_flags(IR_outstream &out, El_data_source_type_info *stype)
{
    El_data_stype_flags flag;

    for (flag = El_data_stype_flags(0); flag != EL_DATA_ST_LAST;
	 flag = El_data_stype_flags(int(flag) + 1)) {
	if (stype->flags.bit(int(flag)) == 0)
	    continue;
        switch (flag) {
	    case EL_DATA_ST_CONST:
	        out << SPACE << L_PAREN << "const" << R_PAREN;
	        break;
	    case EL_DATA_ST_VOLATILE:
	        out << SPACE << L_PAREN << "volatile" << R_PAREN;
	        break;
	    case EL_DATA_ST_NOALIAS:
	        out << SPACE << L_PAREN << "noalias" << R_PAREN;
	        break;
	    case EL_DATA_ST_REGISTER:
	        out << SPACE << L_PAREN << "register" << R_PAREN;
	        break;
	    case EL_DATA_ST_AUTO:
	        out << SPACE << L_PAREN << "auto" << R_PAREN;
	        break;
	    case EL_DATA_ST_STATIC:
	        out << SPACE << L_PAREN << "static" << R_PAREN;
	        break;
	    case EL_DATA_ST_EXTERN:
	        out << SPACE << L_PAREN << "extern" << R_PAREN;
	        break;
	    case EL_DATA_ST_GLOBAL:
	        out << SPACE << L_PAREN << "global" << R_PAREN;
	        break;
	    case EL_DATA_ST_PARAMETER:
	        out << SPACE << L_PAREN << "parameter" << R_PAREN;
	        break;
	    case EL_DATA_ST_VOID:
	        out << SPACE << L_PAREN << "void" << R_PAREN;
	        break;
	    case EL_DATA_ST_CHAR:
	        out << SPACE << L_PAREN << "char" << R_PAREN;
	        break;
	    case EL_DATA_ST_SHORT:
	        out << SPACE << L_PAREN << "short" << R_PAREN;
	        break;
	    case EL_DATA_ST_INT:
	        out << SPACE << L_PAREN << "int" << R_PAREN;
	        break;
	    case EL_DATA_ST_LONG:
	        out << SPACE << L_PAREN << "long" << R_PAREN;
	        break;
	    case EL_DATA_ST_LLONG:
	        out << SPACE << L_PAREN << "longlong" << R_PAREN;
	        break;
	    case EL_DATA_ST_FLOAT:
	        out << SPACE << L_PAREN << "float" << R_PAREN;
	        break;
	    case EL_DATA_ST_DOUBLE:
	        out << SPACE << L_PAREN << "double" << R_PAREN;
	        break;
	    case EL_DATA_ST_SIGNED:
	        out << SPACE << L_PAREN << "signed" << R_PAREN;
	        break;
	    case EL_DATA_ST_UNSIGNED:
	        out << SPACE << L_PAREN << "unsigned" << R_PAREN;
	        break;
	    case EL_DATA_ST_STRUCT:
	        out << SPACE << L_PAREN << "struct " << stype->struct_name << R_PAREN;
	        break;
	    case EL_DATA_ST_UNION:
	        out << SPACE << L_PAREN << "union " << stype->struct_name << R_PAREN;
	        break;
	    case EL_DATA_ST_ENUM:
	        out << SPACE << L_PAREN << "enum " << stype->struct_name << R_PAREN;
	        break;
	    case EL_DATA_ST_VARARG:
	        out << SPACE << L_PAREN << "vararg" << R_PAREN;
	        break;
	    default:
		El_punt("print_data_stype_flags: unknown flag value: %d", flag);
		break;
        }
    }
}

IR_outstream &operator<<(IR_outstream &out, El_data_access_declarator *dcltr)
{
    if (dcltr == NULL)
	return (out);

    switch (dcltr->method) {
	case EL_DATA_ACCESS_ARRAY:
	    out << SPACE << L_PAREN << "arr";
	    if (dcltr->index != NULL)
		out << SPACE << dcltr->index << R_PAREN;
	    else
		out << R_PAREN;
	    break;
	case EL_DATA_ACCESS_POINTER:
	    out << SPACE << L_PAREN << "ptr" << R_PAREN;
	    break;
	case EL_DATA_ACCESS_FUNCTION:
	    out << SPACE << L_PAREN << "fun" << R_PAREN;
	    break;
	default:
	    El_punt("<< El_data_access_declarator: unknown method: %d", dcltr->method);
	    break;
    }
    return (out);
}

IR_outstream &operator<<(IR_outstream &out, El_data_source_type_info *stype)
{
    if (stype == NULL)
	return (out);

    print_data_stype_flags(out, stype);

    for (List_iterator<El_data_access_declarator*> li(stype->access_dcltrs);
		li!=0; li++) {
	El_data_access_declarator *dcltr = *li;
	out << dcltr;
    }

    return (out);
}

IR_outstream &operator<<(IR_outstream &out, El_data *data)
{
    if (data == NULL)
	return (out);

    out << tab << L_PAREN;
    switch (data->t_type) {
	case EL_DATA_TOKEN_MS:
	    out << "ms" << data->ms_type;
	    break;
	case EL_DATA_TOKEN_VOID:
	    out << "void" << SPACE;
	    print_symbol_expr_value(out, data->address);
	    break;
	case EL_DATA_TOKEN_BYTE:
	    out << "byte" << SPACE << data->N << SPACE;
	    print_symbol_expr_value(out, data->address);
	    if (data->value)
		out << SPACE << (data->value);
	    break;
	case EL_DATA_TOKEN_WORD:
	    out << "word" << SPACE << data->N << SPACE;
	    print_symbol_expr_value(out, data->address);
	    if (data->value)
		out << SPACE << (data->value);
	    break;
	case EL_DATA_TOKEN_LONG:
	    out << "long" << SPACE << data->N << SPACE;
	    print_symbol_expr_value(out, data->address);
	    if (data->value)
		out << SPACE << (data->value);
	    break;
	case EL_DATA_TOKEN_LLONG:
	    out << "longlong" << SPACE << data->N << SPACE;
	    print_symbol_expr_value(out, data->address);
	    if (data->value)
		out << SPACE << (data->value);
	    break;
	case EL_DATA_TOKEN_FLOAT:
	    out << "float" << SPACE << data->N << SPACE;
	    print_symbol_expr_value(out, data->address);
	    if (data->value)
		out << SPACE << (data->value);
	    break;
	case EL_DATA_TOKEN_DOUBLE:
	    out << "double" << SPACE << data->N << SPACE;
	    print_symbol_expr_value(out, data->address);
	    if (data->value)
		out << SPACE << (data->value);
	    break;
	case EL_DATA_TOKEN_ALIGN:
	    out << "align" << SPACE << data->N << SPACE;
	    print_symbol_expr_value(out, data->address);
	    if (data->value)
		out << SPACE << (data->value);
	    break;
	case EL_DATA_TOKEN_ASCII:
	    out << "ascii" << SPACE;
	    print_string_expr_value(out, data->value);
	    out << SPACE;
	    print_symbol_expr_value(out, data->address);
	    break;
	case EL_DATA_TOKEN_ASCIZ:
	    out << "asciz" << SPACE;
	    print_string_expr_value(out, data->value);
	    out << SPACE;
	    print_symbol_expr_value(out, data->address);
	    break;
	case EL_DATA_TOKEN_RESERVE:
	    out << "reserve" << SPACE << data->N;
	    break;
	case EL_DATA_TOKEN_GLOBAL:
	    out << "global" << SPACE;
	    print_symbol_expr_value(out, data->address);
            if (data->id)
              out << SPACE << L_PAREN << "objid i" << data->id << R_PAREN;
	    out << data->src_type;
	    break;
	case EL_DATA_TOKEN_WB:
	    out << "wb" << SPACE << (data->address) << SPACE << (data->value);
	    break;
	case EL_DATA_TOKEN_WW:
	    out << "ww" << SPACE << (data->address) << SPACE << (data->value);
	    break;
	case EL_DATA_TOKEN_WI:
	    out << "wi" << SPACE << (data->address) << SPACE << (data->value);
	    break;
        case EL_DATA_TOKEN_WQ:
	    out << "wq" << SPACE << (data->address) << SPACE << (data->value);
	    break;
	case EL_DATA_TOKEN_WF:
	    out << "wf" << SPACE << (data->address) << SPACE << (data->value);
	    break;
	case EL_DATA_TOKEN_WF2:
	    out << "wf2" << SPACE << (data->address) << SPACE << (data->value);
	    break;
	case EL_DATA_TOKEN_WS:
	    out << "ws" << SPACE << (data->address) << SPACE << (data->value);
	    break;
	case EL_DATA_TOKEN_ELEMENT_SIZE:
	    out << "element_size" << SPACE << data->N << SPACE;
	    print_symbol_expr_value(out, data->address);
	    if (data->value)
		out << SPACE << (data->value);
	    break;
	case EL_DATA_TOKEN_DEF_STRUCT:
	    out << "def_struct" << SPACE;
	    print_symbol_expr_value(out, data->address);
	    break;
	case EL_DATA_TOKEN_DEF_UNION:
	    out << "def_union" << SPACE;
	    print_symbol_expr_value(out, data->address);
	    break;
	case EL_DATA_TOKEN_DEF_ENUM:
	    out << "def_enum" << SPACE;
	    print_symbol_expr_value(out, data->address);
	    break;
	case EL_DATA_TOKEN_FIELD:
	    out << "field" << SPACE;
	    print_symbol_expr_value(out, data->address);
	    out << data->src_type;
	    if (data->value)
		out << SPACE << (data->value);
	    break;
	case EL_DATA_TOKEN_ENUMERATOR:
	    out << "enumerator" << SPACE;
	    print_symbol_expr_value(out, data->address);
	    out << data->src_type;
	    if (data->value)
		out << SPACE << (data->value);
	    break;
	default:
	    El_punt("<< El_data: illegal type for data");
	    break;
    }
    out << R_PAREN << endline;

    return (out);
}

/*
 *	El_datalist output routines
 */

IR_outstream &operator<<(IR_outstream &out, El_datalist *datalist)
{
    if (datalist == NULL)
	return (out);

    out << "data" << SPACE << L_PAREN << endline;
    out << indent;
    for (List_iterator<El_data *> li(datalist->elements); li!=0; li++) {
	out << *li;
    }
    out << outdent << R_PAREN << endline;

    return (out);
}

//--------------------------- Reader ------------------------------

/*
 *	El_expression input routines
 */

El_expression *El_read_compound_expression(IR_instream &in)
{
    IR_token token;
    El_compound_expression *cexpr = NULL;
    El_op_expression *oexpr;
    El_expression *expr;

    /* Read in the opcode first */
    in >> token;

    if (token == (const char*)"add") {
	cexpr = new El_compound_expression(3);
	oexpr = new El_op_expression(ELEX_ADD);
	cexpr->args[0] = (El_expression *) oexpr;
	cexpr->args[1] = El_read_expression(in);
	cexpr->args[2] = El_read_expression(in);
    }
    else if (token == (const char*)"sub") {
	cexpr = new El_compound_expression(3);
	oexpr = new El_op_expression(ELEX_SUB);
	cexpr->args[0] = (El_expression *) oexpr;
	cexpr->args[1] = El_read_expression(in);
	cexpr->args[2] = El_read_expression(in);
    }
    else if (token == (const char*)"mul") {
	cexpr = new El_compound_expression(3);
	oexpr = new El_op_expression(ELEX_MUL);
	cexpr->args[0] = (El_expression *) oexpr;
	cexpr->args[1] = El_read_expression(in);
	cexpr->args[2] = El_read_expression(in);
    }
    else if (token == (const char*)"div") {
	cexpr = new El_compound_expression(3);
	oexpr = new El_op_expression(ELEX_DIV);
	cexpr->args[0] = (El_expression *) oexpr;
	cexpr->args[1] = El_read_expression(in);
	cexpr->args[2] = El_read_expression(in);
    }
    else if (token == (const char*)"neg") {
	cexpr = new El_compound_expression(2);
	oexpr = new El_op_expression(ELEX_NEG);
	cexpr->args[0] = (El_expression *) oexpr;
	cexpr->args[1] = El_read_expression(in);
    }
    else if (token == (const char*)"com") {
	cexpr = new El_compound_expression(2);
	oexpr = new El_op_expression(ELEX_COM);
	cexpr->args[0] = (El_expression *) oexpr;
	cexpr->args[1] = El_read_expression(in);
    }
    else {
	El_punt("El_read_compound_expression: unknown opcode!");
    }

    expr = (El_expression *) cexpr;
    return (expr);
}

El_expression *El_read_expression(IR_instream &in, bool left_paren_read)
{
    El_expression *expr;
    El_value_expression *vexpr;
    El_symbol_expression *lexpr;
    El_string_expression *sexpr;
    int ival;
    bool bval;
    float fval;
    double dval;
    char *sval;
    IR_token token;
    eString str;

    if (! left_paren_read)
        in >> token;

    if ((left_paren_read) || (token == L_PAREN)) {
        in >> token;
	if ((left_paren_read) && (token == R_PAREN)) {
	    in.unget_token();
	    return (NULL);
	}
        if (token == (const char*)"i") {
            in >> ival;
            vexpr = new El_value_expression();
            vexpr->type = EL_EXPRESSION_INT_VALUE;
            vexpr->value.i = ival;
	    expr = (El_expression *)vexpr;
        }
        else if (token == (const char*)"p") {
            in >> bval;
            vexpr = new El_value_expression();
            vexpr->type = EL_EXPRESSION_PREDICATE_VALUE;
            vexpr->value.p = bval;
	    expr = (El_expression *)vexpr;
        }
        else if (token == (const char*)"f") {
            in >> fval;
            vexpr = new El_value_expression();
            vexpr->type = EL_EXPRESSION_FLOAT_VALUE;
            vexpr->value.f = fval;
	    expr = (El_expression *)vexpr;
        }
        else if (token == (const char*)"f2") {
            in >> dval;
            vexpr = new El_value_expression();
            vexpr->type = EL_EXPRESSION_DOUBLE_VALUE;
            vexpr->value.f2 = dval;
	    expr = (El_expression *)vexpr;
        }
        else if (token == (const char*)"l") {
            in >> sval;
            lexpr = new El_symbol_expression(sval);
	    expr = (El_expression *)lexpr;
        }
        else if (token == (const char*)"s") {
            in >> sval;
	    str = DOUBLE_QUOTE;
            str |= sval;
            str |= DOUBLE_QUOTE;

            sexpr = new El_string_expression(str);
	    expr = (El_expression *)sexpr;
        }
        else {
	    in.unget_token();
            expr = El_read_compound_expression(in);
        }

        in >> R_PAREN;
	return (expr);
    }
    else {
	if (! left_paren_read)
	    in.unget_token();
	return (NULL);
    }
}

El_expression *El_read_symbol_expr_value(IR_instream &in)
{
    El_symbol_expression *sexpr;
    El_expression *expr;
    char *val;

    in >> val;
    sexpr = new El_symbol_expression(val);

    expr = (El_expression *)sexpr;
    return (expr);
}

El_expression *El_read_string_expr_value(IR_instream &in)
{
    El_string_expression *sexpr;
    El_expression *expr;
    char *val;
    eString str;

    in >> val;
    str = DOUBLE_QUOTE;
    str |= val;
    str |= DOUBLE_QUOTE;

    sexpr = new El_string_expression(str);

    expr = (El_expression *)sexpr;
    return (expr);
}

/*
 *	El_data input routines
 */

El_data_memory_segment_type El_read_ms_type(IR_instream &in)
{
    IR_token token;

    in >> token;
    if (token == (const char*)"text") {
	return (EL_DATA_MS_TEXT);
    }
    else if (token == (const char*)"data") {
	return (EL_DATA_MS_DATA);
    }
    else if (token == (const char*)"data1") {
	return (EL_DATA_MS_DATA1);
    }
    else if (token == (const char*)"data2") {
	return (EL_DATA_MS_DATA2);
    }
    else if (token == (const char*)"bss") {
	return (EL_DATA_MS_BSS);
    }
    else if (token == (const char*)"sync") {
	return (EL_DATA_MS_SYNC);
    }
    else {
	El_punt("El_read_ms_type: unknown ms type");
	return (EL_DATA_MS_UNKNOWN);
    }
}

bool El_read_data_stype_flag(IR_instream &in, El_data_stype_flags &flag)
{
    IR_token token;
    in >> token;
    if (token.token_type() != IDENTIFIER) {
        in.unget_token();
        return false;
    }
    else if (token == (const char*)"const") flag = EL_DATA_ST_CONST;
    else if (token == (const char*)"volatile") flag = EL_DATA_ST_VOLATILE;
    else if (token == (const char*)"noalias") flag = EL_DATA_ST_NOALIAS;
    else if (token == (const char*)"register") flag = EL_DATA_ST_REGISTER;
    else if (token == (const char*)"auto") flag = EL_DATA_ST_AUTO;
    else if (token == (const char*)"static") flag = EL_DATA_ST_STATIC;
    else if (token == (const char*)"extern") flag = EL_DATA_ST_EXTERN;
    else if (token == (const char*)"global") flag = EL_DATA_ST_GLOBAL;
    else if (token == (const char*)"parameter") flag = EL_DATA_ST_PARAMETER;
    else if (token == (const char*)"void") flag = EL_DATA_ST_VOID;
    else if (token == (const char*)"char") flag = EL_DATA_ST_CHAR;
    else if (token == (const char*)"short") flag = EL_DATA_ST_SHORT;
    else if (token == (const char*)"int") flag = EL_DATA_ST_INT;
    else if (token == (const char*)"long") flag = EL_DATA_ST_LONG;
    else if (token == (const char*)"longlong") flag = EL_DATA_ST_LLONG;
    else if (token == (const char*)"float") flag = EL_DATA_ST_FLOAT;
    else if (token == (const char*)"double") flag = EL_DATA_ST_DOUBLE;
    else if (token == (const char*)"signed") flag = EL_DATA_ST_SIGNED;
    else if (token == (const char*)"unsigned") flag = EL_DATA_ST_UNSIGNED;
    else if (token == (const char*)"struct") flag = EL_DATA_ST_STRUCT;
    else if (token == (const char*)"union") flag = EL_DATA_ST_UNION;
    else if (token == (const char*)"enum") flag = EL_DATA_ST_ENUM;
    else if (token == (const char*)"vararg") flag = EL_DATA_ST_VARARG;
    else {
	in.unget_token();
	return false;
    }
    return true;
}

bool El_read_data_access_method(IR_instream &in, El_data_access_method &meth)
{
    IR_token token;

    in >> token;
    if (token == (const char*)"arr") 
	meth = EL_DATA_ACCESS_ARRAY;
    else if (token == (const char*)"ptr")
	meth = EL_DATA_ACCESS_POINTER;
    else if (token == (const char*)"fun")
	meth = EL_DATA_ACCESS_FUNCTION;
    else {
	in.unget_token();
	return false;
    }
    return true;
}

El_data_access_declarator *El_read_data_access_declarator(IR_instream &in)
{
    IR_token token;
    El_data_access_method meth;
    El_data_access_declarator *dcltr=NULL;

    if (El_read_data_access_method(in, meth)) {
	dcltr = new El_data_access_declarator();
	dcltr->method = meth;
	if (meth == EL_DATA_ACCESS_ARRAY)
	    dcltr->index = El_read_expression(in);
    }

    return (dcltr);
}

/* Assume L_PAREN already read when get to here */
El_data_source_type_info *El_read_source_type_info(IR_instream &in)
{
    IR_token token;
    El_data_stype_flags sflag;
    El_data_access_declarator *dcltr=NULL;
    El_data_source_type_info *stype=NULL;
    char *name;

    while (1) {
        in >> token;	/* Gets L_PAREN */
	if (token != L_PAREN) {
	    in.unget_token();
	    return (stype);
	}
	if (El_read_data_stype_flag(in, sflag)) {
	    if (stype == NULL)
		stype = new El_data_source_type_info();
	    stype->flags.set_bit((int)sflag);
	    if ((sflag == EL_DATA_ST_STRUCT) || (sflag == EL_DATA_ST_UNION) ||
		(sflag == EL_DATA_ST_ENUM)) {
		in >> name;
		stype->struct_name = name;
	    }
	}
	else if ((dcltr = El_read_data_access_declarator(in)) != NULL) {
	    if (stype == NULL)
		stype = new El_data_source_type_info();
	    stype->access_dcltrs.add_tail(dcltr);
	}
	else {
	    // in.unget_token();
	    return (stype);
	}
	in >> R_PAREN;	/* Matching right paren */
    }

    assert(0);	/* Should never reach this point */
    return (NULL);
}

El_data *El_read_data(IR_instream &in)
{
    IR_token token;
    int ival;
    El_data *data;

    in >> token;

    if (token == L_PAREN) {
        in >> token;
	data = new El_data;

        if (token == (const char*)"ms") {
	    data->t_type = EL_DATA_TOKEN_MS;
	    data->ms_type = El_read_ms_type(in);
	}
        else if (token == (const char*)"void") {
	    data->t_type = EL_DATA_TOKEN_VOID;
	    data->address = El_read_symbol_expr_value(in);
	}
        else if (token == (const char*)"byte") {
	    data->t_type = EL_DATA_TOKEN_BYTE;
	    in >> ival;
	    data->N = ival;
	    data->address = El_read_symbol_expr_value(in);
	    data->value = El_read_expression(in);
	}
        else if (token == (const char*)"word") {
	    data->t_type = EL_DATA_TOKEN_WORD;
	    in >> ival;
	    data->N = ival;
	    data->address = El_read_symbol_expr_value(in);
	    data->value = El_read_expression(in);
	}
        else if (token == (const char*)"long") {
	    data->t_type = EL_DATA_TOKEN_LONG;
	    in >> ival;
	    data->N = ival;
	    data->address = El_read_symbol_expr_value(in);
	    data->value = El_read_expression(in);
	}
        else if (token == (const char*)"longlong") {
	    data->t_type = EL_DATA_TOKEN_LLONG;
	    in >> ival;
	    data->N = ival;
	    data->address = El_read_symbol_expr_value(in);
	    data->value = El_read_expression(in);
	}
        else if (token == (const char*)"float") {
	    data->t_type = EL_DATA_TOKEN_FLOAT;
	    in >> ival;
	    data->N = ival;
	    data->address = El_read_symbol_expr_value(in);
	    data->value = El_read_expression(in);
	}
        else if (token == (const char*)"double") {
	    data->t_type = EL_DATA_TOKEN_DOUBLE;
	    in >> ival;
	    data->N = ival;
	    data->address = El_read_symbol_expr_value(in);
	    data->value = El_read_expression(in);
	}
        else if (token == (const char*)"align") {
	    data->t_type = EL_DATA_TOKEN_ALIGN;
	    in >> ival;
	    data->N = ival;
	    data->address = El_read_symbol_expr_value(in);
	    data->value = El_read_expression(in);
	}
        else if (token == (const char*)"ascii") {
	    data->t_type = EL_DATA_TOKEN_ASCII;
	    data->value = El_read_string_expr_value(in);
	    data->address = El_read_symbol_expr_value(in);
	}
        else if (token == (const char*)"asciz") {
	    data->t_type = EL_DATA_TOKEN_ASCIZ;
	    data->value = El_read_string_expr_value(in);
	    data->address = El_read_symbol_expr_value(in);
	}
        else if (token == (const char*)"reserve") {
	    data->t_type = EL_DATA_TOKEN_RESERVE;
	    in >> ival;
	    data->N = ival;
	}
        else if (token == (const char*)"global") {
	    data->t_type = EL_DATA_TOKEN_GLOBAL;
	    data->address = El_read_symbol_expr_value(in);
	    data->src_type = El_read_source_type_info(in);
            // L_PAREN is already read in
            in >> token;
            if (token == (const char*)"objid") {
              El_expression *expr = El_read_expression(in, true);
	      data->id = ((El_value_expression *)(expr))->value.i;
	      data->src_type = El_read_source_type_info(in);
            } else
	      in.unget_token();
	}
        else if (token == (const char*)"wb") {
	    data->t_type = EL_DATA_TOKEN_WB;
	    data->address = El_read_expression(in);
	    data->value = El_read_expression(in);
	}
        else if (token == (const char*)"ww") {
	    data->t_type = EL_DATA_TOKEN_WW;
	    data->address = El_read_expression(in);
	    data->value = El_read_expression(in);
	}
        else if (token == (const char*)"wi") {
	    data->t_type = EL_DATA_TOKEN_WI;
	    data->address = El_read_expression(in);
	    data->value = El_read_expression(in);
	}
        else if (token == (const char*)"wq") {
	    data->t_type = EL_DATA_TOKEN_WQ;
	    data->address = El_read_expression(in);
	    data->value = El_read_expression(in);
	}
        else if (token == (const char*)"wf") {
	    data->t_type = EL_DATA_TOKEN_WF;
	    data->address = El_read_expression(in);
	    data->value = El_read_expression(in);
	}
        else if (token == (const char*)"wf2") {
	    data->t_type = EL_DATA_TOKEN_WF2;
	    data->address = El_read_expression(in);
	    data->value = El_read_expression(in);
	}
        else if (token == (const char*)"ws") {
	    data->t_type = EL_DATA_TOKEN_WS;
	    data->address = El_read_expression(in);
	    data->value = El_read_expression(in);
	}
        else if (token == (const char*)"element_size") {
	    data->t_type = EL_DATA_TOKEN_ELEMENT_SIZE;
	    in >> ival;
	    data->N = ival;
	    data->address = El_read_symbol_expr_value(in);
	    data->value = El_read_expression(in);
	}
	else if (token == (const char*)"def_struct") {
	    data->t_type = EL_DATA_TOKEN_DEF_STRUCT;
	    data->address = El_read_symbol_expr_value(in);
	}
	else if (token == (const char*)"def_union") {
	    data->t_type = EL_DATA_TOKEN_DEF_UNION;
	    data->address = El_read_symbol_expr_value(in);
	}
	else if (token == (const char*)"def_enum") {
	    data->t_type = EL_DATA_TOKEN_DEF_ENUM;
	    data->address = El_read_symbol_expr_value(in);
	}
	else if (token == (const char*)"field") {
	    data->t_type = EL_DATA_TOKEN_FIELD;
	    data->address = El_read_symbol_expr_value(in);
	    data->src_type = El_read_source_type_info(in);
	    data->value = El_read_expression(in, true);
	}
	else if (token == (const char*)"enumerator") {
	    data->t_type = EL_DATA_TOKEN_ENUMERATOR;
	    data->address = El_read_symbol_expr_value(in);
	    data->value = El_read_expression(in);
	}
	else {
	    El_punt("El_read_data: unknown data type");
	}

	in >> R_PAREN;
	return (data);
    }
    else {
	in.unget_token();
	return (NULL);
    }
}

/*
 *	El_datalist input routines
 */

El_datalist *El_read_datalist(IR_instream &in)
{
    IR_token token;
    El_datalist *datalist;
    El_data *data;

    in >> token;

    if (token == (const char*)"data") {
	in >> L_PAREN;
	datalist = new El_datalist;
	while ((data = El_read_data(in)) != NULL) {
	    datalist->elements.add_tail(data);
	}
	in >> R_PAREN;
	return (datalist);
    }
    else {
	in.unget_token();
	return (NULL);
    }
}
