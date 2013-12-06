#include "stdafx.h"
#include "grdSingularFigure.h"


//=============================================================================
grdSingularFigure::grdSingularFigure(grdRingStack& a_stack)
//
// lav 25/10/13 written.
//
{
  m_points = a_stack;
}

//=============================================================================
bool grdSingularFigure::is_valid() const
//
// lav 25/10/13 written.
//
{
  return m_points.is_convex() == CC_BORDER_POINTS_ONLY &&
     m_points.get_size() > 0;
}

//=============================================================================
eMainCalculationResult grdSingularFigure::calculate_gravity_centre_and_weight(
  Girder& a_result
)
//
// lav 25/10/13 written.
// lav 22/11/13 return type changed.
//
{
  const int i_points_quantity = (int)m_points.get_size();
  if (i_points_quantity < 1) {
    return MCR_FAIL;
  } else if (i_points_quantity == 1) {
    a_result.m_weight = 0;
    a_result.m_gravity_centre = *m_points.get_point_by_ind(0);
    return MCR_SUCCEEDED;
  }
  
  a_result.m_gravity_centre = matPoint2D(0,0);
  VectorPoints::const_iterator itr = m_points.get_stack_begin();
  do {

    a_result.m_gravity_centre.X += (*itr)->X;
    a_result.m_gravity_centre.Y += (*itr)->Y;

  } while (++itr != m_points.get_stack_end());

  a_result.m_gravity_centre.X /= i_points_quantity;
  a_result.m_gravity_centre.Y /= i_points_quantity;
  a_result.m_weight = 0;

  return MCR_SUCCEEDED;
}

//=============================================================================