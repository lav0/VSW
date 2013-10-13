
#include "matVector2DUnit.h"

//=============================================================================
matVector2DUnit::matVector2DUnit(double a_d_x, double a_d_y)
  : matVector2D(a_d_x, a_d_y)
//
// lav 12/10/13 written.
//
{
  normalize();
}

//=============================================================================
matVector2DUnit::matVector2DUnit(matPoint2D a_pn) 
  : matVector2D(a_pn)
//
// lav 12/10/13 written.
//
{
  normalize();
}

//=============================================================================
matVector2DUnit::matVector2DUnit(matPoint2D a_pn_first, matPoint2D a_pn_second)
  : matVector2D(a_pn_first, a_pn_second)
//
// lav 12/10/13 written.
//
{
  normalize();
}

//=============================================================================
double matVector2DUnit::get_norm() const
//
// lav 12/10/13 written.
//
{
  assert(IS_ZERO(get_squared_norm() - 1));

  return 1;
}

//=============================================================================
