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
//      File:           el_opcode_usage.cpp.cpp
//      Author:         Scott A. Mahlke
//      Created:        July 1996
//      Description:    Calculate static/dynamic opcode usage stats
//
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>

#include "region.h"
#include "iterators.h"
#include "opcode.h"
#include "hash_functions.h"
#include "hash_map.h"
#include "opcode_properties.h"
#include "edge_utilities.h"
#include "edge_attributes.h"
#include "attributes.h"
#include "el_bb_tools.h"
#include "el_stats_init.h"
#include "el_io.h"

using namespace std;

/* Global vars: need these since want to keep track of info across multiple
   input function */
static int last_id = 0;
static int total_static = 0;
static double total_dynamic = 0.0;
Hash_map<Opcode, int> El_opcode_map(hash_opcode, 1023);
Vector<int> El_static_opcode_stats(4096, 0);
Vector<double> El_dynamic_opcode_stats(4096, 0.0);

void El_compute_opcode_stats(Procedure *f)
{
    Op *op;
    Compound_region *reg;
    int index;
    double weight;
    Opcode opcode;

    El_form_basic_blocks(f);

    for (Region_all_ops ri(f); ri!=0; ri++) {
        op = *ri;
        reg = (Compound_region *)op->parent();
        if (! reg->is_bb())
            El_punt("El_compute_opcode_stats: op parent should be BB");

        weight = reg->weight;
        opcode = op->opcode();

        if (is_pseudo(opcode))
            continue;

        if (! El_opcode_map.is_bound(opcode)) {
            El_opcode_map.bind(opcode, last_id);
            last_id++;
        }

        index = El_opcode_map.value(opcode);
        El_static_opcode_stats[index] += 1;
        El_dynamic_opcode_stats[index] += weight;
        total_static += 1;
        total_dynamic += weight;

    }

    El_remove_basic_blocks(f);
}

void El_print_opcode_stats()
{
    Vector<Opcode> opcode_array(4096, (Opcode) 0);
    Vector<int> static_opcode_stats(4096, 0);
    Vector<double> dynamic_opcode_stats(4096, 0.0);
    int i, j, index, tmp_int;
    Opcode opcode, tmp_opcode;
    double tmp_dbl, ratio_static, ratio_dynamic, cum_ratio_dynamic;

    i = 0;
    for (Hash_map_iterator<Opcode,int> hi(El_opcode_map); hi!=0; hi++) {
        opcode = (*hi).first;
        index = (*hi).second;
        opcode_array[i] = opcode;
        static_opcode_stats[i] = El_static_opcode_stats[index];
        dynamic_opcode_stats[i] = El_dynamic_opcode_stats[index];
        i++;
    }

    /* Sort according to dynamic weight */
    for (i=0; i<last_id; i++) {
        for (j=i+1; j<last_id; j++) {
            if (dynamic_opcode_stats[j] > dynamic_opcode_stats[i]) {
                tmp_opcode = opcode_array[i];
                opcode_array[i] = opcode_array[j];
                opcode_array[j] = tmp_opcode;
                tmp_int = static_opcode_stats[i];
                static_opcode_stats[i] = static_opcode_stats[j];
                static_opcode_stats[j] = tmp_int;
                tmp_dbl = dynamic_opcode_stats[i];
                dynamic_opcode_stats[i] = dynamic_opcode_stats[j];
                dynamic_opcode_stats[j] = tmp_dbl;
            }
        }
    }

    /* Print out to cout in tabular format */
    EL_OPCODE_STREAM->setf(ios::fixed);
    EL_OPCODE_STREAM->precision(3);
    EL_OPCODE_STREAM->setf(ios::left,ios::adjustfield);

    EL_OPCODE_STREAM->width(26);
    (*EL_OPCODE_STREAM) << "#     Opcode";
    EL_OPCODE_STREAM->width(17);
    (*EL_OPCODE_STREAM) << "Static";
    EL_OPCODE_STREAM->width(24);
    (*EL_OPCODE_STREAM) << "Dynamic";
    (*EL_OPCODE_STREAM) << "Cumul-Dyn" << endl;
    (*EL_OPCODE_STREAM) << "----------------------------------------------------------------------------" << endl;
    cum_ratio_dynamic = 0.0;
    for (i=0; i<last_id; i++) {
        ratio_static = (double) static_opcode_stats[i] / (double) total_static;
        ratio_dynamic = dynamic_opcode_stats[i] / total_dynamic;
        cum_ratio_dynamic += ratio_dynamic;
        (*EL_OPCODE_STREAM) << "[";
        EL_OPCODE_STREAM->width(3);
        (*EL_OPCODE_STREAM) << i << "] ";
        EL_OPCODE_STREAM->width(20);
        (*EL_OPCODE_STREAM) << opcode_array[i];
        EL_OPCODE_STREAM->width(5);
        (*EL_OPCODE_STREAM) << static_opcode_stats[i] << " (" << ratio_static;
        EL_OPCODE_STREAM->width(5);
        (*EL_OPCODE_STREAM) << ")" ;
        EL_OPCODE_STREAM->width(13);
        EL_OPCODE_STREAM->precision(0);
        (*EL_OPCODE_STREAM) << dynamic_opcode_stats[i];
        EL_OPCODE_STREAM->precision(3);
        (*EL_OPCODE_STREAM) << " (" << ratio_dynamic << ")";
        (*EL_OPCODE_STREAM) << "   " << cum_ratio_dynamic ;
        (*EL_OPCODE_STREAM) << endl;
    }
}
