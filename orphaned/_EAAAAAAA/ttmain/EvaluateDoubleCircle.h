// EvaluateDoubleCircle.h: interface for the CEvaluateDoubleCircle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVALUATEDOUBLECIRCLE_H__997E8001_E655_11D4_8E7B_F42F30837B37__INCLUDED_)
#define AFX_EVALUATEDOUBLECIRCLE_H__997E8001_E655_11D4_8E7B_F42F30837B37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EvaluateCircle.h"

class CEvaluateDoubleCircle : public CEvaluateCircle  
{
public:
	double EvaluateAngleMid(double dRadius, double dFrameThickness);
	double EvalArcLength(double dRadius, double dChordLength, double dFrameThickness);
	BOOL EvalAngleWithFrame(double dRadius, double dChord, double dChordHeight, double dFrameThickness, double *pdFrameAngle, double *pdArcAngle);
	CEvaluateDoubleCircle();
	virtual ~CEvaluateDoubleCircle();

private:
    double  m_dRadius;              // Aussenradius des Bogens
    double  m_dFrameThickness;      // Dicke des Profils

};

#endif // !defined(AFX_EVALUATEDOUBLECIRCLE_H__997E8001_E655_11D4_8E7B_F42F30837B37__INCLUDED_)
