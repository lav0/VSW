#pragma once

#include <stdexcept>
#include "matVector2DUnit.h"

class matLine2D 
{

public: /* CONSTRUCTORS */

  matLine2D() : m_vc2(0,0), m_p(0), m_d_defined(false) {}
  matLine2D(matVector2DUnit, double);
  matLine2D(matPoint2D, matPoint2D);

protected: /* INNER METHODS */

  bool define_by_two_points(const matPoint2D&, const matPoint2D&);

  /*
    A*x + B*y + C = 0
  */
  bool define_by_common_equation(double A, double B, double C);

public: /* MEMBER FUNCTIONS */

  bool is_point_on(const matPoint2D&, double tolerance) const;
  bool is_parallel(const matLine2D&) const;
  bool is_orthogonal(const matLine2D&) const;
  bool is_equal(const matLine2D&) const;
  bool get_intersection(const matLine2D&, matPoint2D& out) const;
  bool get_x_by_y(const double y, double& x_out) const;
  bool get_y_by_x(const double x, double& y_out) const;

public: /* ACCESSORS */

  const matVector2DUnit& get_normal() const;
  double get_coef() const;

private: /* MEMBERS */

  /*
    normal form of a line
    <m_vc2, (x,y)> = m_p
  */

  matVector2DUnit m_vc2;

  double m_p;

  bool m_d_defined;
};