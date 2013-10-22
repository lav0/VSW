#pragma once

#include "matLine2DSegment.h"


class matTriangle
{
public: /* CONSTRUCTORS */

  matTriangle() {}
  matTriangle(matPoint2D, matPoint2D, matPoint2D);

public:

  bool segment_test() const;

private: /* MEMBERS */

  matPoint2D m_point1;
  matPoint2D m_point2;
  matPoint2D m_point3;
};