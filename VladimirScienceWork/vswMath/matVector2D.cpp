#include "matVector2D.h"

//=============================================================================
matVector2D::matVector2D(double a_d_x, double a_d_y)
  :
m_x(a_d_x),
m_y(a_d_y)
//
// lav 12/10/13 written.
//
{
}

//=============================================================================
matVector2D::matVector2D(matPoint2D a_pn)
  :
m_x(a_pn.X),
m_y(a_pn.Y)
//
// lav 12/10/13 written.
//
{
  
}

//=============================================================================
matVector2D::matVector2D(matPoint2D a_pn_first, matPoint2D a_pn_second)
//
// lav 12/10/13 written.
//
{
  m_x = a_pn_second.X - a_pn_first.X;
  m_y = a_pn_second.Y - a_pn_first.Y;
}

//=============================================================================
void matVector2D::become(double a_d_x, double a_d_y)
//
// lav 12/10/13 written.
//
{
  m_x = a_d_x;
  m_y = a_d_y;
}

//=============================================================================
double matVector2D::get_squared_norm() const
//
// lav 12/10/13 written.
//
{
  return scal_prod(*this);
}

//=============================================================================
double matVector2D::get_norm() const
//
// lav 12/10/13 written.
//
{
  return sqrt(get_squared_norm());
}

//=============================================================================
double matVector2D::scal_prod(const matVector2D& a_vc2) const
//
// lav 12/10/13 written.
//
{
  return m_x * a_vc2.m_x + m_y * a_vc2.m_y;
}

//=============================================================================
double matVector2D::cross_prod_z(const matVector2D& a_vc2) const
//
// lav 12/10/13 written.
//
{
  return m_x * a_vc2.m_y - m_y * a_vc2.m_x;
}

//=============================================================================
const matVector2D matVector2D::get_normalized_vector() const
//
// lav 12/10/13 written.
//
{
  double d_norm = get_norm();

  if (IS_ZERO(d_norm)) {
    // to be careful take a look if this is happening
    _ASSERT(0); 
    return matVector2D(0.0, 0.0);
  }
  if (IS_ZERO(d_norm - 1)) {
    return *this;
  }
  return matVector2D(
    m_x / d_norm,
    m_y / d_norm
  );
}

//=============================================================================
bool matVector2D::is_orthogonal(const matVector2D& a_vc2) const
//
// lav 12/10/13 written.
//
{
  return IS_ZERO(scal_prod(a_vc2));
}

//=============================================================================
bool matVector2D::is_colinear(const matVector2D& a_vc2) const
//
// lav 12/10/13 written.
//
{
  return IS_ZERO(cross_prod_z(a_vc2));
}

//=============================================================================
bool matVector2D::is_parallel(const matVector2D& a_vc2) const
//
// lav 12/10/13 written.
//
{
  return is_colinear(a_vc2) && scal_prod(a_vc2) > 0;
}

//=============================================================================
bool matVector2D::is_antiparallel(const matVector2D& a_vc2) const
//
// lav 12/10/13 written.
//
{
  return is_colinear(a_vc2) && scal_prod(a_vc2) < 0;
}

//=============================================================================
bool matVector2D::is_zero_vector() const
//
// lav 12/10/13 written.
//
{
  return IS_ZERO(get_norm());
}

//=============================================================================
void matVector2D::normalize()
//
// lav 12/10/13 written.
//
{
  *this = get_normalized_vector();
}

//=============================================================================
const matPoint2D matVector2D::get_point() const 
//
// lav 12/10/13 written.
//
{
  matPoint2D pn2;
  pn2.X = m_x;
  pn2.Y = m_y;

  return pn2;
}

//=============================================================================
const matVector2D matVector2D::operator+(const matVector2D& a_vc2) const
//
// lav 12/10/13 written.
//
{
  return matVector2D(m_x + a_vc2.m_x, m_y + a_vc2.m_y);
}

//=============================================================================
const matVector2D matVector2D::operator-(const matVector2D& a_vc2) const
//
// lav 12/10/13 written.
//
{
  return matVector2D(m_x - a_vc2.m_x, m_y - a_vc2.m_y);
}
//=============================================================================