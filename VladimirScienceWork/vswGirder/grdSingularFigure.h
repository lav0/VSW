#include "stdafx.h"
#include "IGirder.h"
#include "grdRingStack.h"

class grdSingularFigure : public IGirder
{
public: /* CONSTRUCTORS*/

  grdSingularFigure() {}
  grdSingularFigure(grdRingStack&);

public:

  bool is_valid() const;

public: /* OVERRIDEN METHOD */

  eMainCalculationResult calculate_gravity_centre_and_weight(Girder&);

private: /* MEMBER */

  grdRingStack m_points;

};