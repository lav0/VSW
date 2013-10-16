#pragma once

#include "matLine2D.h"

enum eIntersectionPlace {
  IP_ERROR = -1,
  IP_NONE = 0,
  IP_BOTH = IP_NONE + 1,
  IP_MIDDLE = IP_BOTH + 1,
  IP_EDGE = IP_MIDDLE + 1,
  IP_FULL = IP_EDGE + 1,
  IP_SAME = IP_FULL
};

enum ePointerPlace {
  PP_LEFT,
  PP_RIGHT,
  PP_BOTTOM,
  PP_TOP
};


class matLine2DSegment : public matLine2D
{
  enum ePointOnEdgeResult {
    POER_UNKNOWN,
    POER_NONE,
    POER_FIRST,
    POER_SECOND
  };

public: /* CONSTRUCTORS */

  matLine2DSegment() : matLine2D(){ set_defined(false); }
  matLine2DSegment(matPoint2D, matPoint2D);
  
  ~matLine2DSegment();

private: /* INNER FUNCTIONS */

  void define_pointers();

  bool is_crossed(const matLine2DSegment&) const;

  matPoint2D get_segment_point(double a_d_lambda, bool b_leftright) const;

  ePointOnEdgeResult is_point_on_edge(const matPoint2D&) const;

public: /* MAIN FUNCTIONS */

  bool is_point_within(const matPoint2D&) const;
  bool is_equal_segment(const matLine2DSegment&) const;

  double get_length() const;

  const matVector2D get_start_end_vector() const;
  const matVector2D get_end_start_vector() const;

  eIntersectionPlace recognize_intersection(
    const matLine2DSegment&,
    boost::shared_ptr<matPoint2D>&
  ) const;

public: /*  ACCESSORS */

  const matPoint2D get_first_point() const;
  const matPoint2D get_second_point() const;

  const boost::shared_ptr<matPoint2D> get_pointer(
    ePointerPlace a_e_place
  ) const;

private: /* MEMBERS */
  
  boost::shared_ptr<matPoint2D> m_shp_point1;
  boost::shared_ptr<matPoint2D> m_shp_point2;

  boost::weak_ptr<matPoint2D> m_wkp_left;
  boost::weak_ptr<matPoint2D> m_wkp_right;
  boost::weak_ptr<matPoint2D> m_wkp_bottom;
  boost::weak_ptr<matPoint2D> m_wkp_top;

};