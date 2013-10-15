#include "matLine2D.h"

//=============================================================================
matLine2D::matLine2D(matVector2DUnit a_vc2u, double a_d_p)
  :
m_vc2(a_vc2u),
m_p(a_d_p)
//
// lav 14/10/13 written.
//
{
  if (a_d_p < 0) {
    // p must be greater than zero
    _ASSERT(0);
    a_d_p = -a_d_p;
    m_vc2 = m_vc2.get_reverted();
  }
  m_b_defined = !m_vc2.is_zero_vector();
}

//=============================================================================
matLine2D::matLine2D(
  double a_d_x1, double a_d_y1, double a_d_x2, double a_d_y2
)
//
// lav 14/10/13 written.
//
{
  matPoint2D pn1, pn2;
  pn1.X = a_d_x1;
  pn1.Y = a_d_y1;
  pn2.X = a_d_x2;
  pn2.Y = a_d_y2;

  *this = matLine2D(pn1, pn2);
}

//=============================================================================
matLine2D::matLine2D(matPoint2D a_pn1, matPoint2D a_pn2)
//
// lav 14/10/13 written.
//
{
  m_b_defined = define_by_two_points(a_pn1, a_pn2);
}

//=============================================================================
matLine2D::~matLine2D()
//
// lav 14/10/13 written.
//
{
   
}
//=============================================================================
bool matLine2D::define_by_common_equation(
  double a_x_coef, 
  double a_y_coef, 
  double a_free_coef
) 
//
// Having A*x + B*y + C = 0 equation we convert it to normal form shown below:
//   <V, X> = p  // (V - unit vector, X = (x,y) vector)
//
///////////////////////////////////////////////////////////////////////////////
//
// lav 14/10/13 written.
//
{  
  _ASSERT(m_b_defined);

  int i_sign = (a_free_coef < 0) ? 1 : -1;

  try {

    double d_norm = sqrt(a_x_coef*a_x_coef + a_y_coef*a_y_coef);

    if (IS_ZERO(d_norm)) throw std::overflow_error("BAM");

    m_p = (-i_sign) * a_free_coef / d_norm;
    m_vc2.become(i_sign*a_x_coef, i_sign*a_y_coef);

  } catch(std::overflow_error exp) {
    return false;
  }

  return true;
}

//=============================================================================
bool matLine2D::define_by_two_points(
  const matPoint2D& a_pn1, 
  const matPoint2D& a_pn2
)
//
// Make up a line going through the points determining its common equation
//
///////////////////////////////////////////////////////////////////////////////
//
// lav 14/10/13 written.
//
{
  return define_by_common_equation(
    a_pn1.Y - a_pn2.Y,
    a_pn2.X - a_pn1.X,
    a_pn1.X * a_pn2.Y - a_pn2.X * a_pn1.Y
  );
}

//=============================================================================
bool matLine2D::get_x_by_y(const double a_d_y, double& a_d_x_out) const
//
// lav 14/10/13 written.
//
{
  if (!m_b_defined || m_vc2.is_colinear(matVector2D(0.0, 1.0))) {
    return false;
  }

  a_d_x_out = (m_p - m_vc2.get_point().Y * a_d_y) / m_vc2.get_point().X;
  
  return true;
}

//=============================================================================
bool matLine2D::get_y_by_x(const double a_d_x, double& a_d_y_out) const
//
// lav 14/10/13 written.
//
{
  if (!m_b_defined || m_vc2.is_colinear(matVector2D(1.0, 0.0))) {
    return false;
  }

  a_d_y_out = (m_p - m_vc2.get_point().X * a_d_x) / m_vc2.get_point().Y;

  return true;
}

//=============================================================================
bool matLine2D::is_point_on(const matPoint2D& a_pn, double a_d_tolerance) const
//
// lav 14/10/13 written.
//
{
  _ASSERT(m_b_defined);

  return abs(m_vc2.scal_prod(a_pn) - m_p) < a_d_tolerance;
}

//=============================================================================
bool matLine2D::is_equal(const matLine2D& a_ln) const
//
// lav 14/10/13 written.
//
{
  _ASSERT(m_b_defined);
  _ASSERT(m_p >= 0);
  _ASSERT(a_ln.m_p >= 0);
  
  if (IS_ZERO(m_p)) {
    // Pitfall situation. Lines might be equal having
    // their definition's vectors being antiparallel in case
    // of offset (m_p) being zero
    return IS_ZERO(a_ln.m_p) && is_parallel(a_ln);
  }

  return m_vc2.is_parallel(a_ln.m_vc2) && IS_ZERO(m_p - a_ln.m_p);
}

//=============================================================================
bool matLine2D::is_parallel(const matLine2D& a_ln) const
//
// lav 14/10/13 written.
//
{
  _ASSERT(m_b_defined);

  return m_vc2.is_colinear(a_ln.m_vc2);
}

//=============================================================================
bool matLine2D::is_orthogonal(const matLine2D& a_ln) const
//
// lav 14/10/13 written.
//
{
  _ASSERT(m_b_defined);

  return m_vc2.is_orthogonal(a_ln.m_vc2);
}

//=============================================================================
bool matLine2D::get_intersection(
  const matLine2D& a_ln, 
  boost::shared_ptr<matPoint2D> shp_point
) const
//
// Using Kramer's method
//
///////////////////////////////////////////////////////////////////////////////
//
// lav 14/10/13 written.
//
{
  _ASSERT(m_b_defined && a_ln.is_defined());
  if (is_parallel(a_ln)) {
    shp_point->X = 0;
    shp_point->Y = 0;
    return false;
  }

  double d_det_x = m_p*a_ln.m_vc2.get_point().Y - a_ln.m_p*m_vc2.get_point().Y;
  double d_det_y = a_ln.m_p*m_vc2.get_point().X - m_p*a_ln.m_vc2.get_point().X;
  double d_delta = m_vc2.cross_prod_z(a_ln.m_vc2);

  // to prevent a warning use (b_value == false) instead of (!b_value)
  _ASSERT(IS_ZERO(d_delta) == false);

  shp_point->X = d_det_x / d_delta;
  shp_point->Y = d_det_y / d_delta;

  return true;
}

//=============================================================================
bool matLine2D::is_defined() const
//
// lav 14/10/13 written.
//
{
  return m_b_defined;
}

//=============================================================================
void matLine2D::set_defined(bool a_b_value)
//
// lav 14/10/13 written.
//
{
  m_b_defined = a_b_value;
}

//=============================================================================