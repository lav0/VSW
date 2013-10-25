#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <string>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "../vswMath/matTriangle.h"
#include "../vswGirder/grdFigure.h"
#include "../vswGirder/grdRingStack.h"

#define pPoint shared_ptr<matPoint2D>

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
bool run_segment_intersection(
  double a_d_x1_1, double a_d_y1_1, double a_d_x2_1, double a_d_y2_1,
  double a_d_x1_2, double a_d_y1_2, double a_d_x2_2, double a_d_y2_2,
  double a_d_check_x, double a_d_check_y, eIntersectionPlace a_e_check_place
)
//
// lav 16/10/13 written.
//
{
  matLine2DSegment sg1(
    matPoint2DConstr(a_d_x1_1, a_d_y1_1),
    matPoint2DConstr(a_d_x2_1, a_d_y2_1)
  );
  matLine2DSegment sg2(
    matPoint2DConstr(a_d_x1_2, a_d_y1_2), 
    matPoint2DConstr(a_d_x2_2, a_d_y2_2)
  );

  bool b_total_result = true;
  
  boost::shared_ptr<matPoint2D> shp_intr;
  b_total_result &= 
    a_e_check_place == sg1.recognize_intersection(sg2, shp_intr);

  if (a_e_check_place != IP_NONE) {
    b_total_result &= IS_ZERO(a_d_check_x - shp_intr->X) && 
      IS_ZERO(a_d_check_y - shp_intr->Y);
  }

  return b_total_result;
}

//=============================================================================
bool run_triangle()
//
// lav 16/10/13 written.
//
{
  grdTriangle grd(
    shared_ptr<matPoint2D>(new matPoint2D(2,2)), 
    shared_ptr<matPoint2D>(new matPoint2D(6,3)), 
    shared_ptr<matPoint2D>(new matPoint2D(1,7))
  );
  
  grdTriangle grd1(
    shared_ptr<matPoint2D>(new matPoint2D(12,2)), 
    shared_ptr<matPoint2D>(new matPoint2D(-18,-3)), 
    shared_ptr<matPoint2D>(new matPoint2D(0,0))
  );

  double d_weight;
  grd.weight(d_weight);

  return grd.is_valid() && !grd1.is_valid() &&
    IS_ZERO(d_weight - 10.5);
}

//=============================================================================
bool run_triangle_gravity_centre()
//
// lav 23/10/13 written.
//
{
  grdTriangle grd(
    shared_ptr<matPoint2D>(new matPoint2D(1,1)), 
    shared_ptr<matPoint2D>(new matPoint2D(3,0)), 
    shared_ptr<matPoint2D>(new matPoint2D(2,28))
  );

  matPoint2D pn_gc1;
  grd.gravity_centre(pn_gc1);
  
  return IS_ZERO(pn_gc1.X - 2.0) && IS_ZERO(pn_gc1.Y - 9.6666666670999994);
  
}

//=============================================================================
bool run_stack_convex(
  pPoint shp_p1,
  pPoint shp_p2,
  pPoint shp_p3,
  pPoint shp_p4,
  bool a_b_is_convex
)
//
// lav 20/10/13 written.
//
{
  grdRingStack stack;
  stack.push_back(shp_p1);
  stack.push_back(shp_p2);
  stack.push_back(shp_p3);
  stack.push_back(shp_p4);

  return a_b_is_convex ? stack.is_convex() >= 1 : stack.is_convex() <= 0;
}

//=============================================================================
bool run_convex_figure_gravity_centre_and_weight(
  std::string a_file_name,
  double a_check_weight, double a_check_gc_x, double a_check_gc_y)
//
// lav 23/10/13 written.
// lav 24/10/13 weight added.
// lav 25/10/13 rewritten. renamed.
//
{
  grdFigure figure;

  std::string str_line;
  std::ifstream file(a_file_name);
  if (file.is_open()) {
    std::vector<std::string> str_array;
    while (std::getline(file, str_line)) {
      boost::split(str_array, str_line, boost::is_any_of(","));
      figure.push_back(pPoint(new matPoint2D(
        boost::lexical_cast<double>(*str_array.begin()),
        boost::lexical_cast<double>(*(++str_array.begin()))
      )));
    }
  }
  bool b_total_result = true;
  Girder girder;
  if (figure.calculate_gravity_centre_and_weight(girder)) {
    
    b_total_result &= IS_ZERO(girder.m_weight - a_check_weight);
    b_total_result &= IS_ZERO(girder.m_gravity_centre.X - a_check_gc_x);
    b_total_result &= IS_ZERO(girder.m_gravity_centre.Y - a_check_gc_y);
    
    return b_total_result;
  }
  
  return false;
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

  std::cout << std::endl;

  { // Segment intersection testing 
    show(run_segment_intersection(0,0,2,0, 0,1,1,1, DBL_MIN,DBL_MIN, IP_NONE),
      "[Segment intersection testing]");
    show(run_segment_intersection(0,0,2,0, 1,-1,1,1, 1,0, IP_BOTH),
      "[Segment intersection testing]");
    show(run_segment_intersection(-2,0,0,0, 1,-1,1,1, 1,0, IP_MIDDLE),
      "[Segment intersection testing]");
    show(run_segment_intersection(-2,-1,0,0, 1,-1,1,1, 1,0.5, IP_MIDDLE),
      "[Segment intersection testing]");
    show(run_segment_intersection(0,0,2,1, 1,2,3,0, 2,1, IP_MIDDLE),
      "[Segment intersection testing]");
    show(run_segment_intersection(0,0,2,1, 4,2,3,0, 4,2, IP_EDGE),
      "[Segment intersection testing]");
    show(run_segment_intersection(0,0,2,1, 5,1,4,2, 4,2, IP_EDGE),
      "[Segment intersection testing]");
    show(run_segment_intersection(0,0,2,1, 6,3,4,2, 4,2, IP_FULL),
      "[Segment intersection testing]");
    show(run_segment_intersection(6,3,4,2, 0,0,2,1, 2,1, IP_FULL),
      "[Segment intersection testing]");
    show(run_segment_intersection(6,3,4,2, 6,3,4.00000001,2, 6,3, IP_SAME),
      "[Segment intersection testing]");
  }

  std::cout << std::endl;

  { // Triangle testing
    show(run_triangle(), "[Triangle testing]");
  }

  std::cout << std::endl;

  { // Stack testing
    show(run_stack_convex(
      shared_ptr<matPoint2D>(new matPoint2D(2,2)),
      shared_ptr<matPoint2D>(new matPoint2D(6,3)),
      shared_ptr<matPoint2D>(new matPoint2D(7,8)),
      shared_ptr<matPoint2D>(new matPoint2D(2,6)),
      true
    ), "[Stack testing]");
    show(run_stack_convex(
      shared_ptr<matPoint2D>(new matPoint2D(2,2)),
      shared_ptr<matPoint2D>(new matPoint2D(0,0)),
      shared_ptr<matPoint2D>(new matPoint2D(1,1)),
      shared_ptr<matPoint2D>(new matPoint2D(3,3)),
      true
    ), "[Stack testing]");
    show(run_stack_convex(
      shared_ptr<matPoint2D>(new matPoint2D(2,2)),
      shared_ptr<matPoint2D>(new matPoint2D(0,0)),
      shared_ptr<matPoint2D>(new matPoint2D(0,0)),
      shared_ptr<matPoint2D>(new matPoint2D(3,3)),
      true
    ), "[Stack testing]");
    show(run_stack_convex(
      shared_ptr<matPoint2D>(new matPoint2D(2,2)),
      shared_ptr<matPoint2D>(new matPoint2D(6,2)),
      shared_ptr<matPoint2D>(new matPoint2D(6,8)),
      shared_ptr<matPoint2D>(new matPoint2D(4,3)),
      false
    ), "[Stack testing]");
    show(run_stack_convex(
      shared_ptr<matPoint2D>(new matPoint2D(1,1)),
      shared_ptr<matPoint2D>(new matPoint2D(5,2)),
      shared_ptr<matPoint2D>(new matPoint2D(-1,3)),
      shared_ptr<matPoint2D>(new matPoint2D(1,-2)),
      false
    ), "[Stack testing]");
    
  }

  std::cout << std::endl;

  { // Triangle gravity centre testing
    show(run_triangle_gravity_centre(), "[Triangle gravity centre testing]");
  }

  std::cout << std::endl;

  { // Convex figure gravity centre and weight calculation testing
    show(
      run_convex_figure_gravity_centre_and_weight("file0.txt", 49.5, 0.218855219,-0.1043771),
      "[Convex figure gravity centre and weight calculation testing]"
    );
    show(
      run_convex_figure_gravity_centre_and_weight("file1.txt", 16, 1, 1),
      "[Convex figure gravity centre and weight calculation testing]"
    );
    show(
      run_convex_figure_gravity_centre_and_weight("file2.txt", 14, 1, 1),
      "[Convex figure gravity centre and weight calculation testing]"
    );
    show(
      run_convex_figure_gravity_centre_and_weight("file3.txt", 809.4984,-6.911066314, 11.990353083123319),
      "[Convex figure gravity centre and weight calculation testing]"
    );
    show(
      run_convex_figure_gravity_centre_and_weight("file4.txt", 809.4984,-6.911066314, 11.990353083123319),
      "[Convex figure gravity centre and weight calculation testing]"
    );
    show(
      run_convex_figure_gravity_centre_and_weight("file5.txt", 0.0,-0.5, 3.0),
      "[Convex figure gravity centre and weight calculation testing]"
    );
    show(
      run_convex_figure_gravity_centre_and_weight("file6.txt", 0, 5, 9),
      "[Convex figure gravity centre and weight calculation testing]"
    );
    show(
      run_convex_figure_gravity_centre_and_weight("file7.txt", 0, 2.5, 3.25),
      "[Convex figure gravity centre and weight calculation testing]"
    );
  }

  std::cout << std::endl << "Passed: " << s_passed <<
    "; Failed: " << s_failed << ";\n";

  _getche();
  return 0;
}