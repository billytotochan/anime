#include "catmullromcurveevaluator.h"

void CatMullRomCurveEvaluator::evaluateCurve(
	const vector<Point>& ptvCtrlPts,
	vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const {

	int numOfCtrlPts = ptvCtrlPts.size();

	if (numOfCtrlPts < 4){	// Not enough Control Points, Do Linear
		LinearCurveEvaluator linear;
		linear.evaluateCurve(ptvCtrlPts, ptvEvaluatedCurvePts, fAniLength, bWrap);
		return;
	}

	ptvEvaluatedCurvePts.clear();

	// Copy a local version of control points
	vector<Point> ctrl_pts;
	ctrl_pts.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());
	//Do wrapping
	if (bWrap) {
		Point wrapped_pt0(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y);
		ctrl_pts.push_back(wrapped_pt0);
		Point wrapped_pt1(ptvCtrlPts[numOfCtrlPts - 1].x - fAniLength,
			ptvCtrlPts[numOfCtrlPts - 1].y);
		ctrl_pts.insert(ctrl_pts.begin(), wrapped_pt1);
		numOfCtrlPts = ctrl_pts.size();
	}

	//correcting the tangent for the first and last interpolation point
	//First
	Point tmp0(2 * ctrl_pts[0].x - ctrl_pts[1].x,
		2 * ctrl_pts[0].y - ctrl_pts[1].y);
	ctrl_pts.insert(ctrl_pts.begin(), tmp0);

	//Last
	Point tmp1(2 * ctrl_pts[numOfCtrlPts - 1].x - ctrl_pts[numOfCtrlPts - 2].x,
		2 * ctrl_pts[numOfCtrlPts - 1].y - ctrl_pts[numOfCtrlPts - 2].y);
	ctrl_pts.push_back(tmp1);

	numOfCtrlPts = ctrl_pts.size();

	int index = 0;
	int step_length = 1;
	int n_sample = 100;
	Mat4f cat_matrix(-1, 3, -3, 1,
		2, -5, 4, -1,
		-1, 0, 1, 0,
		0, 2, 0, 0); 
		cat_matrix /= 2;
	//For each four points, we do sample the curve
	//In C_0 continuity
		for (; index < numOfCtrlPts - 3; index += step_length) {
		//Get the four point group
		Point p1 = ctrl_pts[index];
		Point p2 = ctrl_pts[index + 1];
		Point p3 = ctrl_pts[index + 2];
		Point p4 = ctrl_pts[index + 3];

		Vec4f x_val(p1.x, p2.x, p3.x, p4.x);
		Vec4f y_val(p1.y, p2.y, p3.y, p4.y);

		x_val = cat_matrix*x_val;
		y_val = cat_matrix*y_val;

		//pick samples
		for (int i = 0; i<n_sample; i++) {
			//length betwen each sample
			float t = i / (float)n_sample;
			float cat_x = t*t*t * x_val[0] + t*t*x_val[1] + t*x_val[2] + x_val[3];
			float cat_y = t*t*t * y_val[0] + t*t*y_val[1] + t*y_val[2] + y_val[3];

			if (cat_x < 0 || cat_x > fAniLength)
				continue;
			Point curr_pt(cat_x, cat_y);
			ptvEvaluatedCurvePts.push_back(curr_pt);
		}
	}

	if (!bWrap){
		Point first_pt(0, ptvCtrlPts[0].y);
		ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin(), first_pt);

		Point last_pt(fAniLength, ptvCtrlPts[ptvCtrlPts.size() - 1].y);
		ptvEvaluatedCurvePts.push_back(last_pt);
	}

}
