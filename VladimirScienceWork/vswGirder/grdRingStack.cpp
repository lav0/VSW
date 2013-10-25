#include "stdafx.h"
#include "grdRingStack.h"

//=============================================================================
grdRingStack::grdRingStack(VectorPoints& a_vector)
//
// lav 20/10/13 written.
//
{
  m_stack = a_vector;
}

//=============================================================================
void grdRingStack::push_back(boost::shared_ptr<matPoint2D>& a_shp_point) 
//
// lav 20/10/13 written.
//
{
  m_stack.push_back(a_shp_point);
}

//=============================================================================
size_t grdRingStack::get_size() const
//
// lav 20/10/13 written.
//
{
  return m_stack.size();
}

//=============================================================================
boost::shared_ptr<matPoint2D> grdRingStack::get_point_by_ind(int a_ind) const
//
// lav 20/10/13 written.
//
{
  _ASSERT(a_ind < (int) get_size());

  return m_stack[a_ind];
}

//=============================================================================
eVectorsTurn grdRingStack::determine_turn(
  const matPoint2D& a_pn1,
  const matPoint2D& a_pn2,
  const matPoint2D& a_pn3
) const
//
// lav 22/10/13 written.
// lav 25/10/13 rewritten.
//
{
  double d_cross_prod_result = matVector2D(a_pn1, a_pn2).cross_prod_z(
    matVector2D(a_pn2, a_pn3));
  if (IS_ZERO(d_cross_prod_result)) {
    return VT_NONE;
  } else if (d_cross_prod_result > 0) {
    return VT_LEFT;
  } else if (d_cross_prod_result < 0) {
    return VT_RIGHT;
  }
  _ASSERT(0);
  return VT_ERROR;
}

//=============================================================================
eConvexityCase grdRingStack::is_convex() const
//
// lav 20/10/13 written.
// lav 25/10/13 used determine_turn(). refactored.
//
{
  if (get_size() < 3) {
    // just point or line case
    return CC_LIMIT_POINTS_ONLY;
  }

  VectorPoints::const_iterator itr_start = m_stack.begin();
  VectorPoints::const_iterator itr_end = ++m_stack.begin();
  VectorPoints::const_iterator itr_third = ++(++m_stack.begin());
  eVectorsTurn e_first_turn = VT_NONE;
  while (e_first_turn == VT_NONE) {
    e_first_turn = determine_turn(
      *(itr_start->get()), 
      *(itr_end->get()),
      *(itr_third->get())
    );
    if (++itr_third == m_stack.end()) {
      // wow, the figure consists of limit points only
      return CC_LIMIT_POINTS_ONLY;
    }
    ++itr_start;
    ++itr_end;
  }
  
  for (itr_start; itr_start!= m_stack.end(); ++itr_start) {

    eVectorsTurn e_turn = determine_turn(
      *(itr_start->get()), 
      *(itr_end->get()),
      *(itr_third->get())
    );
    if (e_turn != VT_NONE && e_first_turn != e_turn) {
      // turn direction has changed. not convex
      return CC_NOT_CONVEX;
    }
    
    if (++itr_third == m_stack.end()) {
      itr_third = m_stack.begin();
    }
    if (++itr_end == m_stack.end()) {
      itr_end = m_stack.begin();
      itr_third = ++m_stack.begin();
    }
  }

  return CC_CONVEX;
}

//=============================================================================