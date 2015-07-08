/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   Copyright (c) 2014,2015 The plumed team
   (see the PEOPLE file at the root of the distribution for a list of names)

   See http://www.plumed-code.org for more information.

   This file is part of plumed, version 2.

   plumed is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   plumed is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with plumed.  If not, see <http://www.gnu.org/licenses/>.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
#ifndef __PLUMED_adjmat_DFSClustering_h
#define __PLUMED_adjmat_DFSClustering_h

#include "ActionWithInputMatrix.h"
#include "multicolvar/MultiColvarFunction.h"
#include "multicolvar/AtomValuePack.h"

namespace PLMD {
namespace adjmat {

class DFSClustering : public ActionWithInputMatrix {
private:
/// Used to identify the cluster we are working on 
  int number_of_cluster;
/// The number of neighbors each atom has
  std::vector<unsigned> nneigh;
/// The adjacency list
  Matrix<unsigned> adj_list;
/// Vector that stores the sizes of the current set of clusters
  std::vector< std::pair<unsigned,unsigned> > cluster_sizes;
/// Vector that identifies the cluster each atom belongs to
  std::vector<unsigned> which_cluster;
/// The color that tells us whether a node has been visited
  std::vector<unsigned> color;
/// This is here for the time being but I want to get rid of it
  multicolvar::MultiColvarFunction* myfunction;
/// Get the store data vessel that has in the data of interest
  vesselbase::StoreDataVessel* mydata;
/// The recursive function at the heart of this method
  int explore( const unsigned& index );
protected:
/// Get the number of clusters that have been found
  unsigned getNumberOfClusters() const ;
/// Check if one of the stored values is active
  bool isCurrentlyActive( const unsigned& ind ) const ; 
/// Get the atoms in one of the clusters
  void retrieveAtomsInCluster( const unsigned& clust, std::vector<unsigned>& myatoms ) const ;
/// Get the vector for task ind
  void getVectorForTask( const unsigned& ind, const bool& normed, std::vector<double>& orient0 ) const ;
/// Get vector derivatives
  void getVectorDerivatives( const unsigned& ind, const bool& normed, MultiValue& myder0 ) const ;
public:
/// Create manual
  static void registerKeywords( Keywords& keys );
/// Constructor
  explicit DFSClustering(const ActionOptions&);
/// Required as we have to be able to deal with vectors
  unsigned getNumberOfQuantities();
/// This checks whether derivatives can be computed given the base multicolvar
  void turnOnDerivatives();
/// Do the matrix calculation
  void calculate();
/// Derivatives of elements of adjacency matrix are unimportant.  We thus
/// overwrite this routine as this makes the code faster
  void updateActiveAtoms( multicolvar::AtomValuePack& myatoms ) const {}
/// Do the calculation on the cluster
  virtual void doCalculationOnCluster()=0;
};

inline
unsigned DFSClustering::getNumberOfClusters() const {
  return number_of_cluster + 1;
}

}
}

#endif
