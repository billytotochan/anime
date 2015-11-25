#include "beziercurveevaluator.h"

void BezierCurveEvaluator::evaluateCurve(
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

	int index = 0;
	int step_length = 3;
	int n_sample = 100;

	//Do wrapping
	if (bWrap) {
		/*
		for (int i = 0; i<3; i++) {
			Point wrapped_pt(ptvCtrlPts[i].x + fAniLength, ptvCtrlPts[i].y);
			ctrl_pts.push_back(wrapped_pt);
		}
		Point wrapped_pt(ptvCtrlPts[numOfCtrlPts - 1].x - fAniLength,
			ptvCtrlPts[numOfCtrlPts - 1].y);
		ctrl_pts.insert(ctrl_pts.begin(), wrapped_pt);
		*/

		Point wrapped_pt(ptvCtrlPts[0].x + fAniLength,
			ptvCtrlPts[0].y);
		ctrl_pts.push_back(wrapped_pt);

	}

	numOfCtrlPts = ctrl_pts.size();
	Mat4f beizer_basis_matrix(-1, 3, -3, 1,
		3, -6, 3, 0,
		-3, 3, 0, 0,
		1, 0, 0, 0);
	//For each four points, we do sample the curve
	//In C_0 continuity

	int front = 0;

	for (; index < numOfCtrlPts - 3; index += step_length) {
		//Get the four point group
		Point p1 = ctrl_pts[index];
		Point p2 = ctrl_pts[index + 1];
		Point p3 = ctrl_pts[index + 2];
		Point p4 = ctrl_pts[index + 3];

		Vec4f x_val(p1.x, p2.x, p3.x, p4.x);
		Vec4f y_val(p1.y, p2.y, p3.y, p4.y);

		x_val = beizer_basis_matrix*x_val;
		y_val = beizer_basis_matrix*y_val;

		//pick samples
		for (int i = 0; i<n_sample; i++) {
			//length betwen each sample
			float t = i / (float)n_sample;
			float bezier_x = t*t*t * x_val[0] + t*t*x_val[1] + t*x_val[2] + x_val[3];
			float bezier_y = t*t*t * y_val[0] + t*t*y_val[1] + t*y_val[2] + y_val[3];

			if (bezier_x > fAniLength){
				Point curr_pt(bezier_x - fAniLength, bezier_y);
				ptvEvaluatedCurvePts.insert( ptvEvaluatedCurvePts.begin() + front, curr_pt);
				front++;
			}
			else{
				Point curr_pt(bezier_x, bezier_y);
				ptvEvaluatedCurvePts.push_back(curr_pt);
			}


		}
	}

	for (; index < numOfCtrlPts; index++){
		Point p = ctrl_pts[index];
		ptvEvaluatedCurvePts.push_back(p);
	}
	if (!bWrap){
		Point first_pt(0, ctrl_pts[0].y);
		ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() , first_pt);

		Point last_pt(fAniLength, ctrl_pts[numOfCtrlPts - 1].y);
		ptvEvaluatedCurvePts.push_back(last_pt);
	}
	else {
		if (numOfCtrlPts % 4 == 3)
			return;
		float y1;
		if ((ptvCtrlPts[0].x + fAniLength) - ptvCtrlPts[ptvCtrlPts.size() - 1].x > 0.0f) {
			y1 = (ptvCtrlPts[0].y * (fAniLength - ptvCtrlPts[ptvCtrlPts.size() - 1].x) +
				ptvCtrlPts[ptvCtrlPts.size() - 1].y * ptvCtrlPts[0].x) /
				(ptvCtrlPts[0].x + fAniLength - ptvCtrlPts[ptvCtrlPts.size() - 1].x);
		}
		else
			y1 = ptvCtrlPts[0].y;
		Point p(0, y1);
		ptvEvaluatedCurvePts.insert( ptvEvaluatedCurvePts.begin(), p);
	}
		

	

}
