#pragma once  

#include "stdafx.h"
#include "../vswMath/matVector2D.h"

#include <vector>
#include <boost/smart_ptr/shared_ptr.hpp>

typedef std::vector<boost::shared_ptr<matPoint2D>> VectorPoints;

class grdRingStack
{

public: /* CONSTRUCTORS */ 

  grdRingStack() {}
  grdRingStack(VectorPoints&);

private: /* INNER METHODS */

  bool is_right_turn(
    const matPoint2D&,
    const matPoint2D&,
    const matPoint2D&
  ) const;

public: /* MAIN METHODS */

  bool is_convex() const;

  int get_size() const;

public: /* ACCESSORS */

  void push_back(boost::shared_ptr<matPoint2D>&);
  
  boost::shared_ptr<matPoint2D> get_point_by_ind(int) const;

private: /* MEMBERS */ 

  VectorPoints m_stack;

};