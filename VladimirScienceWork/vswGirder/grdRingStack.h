#pragma once  

#include "stdafx.h"
#include "../vswMath/matVector2D.h"

#include <vector>
#include <boost/smart_ptr/shared_ptr.hpp>

typedef std::vector<boost::shared_ptr<matPoint2D>> VectorPoints;

enum eVectorsTurn{
  VT_ERROR = -1,
  VT_NONE = 0,
  VT_LEFT = 1,
  VT_RIGHT = 2
};

enum eConvexityCase {
  CC_ERROR = -1,
  CC_NOT_CONVEX = 0,
  CC_BORDER_POINTS_ONLY = 1,
  CC_CONVEX = 2
};

class grdRingStack
{

public: /* CONSTRUCTORS */ 

  grdRingStack() {}
  grdRingStack(VectorPoints&);

private: /* INNER METHODS */

  eVectorsTurn determine_turn(
    const matPoint2D&,
    const matPoint2D&,
    const matPoint2D&
  ) const;
  
public: /* MAIN METHODS */

  bool detect_rotation_direction() const;
  bool get_itr_by_content(const matPoint2D*,VectorPoints::iterator&) ;

  eConvexityCase is_convex() const;

  size_t get_size() const;
  
public: /* ACCESSORS */

  void push_back(boost::shared_ptr<matPoint2D>&);
  
  boost::shared_ptr<matPoint2D> get_point_by_ind(int) const;
  
  VectorPoints::const_iterator get_stack_begin() const { return m_stack.begin(); }
  VectorPoints::const_iterator get_stack_end() const { return m_stack.end(); }
  VectorPoints::iterator get_stack_begin_not_const() { return m_stack.begin(); }
  VectorPoints::iterator get_stack_end_not_const() { return m_stack.end(); }
  
private: /* MEMBERS */ 

  VectorPoints m_stack;

};