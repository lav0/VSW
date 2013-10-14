#pragma once

#include "matVector2D.h"

class matVector2DUnit : public matVector2D {

public: /* CONTRUCTORS */

  matVector2DUnit() : matVector2D() {}
  matVector2DUnit(double, double);
  matVector2DUnit(matPoint2D);
  matVector2DUnit(matPoint2D, matPoint2D);
  void become(double, double);

public: /* MEMBER METHODS*/

  double get_norm() const;  

};