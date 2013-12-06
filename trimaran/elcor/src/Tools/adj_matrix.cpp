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
//      File:           adj_matrix.cpp
//      Authors:        Bob Rau, Shail Aditya
//      Created:        February 1997
//      Description:    Implementation of the Adj_matrix class
//
/////////////////////////////////////////////////////////////////////////////

#include "adj_matrix.h"

///////////////////////////////////////////////////////////
//
// Adjacency Matrix
//
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////

Adj_matrix::Adj_matrix (int n) : bMatrix (n)
{
  int i, j;
  
  numNodes = n;
  for (i = 0; i < numNodes; i++)
    for (j = 0; j < numNodes; j++)
      if (i == j)
	bMatrix.set(i, j);
      else
	bMatrix.reset(i,j);
  }

///////////////////////////////////////////////////////////

Adj_matrix::Adj_matrix (const Bitmatrix& conc_matrix) 
  : bMatrix (conc_matrix.size())
{
  int i, j;
  
  numNodes = conc_matrix.size();
  for (i = 0; i < numNodes; i++)
    for (j = 0; j < numNodes; j++)
      if (i == j || conc_matrix.bit(i, j))
        bMatrix.set(i, j);
      else 
        bMatrix.reset(i, j);
}

///////////////////////////////////////////////////////////

Adj_matrix::Adj_matrix (const Adj_matrix& amatrix) 
  : bMatrix (amatrix.numNodes)
{
  
  numNodes = amatrix.numNodes;
  copyMatrix(amatrix);
}

///////////////////////////////////////////////////////////

int Adj_matrix::size() const {
  return numNodes;
}

///////////////////////////////////////////////////////////

void Adj_matrix::setToAllZeros () {
  int i, j;
  
  for (i = 0; i < numNodes; i++)
    for (j = 0; j < numNodes; j++)
      bMatrix.reset(i,j);
}

///////////////////////////////////////////////////////////

void Adj_matrix::setToAllOnes () {
  int i, j;
  
  for (i = 0; i < numNodes; i++)
    for (j = 0; j < numNodes; j++)
      bMatrix.set(i, j);
}

///////////////////////////////////////////////////////////

void Adj_matrix::set (int i, int j) {
  bMatrix.set(i, j);
}

///////////////////////////////////////////////////////////

void Adj_matrix::reset (int i, int j) {
  bMatrix.reset(i, j);
}

///////////////////////////////////////////////////////////

bool Adj_matrix::bit(int i, int j) const {
  return bMatrix.bit(i, j);
}

///////////////////////////////////////////////////////////

void Adj_matrix::complementMatrix (const Adj_matrix& amx) {
  int i, j;
  
  for (i = 0; i < numNodes; i++)
    for (j = 0; j < numNodes; j++)
      if (i == j || amx.bit(i, j) == 0)
        bMatrix.set(i, j);
      else
        bMatrix.reset(i, j);
}

///////////////////////////////////////////////////////////

void Adj_matrix::copyMatrix (const Adj_matrix& amx) {
  for (int i = 0; i < numNodes; i++) {
    for (int j = 0; j < numNodes; j++) {
      if (amx.bit(i, j)) {
	bMatrix.set(i, j);
      } else {
	bMatrix.reset(i, j);
      }
    }
  }
}

///////////////////////////////////////////////////////////

ostream& operator << (ostream& os, Adj_matrix& m) {
  os << "- adj_matrix={" << endl; /*indicate the type*/
  if (m.numNodes==0) {
    os << "NULL MATRIX" << endl;
  } else {
    m.printMatrix(os);
  }
  os << "}" << endl;
  return os;
}

void Adj_matrix::printMatrix (ostream& OutFile) {
  int i, j;
  
  for (i = 0; i < numNodes; i++) {
    for (j = 0; j < numNodes; j++)
      OutFile << (bMatrix.bit(i,j) ? 1 : 0);
    OutFile << endl;
    }
  }

void Adj_matrix::print() {
  cout << *this;
}

///////////////////////////////////////////////////////////

List_set<int>& Adj_matrix::NeighborsOf (int i, List_set<int>& nhbrs) {
  for (int j = 0; j < numNodes; j++)
    if (bMatrix.bit(i, j))
      nhbrs += j;

  return nhbrs;
}

///////////////////////////////////////////////////////////

List_set<int>& Adj_matrix::largerNeighborsOf (int i, List_set<int>& nhbrs) {
  for (int j = i+1; j < numNodes; j++)
    if (bMatrix.bit(i, j))
      nhbrs += j;

  return nhbrs;
}

///////////////////////////////////////////////////////////

bool Adj_matrix::completeGraph (List_set<int>& currentClique, int tryNode) {
  List_set_iterator<int> iter;
  
  for (iter(currentClique); iter!=0; iter++)
    if (!bMatrix.bit(*iter, tryNode))
      return false;

  return true;
}

///////////////////////////////////////////////////////////

bool Adj_matrix::maximal (List_set<int>& currentClique) {
  int nodeIndex;

  for (nodeIndex = 0; nodeIndex < numNodes; nodeIndex++) {
    if (!currentClique.find(nodeIndex) &&
	completeGraph(currentClique, nodeIndex)) {
      return false;
    }
  }
  return true;
}

///////////////////////////////////////////////////////////

void Adj_matrix::recursiveFindCliques (List_set<int>& currentClique,
				       List_set<int> candidateNodes, 
				       List_of_sets& cliqueList) {
  bool firstItern;
  int nextNode;
  List_set<int> prunedNodes, irredundNodes, nhbrs, clique;
  List_iterator<Int_set> setiter;
  
  complexCount++;
  
  if (candidateNodes.is_empty()) {
    if (maximal(currentClique)) {
      cliqueList.add_tail(currentClique);
//       if (sdbg(grph,1)) {
//         cout << "Max Clique added:  ";
//         printClique(currentClique);
//         cout << endl;
//       }
    } else {
//       if (sdbg(grph,3)) {
//         cout << "Failed maximality: ";
//         printClique(currentClique);
//         cout << endl;
//       }
    }
  } else {
    // if the union of currentClique and candidateNodes is a subset of a
    // clique that has already been found, then no new clique can be found
    // down this path.
    if (H1) {
      for (setiter(cliqueList); setiter!=0; setiter++) {
        clique = *setiter;
        if (clique.is_subset(currentClique) && clique.is_subset(candidateNodes)) {
//           if (sdbg(grph,2)) {
//             cout << "PRUNE 1: ";
//             printClique(currentClique);
//             cout << " + ";
//             printClique(candidateNodes);
//             cout << " is subset of ";
//             printClique(clique);
//             cout << endl;
// 	  }
          return;
	}
      }
    }

    firstItern = true;
    irredundNodes = candidateNodes;
    // the pruning of candidate nodes assures that every member of
    // candidateNodes forms a complete graph with currentClique
    for (; !candidateNodes.is_empty(); ) {
      nextNode = candidateNodes.pop();
      if (!completeGraph(currentClique, nextNode)) {
        cout << "ERROR: ";
        printClique(currentClique);
        cout << " + { " << nextNode << " }";
        cout << " is not a complete graph. Candidates are: ";
        printClique(candidateNodes);
        cout << endl;
      } else {
        // add nextNode to the current clique and continue
        currentClique += nextNode;
        irredundNodes -= nextNode;
        prunedNodes = irredundNodes;
        // only those candidate nodes that are also neighbors of nextNode remain 
        // candidates once nextNode is added to currentClique
        nhbrs = NeighborsOf(nextNode, nhbrs.clear());
        prunedNodes *= nhbrs;
//         if (sdbg(grph,3)) {
// 	  cout << "At recursive call with node " << nextNode;
//           cout << "\nCurrent Clique: ";
//           printClique(currentClique);
//           cout << "\nCandidates: ";
//           printClique(prunedNodes);
//           cout << endl;
// 	}
        recursiveFindCliques (currentClique, prunedNodes, cliqueList);
        currentClique -= nextNode;
//         if (sdbg(grph,3)) {
// 	  cout << "After recursive call with node " << nextNode;
//           cout << "\nCurrent Clique: ";
//           printClique(currentClique);
//           cout << "\nCandidates: ";
//           printClique(irredundNodes);
//           cout << endl;
// 	}
        // if all of the candidateNodes are currentClique's neighbors then quit searching
        if ((H3) && (prunedNodes == irredundNodes)) {
//           if (sdbg(grph,2)) {
//             cout << "PRUNE 3: ";
//             printClique(currentClique);
//             cout << "; current nhbrs ";
//             printClique(nhbrs);
//             cout << " includes remaining candidates ";
//             printClique(irredundNodes);
//             cout << endl;
// 	  }
          return;
	}
        // if the union of currentClique and candidateNodes is a subset of a
        // clique that has already been found, then no new clique can be found
        // down this path.
        if (H2) {
          for (setiter(cliqueList); setiter!=0; setiter++) {
            clique = *setiter;
            if (clique.is_subset(currentClique) && clique.is_subset(irredundNodes)) {
//               if (sdbg(grph,2)) {
//                 cout << "PRUNE 2: ";
//                 printClique(currentClique);
//                 cout << " + ";
//                 printClique(irredundNodes);
//                 cout << " is subset of ";
//                 printClique(clique);
//                 cout << endl;
// 	      }
              return;
	    }
	  }
	}

        if ((H4) && (firstItern)) {
//           if (sdbg(grph,2)) {
//             cout << "PRUNE 4: ";
//             printClique(currentClique);
// 	    cout << "; current nhbrs ";
// 	    printClique(nhbrs); 
// 	    cout << " subtracted from candidates ";
//             printClique(candidateNodes);
//             cout << endl;
// 	  }
          candidateNodes -= nhbrs;
	}
        firstItern = false;
      }
    }
  }
}

///////////////////////////////////////////////////////////

// FIX: this was a command line parameter in PICO Synthesis.
// ``heuristic control for finding cliques''
int Synth_find_cliques_mask = 15;

void Adj_matrix::findCliques (List_of_sets& cliqueList) {
  List_set<int> emptySet, universeSet;
  int i, k;
  
  for (i = 0; i < numNodes; i++)
    universeSet += i;
  
  k = Synth_find_cliques_mask;
  H1 = ((k / 8) == 1); k = k % 8;
  H2 = ((k / 4) == 1); k = k % 4;
  H3 = ((k / 2) == 1); k = k % 2;
  H4 = (k == 1);
//   if (sdbg(grph,2)) {
//     cout << (H1 ? "true " : "false ");
//     cout << (H2 ? "true " : "false ");
//     cout << (H3 ? "true " : "false ");
//     cout << (H4 ? "true " : "false ");
//     cout << endl;
//   }
  
  complexCount = 0;
  cliqueList.clear();
  recursiveFindCliques(emptySet, universeSet, cliqueList);
}

///////////////////////////////////////////////////////////

void Adj_matrix::findCliques2 (List_of_sets& cliqueList) {
  List_set<int> emptySet, universeSet;
  int i,j,k;

  for (j = 0; j < numNodes; j++)
    universeSet += j;

  for (i = 1; i <= 15; i = i + 1) {
    k = i;
    
    H1 = ((k / 8) == 1); k = k % 8;
    H2 = ((k / 4) == 1); k = k % 4;
    H3 = ((k / 2) == 1); k = k % 2;
    H4 = (k == 1);
//     if (sdbg(grph,2)) {
//       cout << (H1 ? "true " : "false ");
//       cout << (H2 ? "true " : "false ");
//       cout << (H3 ? "true " : "false ");
//       cout << (H4 ? "true " : "false ");
//       cout << endl;
//     }
    
    cliqueList.clear();
    
    complexCount = 0;
    recursiveFindCliques(emptySet, universeSet, cliqueList);
    printCliques(cliqueList);
    cout << (int)H1 << (int)H2 << (int)H3 << (int)H4
	 << ": Complexity count = " << complexCount << endl;
  }
  
}

///////////////////////////////////////////////////////////

void Adj_matrix::printClique (List_set<int>& clique) {
  List_set_iterator<int> L2;
  
  cout << "{ ";
  for (L2(clique); L2 != 0; L2++)
    cout << *L2 << " ";
  cout << "}" << flush;
  }

///////////////////////////////////////////////////////////

void Adj_matrix::printCliques (List_of_sets& cliqueList) {
  List_iterator<Int_set> L1;
  
  cout << endl << cliqueList.size() << " cliques:" << endl << endl;
  for (L1(cliqueList); L1 != 0; L1++) {
    printClique (*L1);
    cout << endl;
    }
  }

///////////////////////////////////////////////////////////

List_set<int> Adj_matrix::findNextConnComponent (List_set<int>& remainingNodes) {
  int nodeIndex, newNode;
  List_set<int> connectedComponent;
  List<int> queue;

  connectedComponent.clear ();
  //  cardinality = 0;        // use the size () method

  for (nodeIndex = 0; !remainingNodes.find(nodeIndex); nodeIndex++) {
  }

  queue.add_tail(nodeIndex);
  connectedComponent += nodeIndex;
  //  cardinality++;
  remainingNodes -= nodeIndex;

  for (; queue.size () != 0; ) {
    // remove the node at the front of the queue and add all of
    // its neighbors to the queue if not already in the queue
    nodeIndex = queue.pop ();
    for (newNode = 0; newNode < numNodes; newNode++)
      if (bit(nodeIndex, newNode) && remainingNodes.find(newNode)) {
        queue.add_tail(newNode);
        connectedComponent += newNode;
        //cardinality ++;
        remainingNodes -= newNode;
      }
  }

  return connectedComponent;
}

// end of adj_matrix.cpp
