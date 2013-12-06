/*****************************************************************************\
 *
 *		      Illinois Open Source License
 *                     University of Illinois/NCSA
 *                         Open Source License
 *
 * Copyright (c) 2004, The University of Illinois at Urbana-Champaign.
 * All rights reserved.
 *
 * Developed by:             
 *
 *		IMPACT Research Group
 *
 *		University of Illinois at Urbana-Champaign
 *
 *              http://www.crhc.uiuc.edu/IMPACT
 *              http://www.gelato.org
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal with the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimers.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimers in
 * the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the names of the IMPACT Research Group, the University of
 * Illinois, nor the names of its contributors may be used to endorse
 * or promote products derived from this Software without specific
 * prior written permission.  THE SOFTWARE IS PROVIDED "AS IS",
 * WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS WITH THE SOFTWARE.
 *
\*****************************************************************************/
/*===========================================================================
 *      File :          l_lb_graph.h
 *      Description :   Tools for dealing with Lblock graphs
 *      Creation Date : September 1997
 *      Authors :       David August, Kevin Crozier
 *
 *      (C) Copyright 1997, David August, Kevin Crozier
 *      All rights granted to University of Illinois Board of Regents.
 *
 *==========================================================================*/
#ifndef L_LB_GRAPH_H
#define L_LB_GRAPH_H

/* 10/29/02 REK Adding config.h */
#include <config.h>
#include <library/i_graph.h>
#include <Lcode/l_code.h>

/* Graph node types */
#define CB    1
#define START 2
#define STOP  3

/* Arc types - arcs correspond to taken or fallthru flows */
#define FALLTHRU 0
#define TAKEN    1

/* the first of maybe many possible LB_BB flags */
#define CONNECT_TO_STOP 1

/* macro to get a cb from a graph node */
#define GraphNodeCB(n) ((LB_BB *) GraphNodeContents(n))->cb

/* A poorly named struct that hangs off graph nodes to point to cbs */
typedef struct _LB_BB
{
  int type;			/* Indicates type of node */
  int flag;
  L_Cb *cb;			/* Pointer to the actual Cb this bb is */
  GraphNode node;		/* Points to the node that this struct is apart of */
}
LB_BB;

#ifdef __cplusplus
extern "C"
{
#endif

/*====================
 * External Function Prototypes
 *====================*/
  extern Graph LB_create_flow_graph (L_Func *, L_Cb *, Set);
  extern Graph LB_free_flow_graph (Graph);
  extern Graph LB_add_cb_to_graph (Graph, L_Cb *, L_Cb *, int);
  extern Graph LB_finish_frp_graph (Graph);
  extern void LB_bb_print_hook (FILE *, GraphNode);

#ifdef __cplusplus
}
#endif

#endif
