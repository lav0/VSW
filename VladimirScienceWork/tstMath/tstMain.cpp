#include <iostream>
#include <conio.h>
#include <random>

#include "../vswMath/matLine2D.h"


//=============================================================================
bool run_vector2D(
  double a_d_x, double a_d_y
)
//
// lav 13/10/13 written.
//
{
  matVector2D vc2_test(a_d_x, a_d_y);
  bool b_total_result = true;

  b_total_result &= IS_ZERO(vc2_test.get_point().X - a_d_x);
  b_total_result &= IS_ZERO(vc2_test.get_point().Y - a_d_y);

  b_total_result &= IS_ZERO(
    vc2_test.get_norm()*vc2_test.get_norm() - (a_d_x * a_d_x + a_d_y*a_d_y)
  );

  return b_total_result;
}

//=============================================================================
bool run_vector2DUnit(
  double a_d_x, double a_d_y
)
//
// lav 13/10/13 written.
//
{
  matVector2DUnit vc2u_test(a_d_x, a_d_y);
  
  bool b_total_result = true;

  b_total_result &= IS_ZERO(
    vc2u_test.get_norm()*vc2u_test.get_norm() - 1
  );

  return b_total_result;
}

//=============================================================================
int main()
//
// lav 13/10/13 written.
//
{
  double x, y;
  for (int i=0; i<10; i++) {
    x = (double) i*1.4 + (i-0.5)*i;
    y = (double) i*(-0.8) + log(double(i+3));

    std::cout << (run_vector2D(x, y) ? "Passed " : "Failed ") <<
      x << " " << y << std::endl <<
      (run_vector2DUnit(x, y) ? "Passed " : "Failed ") <<
      x << " " << y << std::endl;
  }
  
  _getche();
  return 0;
}