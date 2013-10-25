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
eConvexityCase grdFigure::check_for_convexity() const
//
// lav 23/10/13 written.
//
{
  _ASSERT(get_size() > 0);

  return m_points.is_convex();
}

//=============================================================================
const std::vector<shared_ptr<IGirder>>& grdFigure::get_sub_list() const
//
// lav 24/10/13 written.
//
{ 
  return m_subfig_list; 
}

//=============================================================================
bool grdFigure::singular_to_member()
//
// lav 25/10/13 written.
//
{
  shared_ptr<grdSingularFigure> shp_singular_figure(
    new grdSingularFigure(m_points)
  );

  if (shp_singular_figure->is_valid()) {
    m_subfig_list.push_back(shp_singular_figure);
    return true;
  }

  return false;
}

//=============================================================================
int grdFigure::triangulate_to_member()
//
// lav 24/10/13 written.
//
{
  std::vector<grdTriangle> triangle_list;
  if (triangulate(triangle_list) == -1) {
    return -1;
  }

  m_subfig_list.clear();
  std::vector<grdTriangle>::const_iterator itr = triangle_list.begin();
  while (itr!= triangle_list.end())  {
    m_subfig_list.push_back(shared_ptr<grdTriangle>(new grdTriangle(*itr)));
    ++itr;
  } 

  return m_subfig_list.size();
}

//=============================================================================
int grdFigure::triangulate(
  std::vector<grdTriangle>& a_triangle_array_out
) const
//
// lav 23/10/13 written.
//
{
  if (check_for_convexity() != CC_CONVEX) {
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
    if (!(--(a_triangle_array_out.end()))->is_valid()) {
      a_triangle_array_out.pop_back();
    }
    ++itr1;
    ++itr2;
  }
  
  return (int) a_triangle_array_out.size();
}

//=============================================================================
bool handle_all_limits_point_case()
//
// lav 25/10/13 written.
//
{
  return true;
}

//=============================================================================
bool grdFigure::calculate_gravity_centre_and_weight(Girder& a_result)
//
// lav 24/10/13 written.
// lav 25/10/13 extended.
//
{
  eConvexityCase e_convexity = check_for_convexity();
  if (e_convexity == CC_CONVEX) {
    int i_triangle_num = triangulate_to_member();

    _ASSERT(i_triangle_num != -1);
  } else if (e_convexity == CC_LIMIT_POINTS_ONLY) {
    bool b_singular_pushed = singular_to_member();

    _ASSERT(b_singular_pushed);
  } else {
    _ASSERT(e_convexity == CC_NOT_CONVEX);
    // not supported yet
  }

  return average(m_subfig_list, a_result);
}

//=============================================================================