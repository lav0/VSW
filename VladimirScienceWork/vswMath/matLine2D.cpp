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
  m_d_defined = !m_vc2.is_zero_vector();
}

//=============================================================================
matLine2D::matLine2D(matPoint2D a_pn1, matPoint2D a_pn2)
//
// lav 14/10/13 written.
//
{
  m_d_defined = define_by_two_points(a_pn1, a_pn2);
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
  if (!m_d_defined || m_vc2.is_colinear(matVector2D(0.0, 1.0))) {
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
  if (!m_d_defined || m_vc2.is_colinear(matVector2D(1.0, 0.0))) {
    return false;
  }

  a_d_y_out = (m_p - m_vc2.get_point().X * a_d_x) / m_vc2.get_point().Y;

  return true;
}

//=============================================================================