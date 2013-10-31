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
bool grdFigure::separate_to_member(
  const matPoint2D* a_p_ray_p2,
  const matPoint2D* a_p_pierced_p2,
  const matPoint2D& a_pn_separator
)
//
// lav 29/10/13 written.
// lav 31/10/13 refactored.
//
{
  grdFigure fig0;
  grdFigure fig1;

  bool b_come_in_first = true;
  size_t i_warn = 0;
  VectorPoints::iterator itr;
  if (!m_points.get_itr_by_content(a_p_ray_p2, itr)) {
    _ASSERT(0);
    return false;
  }
  _ASSERT(itr->get() == a_p_ray_p2);

  do {
    if (i_warn > m_points.get_size()) {
      return false;
    }
    if (itr->get() == a_p_ray_p2) {
      _ASSERT(b_come_in_first);
      b_come_in_first = false;
    }
    if (itr->get() == a_p_pierced_p2) {
      _ASSERT(!b_come_in_first);
      shared_ptr<matPoint2D> shp_tmp(new matPoint2D(a_pn_separator));
      fig1.push_back(shp_tmp);
      fig0.push_back(shp_tmp);
      b_come_in_first = true;
    }

    (b_come_in_first ? fig0 : fig1).push_back(*itr);

    ++i_warn;
    if (++itr == m_points.get_stack_end_not_const()) {
      itr = m_points.get_stack_begin_not_const(); 
    }
  } while (itr->get() != a_p_ray_p2);

  m_subfig_list.push_back(shared_ptr<grdFigure>(new grdFigure(fig0)));
  m_subfig_list.push_back(shared_ptr<grdFigure>(new grdFigure(fig1)));

  return true;
}

//=============================================================================
bool grdFigure::split()
//
// lav 29/10/13 written.
//
{
  if (m_points.get_size() <= 3) {
    // nothing to split
    return false;
  }
  bool b_is_counter_clockwise = m_points.detect_rotation_direction();

  matPoint2D* p_point0;
  matPoint2D* p_point1;
  matPoint2D* p_point2;
  for (int i=0; i<m_points.get_size(); i++) {
    if (i==0) { 
      p_point0 = m_points.get_point_by_ind(i).get();
      continue;
    } else if (i==1) {
      p_point1 = m_points.get_point_by_ind(i).get();
      continue;
    }
    else if (i==2) {p_point2 = m_points.get_point_by_ind(i).get(); }
   
    matVector2D vc0(*p_point0, *p_point1);
    matVector2D vc1(*p_point1, *p_point2);
    if ((vc0.cross_prod_z(vc1) > 0) != b_is_counter_clockwise) {
      int i_pierced = -1;
      shared_ptr<matPoint2D> shp_separator(new matPoint2D);
      if (find_further_intersection(
        matLine2DSegment(*p_point0, *p_point1),
        i,
        shp_separator,
        i_pierced
      )) {
        return separate_to_member(
          p_point1,
          m_points.get_point_by_ind(i_pierced).get(),
          *shp_separator.get()
        );
      }
    }

    p_point0 = p_point1;
    p_point1 = p_point2;
    p_point2 = m_points.get_point_by_ind(
      (i + 1 == m_points.get_size()) ? 0 : i+1
    ).get();
  }
  _ASSERT(m_points.is_convex());
  return false;
}

//=============================================================================
bool grdFigure::find_further_intersection(
  const matLine2DSegment& a_sg_ray,
  int a_i_start_from,
  shared_ptr<matPoint2D>& a_shp_out,
  int& a_i_pierced_segment_out
) const
//
// lav 29/10/13 written.
//
{
  if (m_points.get_size() <= a_i_start_from) {
    return true;
  }
  
  int i_end=-1;
  matPoint2D* p_point0;
  matPoint2D* p_point1;
  for (int i=a_i_start_from; i<m_points.get_size(); i++) {
    if (i+1 >= m_points.get_size()) {
      i_end = 0;
    } else {
      i_end = i+1;
    }
    p_point0 = m_points.get_point_by_ind(i).get();
    p_point1 = m_points.get_point_by_ind(i_end).get();
    matLine2DSegment segment(*p_point0, *p_point1);
    if (a_sg_ray.recognize_intersection(segment, a_shp_out) == IP_MIDDLE) {
      a_i_pierced_segment_out = i_end;
      return true;
    }
  }

  return false;
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
  } else if (e_convexity == CC_BORDER_POINTS_ONLY) {
    bool b_singular_pushed = singular_to_member();

    _ASSERT(b_singular_pushed);
  } else {
    _ASSERT(e_convexity == CC_NOT_CONVEX);
    split();
  }

  return average(m_subfig_list, a_result);
}

//=============================================================================