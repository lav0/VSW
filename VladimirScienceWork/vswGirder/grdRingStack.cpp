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
shared_ptr<matPoint2D> grdRingStack::get_point_by_ind(size_t a_ind) const
//
// lav 20/10/13 written.
// lav 01/11/13 assertion condition strengthened.
//
{
  _ASSERT(a_ind < get_size() && a_ind >= 0);

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
    return CC_BORDER_POINTS_ONLY;
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
      // the figure consists of border points only if there were not any turns
      return e_first_turn == VT_NONE ? CC_BORDER_POINTS_ONLY : CC_CONVEX;
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
bool grdRingStack::detect_rotation_direction() const
//
// lav 29/10/13 written.
//
{
  if (m_stack.size() < 3) {
    return false;
  }
  size_t i_ind0 = 0;
  size_t i_ind1 = 0;
  size_t i_ind2 = 1;
  double d_angle_value = 0;
  for (i_ind0; i_ind0 < m_stack.size(); i_ind0++) {
    if (++i_ind1 >= m_stack.size()) {
      i_ind1 = 0;
    }
    if (++i_ind2 >= m_stack.size()) {
      i_ind2 = 0;
    }
    d_angle_value += matVector2D(*m_stack[i_ind0], *m_stack[i_ind1]).angle(
      matVector2D(*m_stack[i_ind1], *m_stack[i_ind2])
    );
  }
  _ASSERT(IS_ZERO(fmod(abs(d_angle_value), 2*PI)));
  _ASSERT(IS_ZERO(d_angle_value) == false);

  return d_angle_value > 0;
}

//=============================================================================
bool grdRingStack::get_itr_by_content(
  const matPoint2D* a_p_point,
  VectorPoints::iterator& a_itr_out
) 
//
// lav 31/10/13 written.
//
{
  a_itr_out = m_stack.begin();
  while (a_itr_out != m_stack.end()) {
    if (a_itr_out->get() == a_p_point) {
      return true;
    }
    ++a_itr_out;
  }
  _ASSERT(0);
  return false;
}

//=============================================================================
std::vector<matLine2DSegment> grdRingStack::get_segment_list() const
//
// lav 01/11/13 stated writing
// lav 06/11/13 ended.
//
{
  std::vector<matLine2DSegment> result_list;
  if (m_stack.size() == 0) {
    return result_list;
  }
  shared_ptr<matPoint2D> shp_point1 = *m_stack.begin();
  VectorPoints::const_iterator itr = m_stack.begin();
  while (++itr != m_stack.end()) {

    shared_ptr<matPoint2D> shp_point2 = *itr;
    result_list.push_back(matLine2DSegment(shp_point1, shp_point2));

    shp_point1 = shp_point2;
  }

  result_list.push_back(matLine2DSegment(shp_point1, *m_stack.begin()));

  return result_list;
}

//=============================================================================
bool grdRingStack::is_valid() const
//
// lav 01/11/13 written.
//
{
  if (m_stack.size() == 0) {
    return false;
  }

  if (m_stack.size() <= 3) {
    return true;
  }

  // check for self-intersection
  std::vector<matLine2DSegment> segment_list = get_segment_list();
  std::vector<matLine2DSegment>::const_iterator itr0 = segment_list.begin();
  std::vector<matLine2DSegment>::const_iterator itr1 = ++segment_list.begin();
  while (itr0 != --segment_list.end()) {
    while (itr1 != segment_list.end()) {
      shared_ptr<matPoint2D> shp_useless;
      if (itr0->recognize_intersection(*itr1, shp_useless) == IP_BOTH) {
        return false;
      }
      ++itr1;
    }
    ++itr0;
    itr1 = itr0;
    ++itr1;
  }

  return true;
}

//=============================================================================
