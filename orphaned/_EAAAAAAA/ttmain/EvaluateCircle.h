// EvaluateCircle.h: interface for the CEvaluateCircle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVALUATECIRCLE_H__BAAE0F25_DE7B_11D4_8E7B_ABD7149E7437__INCLUDED_)
#define AFX_EVALUATECIRCLE_H__BAAE0F25_DE7B_11D4_8E7B_ABD7149E7437__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <math.h>

const double g_Pi = 3.141592654;


class CEvaluateCircle  
{
public:
	double EvalY(double dRadius, double dX);
	double DegToRad(double dDegVal);
	double RadToDeg(double dRadVal);
	double EvalTangentAngle(double dRadius, double dChordLength);
	double EvalChordHeight(double dRadius, double dChordLength);
	double EvalChordLength(double dRadius, double dChordHeight);
	double EvalArcLength(double dRadius, double dChordLength);
	double EvalRadius(double  dChordLength, double dChordHeight);
	CEvaluateCircle();
	virtual ~CEvaluateCircle();

};

#endif // !defined(AFX_EVALUATECIRCLE_H__BAAE0F25_DE7B_11D4_8E7B_ABD7149E7437__INCLUDED_)
