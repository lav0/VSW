#include "stdafx.h"
#include "grdTriangle.h"

using namespace boost;

class grdFigure : public IGirder 
{
public: /* CONSTRUCTORS */

  grdFigure() {}
  grdFigure(grdRingStack);

private: /* INNER */

  int triangulate_to_member();

public: /* MAIN FUNCTIONS */

  bool calculate_gravity_centre_and_weight(Girder&);

  double weight() const;

  matPoint2D gravity_centre() const { return *m_points.get_point_by_ind(0); }
  
  void push_back(shared_ptr<matPoint2D>&);

  bool check_for_convexity() const;

  int get_size() const;
  int triangulate(std::vector<grdTriangle>&) const;
  
public: /* ACCESSORS */

  const std::vector<shared_ptr<IGirder>>& get_sub_list() const;

private: /* MEMBERS */

  grdRingStack m_points;

  std::vector<shared_ptr<IGirder>> m_subfig_list;
  
};