#include "stdafx.h"
#include "../vswMath/matLine2DSegment.h"

struct IGirder
{

  virtual matPoint2D gravity_center() const = 0;

  virtual double weight() const = 0;

};