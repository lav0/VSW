#include "stdafx.h"
#include "../vswMath/matLine2DSegment.h"

struct IGirder
{

  virtual matPoint2D gravity_centre() const = 0;

  virtual double weight() const = 0;

};