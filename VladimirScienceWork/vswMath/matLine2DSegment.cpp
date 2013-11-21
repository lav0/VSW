#include "matLine2DSegment.h"

//=============================================================================
matLine2DSegment::matLine2DSegment(matPoint2D a_pn1, matPoint2D a_pn2)
  : 
matLine2D(a_pn1, a_pn2)
//
// lav 15/10/13 written.
//
{
  _ASSERT(is_defined());
  m_shp_point1 = shared_ptr<matPoint2D>(new matPoint2D(a_pn1));
  m_shp_point2 = shared_ptr<matPoint2D>(new matPoint2D(a_pn2));

  define_pointers();
}

//=============================================================================
matLine2DSegment::matLine2DSegment(
  shared_ptr<matPoint2D> a_shp_pn1, 
  shared_ptr<matPoint2D> a_shp_pn2
) : 
matLine2D(*a_shp_pn1, *a_shp_pn2)
//
// lav 06/11/13 written.
//
{
  _ASSERT(is_defined());
  m_shp_point1 = a_shp_pn1;
  m_shp_point2 = a_shp_pn2;

  define_pointers();
}

//=============================================================================
matLine2DSegment::~matLine2DSegment()
//
// lav 15/10/13 written.
//
{
}

//=============================================================================
bool matLine2DSegment::assign(matPoint2D a_pn1, matPoint2D a_pn2)
//
// lav 23/10/13 written.
//
{
  if (!define_by_two_points(a_pn1, a_pn2)) {
    return false;
  }

  define_pointers();

  return true;
}

//=============================================================================
void matLine2DSegment::define_pointers()
//
// lav 15/10/13 written.
//
{
  if (m_shp_point1->X <= m_shp_point2->X) {
    m_wkp_left = boost::weak_ptr<matPoint2D>(m_shp_point1);
    m_wkp_right = boost::weak_ptr<matPoint2D>(m_shp_point2);
  } else {
    m_wkp_left = boost::weak_ptr<matPoint2D>(m_shp_point2);
    m_wkp_right = boost::weak_ptr<matPoint2D>(m_shp_point1);
  }
  if (m_shp_point1->Y <= m_shp_point2->Y) {
    m_wkp_bottom = boost::weak_ptr<matPoint2D>(m_shp_point1);
    m_wkp_top = boost::weak_ptr<matPoint2D>(m_shp_point2);
  } else {
    m_wkp_bottom = boost::weak_ptr<matPoint2D>(m_shp_point2);
    m_wkp_top = boost::weak_ptr<matPoint2D>(m_shp_point1);
  }
}

//=============================================================================
const matPoint2D* matLine2DSegment::get_first_pointer() const
//
// lav 08/11/13 written.
//
{
  return m_shp_point1.get();
}

//=============================================================================
const matPoint2D* matLine2DSegment::get_second_pointer() const
//
// lav 08/11/13 written.
//
{
  return m_shp_point2.get();
}

//=============================================================================
const boost::shared_ptr<matPoint2D> matLine2DSegment::get_pointer(
  ePointerPlace a_e_place
) const
//
// lav 15/10/13 written.
//
{
  static boost::shared_ptr<matPoint2D> s_shp_null;

  switch(a_e_place) {
    case PP_LEFT:
      _ASSERT_NULL_POINTER(m_wkp_left.lock());
      return m_wkp_left.lock();
    case PP_RIGHT:
      _ASSERT_NULL_POINTER(m_wkp_right.lock());
      return m_wkp_right.lock();
    case PP_BOTTOM:
      _ASSERT_NULL_POINTER(m_wkp_bottom.lock());
      return m_wkp_bottom.lock();
    case PP_TOP:
      _ASSERT_NULL_POINTER(m_wkp_top.lock());
      return m_wkp_top.lock();
  }
  _ASSERT(0);
  return s_shp_null;
}

//=============================================================================
matPoint2D matLine2DSegment::get_segment_point(
  double a_d_lambda, 
  bool b_leftright
) const
//
// Inner method! Gives point from LEFT to RIGHT if (b_leftright)
// otherwise point from  BOTTOM to TOP
//
///////////////////////////////////////////////////////////////////////////////
//
// lav 16/10/13 written.
//
{
  _ASSERT(abs(a_d_lambda-0.5) <= 0.5);

  matPoint2D pn;
  ePointerPlace e_pp_first;
  ePointerPlace e_pp_second;

  if (b_leftright) {
    e_pp_first = PP_LEFT;
    e_pp_second = PP_RIGHT;
  } else {
    e_pp_first = PP_BOTTOM;
    e_pp_second = PP_TOP;
  }

  pn.X = get_pointer(e_pp_first)->X * (1-a_d_lambda) + 
    get_pointer(e_pp_second)->X * a_d_lambda;
  pn.Y = get_pointer(e_pp_first)->Y * (1-a_d_lambda) + 
    get_pointer(e_pp_second)->Y * a_d_lambda;

  return pn;
}

//=============================================================================
const matPoint2D matLine2DSegment::get_point_by_lambda(double a_d_lam) const
//
// lav 23/10/13 written.
//
{
  _ASSERT(abs(a_d_lam-0.5) <= 0.5);

  matPoint2D pn;

  pn.X = m_shp_point1->X * (1-a_d_lam) + m_shp_point2->X * a_d_lam;
  pn.Y = m_shp_point1->Y * (1-a_d_lam) + m_shp_point2->Y * a_d_lam;

  return pn;
}

//=============================================================================
bool matLine2DSegment::is_point_within(const matPoint2D& a_pn) const
//
// lav 15/10/13 written.
//
{
  if (!is_point_on(a_pn)) {
    return false;
  }
  
  double d_lam = _POINTS_COMPARISON;

  _ASSERT(!are_points_equal(*(get_pointer(PP_BOTTOM)), *(get_pointer(PP_TOP)), d_lam) ||
    !are_points_equal(*(get_pointer(PP_LEFT)), *(get_pointer(PP_RIGHT)), d_lam));

  double d_x_min = get_segment_point(d_lam, true).X;
  double d_x_max = get_segment_point(1-d_lam, true).X;
  double d_y_min = get_segment_point(d_lam, false).Y;
  double d_y_max = get_segment_point(1-d_lam, false).Y;

  return d_x_min <= a_pn.X && d_x_max >= a_pn.X &&
    d_y_min <= a_pn.Y && d_y_max >= a_pn.Y;
}

//=============================================================================
bool matLine2DSegment::is_equal_segment(const matLine2DSegment& a_sg) const
//
// lav 15/10/13 written.
//
{
  return is_equal(a_sg) && (
    (IS_ZERO(get_pointer(PP_LEFT)->X - a_sg.get_pointer(PP_LEFT)->X) &&
    IS_ZERO(get_pointer(PP_LEFT)->Y - a_sg.get_pointer(PP_LEFT)->Y) &&
    IS_ZERO(get_pointer(PP_RIGHT)->X - a_sg.get_pointer(PP_RIGHT)->X) &&
    IS_ZERO(get_pointer(PP_RIGHT)->Y - a_sg.get_pointer(PP_RIGHT)->Y)) ||
    (IS_ZERO(get_pointer(PP_BOTTOM)->X - a_sg.get_pointer(PP_BOTTOM)->X) &&
    IS_ZERO(get_pointer(PP_BOTTOM)->Y - a_sg.get_pointer(PP_BOTTOM)->Y) &&
    IS_ZERO(get_pointer(PP_TOP)->X - a_sg.get_pointer(PP_TOP)->X) &&
    IS_ZERO(get_pointer(PP_TOP)->Y - a_sg.get_pointer(PP_TOP)->Y))
  );
}

//=============================================================================
double matLine2DSegment::get_length() const
//
// lav 15/10/13 written.
//
{
  _ASSERT(is_defined());

  matVector2D vc2(*m_shp_point1.get(), *m_shp_point2.get());
  return vc2.get_norm();
}

//=============================================================================
const matVector2D matLine2DSegment::get_start_end_vector() const
//
// lav 15/10/13 written.
//
{
  _ASSERT(is_defined());

  return matVector2D(*m_shp_point1.get(), *m_shp_point2.get());
}

//=============================================================================
const matVector2D matLine2DSegment::get_end_start_vector() const
//
// lav 15/10/13 written.
//
{
  _ASSERT(is_defined());

  return get_start_end_vector().get_reverted();
}

//=============================================================================
matLine2DSegment::ePointOnEdgeResult matLine2DSegment::is_point_on_edge(
  const matPoint2D& a_pn
) const
//
// lav 15/10/13 written.
//
{
  if (is_point_on(a_pn)) {
    
    if (are_points_equal(*m_shp_point1.get(), a_pn, _POINTS_COMPARISON)) {

      return POER_FIRST;

    } else if (are_points_equal(*m_shp_point2.get(), a_pn, _POINTS_COMPARISON)) {

      return POER_SECOND;

    }
  }
  return POER_NONE;
}

//=============================================================================
bool matLine2DSegment::is_crossed(const matLine2DSegment& a_sg) const
//
// lav 16/10/13 written.
//
{
  boost::shared_ptr<matPoint2D> shp_intr(new matPoint2D);
  if (!get_intersection(a_sg, shp_intr)) {
    return false;
  }

  bool b_x = get_pointer(PP_LEFT)->X <= shp_intr->X && 
    get_pointer(PP_RIGHT)->X >= shp_intr->X && a_sg.get_pointer(PP_LEFT)->X <= shp_intr->X &&
    a_sg.get_pointer(PP_RIGHT)->X >= shp_intr->X;

  bool b_y = get_pointer(PP_BOTTOM)->Y <= shp_intr->Y && 
    get_pointer(PP_TOP)->Y >= shp_intr->Y && a_sg.get_pointer(PP_BOTTOM)->Y <= shp_intr->Y &&
    a_sg.get_pointer(PP_TOP)->Y >= shp_intr->Y;
  
  return b_x && b_y;
}

//=============================================================================
eIntersectionPlace matLine2DSegment::recognize_intersection(
  const matLine2DSegment& a_sg, 
  boost::shared_ptr<matPoint2D>& a_shp_output
) const
//
// lav 15/10/13 written.
// lav 16/10/13 extended.
//
{
  _ASSERT(is_defined());

  if (!is_equal(a_sg)) {
    if (is_parallel(a_sg)) {
      // no intersection
      return IP_NONE;
    }

    boost::shared_ptr<matPoint2D> shp_intr(new matPoint2D);
    if (get_intersection(a_sg, shp_intr)) {
      _ASSERT_NULL_POINTER(shp_intr);
    } else {
      _ASSERT(0);
      return IP_ERROR;
    }

    if (a_sg.is_point_within(*shp_intr.get())) {
      
      a_shp_output = shp_intr;
      if (is_point_within(*shp_intr.get())) {
        // intersection belongs to both segments
        return IP_BOTH;
      }
      // intersection is in the second segment
      return IP_MIDDLE;

    } else if (a_sg.is_point_on_edge(*shp_intr.get()) == ePointOnEdgeResult::POER_FIRST) {

      a_shp_output = a_sg.m_shp_point1;
      // intersection is on the edge of the second segment
      return IP_EDGE;

    } else if (a_sg.is_point_on_edge(*shp_intr.get()) == ePointOnEdgeResult::POER_SECOND) {

      a_shp_output = a_sg.m_shp_point2;
      return IP_EDGE;

    } else {
      return IP_NONE;
    }
  }

  if (is_equal_segment(a_sg)) {
    a_shp_output = m_shp_point1;
    return IP_SAME;
  }
  
  if (get_pointer(PP_RIGHT)->X < a_sg.get_pointer(PP_LEFT)->X) {
    a_shp_output = a_sg.get_pointer(PP_LEFT);
  } else if (get_pointer(PP_LEFT)->X > a_sg.get_pointer(PP_RIGHT)->X) {
    a_shp_output = a_sg.get_pointer(PP_RIGHT);
  } else {
    if (get_pointer(PP_TOP)->Y < a_sg.get_pointer(PP_BOTTOM)->Y) {
      a_shp_output = a_sg.get_pointer(PP_BOTTOM);
    } else if (get_pointer(PP_BOTTOM)->Y > a_sg.get_pointer(PP_TOP)->Y) {
      a_shp_output = a_sg.get_pointer(PP_TOP);
    } else {
      _ASSERT(0);
      return IP_ERROR;
    }
  }
  // segments belong to the same line
  return IP_FULL;
}

//=============================================================================