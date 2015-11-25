#include "curveevaluator.h"
#include "beziercurveevaluator.h"
#include "linearcurveevaluator.h"
#include "vec.h"
#include "mat.h"

#ifndef BSPLINE_CURVE
#define BSPINE_CURVE

using std::vector;
class BsplineCurveEvaluator : public CurveEvaluator {
public:
	void evaluateCurve(
		const vector<Point>& ptvCtrlPts,
		vector<Point>& pvtEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;
};
#endif