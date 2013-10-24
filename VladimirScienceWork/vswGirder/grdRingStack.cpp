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
bool grdRingStack::is_right_turn(
  const matPoint2D& a_pn1,
  const matPoint2D& a_pn2,
  const matPoint2D& a_pn3
) const
//
// lav 22/10/13 written.
//
{
  return matVector2D(a_pn1, a_pn2).cross_prod_z(
    matVector2D(a_pn2, a_pn3)) >= 0;
}

//=============================================================================
bool grdRingStack::is_convex() const
//
// lav 20/10/13 written.
//
{
  if (get_size() < 4) {
    return true;
  }

  VectorPoints::const_iterator itr_start = m_stack.begin();
  VectorPoints::const_iterator itr_end = ++m_stack.begin();
  VectorPoints::const_iterator itr_third = itr_end;
  bool b_cross_prod_sign = is_right_turn(
    *(*itr_start++).get(), 
    *(*itr_end).get(),
    *(*(++itr_third)).get()
  );
  
  for (itr_start; itr_start!= m_stack.end(); ++itr_start) {

    if (++itr_third == m_stack.end()) {
      itr_third = m_stack.begin();
    }
    if (++itr_end == m_stack.end()) {
      itr_end = m_stack.begin();
      itr_third = ++m_stack.begin();
    }

    if (b_cross_prod_sign != is_right_turn(
      *(*itr_start).get(), 
      *(*itr_end).get(),
      *(*(itr_third)).get()
    )) {

      return false;
    }
  }

  return true;
}

//=============================================================================