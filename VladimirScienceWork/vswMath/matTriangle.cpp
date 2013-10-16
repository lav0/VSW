#include "matTriangle.h"

matTriangle::matTriangle(matPoint2D a_pn1, matPoint2D a_pn2, matPoint2D a_pn3) 
//
// lav 16/10/13 written.
//
{
  m_point1 = a_pn1;
  m_point2 = a_pn2;
  m_point3 = a_pn3;
}

bool matTriangle::segment_test() const

{
  matLine2DSegment sg1(m_point1, m_point2);
  matLine2DSegment sg2(m_point2, m_point3);
  matLine2DSegment sg3(m_point3, m_point1);

  boost::shared_ptr<matPoint2D> shp_out(new matPoint2D);
  eIntersectionPlace ip1 = sg1.recognize_intersection(sg2, shp_out);
  eIntersectionPlace ip2 = sg1.recognize_intersection(sg3, shp_out);
  eIntersectionPlace ip3 = sg2.recognize_intersection(sg3, shp_out);

  return ip1 == ip2 && ip1 == ip3;
}