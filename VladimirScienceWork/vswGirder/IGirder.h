#pragma once

#include "stdafx.h"
#include "../vswMath/matLine2DSegment.h"

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/shared_array.hpp>
#include <vector>

using namespace boost;

struct Girder
{
  double m_weight;
  matPoint2D m_gravity_centre;
};

struct IGirder
{
  virtual bool calculate_gravity_centre_and_weight(Girder&) = 0;
};


//=============================================================================
static bool average(std::vector<shared_ptr<IGirder>> a_list, Girder& a_result)
//
// Find weight and centre of the whole income list
//
///////////////////////////////////////////////////////////////////////////////
//
// lav 24/10/13 written.
//
{
  double d_total_weight = 0;
  
  shared_array<Girder> girders_list(new Girder[a_list.size()]);
  std::vector<shared_ptr<IGirder>>::const_iterator itr = a_list.begin();

  if (itr == a_list.end()) {
    return false;
  }
  size_t i_index = 0;
  for (i_index; i_index < a_list.size(); i_index++) {
    if (!(*itr)->calculate_gravity_centre_and_weight(girders_list[i_index])) {
      return false;
    }
    d_total_weight += girders_list[i_index].m_weight;
    ++itr;
  }
  
  if (a_list.size() == 1) {
    a_result = girders_list[0];
    return true;
  }

  const double d_inverse_total_weight = 1 / d_total_weight;  
  Girder result;
  result.m_weight = d_total_weight;
  result.m_gravity_centre = matPoint2D(0,0);
  do {
    --i_index;
    result.m_gravity_centre.X += d_inverse_total_weight *
      girders_list[i_index].m_weight * girders_list[i_index].m_gravity_centre.X;
    result.m_gravity_centre.Y += d_inverse_total_weight *
      girders_list[i_index].m_weight * girders_list[i_index].m_gravity_centre.Y;
  } while (i_index > 0);

  a_result = result;
  return true;
}