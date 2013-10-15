#include <iostream>
#include <conio.h>
#include <cstdlib>

#include "../vswMath/matLine2DSegment.h"

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

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

//=============================================================================
bool run_line2D_intersection(
  double a_d_ln1x1, double a_d_ln1y1, double a_d_ln1x2, double a_d_ln1y2,
  double a_d_ln2x1, double a_d_ln2y1, double a_d_ln2x2, double a_d_ln2y2,
  double a_d_check_x, double a_d_check_y
)
//
// lav 14/10/13 written.
//
{
  matLine2D ln1(a_d_ln1x1, a_d_ln1y1, a_d_ln1x2, a_d_ln1y2);
  matLine2D ln2(a_d_ln2x1, a_d_ln2y1, a_d_ln2x2, a_d_ln2y2);
  bool b_total_result = true;
  
  boost::shared_ptr<matPoint2D> shp_point(new matPoint2D);
  
  b_total_result &= ln1.get_intersection(ln2, /* out */ shp_point);
  b_total_result &= 
    IS_ZERO(shp_point->X - a_d_check_x) && IS_ZERO(shp_point->Y - a_d_check_y);
  b_total_result &= ln2.get_intersection(ln1, /* out */ shp_point);
  b_total_result &= 
    IS_ZERO(shp_point->X - a_d_check_x) && IS_ZERO(shp_point->Y - a_d_check_y);

  matLine2D ln_colin_to_ln1(
    a_d_ln1x1, a_d_ln1y1 - 2, a_d_ln1x2, a_d_ln1y2 - 2
  );

  b_total_result &= !ln1.get_intersection(
    ln_colin_to_ln1, 
    /* out */ shp_point
  );

  return b_total_result;
}

//=============================================================================
bool run_segment(double a_d_x1, double a_d_y1, double a_d_x2, double a_d_y2)
//
// lav 15/10/13 written.
//
{
  matPoint2D pn1, pn2;
  pn1.X = a_d_x1;
  pn1.Y = a_d_y1;
  pn2.X = a_d_x2;
  pn2.Y = a_d_y2;

  matLine2DSegment sg(pn1, pn2);
  
  bool b_total_result = true;
 
  b_total_result &= IS_ZERO(
    sg.get_pointer(PP_LEFT)->X - min(pn1.X, pn2.X)
  );
  b_total_result &= IS_ZERO(
    sg.get_pointer(PP_BOTTOM)->Y - min(pn1.Y, pn2.Y)
  );

  b_total_result &= IS_ZERO(
    sg.get_pointer(PP_RIGHT)->X - max(pn1.X, pn2.X)
  );
  b_total_result &= IS_ZERO(
    sg.get_pointer(PP_TOP)->Y - max(pn1.Y, pn2.Y)
  );

  // take some point on the line 
  // if (d_lam) is between [0,1] than point is on the segment 
  const double d_lam = double(rand() % 140) / 100 - 0.2;
  matPoint2D pn_on;
  pn_on.X = a_d_x1 * (1-d_lam) + a_d_x2 * d_lam;
  pn_on.Y = a_d_y1 * (1-d_lam) + a_d_y2 * d_lam;

  b_total_result &= sg.is_point_within(pn_on) || !(abs(d_lam-0.5)<=0.5);

  matLine2DSegment sg_rev(pn2, pn1);

  b_total_result &= sg.is_equal(sg_rev);
  b_total_result &= sg_rev.is_equal(sg);

  b_total_result &= IS_ZERO(sg.get_length() - sqrt(
    (a_d_x2 - a_d_x1)*(a_d_x2 - a_d_x1) + (a_d_y2 - a_d_y1)*(a_d_y2 - a_d_y1)
  ));

  return b_total_result;
}

//=============================================================================
void show(bool a_b, char* a_c_info)
//
// lav 13/10/13 written.
//
{
  a_b ? ++s_passed : ++s_failed;
  std::cout << (a_b ? "Passed " : "Failed ");
  
  while (*a_c_info != ']') {
    std::cout << *a_c_info;
    ++a_c_info;
  }
  std::cout << ']' << std::endl;
}

//=============================================================================
int main()
//
// lav 13/10/13 written.
// lav 14/10/13 run_line2D_by_points(), run_line2D_intersection() added.
// lav 15/10/13 run_segment() added.
//
{
  { // Vectors testing
    double x, y;
    for (int i=0; i<10; i++) {
      x = (double) i*1.4 + (i-0.5)*i;
      y = (double) i*(-0.8) + log(double(i+3));

      show(run_vector2D(x, y), "[Vector testing]");
      show(run_vector2DUnit(x, y), "[Vector testing (unit)]");
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
        run_line2D_by_points(pn1, pn2), "[Line creation testing]");
    }
  }

  std::cout << std::endl;

  { // line intersection testing
    show(
      run_line2D_intersection(
        0,1,3,4,
        3,1,1,5,
        2, 3
      ), 
      "[Line intersection testing]"
    );

    show(
      run_line2D_intersection(
        1,-2,2,4,
        2,-3,1,5,
        1.5, 1
      ),
      "[Line intersection testing]"
    );

    show(
      run_line2D_intersection(
        0,0,0.1,0,
        20,-1,20,1,
        20, 0
      ),
      "[Line intersection testing]"
    );
  }

  std::cout << std::endl;

  { // Segment testing
    show(run_segment(-1, 1, 5, 2), "[Segment testing]");
    show(run_segment(-2, 3, 5, 1), "[Segment testing]");
    show(run_segment(5, 1, 0, 0), "[Segment testing]");
    show(run_segment(5, 1, 0, 8), "[Segment testing]");
    show(run_segment(5, 1, 0, 1), "[Segment testing]");
    show(run_segment(
      rand() % 20 - 10, rand() % 20 - 10, 
      rand() % 20 + 10, rand() % 20 + 10
    ), "[Segment testing]");
  }

  std::cout << std::endl << "Passed: " << s_passed <<
    "; Failed: " << s_failed << ";\n";

  _getche();
  return 0;
}