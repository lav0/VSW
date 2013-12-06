// vswConsoleApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <conio.h>
#include "cnsReader.h"

using namespace std;

//=============================================================================
void round_value(double& a_d_ref)
//
// lav 22/11/13 written.
//
{
  if (IS_ZERO(a_d_ref)) {
    a_d_ref = 0.0;
  }
}

//=============================================================================
void round_value(Girder& a_grd_ref)
//
// lav 22/11/13 written.
//
{
  round_value(a_grd_ref.m_gravity_centre.X);
  round_value(a_grd_ref.m_gravity_centre.Y);
  round_value(a_grd_ref.m_weight);
}

//=============================================================================
bool communicate_to_user()
//
// lav 22/11/13 written.
//
{
  string file_name;
  cout << "Input file name" << endl;
  int i_answer = _getche();
  
  if (i_answer == 27) {
    return false;
  }

  cin >> file_name;
  file_name = (char)i_answer + file_name;

  cnsReader reader(file_name);

  grdFigure figure;
  boost::shared_ptr<matPoint2D> shp_pn(new matPoint2D);
  while (reader.get_next_point(shp_pn)) {
    figure.push_back(shp_pn);
    shp_pn.reset(new matPoint2D);
  }

  Girder girder;
  eMainCalculationResult e_main_result = 
    figure.calculate_gravity_centre_and_weight(girder);

  cout << endl;
  switch (e_main_result) {
  case MCR_UNKNOWN :
    cout << "Unknown error";
    break;
  case MCR_FAIL :
    cout << "Cannot calculate";
    break;
  case MCR_INVALID_FIGURE:
    cout << "Invalid figure";
    break;
  case MCR_SUCCEEDED:
    round_value(girder);
    cout << girder.m_gravity_centre.X << ", " << 
      girder.m_gravity_centre.Y << " - gravity centre" << endl <<
      girder.m_weight << " - weight";
    break;
  }
  cout << endl << endl << "Press Esc to escape or";
  return true;
}

//=============================================================================
int _tmain(int argc, _TCHAR* argv[])
//
// lav 21/11/13 written.
//
{
  while (communicate_to_user()) { cout << endl; }
  
	return 0;
}

//=============================================================================
