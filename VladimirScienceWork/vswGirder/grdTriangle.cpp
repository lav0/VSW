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
matVector2D grdTriangle::vector12() const
//
// lav 23/10/13 written.
//
{
  return matVector2D(
    *(m_points.get_point_by_ind(0).get()), 
    *(m_points.get_point_by_ind(1).get())
  );
}

//=============================================================================
matVector2D grdTriangle::vector23() const
//
// lav 23/10/13 written.
//
{
  return matVector2D(
    *(m_points.get_point_by_ind(1).get()), 
    *(m_points.get_point_by_ind(2).get())
  );
}

//=============================================================================
matVector2D grdTriangle::vector31() const
//
// lav 23/10/13 written.
//
{
  return matVector2D(
    *(m_points.get_point_by_ind(2).get()), 
    *(m_points.get_point_by_ind(0).get())
  );
}


//=============================================================================
bool grdTriangle::is_valid() const 
//
// lav 23/10/13 written.
//
{
  _ASSERT(m_points.get_size() > 0);

  return IS_ZERO(vector12().cross_prod_z(vector23())) == false;
}

//=============================================================================
double grdTriangle::semi_perimeter() const
//
// lav 23/10/13 written.
//
{
  return 0.5 *
    (vector12().get_norm() + vector23().get_norm() + vector31().get_norm());
}

//=============================================================================
matLine2DSegment grdTriangle::median(int a_ind) const
//
// lav 23/10/13 written.
//
{  
  int i_point = -1;
  int i_segment1 = -1;
  int i_segment2 = -1;
  if (a_ind == 0) {
    i_point = 0;
    i_segment1 = 1;
    i_segment2 = 2;
  } else if (a_ind == 1) {
    i_point = 1;
    i_segment1 = 0;
    i_segment2 = 2;
  } else if (a_ind == 2) {
    i_point = 2;
    i_segment1 = 1;
    i_segment2 = 0;
  } else {
    _ASSERT(0);
  }

  matLine2DSegment segment(
    *get_shp_point(i_segment1), 
    *get_shp_point(i_segment2)
  );
  matPoint2D point = *get_shp_point(i_point);;
  return matLine2DSegment(point, segment.get_point_by_lambda(0.5));
}

//=============================================================================
bool grdTriangle::weight(double& a_d_out) const 
//
// Here, weight's meaning equal is a area of the figure
//
///////////////////////////////////////////////////////////////////////////////
//
// lav 23/10/13 written.
// lav 25/10/13 return type made bool. output goes by ref.
// lav 19/11/13 optimized.
//
{
  if (!is_valid()) {
    return false;
  }

  double d_semi_perimeter = semi_perimeter();

  a_d_out =  sqrt(
    d_semi_perimeter * (
      d_semi_perimeter - vector12().get_norm()) * (
      d_semi_perimeter - vector23().get_norm()) * (
      d_semi_perimeter - vector31().get_norm()
    )
  );
  return a_d_out > _ZERO;
}

//=============================================================================
bool grdTriangle::gravity_centre(matPoint2D& a_vc_out) const 
//
// lav 23/10/13 written.
// lav 25/10/13 return type made bool. output goes by ref
//
{
  if (!is_valid()) {
    return false;
  }

  a_vc_out = median(0).get_point_by_lambda(0.6666666667);
  return true;
}

//=============================================================================
eMainCalculationResult grdTriangle::calculate_gravity_centre_and_weight(Girder& a_result)
//
// lav 24/10/13 written.
// lav 25/10/13 rewritten.
// lav 22/11/13 return type changed.
//
{
  return 
    weight(a_result.m_weight) && gravity_centre(a_result.m_gravity_centre) ?
    MCR_SUCCEEDED : MCR_FAIL;
}

//=============================================================================