#pragma once

#include <math.h>
#include <assert.h>
#include <crtdbg.h>

#define _ASSERT_NULL_POINTER(p)             _ASSERT(p!=NULL)
#define PI                                  3.14159265358979323846
#define _POINTS_COMPARISON                  1e-5
#define _LINE_DIFF                          1e-6
#define _ZERO                               1e-8
#define IS_ZERO(a)                          abs(a) < _ZERO

struct matPoint2D {
  matPoint2D(){}
  matPoint2D(double a_x, double a_y) { X=a_x; Y=a_y; }
  double X;
  double Y;
};

static bool are_points_equal(
  const matPoint2D& a_pn1, 
  const matPoint2D& a_pn2,
  double a_d_tol
)
{
  bool b_xdif = abs(a_pn1.X - a_pn2.X) < a_d_tol;
  bool b_ydif = abs(a_pn1.Y - a_pn2.Y) < a_d_tol;

  return b_xdif && b_ydif;
}

/*
  CLASS DECLARATION ITSELF GOES FURTHER
*/

class matVector2D {

public: /* CONTRUCTORS */

  matVector2D() : m_x(0), m_y(0){}
  matVector2D(double, double);
  matVector2D(matPoint2D);
  matVector2D(matPoint2D, matPoint2D);

  void become(double, double);

protected: /* INNER METHODS */

  double get_squared_norm() const;
  void normalize();

public: /* MEMBER FUNCTIONS */

  bool is_colinear(const matVector2D&) const;
  bool is_parallel(const matVector2D&) const;
  bool is_antiparallel(const matVector2D&) const;
  bool is_orthogonal(const matVector2D&) const;
  bool is_zero_vector() const;

  double get_norm() const;
  double scal_prod(const matVector2D&) const;
  double cross_prod_z(const matVector2D&) const;
  double angle(const matVector2D&) const;

  const matVector2D get_normalized_vector() const;
  const matVector2D get_reverted() const;
  const matVector2D operator+(const matVector2D&) const;
  const matVector2D operator-(const matVector2D&) const;
  
  matVector2D& operator=(const matVector2D&);

public: /* ACCESSORS */

  const matPoint2D get_point() const;

private: /* MEMBERS */

  double m_x;
  double m_y;
};