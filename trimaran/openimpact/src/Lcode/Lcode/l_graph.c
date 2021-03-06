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
/* The IMPACT Research Group (www.crhc.uiuc.edu/IMPACT)                      */
/*****************************************************************************\
 *      File:   l_graph.c
 *      Author: David August, Wen-mei Hwu
 *      Creation Date:  August 1997
 *
\*****************************************************************************/

/* 10/29/02 REK Adding config.h */
#include <config.h>
#include <Lcode/l_main.h>

#undef DEBUG_WEIGHTED_BB_GRAPH

L_Alloc_Pool *L_BB_Pool = NULL;
L_Alloc_Pool *L_BB_arc_Pool = NULL;
L_Alloc_Pool *L_BB_graph_Pool = NULL;


L_BB *
L_new_bb (L_Cb * cb, GraphNode node)
{
  L_BB *bb;

  if (L_BB_Pool == NULL)
    {
      L_BB_Pool = L_create_alloc_pool ("L_BB", sizeof (L_BB), 128);
    }

  bb = (L_BB *) L_alloc (L_BB_Pool);
  bb->node = node;
  bb->cb = cb;
  bb->first_op = NULL;
  bb->last_op = NULL;
  bb->ptr = NULL;
  /* SER: For PCE. */
  bb->pf_bb = NULL;
  bb->ptr_exit = NULL;
  bb->type = BB_MN_MX;
  bb->bb_size = 0;
  bb->cutset_size = 0;

  return bb;
}

L_BB *
L_delete_bb (L_BB * bb)
{
  L_free (L_BB_Pool, bb);

  return NULL;
}

L_BB_arc *
L_new_bb_arc (GraphArc arc, L_Oper * branch, int taken)
{
  L_BB_arc *bb_arc;

  if (L_BB_arc_Pool == NULL)
    {
      L_BB_arc_Pool =
        L_create_alloc_pool ("L_BB_arc", sizeof (L_BB_arc), 128);
    }

  bb_arc = (L_BB_arc *) L_alloc (L_BB_arc_Pool);
  bb_arc->branch = branch;
  bb_arc->taken = taken;

  return bb_arc;
}

L_BB_arc *
L_delete_bb_arc (L_BB_arc * bb_arc)
{
  L_free (L_BB_arc_Pool, bb_arc);

  return NULL;
}


L_BB_graph *
L_new_bb_graph (L_Func * fn, Graph graph)
{
  L_BB_graph *bb_graph;

  if (L_BB_graph_Pool == NULL)
    {
      L_BB_graph_Pool =
        L_create_alloc_pool ("L_BB_graph", sizeof (L_BB_graph), 4);
    }

  bb_graph = (L_BB_graph *) L_alloc (L_BB_graph_Pool);
  bb_graph->graph = graph;
  bb_graph->fn = fn;
  bb_graph->hash_cb_bb = HashTable_create (128);

  return bb_graph;
}

Graph
L_delete_bb_graph (Graph graph)
{
  L_BB_graph *bb_graph;
  GraphNode node;
  GraphArc arc;

  bb_graph = (L_BB_graph *) GraphContents (graph);

  HashTable_free (bb_graph->hash_cb_bb);
  L_free (L_BB_graph_Pool, bb_graph);

  List_start (graph->nodes);
  while ((node = (GraphNode) List_next (graph->nodes)))
    L_delete_bb ((L_BB *) GraphNodeContents (node));

  List_start (graph->arcs);
  while ((arc = (GraphArc) List_next (graph->arcs)))
    L_delete_bb_arc ((L_BB_arc *) GraphArcContents (arc));

  Graph_free_graph (graph);

  if (L_BB_Pool)
    L_print_alloc_info (stderr, L_BB_Pool, 0);
  if (L_BB_arc_Pool)
    L_print_alloc_info (stderr, L_BB_arc_Pool, 0);
  if (L_BB_graph_Pool)
    L_print_alloc_info (stderr, L_BB_graph_Pool, 0);

  return NULL;
}

GraphNode
L_bb_branch (Graph bb_graph, GraphNode bb_node, L_Oper * oper, int weight_flag)
{
  GraphNode dest_bb_node;
  GraphNode new_bb_node;
  GraphArc arc;
  L_BB *bb;
  L_BB *new_bb;
  L_BB_graph *bb_graph_info;
  L_BB_arc *new_bb_arc;

  /* PCE */
  L_Flow * flow;
  unsigned int weight = 0, flow_weight = 0;

  bb_graph_info = (L_BB_graph *) GraphContents (bb_graph);
  bb = (L_BB *) GraphNodeContents (bb_node);
  dest_bb_node = (GraphNode) HashTable_find (bb_graph_info->hash_cb_bb,
                                             (L_find_branch_dest (oper)->id));

  arc = Graph_connect_nodes (bb_graph, bb_node, dest_bb_node);
  new_bb_arc = L_new_bb_arc (arc, oper, TRUE);
  GraphArcContents (arc) = new_bb_arc;

  bb->last_op = oper;

  /* PCE */
  if (weight_flag)
    {
      weight = bb->weight;

      flow = L_find_flow_for_branch (bb->cb, oper);
      if (flow->weight < 0.0)
	{
	  fprintf (stderr, "PCE: negative weight for flow at oper %d: %f\n",
		   oper->id, flow->weight);
	  flow_weight = 1;
	}
      else if ((flow->weight) > ITMAXU32)
	{
	  fprintf (stderr, "PCE: overflow of flow weight at oper %d.\n",
		   oper->id);
	  flow_weight = UNSIGNED_INT_MAX;
	}
      else
	{
	  flow_weight = (unsigned int) flow->weight;
	  if (flow_weight == 0)
	    flow_weight = 1;
	}
      new_bb_arc->weight = flow_weight;
      if (flow_weight >= weight)
	weight = 1;
      else
	weight -= flow_weight;
    }

  /* If the unconditional branch is predicated and not the last op of a cb 
     that has no fall thru, then it is "really" a conditional branch */
  if (((L_cond_branch (oper)) &&
       (!(L_EXTRACT_BIT_VAL (bb->cb->flags, L_CB_HYPERBLOCK_NO_FALLTHRU) &&
          oper->next_op == NULL))) || L_check_branch_opcode (oper))
    {
      if (oper->next_op)
        {
          new_bb_node = Graph_create_graph_node (bb_graph);
          new_bb = L_new_bb (bb->cb, new_bb_node);
          GraphNodeContents (new_bb_node) = new_bb;

          arc = Graph_connect_nodes (bb_graph, bb_node, new_bb_node);
          new_bb_arc = L_new_bb_arc (arc, oper, FALSE);
          GraphArcContents (arc) = new_bb_arc;

	  /* PCE */
          new_bb->first_op = oper->next_op;
	  if (weight_flag)
	    {
	      new_bb->type = BB_SINGLE_ENTRY;
	      new_bb->weight = weight;
	      new_bb_arc->weight = weight;

	      if (L_uncond_branch_opcode (new_bb->first_op))
		new_bb->type |= BB_REMOVABLE;
	    }

          bb_node = new_bb_node;
        }
    }
  else
    {
      bb->type |= BB_SINGLE_EXIT;
      bb_node = NULL;
    }
  return bb_node;
}


GraphNode
L_bb_register_branch (Graph bb_graph, GraphNode bb_node, L_Oper * oper,
		      int weight_flag)
{
  GraphNode dest_bb_node;
  GraphArc arc;
  L_BB *bb;
  L_BB_graph *bb_graph_info;
  L_Flow *flow;
  L_BB_arc *new_bb_arc;
  /* PCE */
  unsigned int weight = 0;
  int exits = 0;

  bb_graph_info = (L_BB_graph *) GraphContents (bb_graph);
  bb = (L_BB *) GraphNodeContents (bb_node);

  for (flow = L_find_flow_for_branch (bb->cb, oper);
       flow != NULL; flow = flow->next_flow)
    {
      exits++;
      dest_bb_node = (GraphNode) HashTable_find (bb_graph_info->hash_cb_bb,
                                                 flow->dst_cb->id);
      arc = Graph_connect_nodes (bb_graph, bb_node, dest_bb_node);
      new_bb_arc = L_new_bb_arc (arc, oper, TRUE);
      GraphArcContents (arc) = new_bb_arc;
      if (weight_flag)
	{
	  if (flow->weight < 0.0)
	    {
	      fprintf (stderr, "PCE: negative weight for register branch at "
		       "oper %d: %f\n", oper->id, flow->weight);
	      weight = 1;
	    }
	  else if ((flow->weight) > ITMAXU32)
	    {
	      fprintf (stderr, "PCE: overflow of flow weight at oper %d.\n",
		       oper->id);
	      weight = UNSIGNED_INT_MAX;
	    }
	  else
	    {
	      weight = (unsigned int) flow->weight;
	      if (weight == 0)
		weight = 1;
	    }
	  new_bb_arc->weight = weight;
	}
    }
  bb->last_op = oper;
  if (exits == 1)
    bb->type |= BB_SINGLE_EXIT;
  return NULL;
}


GraphNode
L_bb_return_branch (Graph bb_graph, GraphNode bb_node, L_Oper * oper,
		    int weight_flag)
{
  GraphNode new_bb_node;
  GraphArc arc;
  L_BB *bb;
  L_BB *new_bb;
  L_BB_arc *new_bb_arc;

  /* PCE */
  L_Flow * flow;
  unsigned int weight, flow_weight = 0;

  bb = (L_BB *) GraphNodeContents (bb_node);
  bb->last_op = oper;

  if ((L_cond_ret (oper)) &&
      (!(L_EXTRACT_BIT_VAL (bb->cb->flags, L_CB_HYPERBLOCK_NO_FALLTHRU) &&
         oper->next_op == NULL)))
    {
      new_bb_node = Graph_create_graph_node (bb_graph);
      new_bb = L_new_bb (bb->cb, new_bb_node);
      GraphNodeContents (new_bb_node) = new_bb;

      arc = Graph_connect_nodes (bb_graph, bb_node, new_bb_node);
      new_bb_arc = L_new_bb_arc (arc, oper, FALSE);
      GraphArcContents (arc) = new_bb_arc;

      /* PCE */
      new_bb->first_op = oper->next_op;
      if (weight_flag)
	{
	  new_bb->type = BB_SINGLE_ENTRY;
	  flow = L_find_flow_for_branch (bb->cb, oper);
	  if (flow->weight < 0.0)
	    {
	      fprintf (stderr, "PCE: negative weight for flow at oper %d: "
		       "%f\n", oper->id, flow->weight);
	      flow_weight = 0;
	    }
	  else if ((flow->weight) > ITMAXU32)
	    {
	      fprintf (stderr, "PCE: overflow of flow weight at oper %d.\n",
		       oper->id);
	      flow_weight = UNSIGNED_INT_MAX;
	    }
	  else
	    {
	      flow_weight = (unsigned int) flow->weight;
	    }

	  if (bb->weight <= flow_weight)
	    weight = 1;
	  else
	    weight = bb->weight - flow_weight;
	  new_bb->weight = weight;
	  new_bb_arc->weight = weight;

	  if (L_uncond_branch_opcode (new_bb->first_op))
	    new_bb->type |= BB_REMOVABLE;
	}

      bb_node = new_bb_node;
    }
  else
    {
      bb->type |= BB_SINGLE_EXIT;
      bb_node = NULL;
    }

  return bb_node;
}


GraphNode
L_bb_complete_cb (Graph bb_graph, GraphNode bb_node, int weight_flag)
{
  GraphNode dest_bb_node;
  GraphArc arc;
  L_BB *bb;
  L_BB_graph *bb_graph_info;
  L_BB_arc *new_bb_arc;
  L_Oper * last_op;

  /* PCE */
  L_Flow * flow;
  unsigned int flow_weight = 0;

  bb_graph_info = (L_BB_graph *) GraphContents (bb_graph);
  bb = (L_BB *) GraphNodeContents (bb_node);

  last_op = bb->cb->last_op;
  bb->last_op = bb->cb->last_op;

  if (bb->cb->next_cb)
    {
      dest_bb_node = (GraphNode) HashTable_find (bb_graph_info->hash_cb_bb,
                                                 bb->cb->next_cb->id);

      arc = Graph_connect_nodes (bb_graph, bb_node, dest_bb_node);
      new_bb_arc = L_new_bb_arc (arc, NULL, FALSE);
      GraphArcContents (arc) = new_bb_arc;

      /* PCE */
      if (weight_flag)
	{
	  /* If no branch, then fallthrough arc. */
	  if (!(L_uncond_branch_opcode (last_op) ||
		L_cond_branch_opcode (last_op) ||
		L_check_branch_opcode (last_op)))
	    {
	      bb->type |= BB_SINGLE_EXIT;
	      new_bb_arc->weight = bb->weight;
	    }
	  /* Otherwise need to find flow for op, get fallthrough weight */
	  else
	    {
	      flow = L_find_flow_for_branch (bb->cb, last_op);
	      if (flow->weight < 0.0)
		{
		  fprintf (stderr, "PCE: negative weight for flow at oper "
			   "%d: %f\n", last_op->id, flow->weight);
		  flow_weight = 0;
		}
	      else if ((flow->weight) > ITMAXU32)
	    	{
		  fprintf (stderr, "PCE: overflow of fallthrough flow weight "
			   "at cb %d.\n", bb->cb->id);
		  flow_weight = UNSIGNED_INT_MAX;
		}
	      else
		{
		  flow_weight = (unsigned int) flow->weight;
		}
	      if (bb->weight <= flow_weight)
		new_bb_arc->weight = 1;
	      else
	        new_bb_arc->weight = bb->weight - flow_weight;
	    }
	}
    }
  return NULL;
}


Graph
L_create_bb_graph (L_Func * fn)
{
  L_BB *bb;
  L_BB_graph *bb_graph_info;
  L_Cb *cb;
  L_Oper *oper;
  Graph bb_graph;
  GraphNode bb_node;

  bb_graph = Graph_create_graph ();
  bb_graph_info = L_new_bb_graph (fn, bb_graph);
  GraphContents (bb_graph) = bb_graph_info;

  for (cb = fn->first_cb; cb; cb = cb->next_cb)
    {
      bb_node = Graph_create_graph_node (bb_graph);
      bb = L_new_bb (cb, bb_node);
      GraphNodeContents (bb_node) = bb;

      bb->first_op = cb->first_op;

      if (!GraphRootNode (bb_graph))
        GraphRootNode (bb_graph) = bb_node;

      HashTable_insert (bb_graph_info->hash_cb_bb, cb->id, bb_node);
    }

  for (cb = fn->first_cb; cb; cb = cb->next_cb)
    {
      bb_node =
        (GraphNode) HashTable_find (bb_graph_info->hash_cb_bb, cb->id);

      for (oper = cb->first_op; oper; oper = oper->next_op)
        {
	  if (!bb_node)
	    {
	      printf("L_create_bb_graph: Unreachable code in cb %d\n",cb->id);
	      break;
	    }

	  if (L_uncond_branch_opcode (oper) || L_cond_branch_opcode (oper)
              || L_check_branch_opcode (oper))
            bb_node = L_bb_branch (bb_graph, bb_node, oper, 0);
          else if (L_register_branch_opcode (oper))
            bb_node = L_bb_register_branch (bb_graph, bb_node, oper, 0);
          else if (L_subroutine_return_opcode (oper))
            bb_node = L_bb_return_branch (bb_graph, bb_node, oper, 0);
        }
      if (bb_node)
        bb_node = L_bb_complete_cb (bb_graph, bb_node, 0);
    }
  return bb_graph;
}


Graph
L_create_weighted_bb_graph (L_Func * fn, PRED_FLOW * pred_flow)
{
  L_BB *bb;
  L_BB_graph *bb_graph_info;
  L_Cb *cb;
  L_Oper *oper;
  L_Flow *flow;
  Graph bb_graph;
  GraphNode bb_node;
  /* PCE */
  int bb_size;
  int cutset_size;

  bb_graph = Graph_create_graph ();
  bb_graph_info = L_new_bb_graph (fn, bb_graph);
  GraphContents (bb_graph) = bb_graph_info;

#ifdef DEBUG_WEIGHTED_BB_GRAPH
  fprintf (stderr, "Building weighted BB graph.\n");
#endif

  for (cb = fn->first_cb; cb; cb = cb->next_cb)
    {
      bb_node = Graph_create_graph_node (bb_graph);
      bb = L_new_bb (cb, bb_node);
      GraphNodeContents (bb_node) = bb;
      if ((cb->weight) > ITMAXU32)
	{
	  fprintf (stderr, "PCE: overflow of cb weight at cb %d.\n", cb->id);
	  bb->weight = UNSIGNED_INT_MAX;
	}
      else
	{
	  bb->weight = (unsigned int) cb->weight; /* PCE */
	  if (bb->weight == 0)
	    bb->weight = 1;
	}
      bb->first_op = cb->first_op;
      if (L_uncond_branch_opcode (cb->first_op))
	bb->type = BB_REMOVABLE;

      /* PCE */
      if (pred_flow)
	bb->pf_bb = D_find_pf_bb (pred_flow, cb, cb->first_op);
      flow = cb->src_flow;
      if (!flow || (!(flow->next_flow) && !(flow->prev_flow)))
	bb->type |= BB_SINGLE_ENTRY;

      if (!GraphRootNode (bb_graph))
        GraphRootNode (bb_graph) = bb_node;

      HashTable_insert (bb_graph_info->hash_cb_bb, cb->id, bb_node);
    }

  for (cb = fn->first_cb; cb; cb = cb->next_cb)
    {
      bb_node =
        (GraphNode) HashTable_find (bb_graph_info->hash_cb_bb, cb->id);
      bb_size = 0;
      cutset_size = Set_size (L_get_cb_IN_set (cb));
      for (oper = cb->first_op; oper; oper = oper->next_op)
        {
	  bb_size++;
	  if (!bb_node)
	    {
	      printf("L_create_bb_graph: Unreachable code in cb %d\n",cb->id);
	      break;
	    }

	  if (L_uncond_branch_opcode (oper) || L_cond_branch_opcode (oper)
              || L_check_branch_opcode (oper))
	    {
	      bb = (L_BB *) (bb_node->ptr);
	      bb->bb_size = bb_size;
	      bb->cutset_size = cutset_size;

	      bb_node = L_bb_branch (bb_graph, bb_node, oper, 1);
	      if (pred_flow && bb_node)
		{
		  bb = (L_BB *) GraphNodeContents (bb_node);
		  bb->pf_bb = D_find_pf_bb (pred_flow, cb, bb->first_op);
		}
	      bb_size = 0;
	    }
          else if (L_register_branch_opcode (oper))
	    {
	      bb = (L_BB *) (bb_node->ptr);
	      bb->bb_size = bb_size;
	      bb->cutset_size = cutset_size;

	      bb_node = L_bb_register_branch (bb_graph, bb_node, oper, 1);
	      if (pred_flow && bb_node)
		{
		  bb = (L_BB *) GraphNodeContents (bb_node);
		  bb->pf_bb = D_find_pf_bb (pred_flow, cb, bb->first_op);
		}
	      bb_size = 0;
	    }
          else if (L_subroutine_return_opcode (oper))
	    {
	      bb = (L_BB *) (bb_node->ptr);
	      bb->bb_size = bb_size;
	      bb->cutset_size = cutset_size;

	      bb_node = L_bb_return_branch (bb_graph, bb_node, oper, 1);
	      if (pred_flow && bb_node)
		{
		  bb = (L_BB *) GraphNodeContents (bb_node);
		  bb->pf_bb = D_find_pf_bb (pred_flow, cb, bb->first_op);
		}
	      bb_size = 0;
	    }
        }
      if (bb_node)
	{
	  bb = (L_BB *) (bb_node->ptr);
	  bb->bb_size = bb_size;
	  bb->cutset_size = cutset_size;

	  bb_node = L_bb_complete_cb (bb_graph, bb_node, 1);
	}
    }

#ifdef DEBUG_WEIGHTED_BB_GRAPH
  fprintf (stderr, "Printing weighted BB graph.\n");
  List_start (bb_graph->nodes);
  while (bb_node = (GraphNode) List_next (bb_graph->nodes))
    {
      GraphArc arc;
      bb = (L_BB *) bb_node->ptr;
      fprintf (stderr, "BB in cb %d, first op %d, weight %u, type %x, "
	       "pf_bb %x, pointer %x.\n", bb->cb->id, 
	       bb->first_op ? bb->first_op->id:0, bb->weight, bb->type, 
	       bb->pf_bb, bb->ptr);
      List_start (bb_node->pred);
      fprintf (stderr, "\tIncoming arcs:\n");
      while (arc = (GraphArc) List_next (bb_node->pred))
	{
	  if (((L_BB_arc *)(arc->ptr))->branch)
	    fprintf (stderr, "\t\tArc from branch op %d, cb %d.\n",
		     ((L_BB_arc *)(arc->ptr))->branch->id,
		     ((L_BB *)(arc->pred->ptr))->cb->id);
	  else
	    fprintf (stderr, "\t\tArc from fall-through from op %d, cb %d.\n",
		     ((L_BB *)(arc->pred->ptr))->last_op ?
		     ((L_BB *)(arc->pred->ptr))->last_op->id : 0,
		     ((L_BB *)(arc->pred->ptr))->cb->id);
	}
      List_start (bb_node->succ);
      fprintf (stderr, "\tOutgoing arcs:\n");
      while (arc = (GraphArc) List_next (bb_node->succ))
	{
	  if (((L_BB_arc *)(arc->ptr))->branch)
	    fprintf (stderr, "\t\tArc from branch op %d to dest cb %d.\n",
		     ((L_BB_arc *)(arc->ptr))->branch->id,
		     ((L_BB *)(arc->succ->ptr))->cb->id);
	  else
	    fprintf (stderr, "\t\tFall-through from op %d to dest BB at op "
		     "%d, cb %d.\n", bb->last_op ? bb->last_op->id : 0,
		     ((L_BB *)(arc->succ->ptr))->first_op ? 
		     ((L_BB *)(arc->succ->ptr))->first_op->id : 0,
		     ((L_BB *)(arc->succ->ptr))->cb->id);
	}
    }
#endif

  return bb_graph;
}


Graph
L_create_cb_graph (L_Func * fn)
{
  L_Cb *cb;
  Graph cb_graph;
  GraphNode cb_node;
  GraphNode dest_cb_node;
  HashTable hash_cb;
  L_Flow *flow;

  cb_graph = Graph_create_graph ();

  hash_cb = HashTable_create (128);

  for (cb = fn->first_cb; cb; cb = cb->next_cb)
    {
      cb_node = Graph_create_graph_node (cb_graph);
      GraphNodeContents (cb_node) = cb;

      if (!GraphRootNode (cb_graph))
        GraphRootNode (cb_graph) = cb_node;

      HashTable_insert (hash_cb, cb->id, cb_node);
    }

  for (cb = fn->first_cb; cb; cb = cb->next_cb)
    {
      cb_node = (GraphNode) HashTable_find (hash_cb, cb->id);
      for (flow = cb->dest_flow; flow; flow = flow->next_flow)
        {
          dest_cb_node =
            (GraphNode) HashTable_find (hash_cb, flow->dst_cb->id);

          Graph_connect_nodes (cb_graph, cb_node, dest_cb_node);
        }
    }

  HashTable_free (hash_cb);

  return cb_graph;
}

/***********************/

void
L_bb_print_hook (FILE * file, GraphNode node)
{
  L_BB *bb;

  bb = (L_BB *) GraphNodeContents (node);

  if (node->imm_post_dom && node->imm_dom)
    fprintf (file, "id %d: cb %d, ops %d -> %d (%d) imm_dom %d,  imm_pdom %d",
             node->id, bb->cb->id, bb->first_op->id, bb->last_op->id,
             node->info, node->imm_dom->id, node->imm_post_dom->id);
  else if (node->imm_post_dom)
    fprintf (file, "id %d: cb %d, ops %d -> %d (%d) imm_dom X,  imm_pdom %d",
             node->id, bb->cb->id, bb->first_op->id, bb->last_op->id,
             node->info, node->imm_post_dom->id);
  else if (node->imm_dom)
    fprintf (file, "id %d: cb %d, ops %d -> %d (%d) imm_dom %d,  imm_pdom X",
             node->id, bb->cb->id, bb->first_op->id, bb->last_op->id,
             node->info, node->imm_dom->id);
  else
    fprintf (file, "id %d: cb %d, ops %d -> %d (%d) imm_dom X,  imm_pdom X",
             node->id, bb->cb->id, bb->first_op->id, bb->last_op->id,
             node->info);
}

void
L_cb_print_hook (FILE * file, GraphNode node)
{
  L_Cb *cb;

  cb = (L_Cb *) node->ptr;

  fprintf (file, "cb %d", cb->id);
}

void
L_bb_node_del (GraphNode node)
{
  L_BB *bb;

  bb = (L_BB *) GraphNodeContents (node);

  L_delete_bb (bb);
}

void
L_bb_arc_del (GraphArc arc)
{
  if (GraphArcContents (arc) != NULL)
    L_punt ("L_bb_arc_del: arc->ptr not null.");

  return;
}


void
graph_test (L_Func * fn)
{
  Graph bb_g;

  bb_g = L_create_cb_graph (fn);

  Graph_dominator (bb_g);
  Graph_post_dominator (bb_g);
  Graph_topological_sort (bb_g);

  printf ("This should be true %d == %d\n", List_size (bb_g->topo_list),
          List_size (bb_g->nodes));
  Graph_imm_dominator (bb_g);
  Graph_imm_post_dominator (bb_g);
  Graph_control_dependence (bb_g);
  Graph_equiv_cd (bb_g);
  Graph_daVinci (bb_g, "TEST.davinci", L_cb_print_hook);
}
