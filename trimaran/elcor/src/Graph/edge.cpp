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
//      File:           edge.cpp
//      Authors:        Vinod Kathail, Sadun Anik, Santosh Abraham
//      Created:        January 1994
//      Description:    Edge class definition
//
/////////////////////////////////////////////////////////////////////////////

#include "edge.h"
#include "op.h"
#include "string_class.h"
#include "attributes.h"

/* Edge base class */

Edge::Edge(E_usage usage, E_status status)
   : scratch_int(0) ,dest_op(NULL), src_op(NULL), lat(1) ,
     usage_rep(usage), status_rep(status)
{
   attributes = new Graph_attribute ;
}

Edge::Edge(const Edge& edge)
   : scratch_int(edge.scratch_int), dest_op(edge.dest_op),
     src_op(edge.src_op), dest_p(edge.dest_p),
     src_p(edge.src_p), dest_altr(edge.dest_altr), 
     src_altr(edge.src_altr), lat(edge.lat),
     usage_rep(edge.usage_rep), status_rep(edge.status_rep)
{
   attributes = new Graph_attribute ;
}      

Edge::~Edge() {
   delete attributes ;
}

Edge& Edge::operator=(const Edge& edge) {
  if (this != &edge) {
     dest_op = NULL;
    src_op = NULL;
    dest_p = edge.dest_p;
    src_p = edge.src_p;
    dest_altr = edge.dest_altr;
    src_altr = edge.src_altr;
    usage_rep = edge.usage_rep;
    status_rep = edge.status_rep;
    lat = edge.lat;
    scratch_int = edge.scratch_int;
    delete attributes;
    attributes = new Graph_attribute(*(edge.attributes)) ;
  }      
  return *this;
}

bool Edge::operator==(const Edge& edge) const {
  if ((dest_op == edge.dest_op) && (src_op == edge.src_op) &&
      (dest_p == edge.dest_p) && (src_p == edge.src_p) &&
      (dest_altr == edge.dest_altr) && (src_altr == edge.src_altr))
    return true;
  else return false;
}
      
  
bool Edge::operator!=(const Edge& edge) const {
  return !operator==(edge);
}

Op* Edge::dest() const {return dest_op;}
Op* Edge::src() const {return src_op;}
Port_num Edge::dest_port() const {return dest_p;}
Port_num Edge::src_port() const {return src_p;}
Alt_num Edge::dest_alt() const {return dest_altr;}
Alt_num Edge::src_alt() const {return src_altr;}
int Edge::latency() const {return lat;}

Edge& Edge::set_src_alt(Alt_num alt) {
  src_altr = alt;
  return *this;
}

Edge& Edge::set_dest_alt(Alt_num dest) {
  dest_altr = dest;
  return *this;
}

Edge& Edge::set_dest(Op* op, Port_num port, Alt_num alt) {
   dest_op = op;
   dest_p = port;
   dest_altr = alt;
  return *this;
}

Edge& Edge::set_src(Op* op, Port_num port, Alt_num alt) {
   src_op = op;
   src_p = port;
   src_altr = alt;
  return *this;
}

Edge& Edge::set_latency(int latency) {
  lat = latency;
  return *this;
}

Edge& Edge::set_usage(E_usage usage) {
  usage_rep = usage;
  return *this;
}

Edge& Edge::set_status(E_status status) {
  status_rep = status;
  return *this;
}

bool Edge::is_control() const {return false;}
bool Edge::is_reg_flow() const {return false;}
bool Edge::is_reg_anti() const {return false;}
bool Edge::is_reg_out() const {return false;}
bool Edge::is_reg_dep() const {return false;}
bool Edge::is_mem() const {return false;}
bool Edge::is_seq() const {return false;}
     
bool Edge::is_semantic() const {
  if (usage_rep == E_SEMANTIC) return true;
  else return false;
}

bool Edge::is_certain() const {
  if (status_rep == E_CERTAIN) return true;
  else return false;
}

void Edge::print(ostream& os) const {
  if (usage_rep == E_SEMANTIC) os << "*";
  else os << "+";  
  os << " ";
  if (src_op != NULL) {
    os << src_op->id() << ":" << port_type_to_text(src_port_type())
       << (int) src_p;
    if (src_altr != 0) 
      os << "." << src_altr;
  }
  else os << "???";
  os <<  " -> ";
  if (dest_op != NULL) {
    os << dest_op->id() << ":" << port_type_to_text(dest_port_type())
       << (int) dest_p;
    if (dest_altr != 0) 
      os << "." << dest_altr;
  }
  else os << "???";
  os << " {" << lat << "}";
  os << " [" << scratch_int << "]";
}

ostream& operator<<(ostream& os, const Edge& edge){
  edge.print(os);
  return os;
}


/* Control edge class */
Control::Control(E_usage usage, E_status status) 
	        :Edge(usage, status) {}

Control::Control(const Control& edge)
                :Edge(edge) {}

Edge* Control::clone() const{
  Control* temp = new Control(*this);
  return temp;
}

Control::~Control(){}

Control& Control::operator=(const Control& edge) {
  Edge::operator=(edge);
  return *this;
}

bool Control::operator==(const Edge& edge) const {
  if (is_control()) return (Edge::operator==(edge));
  else return false;
}  

Port_type Control::dest_port_type() const { return CONTROL;}
Port_type Control::src_port_type() const {return CONTROL;}

Edge& Control::set_latency() {
  lat = src_op->flow_time(CONTROL, src_p, OUT) - 
     dest_op->flow_time(CONTROL, dest_p, IN);
  return *this;
}

bool Control::is_control() const {return true;}

bool Control::is_seq() const {
  if ((dest_p == CONTROL0) && (src_p == CONTROL0)) return true;
  else return false;
}
     
void Control::print(ostream& os) const {
  os << "C";
  Edge::print(os);
}


/* Reg_flow edge class */
Reg_flow::Reg_flow(E_usage usage, E_status status) 
	        :Edge(usage, status) {}

Reg_flow::Reg_flow(const Reg_flow& edge)
                :Edge(edge) {}

Edge* Reg_flow::clone() const{
  Reg_flow* temp = new Reg_flow(*this);
  return temp;
}

Reg_flow::~Reg_flow(){}

Reg_flow& Reg_flow::operator=(const Reg_flow& edge) {
  Edge::operator=(edge);
  return *this;
}

bool Reg_flow::operator==(const Edge& edge) const {
  if (is_reg_flow()) return (Edge::operator==(edge));
  else return false;
}  

Port_type Reg_flow::dest_port_type() const {return SRC;}
Port_type Reg_flow::src_port_type() const {return DEST;}

Edge& Reg_flow::set_latency() {
  lat = src_op->flow_time(DEST, src_p) - 
     dest_op->flow_time(SRC, dest_p);
  return *this;
}

bool Reg_flow::is_reg_flow() const {return true;}
bool Reg_flow::is_reg_dep() const {return true;}
     
void Reg_flow::print(ostream& os) const {
  os << "RF";
  Edge::print(os);
}



/* Reg_anti edge class */
Reg_anti::Reg_anti(E_usage usage, E_status status) 
	        :Edge(usage, status) {}

Reg_anti::Reg_anti(const Reg_anti& edge)
                :Edge(edge) {}

Edge* Reg_anti::clone() const{
  Reg_anti* temp = new Reg_anti(*this);
  return temp;
}

Reg_anti::~Reg_anti(){}

Reg_anti& Reg_anti::operator=(const Reg_anti& edge) {
  Edge::operator=(edge);
  return *this;
}

bool Reg_anti::operator==(const Edge& edge) const {
  if (is_reg_anti()) return (Edge::operator==(edge));
  else return false;
}  

Port_type Reg_anti::dest_port_type() const {return DEST;}
Port_type Reg_anti::src_port_type() const {return SRC;}

Edge& Reg_anti::set_latency() {
  lat = src_op->anti_time(SRC, src_p) - 
     dest_op->anti_time(DEST, dest_p);
  return *this;
}

bool Reg_anti::is_reg_anti() const {return true;}
bool Reg_anti::is_reg_dep() const {return true;}
     
void Reg_anti::print(ostream& os) const {
  os << "RA";
  Edge::print(os);
}

/* Reg_out edge class */
Reg_out::Reg_out(E_usage usage, E_status status) 
	        :Edge(usage, status) {}

Reg_out::Reg_out(const Reg_out& edge)
                :Edge(edge) {}

Edge* Reg_out::clone() const{
  Reg_out* temp = new Reg_out(*this);
  return temp;
}

Reg_out::~Reg_out(){}

Reg_out& Reg_out::operator=(const Reg_out& edge) {
  Edge::operator=(edge);
  return *this;
}

bool Reg_out::operator==(const Edge& edge) const {
  if (is_reg_out()) return (Edge::operator==(edge));
  else return false;
}  

Port_type Reg_out::dest_port_type() const {return DEST;}
Port_type Reg_out::src_port_type() const {return DEST;}

Edge& Reg_out::set_latency() {
  lat = src_op->flow_time(DEST, src_p) - 
     dest_op->anti_time(DEST, dest_p);
  return *this;
}

bool Reg_out::is_reg_out() const {return true;}
bool Reg_out::is_reg_dep() const {return true;}
     
void Reg_out::print(ostream& os) const {
  os << "RO";
  Edge::print(os);
}

/* Mem edge class */
Mem::Mem(E_usage usage, E_status status) 
	        :Edge(usage, status) {}

Mem::Mem(const Mem& edge)
                :Edge(edge) {}

Edge* Mem::clone() const{
  Mem* temp = new Mem(*this);
  return temp;
}

Mem::~Mem(){}

Mem& Mem::operator=(const Mem& edge) {
  Edge::operator=(edge);
  return *this;
}

bool Mem::operator==(const Edge& edge) const {
  if (is_mem()) return (Edge::operator==(edge));
  else return false;
}  

Port_type Mem::dest_port_type() const {return MEM;}
Port_type Mem::src_port_type() const {return MEM;}

Edge& Mem::set_latency() {
  lat = src_op->flow_time(MEM, src_p, OUT) - 
     dest_op->flow_time(MEM, dest_p, IN);
  return *this;
}

bool Mem::is_mem() const {return true;}
     
void Mem::print(ostream& os) const {
  os << "MF";
  Edge::print(os);
}

// end of edge.cpp
