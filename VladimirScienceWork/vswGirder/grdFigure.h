#include "stdafx.h"
#include "grdTriangle.h"
#include "grdSingularFigure.h"

using namespace boost;

class grdFigure : public IGirder 
{
public: /* CONSTRUCTORS */

  grdFigure() {}
  grdFigure(grdRingStack);

private: /* INNER */

  bool split();
  bool singular_to_member();
  bool separate_to_member(
    const matPoint2D* a_p_ray_p2,
    const matPoint2D* a_p_pierced_p2,
    const matPoint2D& a_pn_separator,
    eIntersectionPlace a_e_intr_place
  );
  eIntersectionPlace find_further_intersection(
    const std::vector<matLine2DSegment>&,
    const std::vector<matLine2DSegment>::iterator&,
    shared_ptr<matPoint2D>& a_shp_out,
    std::vector<matLine2DSegment>::const_iterator& a_itr_out
  ) const;

  int triangulate_to_member();
  

public: /* MAIN FUNCTIONS */

  bool calculate_gravity_centre_and_weight(Girder&);

  void push_back(shared_ptr<matPoint2D>&);

  eConvexityCase check_for_convexity() const;

  int get_size() const;
  int triangulate(std::vector<grdTriangle>&) const;
  
public: /* ACCESSORS */

  const std::vector<shared_ptr<IGirder>>& get_sub_list() const;

private: /* MEMBERS */

  grdRingStack m_points;

  std::vector<shared_ptr<IGirder>> m_subfig_list;
  
};