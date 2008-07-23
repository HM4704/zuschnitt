// EvaluateDoubleCircle.cpp: implementation of the CEvaluateDoubleCircle class.
//
//////////////////////////////////////////////////////////////////////

// 24.01.2001: Verusch zur Kommentireung für Checkin

#include "stdafx.h"
#include "EvaluateDoubleCircle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEvaluateDoubleCircle::CEvaluateDoubleCircle()
{
    m_dRadius = 0;
    m_dFrameThickness = 0;
}

CEvaluateDoubleCircle::~CEvaluateDoubleCircle()
{

}


// Function name	: CEvaluateDoubleCircle::EvalAngleWithFrame
// Description	    : berechnet Winkel in der Mitte und am Rand eines 
//                    Torbogens
// Return type		: BOOL 
// Argument         : double dRadius            Außenradius des Bogens
// Argument         : double dChord             Sehnenlänge
// Argument         : double dChordHeight       Sehnenhöhe
// Argument         : double dFrameThickness    Dicke des Bogens
// Argument         : double *pdFrameAngle      Winkel außen am senkrechten Träger
// Argument         : double *pdArcAngle        Winkel außen am Bogen
BOOL CEvaluateDoubleCircle::EvalAngleWithFrame(double dRadius, double dChord, double dChordHeight, double dFrameThickness,
                                               double *pdFrameAngle, double *pdArcAngle)
{
    BOOL bRet = TRUE;
    double dChordX = dChord/2;
    double dAngleTangent = EvalTangentAngle(dRadius, dChord);

    double dRoot = sqrt(dRadius*dRadius-2*dRadius*dFrameThickness+
        2*dChordX*dFrameThickness-dChordX*dChordX);

    // Seite a des Dreiecks
    double dA = dRadius - dChordHeight - dRoot;
    // Winkel für senkrechte Träger
    double dAngleVert = atan(dA/dFrameThickness);
    dAngleVert = RadToDeg(dAngleVert);
    if (dAngleVert <= 45)
    {
        // alles ok
        *pdFrameAngle = dAngleVert;
        bRet = TRUE;
    }
    else
    {
        //Vorsicht!!! Innenbogen laenger als Aussenbogen
        *pdFrameAngle = INFINITE;
        bRet = FALSE;
    }

    double dAngleArc = 90 - dAngleVert - dAngleTangent;
    *pdArcAngle = dAngleArc;

    return bRet;
}

// alt????
#if 0
double CEvaluateDoubleCircle::EvalArcLength(double dRadius, double dChordLength, double dFrameThickness)
{
    double dArcLength, dCorr;

    double dChord = dChordLength/2;

    /* Faktor */
    double dK = 0.65+.05*log(dRadius/dFrameThickness);

    /* Schenkel dA (Tangenten in den Endpukten) */
    double dTangentAngle = DegToRad(EvalTangentAngle(dRadius, 
                                 dChordLength));
    double dA = dChord/cos(dTangentAngle);
    
    /* Winkel des Bogens */
    double dAngle = asin(dChord/dRadius);
    dAngle = dAngle*2;
    dAngle = RadToDeg(dAngle);

    /* 0° ... 90° */
    if (dAngle <= 90)
    {
        dCorr = 2*(dRadius+dFrameThickness)-g_Pi*((180-dAngle)/180)
            *(dRadius+dFrameThickness/2*dK);
    }
    /* 90° ... 165° */
    else if (dAngle <= 0.917*g_Pi)
    {
        dCorr = 2*(dRadius+dFrameThickness)*tan((g_Pi-dAngle)/2)-
            g_Pi*((g_Pi-dAngle)/g_Pi)*(dRadius+dFrameThickness/2*dK);
    }
    /* 165° ... 180° */
    else
    {
        dCorr = 0;
    }

    dArcLength = 2*dA-dCorr;
    return dArcLength;
}
#endif



// Function name	: CEvaluateDoubleCircle::EvalArcLength
// Description	    : Berechnet Bogenlänge; Annahme: Winkel an den
//                    Rändern ändert sich nicht (90°)
// Return type		: double 
// Argument         : double dRadius    Radius des äuren Bogens
// Argument         : double dChordLength  Sehne des äußeren Kreise!!
// Argument         : double dFrameThickness  Dicke des Bogens
double CEvaluateDoubleCircle::EvalArcLength(double dRadius, double dChordLength, double dFrameThickness)
{
#if 0
    /* 
    AchtungAnnahme: Beim Biegen ändert sich der Winkel an den Schnittfläachen
                        nicht (bleibe 90°)
    */
    double dArcLength;
    double dMiddleRadius = dRadius - dFrameThickness/2;
    double dChord = dChordLength/2;

    double dTangentAngle = EvalTangentAngle(dRadius, dChordLength);
    dTangentAngle = DegToRad(dTangentAngle);
    double dMiddleChord = dChord - (dFrameThickness*cos(g_Pi/2-dTangentAngle));

    dArcLength = 2*dMiddleRadius*asin(dMiddleChord/dMiddleRadius);
#endif
    double dArcLength;
    double dChord = dChordLength/2;
    
    double dAngle = 2*asin(dChord/dRadius);
    
    dArcLength = (dAngle/(2*g_Pi))*2*dRadius*g_Pi;
    return dArcLength;
}


// Function name	: CEvaluateDoubleCircle::EvaluateAngleMid
// Description	    : berechnet den Schnittwinkel in der Mitte des Bogens
// Return type		: double 
// Argument         : double dRadius           Außenradius des Kreises
// Argument         : double dFrameThickness   Bogendicke
double CEvaluateDoubleCircle::EvaluateAngleMid(double dRadius, double dFrameThickness)
{
    double dAngle = dFrameThickness / (dRadius-/*+??*/
                    sqrt(dRadius*dRadius-2*dRadius*dFrameThickness));

    dAngle = RadToDeg(atan(dAngle));
    return dAngle;
}
