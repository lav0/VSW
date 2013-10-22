#include "stdafx.h"
#include "IGirder.h"
#include "grdRingStack.h"

using namespace boost;

class grdTriangle : public IGirder 
{

public:

  grdTriangle() {}
  grdTriangle(
    shared_ptr<matPoint2D>,
    shared_ptr<matPoint2D>,
    shared_ptr<matPoint2D>
  );

public:

  double weight() const {return 0;}

  matPoint2D gravity_center() const {return *m_points.get_point_by_ind(0); }

public:

  bool is_value() const;
  
public:

  const matPoint2D* get_point(int) const;
  shared_ptr<matPoint2D> get_shp_point(int) const;

private:
  
  grdRingStack m_points;

};

