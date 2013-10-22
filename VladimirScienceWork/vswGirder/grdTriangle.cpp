#include "stdafx.h"

#include "grdTriangle.h"

//=============================================================================
grdTriangle::grdTriangle(
  shared_ptr<matPoint2D> a_shp1,
  shared_ptr<matPoint2D> a_shp2,
  shared_ptr<matPoint2D> a_shp3
) :
m_points()
//
// lav 17/10/13 written.
// lav 22/10/13 used grdRingStack.
//
{
  m_points.push_back(a_shp1);
  m_points.push_back(a_shp2);
  m_points.push_back(a_shp3);
}

//=============================================================================
const matPoint2D* grdTriangle::get_point(int a_i) const
//
// lav 17/10/13 written.
//
{
  return get_shp_point(a_i).get();
}

//=============================================================================
shared_ptr<matPoint2D> grdTriangle::get_shp_point(int a_i) const
//
// lav 17/10/13 written.
// lav 22/10/13 used grdRingStack.
//
{
  if (a_i >= 0 && a_i <= 2) {
    return m_points.get_point_by_ind(a_i);
  } 

  _ASSERT(0);
  return shared_ptr<matPoint2D>(NULL);
}

//=============================================================================