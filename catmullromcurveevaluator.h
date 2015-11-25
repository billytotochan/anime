#include "curveevaluator.h"
#include "linearcurveevaluator.h"
#include "vec.h"
#include "mat.h"

using std::vector;
#ifndef CATMULLROMCURVE_H
#define CATMULLROMCURVE_H
class CatMullRomCurveEvaluator : public CurveEvaluator {
public:
	void evaluateCurve(
		const vector<Point>& ptvCtrlPts,
		vector<Point>& pvtEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;
};
#endif