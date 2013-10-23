#include "stdafx.h"
#include "grdFigure.h"

//=============================================================================
grdFigure::grdFigure(grdRingStack a_stack)
//
// lav 23/10/13 written.
//
{
  m_points = a_stack;
}

//=============================================================================
void grdFigure::push_back(shared_ptr<matPoint2D>& a_shp_point)
//
// lav 23/10/13 written.
//
{
  m_points.push_back(a_shp_point);
}

//=============================================================================
int grdFigure::get_size() const
//
// lav 23/10/13 written.
//
{
  return m_points.get_size();
}

//=============================================================================
bool grdFigure::check_for_convexity() const
//
// lav 23/10/13 written.
//
{
  _ASSERT(get_size() > 0);

  return m_points.is_convex();
}

//=============================================================================
int grdFigure::triangulate(
  std::vector<grdTriangle>& a_triangle_array_out
) const
//
// lav 23/10/13 written.
//
{
  if (!check_for_convexity() || get_size() < 3) {
    // Cannot triangulate yet.
    return -1;
  }
  a_triangle_array_out.clear();

  VectorPoints::const_iterator itr1 = ++m_points.get_stack_begin();
  VectorPoints::const_iterator itr2 = ++(++m_points.get_stack_begin());
  while (itr2 != m_points.get_stack_end()) {
    a_triangle_array_out.push_back(
      grdTriangle(
        m_points.get_point_by_ind(0),
        *itr1,
        *itr2
      )
    );
    ++itr1;
    ++itr2;
  }
  
  return (int) a_triangle_array_out.size();
}

//=============================================================================