#include <iostream>
#include <conio.h>
#include <cstdlib>

#include "../vswMath/matLine2D.h"

static int s_passed = 0;
static int s_failed = 0;

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

  matVector2D vc2_test1(3.4, -1.2);
  matVector2D vc2_sum = vc2_test + vc2_test1;
  matVector2D vc2_dif = vc2_test - vc2_test1;

  b_total_result &= IS_ZERO(
    vc2_sum.get_point().X - (vc2_test.get_point().X + vc2_test1.get_point().X)
  );
  b_total_result &= IS_ZERO(
    vc2_sum.get_point().Y - (vc2_test.get_point().Y + vc2_test1.get_point().Y)
  );
  b_total_result &= IS_ZERO(
    vc2_dif.get_point().X - (vc2_test.get_point().X - vc2_test1.get_point().X)
  );
  b_total_result &= IS_ZERO(
    vc2_dif.get_point().Y - (vc2_test.get_point().Y - vc2_test1.get_point().Y)
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

  vc2u_test.become(a_d_x + 12.3, a_d_y - 32.9);

  b_total_result &= IS_ZERO(
    vc2u_test.get_norm()*vc2u_test.get_norm() - 1
  );

  return b_total_result;
}

//=============================================================================
bool run_line2D_by_points(const matPoint2D& a_pn1, const matPoint2D& a_pn2)
//
// lav 14/10/13 written.
//
{
  matLine2D ln(a_pn1, a_pn2);

  bool b_total_result = true;

  double d_ref_x = 0;
  double d_ref_y = 0;

  bool b_get_x = ln.get_x_by_y(a_pn1.Y, d_ref_x);
  bool b_get_y = ln.get_y_by_x(a_pn1.X, d_ref_y);

  b_total_result &= b_get_x || b_get_y;

  if (b_get_x) {
    b_total_result &= IS_ZERO(a_pn1.X - d_ref_x);
   
    double d_some_y = 6.441;
    b_total_result &= ln.get_x_by_y(d_some_y, d_ref_x);
    matPoint2D pn_on_line;
    pn_on_line.X = d_ref_x;
    pn_on_line.Y = d_some_y;
    b_total_result &= ln.is_point_on(pn_on_line, 0.0001);
  } 
  if (b_get_y) {
    b_total_result &= IS_ZERO(a_pn1.Y - d_ref_y);
  }

  return b_total_result;
}

bool run_line2D_intersection()
{
  matLine2D ln1(0,1,3,4);
  matLine2D ln2(3,1,1,5);
  bool b_total_result = true;
  
  boost::shared_ptr<matPoint2D> shp_point(new matPoint2D);

  b_total_result &= ln1.get_intersection(ln2, /* out */ shp_point);
  b_total_result &= IS_ZERO(shp_point->X - 2.0) && IS_ZERO(shp_point->Y - 3.0);
  b_total_result &= ln2.get_intersection(ln1, /* out */ shp_point);
  b_total_result &= IS_ZERO(shp_point->X - 2.0) && IS_ZERO(shp_point->Y - 3.0);

  matLine2D ln_colin_to_ln1(0,-1,2,1);

  b_total_result &= !ln1.get_intersection(
    ln_colin_to_ln1, 
    /* out */ shp_point
  );

  return b_total_result;
}

//=============================================================================
void show(bool a_b)
//
// lav 13/10/13 written.
//
{
  a_b ? ++s_passed : ++s_failed;
  std::cout << (a_b ? "Passed " : "Failed ");
}

//=============================================================================
int main()
//
// lav 13/10/13 written.
//
{
  { // Vectors testing
    double x, y;
    for (int i=0; i<10; i++) {
      x = (double) i*1.4 + (i-0.5)*i;
      y = (double) i*(-0.8) + log(double(i+3));

      show(run_vector2D(x, y));
      std::cout <<  x << " " << y << std::endl;
      show(run_vector2DUnit(x, y));
      std::cout <<  x << " " << y << std::endl;
    }
  }

  std:: cout << std::endl;

  { // Line testing
    matPoint2D pn1;
    matPoint2D pn2;
    for (int i=-1; i<10; i++) {
      
      if (i==-1) {
        //special case matches points
        pn1.X = 4;
        pn1.Y = -3;
        pn2.X = 4;
        pn2.Y = -3;
      } else if (i==0) {
        //special case x
        pn1.X = 4;
        pn1.Y = -3;
        pn2.X = 4;
        pn2.Y = 2;
      } else if (i==1) {
        //special case y
        pn1.X = -1;
        pn1.Y = 2;
        pn2.X = 4;
        pn2.Y = 2;
      } else {
        pn1.X = (double(rand() % 2000) / 10) - 1000;
        pn1.Y = (double(rand() % 2000) / 10) - 1000;
        pn2.X = (double(rand() % 2000) / 10) - 1000;
        pn2.Y = (double(rand() % 2000) / 10) - 1000;
      }
      show((i==-1) ? !run_line2D_by_points(pn1, pn2) : 
        run_line2D_by_points(pn1, pn2));
      std::cout << "[point1(" <<  pn1.X << ", " << pn1.Y << ") point2(" <<
        pn2.X << ", " << pn2.Y << ")]" << std::endl;
    }
  }

  std::cout << std::endl;

  { // line intersection testing
    show(run_line2D_intersection());
    std::cout << "[Intersection testing]" << std::endl;
  }

  std::cout << std::endl << "Passed: " << s_passed <<
    "; Failed: " << s_failed << ";\n";

  _getche();
  return 0;
}