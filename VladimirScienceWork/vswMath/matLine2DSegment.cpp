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
  m_shp_point1 = boost::shared_ptr<matPoint2D>(new matPoint2D(a_pn1));
  m_shp_point2 = boost::shared_ptr<matPoint2D>(new matPoint2D(a_pn2));

  define_pointers();
}

//=============================================================================
matLine2DSegment::matLine2DSegment(
  matLine2D a_ln, 
  double a_d_val1, 
  double a_d_val2, 
  bool a_b
) 
//
// lav 15/10/13 written.
//
{
  matPoint2D pn1, pn2;
  if (a_b) {
    pn1.X = a_d_val1;
    pn2.X = a_d_val2;

    set_defined(get_y_by_x(a_d_val1, pn1.Y));
    set_defined(get_y_by_x(a_d_val2, pn2.Y));
  } else {
    pn1.Y = a_d_val1;
    pn2.Y = a_d_val2;

    set_defined(get_x_by_y(a_d_val1, pn1.X));
    set_defined(get_x_by_y(a_d_val2, pn2.X));
  }

  m_shp_point1 = boost::shared_ptr<matPoint2D>(new matPoint2D(pn1));
  m_shp_point2 = boost::shared_ptr<matPoint2D>(new matPoint2D(pn2));

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
bool matLine2DSegment::is_point_within(const matPoint2D& a_pn) const
//
// lav 15/10/13 written.
//
{
  return is_point_on(a_pn) ?
    (a_pn.X >= get_pointer(PP_LEFT)->X && 
    a_pn.X <= get_pointer(PP_RIGHT)->X) &&
    (a_pn.Y >= get_pointer(PP_BOTTOM)->Y && 
    a_pn.Y <= get_pointer(PP_TOP)->Y) :
    false;
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
ePointOnEdgeResult matLine2DSegment::is_point_on_edge(const matPoint2D& a_pn) const
//
// lav 15/10/13 written.
//
{
  if (is_point_within(a_pn)) {

    bool b_result = true;

    if (are_points_equal(*m_shp_point1.get(), a_pn, _ZERO)) {

      return POER_FIRST;

    } else if (are_points_equal(*m_shp_point2.get(), a_pn, _ZERO)) {

      return POER_SECOND;

    }
  }
  return POER_NONE;
}

//=============================================================================
eIntersectionPlace matLine2DSegment::recognize_intersection(
  const matLine2DSegment& a_sg, boost::shared_ptr<matPoint2D> a_shp_output
) const
//
// lav 15/10/13 written.
//
{
  _ASSERT(is_defined());

  if (!is_equal(a_sg)) {
    if (is_parallel(a_sg)) {
      (*a_shp_output).X = 0;
      (*a_shp_output).Y = 0;
      return IP_NONE;
    }

    boost::shared_ptr<matPoint2D> shp_intr;
    if (get_intersection(a_sg, shp_intr)) {
      _ASSERT_NULL_POINTER(shp_intr);
    } else {
      _ASSERT(0);
      return IP_ERROR;
    }

    if (!is_point_within(*shp_intr.get())) {
      return IP_NONE;
    } else {
      if (is_point_on_edge(*shp_intr.get()) == ePointOnEdgeResult::POER_FIRST) {

        a_shp_output = m_shp_point1;
        return IP_EDGE;

      } else if (is_point_on_edge(*shp_intr.get()) == ePointOnEdgeResult::POER_SECOND) {

        a_shp_output = m_shp_point2;
        return IP_EDGE;

      } else {

        a_shp_output = shp_intr;
        return IP_MIDDLE;
      }
    }
  }

  // Pick any point on the segment
  a_shp_output = m_shp_point1;
  return IP_FULL;
}

//=============================================================================