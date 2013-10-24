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

private: /* INNER METHODS*/
  
  double semi_perimeter() const;

  matVector2D vector12() const;
  matVector2D vector23() const;
  matVector2D vector31() const;

  matLine2DSegment median(int a_ind) const;

public:

  bool calculate_gravity_centre_and_weight(Girder&);

  double weight() const;

  matPoint2D gravity_centre() const;
  
public:

  bool is_valid() const;
  
public:

  const matPoint2D* get_point(int) const;

  shared_ptr<matPoint2D> get_shp_point(int) const;

private:
  
  grdRingStack m_points;
  
};

