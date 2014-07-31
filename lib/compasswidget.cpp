#include "compasswidget.h"
#include <math.h>

namespace SPEEDMETER {

static double angleBetween2Lines(Line const &line1, Line const &line2) {
	double AY=line1.A.Y - line1.B.Y;
	double AX=line1.A.X - line1.B.X;
	double BY=line2.A.Y - line2.B.Y;
	double BX=line2.A.X - line2.B.X;

	double angle1 = atan2(AY,AX);
    double angle2 = atan2(BY,BX);

    return angle1-angle2;
}
}

