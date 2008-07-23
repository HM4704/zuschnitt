// EvaluateCircle.cpp: implementation of the CEvaluateCircle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ttmain.h"
#include "EvaluateCircle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEvaluateCircle::CEvaluateCircle()
{

}

CEvaluateCircle::~CEvaluateCircle()
{

}


// Function name	: CEvaluateCircle::EvalRadius
// Description	    : berechnet Radius (idealer Kreis)
// Return type		: double 
// Argument         : double dChordLength   Sehnenlänge
// Argument         : double dChordHeight   Sehnenhöhe
double CEvaluateCircle::EvalRadius(
                                   double dChordLength, 
                                   double dChordHeight
                                   )
{
    double dChord = dChordLength/2;
    double dRadius = (dChordHeight*dChordHeight + dChord*dChord)
                    / (2*dChordHeight);

    return dRadius;
}



// Function name	: CEvaluateCircle::EvalArcLength
// Description	    : berechnet ideale Bogenlänge eines Kreises
// Return type		: double 
// Argument         : double dRadius
// Argument         : double dChordLength
double CEvaluateCircle::EvalArcLength(
                                      double dRadius, 
                                      double dChordLength
                                     )
{
    double dChord = dChordLength/2;
    
    double dAngle = asin(dChord/dRadius);
    double dArcLength = (2*dAngle / g_Pi) * dRadius*g_Pi;

    return dArcLength;
}


// Function name	: CEvaluateCircle::EvalChordLength
// Description	    : berechnet gesamte Sehnenlänge
// Return type		: double 
// Argument         : double dRadius
// Argument         : double dChordHeight
double CEvaluateCircle::EvalChordLength(
                                        double dRadius, 
                                        double dChordHeight
                                        )
{
    double dChordLength = sqrt(2*dRadius*dChordHeight
                         - dChordHeight*dChordHeight);

    return 2*dChordLength;
}


// Function name	: CEvaluateCircle::EvalChordHeight
// Description	    : berechnet Sehnenhöhe
// Return type		: double 
// Argument         : double dRadius
// Argument         : double dChordLength
double CEvaluateCircle::EvalChordHeight(
                                        double dRadius, 
                                        double dChordLength
                                        )
{
    double dChord = dChordLength/2;
    double dChordHeight = dRadius - sqrt(dRadius*dRadius - dChord*dChord);
    
    return dChordHeight;
}


// Function name	: CEvaluateCircle::EvalTangentAngle
// Description	    : berechnet Winkel der Tangente an einem Punkt des 
//                    Kreises  x = Sehnenlänge (wird durch 2 geteilt!!)
// Return type		: double 
// Argument         : double dRadius        Radius
// Argument         : double dChordLength   x : Sehnenlänge*2!!!
double CEvaluateCircle::EvalTangentAngle(
                                        double dRadius, 
                                        double dChordLength
                                        )
{
    double dChord = dChordLength/2;
    
    double dAngle = fabs(atan(-dChord/sqrt(dRadius*dRadius-dChord*dChord)));

    // Umrechnen in Grad
    dAngle = RadToDeg(dAngle);
    return dAngle;
}


// Function name	: CEvaluateCircle::RadToDeg
// Description	    : Umrechnung Rad nach Deg
// Return type		: double 
// Argument         : double dRadVal
double CEvaluateCircle::RadToDeg(double dRadVal)
{
    return dRadVal*180/g_Pi;
}


// Function name	: CEvaluateCircle::DegToRad
// Description	    : Umrechnung Deg nach Rad
// Return type		: double 
// Argument         : double dDegVal
double CEvaluateCircle::DegToRad(double dDegVal)
{
    return dDegVal*g_Pi/180;
}



// Function name	: CEvaluateCircle::EvalY
// Description	    : berechnet Y-Wert an einem Punkt x
// Return type		: double 
// Argument         : double dRadius    Radius des Kreises
// Argument         : double dX         x-Wert
double CEvaluateCircle::EvalY(double dRadius, double dX)
{
    double dY = sqrt(dRadius*dRadius - dX*dX);
    return dY;
}
