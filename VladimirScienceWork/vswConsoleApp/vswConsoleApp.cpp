// vswConsoleApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <conio.h>
#include "cnsReader.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
  string file_name;
  cout << "Input file name" << endl;
  cin >> file_name;

  cnsReader reader(file_name);

  grdFigure figure;
  boost::shared_ptr<matPoint2D> shp_pn(new matPoint2D);
  bool is_good = true;
  while (reader.get_next_point(shp_pn)) {
    figure.push_back(shp_pn);
    shp_pn.reset(new matPoint2D);
  }

  Girder girder;
  if (figure.calculate_gravity_centre_and_weight(girder)) {
    cout << endl << girder.m_gravity_centre.X << ", " << 
      girder.m_gravity_centre.Y << " - gravity centre" << endl;
    cout << girder.m_weight << " - weight" << endl;
  } else {
    cout << "Error" << endl;
  }

  _getche();

	return 0;
}

