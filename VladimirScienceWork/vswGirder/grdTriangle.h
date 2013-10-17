#include "stdafx.h"
#include "IGirder.h"

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
  grdTriangle(
    matPoint2D* p1,
    matPoint2D* p2,
    matPoint2D* p3
  ) {
    m_p1 = p1;
  }


public:

  double weight() const {return 0;}

  matPoint2D gravity_center() const {return *m_p1; }

public:

  bool is_value() const;
  
public:

  matPoint2D* get_point(int) const;

private:
  matPoint2D* m_p1;
  shared_ptr<matPoint2D> m_shp_pn1;
  shared_ptr<matPoint2D> m_shp_pn2;
  shared_ptr<matPoint2D> m_shp_pn3;
  
};

