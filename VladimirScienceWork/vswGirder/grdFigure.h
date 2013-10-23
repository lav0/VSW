#include "stdafx.h"
#include "grdTriangle.h"

using namespace boost;

class grdFigure : public IGirder 
{
public: /* CONSTRUCTORS */

  grdFigure() {}
  grdFigure(grdRingStack);

public: /* MAIN FUNCTIONS */

  double weight() const { return 0; }

  matPoint2D gravity_centre() const { return *m_points.get_point_by_ind(0); }

  void push_back(shared_ptr<matPoint2D>&);

  bool check_for_convexity() const;

  int get_size() const;
  int triangulate(std::vector<grdTriangle>&) const;

private: /* MEMBERS */

  grdRingStack m_points;
  
};