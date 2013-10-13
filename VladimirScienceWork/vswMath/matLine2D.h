#pragma once

#include "matVector2DUnit.h"

class matLine2D 
{
public: /* CONSTRUCTORS */

  matLine2D(matVector2DUnit, double);
  matLine2D(matPoint2D, matPoint2D);

protected: /* INNER METHODS */

  bool define(const matPoint2D&, const matPoint2D&);

public: /* MEMBER FUNCTIONS */

  bool is_point_on(const matPoint2D&, double tolerance) const;
  bool is_parallel(const matLine2D&) const;
  bool is_orthogonal(const matLine2D&) const;
  bool is_equal(const matLine2D&) const;
  bool get_intersection(const matLine2D&, matPoint2D& out) const;

public: /* ACCESSORS */

  const matVector2DUnit& get_normal() const;
  double get_coef() const;

private: /* MEMBERS */

  // <m_vc2, (x,y)> = m_p
  matVector2DUnit m_vc2;

  double m_p;
};