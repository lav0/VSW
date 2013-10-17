#include "stdafx.h"

#include "grdTriangle.h"

//=============================================================================
grdTriangle::grdTriangle(
  shared_ptr<matPoint2D> a_shp1,
  shared_ptr<matPoint2D> a_shp2,
  shared_ptr<matPoint2D> a_shp3
)
//
// lav 17/10/13 written.
//
{
  m_shp_pn1 = a_shp1;
  m_shp_pn2 = a_shp2;
  m_shp_pn3 = a_shp3;
}

//=============================================================================
matPoint2D* grdTriangle::get_point(int a_i) const
//
// lav 17/10/13 written.
//
{
  if (a_i == 1) {
    return m_p1;
  } else if (a_i == 2) {
    return m_shp_pn2.get();
  } else if (a_i == 3) {
    return m_shp_pn3.get();
  } 

  _ASSERT(0);
  return shared_ptr<matPoint2D>(NULL).get();
}

//=============================================================================