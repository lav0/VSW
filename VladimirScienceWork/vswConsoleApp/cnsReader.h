#pragma once
#pragma warning(disable:4996)

#include "stdafx.h"

#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "../vswGirder/grdFigure.h"

using namespace std;

class cnsReader
{
public:

  cnsReader(string a_file_name);

  bool get_next_point(boost::shared_ptr<matPoint2D>& a_pn_out);

private:
  
  string get_next_node();

  void trim_list();

private:

  bool m_b_is_valid;

  vector<string> m_nodes;
  vector<string>::iterator m_iterator; 
};
