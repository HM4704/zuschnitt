#include "stdafx.h"
#include <string.h>
#include <stdio.h>
//#include <math.h>
#include "datatype.h"
#include "ttdata.h"
#include "ttmainDoc.h"
#include "ttmainView.h"
#include "EvaluateDoubleCircle.h"
#include "TorN.h"


#pragma warning( push )
#pragma warning( disable : 4244 )

//************************************************
// Klasse CTorN
//************************************************
void CTorN::drawTT(HDC hdc, int x, int y, int b, int h)
{
   int i;
   scF = calculateScale(b, h);
   aSp =  max(3, rnd(scF*3));
   iSp =  max(3, rnd(scF*3));
   int Sp = aSp;


    
   Size.ZBreite = 0;
   for (i=0; i<FluegelAnz; i++)
   {
     CFlParam* pFl = (CFlParam*)FlParam->GetAt(i);
     pFl->iZBreite = scF*pFl->iBreite/10;
     Size.ZBreite += pFl->iZBreite;
   }
   Size.ZBreite += 2*aSp+(FluegelAnz-1)*iSp;

   if (bBogen == TRUE)
   {
       POINT points[4];
       double dRadius;
       CEvaluateCircle circMath;

       int Fluegel = ( scF*Size.Breite-2*aSp-(FluegelAnz-1)*iSp)/FluegelAnz;

       //int SehnenHoehe = scF*SEHNENHOEHE; // fest, da Probleme beim Zeichnen!!!!
       double dSehnenHoehe = scF*Size.SehnenHoehe+aSp;
       dRadius = circMath.EvalRadius(Size.ZBreite+2*aSp, dSehnenHoehe-aSp);

       points[0].x = x-aSp; 
       points[0].y = -((y-aSp)+dRadius-circMath.EvalY(dRadius, Size.ZBreite/2+aSp));
       points[1].x = x-Sp;               points[1].y = -(y+dSehnenHoehe+scF*Size.Hoehe+aSp);
       points[2].x = x+Size.ZBreite+aSp; points[2].y = -(y+dSehnenHoehe+scF*Size.Hoehe+aSp);
       points[3].x = x+Size.ZBreite+aSp; 
       points[3].y = -((y-aSp)+dRadius-circMath.EvalY(dRadius, Size.ZBreite/2+aSp));
       drawConnectedPoints(hdc, points, 4);
//       dRadius = circMath.EvalRadius(Size.ZBreite+2*aSp, dSehnenHoehe);
       double dY = -((y-aSp)+dRadius-circMath.EvalY(dRadius, Size.ZBreite/2+aSp));
       Arc(hdc, x+Size.ZBreite/2-dRadius-aSp, -(y-aSp), 
           x+Size.ZBreite/2+dRadius+aSp, -(y-aSp+2*dRadius),
           x+Size.ZBreite+aSp, dY, x-2*Sp, dY);

       dSehnenHoehe = scF*Size.SehnenHoehe;
//       dRadius = circMath.EvalRadius(Size.ZBreite, dSehnenHoehe);
       points[0].x = x; 
       points[0].y = -(y+dRadius-circMath.EvalY(dRadius, Size.ZBreite/2));
       points[1].x = x;              points[1].y = -(y+dSehnenHoehe+aSp+scF*Size.Hoehe);
       points[2].x = x+Size.ZBreite; points[2].y = -(y+dSehnenHoehe+aSp+scF*Size.Hoehe);
       points[3].x = x+Size.ZBreite; 
       points[3].y = -(y+dRadius-circMath.EvalY(dRadius, Size.ZBreite/2));
       drawConnectedPoints(hdc, points, 4);
//       dRadius = circMath.EvalRadius(Size.ZBreite, dSehnenHoehe);
       dY = -(y+dRadius-circMath.EvalY(dRadius, Size.ZBreite/2));
       Arc(hdc, x+Size.ZBreite/2-dRadius, -(y), 
           x+Size.ZBreite/2+dRadius, -(y+2*dRadius),
           x+Size.ZBreite, dY, x, dY);

       dSehnenHoehe = scF*Size.SehnenHoehe-aSp;
//       dRadius = circMath.EvalRadius(FluegelAnz*(Fluegel+iSp), scF*Size.SehnenHoehe-aSp);
//       dRadius = circMath.EvalRadius(FluegelAnz*(Fluegel+iSp), scF*Size.SehnenHoehe-aSp);
       double dX = -(Fluegel+iSp);
       for (i = 0; i<FluegelAnz; i++)
       {
           points[0].x = x+aSp+i*Fluegel+i*iSp; 
           points[0].y = -((y+aSp)+dRadius-circMath.EvalY(dRadius, dX));
           dX += Fluegel;
           points[1].x = x+aSp+i*Fluegel+i*iSp; 
           points[1].y = -(y+dSehnenHoehe+scF*Size.Hoehe+aSp);
           points[2].x = x+aSp+(i+1)*Fluegel+i*iSp; 
           points[2].y = -(y+dSehnenHoehe+scF*Size.Hoehe+aSp);
           points[3].x = x+aSp+(i+1)*Fluegel+i*iSp; 
           points[3].y = -((y+aSp)+dRadius-circMath.EvalY(dRadius, dX));
           drawConnectedPoints(hdc, points, 4);
           dX += 2*iSp;
           
           double x1 = x+Size.ZBreite/2-dRadius; //x+aSp+Fluegel+(i+1)*iSp-dRadius;
           double y1 = -(y+iSp);
           double x2 = x1+2*dRadius;
           double y2 = -(y+iSp+2*dRadius);                          
           double ya = -((y+aSp)+dRadius-circMath.EvalY(dRadius, iSp));
           double yb = -((y+aSp)+dRadius-circMath.EvalY(dRadius, iSp+Fluegel));
           if (i==0)
           {
               double xa = x+Size.ZBreite/2-iSp;
               double xb = x+iSp;
               Arc(hdc, x1, y1, x2, y2, xa, ya, xb, yb);
           }
           else
           {
               double xa = x+aSp+i*Fluegel+i*iSp;//x+Size.ZBreite/2+iSp;
               double xb = x+aSp+(i+1)*Fluegel+i*iSp;//x+3*iSp+2*Fluegel;
               Arc(hdc, x1, y1, x2, y2, xb, yb, xa, ya);
           }
       }
   }
   else
   {
       Rectangle(hdc, x-aSp, -(y-aSp), x+Size.ZBreite+aSp, -(y+ scF*Size.Hoehe+aSp));
       Rectangle(hdc, x, -y, x+Size.ZBreite, -(y+ scF*Size.Hoehe));
       int iPosX = 0;
       for (int i = 0; i<FluegelAnz; i++)
       {
            CFlParam* pFl = (CFlParam*)FlParam->GetAt(i);
            Rectangle(hdc, x+aSp+iPosX+i*iSp, -(y+aSp), x+aSp+(iPosX+pFl->iZBreite)+i*iSp, -(y+ scF*Size.Hoehe-aSp));
            if (pFl->iBreite/10 != Size.Breite/FluegelAnz)
            {
                // ungleiche Fluegel, Breite ausgeben
                char szTemp[20];
                itoa(pFl->iBreite, szTemp, 10);
                OutputAligned(hdc, E_ALIGN_MID, szTemp, 
                  x+aSp+iPosX+i*iSp+pFl->iZBreite/2, -(y+ scF*Size.Hoehe-aSp-60));
            }

            iPosX += pFl->iZBreite;
       }
   }

    int DINx;
    CFlParam* FluegelP;
    if (DIN == L)
    {
       FluegelP = (CFlParam*)FlParam->GetAt(0);
       DINx = x+aSp+FluegelP->iZBreite- scF*(DINBREITE+3);
    }
    else
    {
        FluegelP = (CFlParam*)FlParam->GetAt(0);
        DINx = x+aSp+(FluegelAnz-1)*(FluegelP->iZBreite+iSp)+ scF*3;
    }
    int DINy;

    if (FluegelP->FArt != KEIN || FluegelP->OArt != LEER
        || FluegelP->Geteilt == TRUE)
    {
       DINy = y + scF*(Size.ObtHoehe+Size.SehnenHoehe+10);
    }
    else
    {
       DINy = y+ Size.ZGesamtHoehe-scF*(OEFFNERPOSY);
    }
    DINy = __max(DINy, y+ Size.ZGesamtHoehe-scF*(OEFFNERPOSY));
    drawDIN(hdc, DINx, DINy);

   drawBesch(hdc, x, y);

   if (bBogen == TRUE)
        y += scF*Size.SehnenHoehe;
   int count = FlParam->GetSize();
   int iPosX = 0;
   for (i=0; i<count; i++)
   {
     int abstand, iBreite;
	 CFlParam* FluegelP = (CFlParam*)FlParam->GetAt(i);
     iBreite = FluegelP->iBreite/10;
     if (FluegelP->Geteilt == TRUE)
	 Rectangle(hdc, x+aSp+iPosX+i*iSp, -(y+ scF*Size.ObtHoehe),
		   x+aSp+(iPosX+FluegelP->iZBreite)+i*iSp, -(y+ scF*Size.ObtHoehe+aSp));
     switch(FluegelP->OArt)
     {
       case OzO:
	 Rectangle(hdc, x+aSp+iPosX+i*iSp, -(y+ scF*Size.ObtHoehe-Sp),
		   x+aSp+(iPosX+FluegelP->iZBreite)+i*iSp, -(y+ (scF*Size.ObtHoehe)));
	 Rectangle(hdc, x+aSp+iPosX+i*iSp, -(y+ scF*Size.ObtHoehe),
		   x+aSp+(iPosX+FluegelP->iZBreite)+i*iSp, -(y+ scF*Size.ObtHoehe+Sp));
	 break;
       case OiF:
	 abstand = rnd(scF*115/20);
	 drawOIF(hdc, x+aSp+iPosX+i*iSp+abstand, y+aSp, FluegelP->iZBreite-2*abstand,
		  scF*Size.ObtHoehe, i==1);
	 break;
       default:
         break;
     }
     switch (FluegelP->FArt)
     {
       case OlF:
       case SF:
       case KF:
       case SF2:
	 Rectangle(hdc, x+aSp+iPosX+i*iSp, -(y+ scF*Size.ObtHoehe),
		   x+aSp+iPosX+FluegelP->iZBreite+i*iSp, -(y+ (scF*Size.ObtHoehe)+aSp));
	 if (FluegelP->FArt == SF)
	 {
	   MoveToEx(hdc, x+iPosX+FluegelP->iZBreite/2, -(y+ scF*(Size.Hoehe/2-50)), NULL);
	   LineTo(hdc, x+iPosX+FluegelP->iZBreite/2, -(y+ scF*(Size.Hoehe/2+10)));
	   int xy =  scF*SFPFXY;
	   LineTo(hdc, x+iPosX+FluegelP->iZBreite/2-xy, -(y+ scF*(Size.Hoehe/2+10)-xy));
	   MoveToEx(hdc, x+iPosX+FluegelP->iZBreite/2, -(y+ scF*(Size.Hoehe/2+10)), NULL);
	   LineTo(hdc, x+iPosX+FluegelP->iZBreite/2+xy, -(y+ scF*(Size.Hoehe/2+10)-xy));
	 }
	 else
	   if (FluegelP->FArt == SF2)
	   {
	     int versX = 0;
	     Rectangle(hdc, x+iPosX+FluegelP->iZBreite/2, -(y+aSp),
		   x+iPosX+FluegelP->iZBreite/2+aSp, -(y+scF*Size.ObtHoehe));
	     for (int n=1; n<=2; n++)
	     { 
	      MoveToEx(hdc, x+iPosX+FluegelP->iZBreite/4+versX, -(y+ scF*(Size.Hoehe/2-50)), NULL);
	      LineTo(hdc, x+iPosX+FluegelP->iZBreite/4+versX, -(y+ scF*(Size.Hoehe/2+10)));
	      int xy =  scF*SFPFXY;
	      LineTo(hdc, x+iPosX+FluegelP->iZBreite/4-xy+versX, -(y+ scF*(Size.Hoehe/2+10)-xy));
	      MoveToEx(hdc, x+iPosX+FluegelP->iZBreite/4+versX, -(y+ scF*(Size.Hoehe/2+10)), NULL);
	      LineTo(hdc, x+iPosX+FluegelP->iZBreite/4+xy+versX, -(y+ scF*(Size.Hoehe/2+10)-xy));
	      versX += FluegelP->iZBreite/2;
	     }
	   }
	 else
	   if (FluegelP->FArt == KF)
	   {
	     MoveToEx(hdc, x+aSp+iPosX+i*iSp, -(y+ scF*Size.ObtHoehe), NULL);
	     LineTo(hdc, x+iPosX+FluegelP->iZBreite/2, -(y+aSp));
	     LineTo(hdc, x+aSp+(iPosX+FluegelP->iZBreite)+i*iSp, -(y+ scF*Size.ObtHoehe));
	   }
       break;
       default: break;
     }
     iPosX+= FluegelP->iZBreite;
   }

   y +=  scF*(Size.Hoehe+BEMABSTAND+3) +  30;
}


void CTorN::drawBesch(HDC hdc, int x, int y)
{
   CFlParam* FluegelP1;
   CFlParam* FluegelP2;
   BOOL hasOzO = FALSE;
   int iStartY = 0;

   if (bBogen == TRUE)
     iStartY = scF*Size.SehnenHoehe;
   else
     iStartY = 0;

   float scFH = scF;
   //scF = 1.0;
   int beschBreite =  scF*BESCHBREITE;
   Rectangle(hdc, x-beschBreite, -(y+iStartY+scFH*BESCHTORY), x,
		  -(y+iStartY+scFH*(BESCHTORY+BESCHHOEHE)));

   int count = FlParam->GetSize();
   if (count > 0)
   {
     FluegelP1 = (CFlParam*)FlParam->GetAt(0);
     FluegelP2 = (CFlParam*)FlParam->GetAt(1);
     if (FluegelP1->OArt == OzO || FluegelP2->OArt == OzO)
     {
         hasOzO = TRUE;
	 Rectangle(hdc, x-beschBreite, -(y+iStartY+scFH*((Size.ObtHoehe-BESCHHOEHE/2))),
		  x, -(y+iStartY+scFH*(Size.ObtHoehe+BESCHHOEHE/2)));
     }
     else
       Rectangle(hdc, x-beschBreite, -(y+iStartY+scFH*((Size.Hoehe-BESCHHOEHE)/2)),
		  x, -(y+iStartY+scFH*(Size.Hoehe+BESCHHOEHE)/2));
   }
   else
     Rectangle(hdc, x-beschBreite, -(y+iStartY+scFH*((Size.Hoehe-BESCHHOEHE)/2)),
		  x, -(y+iStartY+scFH*(Size.Hoehe+BESCHHOEHE)/2));
   Rectangle(hdc, x-beschBreite, -(y+iStartY+scFH*(Size.Hoehe-BESCHTORY-BESCHHOEHE)),
		  x, -(y+iStartY+scFH*(Size.Hoehe-BESCHTORY)));

   Rectangle(hdc, x+Size.ZBreite, -(y+iStartY+scFH*BESCHTORY),
		  x+Size.ZBreite+beschBreite, -(y+iStartY+scFH*(BESCHTORY+BESCHHOEHE)));

   if (hasOzO == TRUE)
      Rectangle(hdc, x+Size.ZBreite, -(y+iStartY+scFH*((Size.ObtHoehe-BESCHHOEHE/2))),
		  x+Size.ZBreite+beschBreite, -(y+iStartY+scFH*(Size.ObtHoehe+BESCHHOEHE/2)));
   else
      Rectangle(hdc, x+Size.ZBreite, -(y+iStartY+scFH*((Size.Hoehe-BESCHHOEHE)/2)),
		  x+Size.ZBreite+beschBreite, -(y+iStartY+scFH*(Size.Hoehe+BESCHHOEHE)/2));
   Rectangle(hdc, x+Size.ZBreite, -(y+iStartY+scFH*(Size.Hoehe-BESCHTORY-BESCHHOEHE)),
		  x+Size.ZBreite+beschBreite, -(y+iStartY+scFH*(Size.Hoehe-BESCHTORY)));
}


//************************************************
// Klasse CSchiebeTor
//************************************************
void CSchiebeTor::drawTT(HDC hdc, int x, int y, int b, int h)
{
//   int i;
   scF = calculateScale(b, h);
   aSp =  max(3, rnd(scF*3));
   iSp =  max(3, rnd(scF*3));
   int Sp = aSp;


   int Fluegel = ( scF*Size.Breite-2*aSp-(FluegelAnz-1)*iSp)/FluegelAnz;
   Size.ZBreite = FluegelAnz*Fluegel+2*aSp+(FluegelAnz-1)*iSp;
#if 0
   if (bBogen == TRUE)
   {
       POINT points[4];
       double dRadius;
       CEvaluateCircle circMath;

       //int SehnenHoehe = scF*SEHNENHOEHE; // fest, da Probleme beim Zeichnen!!!!
       double dSehnenHoehe = scF*Size.SehnenHoehe+aSp;
       dRadius = circMath.EvalRadius(Size.ZBreite+2*aSp, dSehnenHoehe-aSp);
#if 1
       points[0].x = x-aSp; 
       points[0].y = -((y-aSp)+dRadius-circMath.EvalY(dRadius, Size.ZBreite/2+aSp));
       points[1].x = x-Sp;               points[1].y = -(y+dSehnenHoehe+scF*Size.Hoehe+aSp);
       points[2].x = x+Size.ZBreite+aSp; points[2].y = -(y+dSehnenHoehe+scF*Size.Hoehe+aSp);
       points[3].x = x+Size.ZBreite+aSp; 
       points[3].y = -((y-aSp)+dRadius-circMath.EvalY(dRadius, Size.ZBreite/2+aSp));
       drawConnectedPoints(hdc, points, 4);
//       dRadius = circMath.EvalRadius(Size.ZBreite+2*aSp, dSehnenHoehe);
       double dY = -((y-aSp)+dRadius-circMath.EvalY(dRadius, Size.ZBreite/2+aSp));
       Arc(hdc, x+Size.ZBreite/2-dRadius-aSp, -(y-aSp), 
           x+Size.ZBreite/2+dRadius+aSp, -(y-aSp+2*dRadius),
           x+Size.ZBreite+aSp, dY, x-2*Sp, dY);
#endif
#if 1
       dSehnenHoehe = scF*Size.SehnenHoehe;
//       dRadius = circMath.EvalRadius(Size.ZBreite, dSehnenHoehe);
       points[0].x = x; 
       points[0].y = -(y+dRadius-circMath.EvalY(dRadius, Size.ZBreite/2));
       points[1].x = x;              points[1].y = -(y+dSehnenHoehe+aSp+scF*Size.Hoehe);
       points[2].x = x+Size.ZBreite; points[2].y = -(y+dSehnenHoehe+aSp+scF*Size.Hoehe);
       points[3].x = x+Size.ZBreite; 
       points[3].y = -(y+dRadius-circMath.EvalY(dRadius, Size.ZBreite/2));
       drawConnectedPoints(hdc, points, 4);
//       dRadius = circMath.EvalRadius(Size.ZBreite, dSehnenHoehe);
       dY = -(y+dRadius-circMath.EvalY(dRadius, Size.ZBreite/2));
       Arc(hdc, x+Size.ZBreite/2-dRadius, -(y), 
           x+Size.ZBreite/2+dRadius, -(y+2*dRadius),
           x+Size.ZBreite, dY, x, dY);
#endif
#if 1
       dSehnenHoehe = scF*Size.SehnenHoehe-aSp;
//       dRadius = circMath.EvalRadius(FluegelAnz*(Fluegel+iSp), scF*Size.SehnenHoehe-aSp);
//       dRadius = circMath.EvalRadius(FluegelAnz*(Fluegel+iSp), scF*Size.SehnenHoehe-aSp);
       double dX = -(Fluegel+iSp);
       for (i = 0; i<FluegelAnz; i++)
       {
           points[0].x = x+aSp+i*Fluegel+i*iSp; 
           points[0].y = -((y+aSp)+dRadius-circMath.EvalY(dRadius, dX));
           dX += Fluegel;
           points[1].x = x+aSp+i*Fluegel+i*iSp; 
           points[1].y = -(y+dSehnenHoehe+scF*Size.Hoehe+aSp);
           points[2].x = x+aSp+(i+1)*Fluegel+i*iSp; 
           points[2].y = -(y+dSehnenHoehe+scF*Size.Hoehe+aSp);
           points[3].x = x+aSp+(i+1)*Fluegel+i*iSp; 
           points[3].y = -((y+aSp)+dRadius-circMath.EvalY(dRadius, dX));
           drawConnectedPoints(hdc, points, 4);
           dX += 2*iSp;
           
           double x1 = x+Size.ZBreite/2-dRadius; //x+aSp+Fluegel+(i+1)*iSp-dRadius;
           double y1 = -(y+iSp);
           double x2 = x1+2*dRadius;
           double y2 = -(y+iSp+2*dRadius);                          
           double ya = -((y+aSp)+dRadius-circMath.EvalY(dRadius, iSp));
           double yb = -((y+aSp)+dRadius-circMath.EvalY(dRadius, iSp+Fluegel));
           if (i==0)
           {
               double xa = x+Size.ZBreite/2-iSp;
               double xb = x+iSp;
               Arc(hdc, x1, y1, x2, y2, xa, ya, xb, yb);
           }
           else
           {
               double xa = x+aSp+i*Fluegel+i*iSp;//x+Size.ZBreite/2+iSp;
               double xb = x+aSp+(i+1)*Fluegel+i*iSp;//x+3*iSp+2*Fluegel;
               Arc(hdc, x1, y1, x2, y2, xb, yb, xa, ya);
           }
       }
#endif
   }
   else
#endif  // if 0

   {
       Rectangle(hdc, x-aSp, -(y-aSp), x+Size.ZBreite+aSp, -(y+ scF*Size.Hoehe+aSp));
       Rectangle(hdc, x, -y, x+Size.ZBreite, -(y+ scF*Size.Hoehe));
       for (int i = 0; i<FluegelAnz; i++)
         Rectangle(hdc, x+aSp+i*Fluegel+i*iSp, -(y+aSp), x+aSp+(i+1)*Fluegel+i*iSp, -(y+ scF*Size.Hoehe-aSp));
   }
#if 0
    int DINx;
    CFlParam* FluegelP;
    if (DIN == L)
    {
       FluegelP = (CFlParam*)FlParam->GetAt(0);
       DINx = x+aSp+Fluegel- scF*(DINBREITE+3);
    }
    else
    {
        DINx = x+aSp+(FluegelAnz-1)*(Fluegel+iSp)+ scF*3;
        FluegelP = (CFlParam*)FlParam->GetAt(1);
    }
    int DINy;

    if (FluegelP->FArt != KEIN || FluegelP->OArt != LEER
        || FluegelP->Geteilt == TRUE)
    {
       DINy = y + scF*(Size.ObtHoehe+Size.SehnenHoehe+10);
    }
    else
    {
       DINy = y+ Size.ZGesamtHoehe-scF*(OEFFNERPOSY);
    }
    DINy = __max(DINy, y+ Size.ZGesamtHoehe-scF*(OEFFNERPOSY));
    drawDIN(hdc, DINx, DINy);

   drawBesch(hdc, x, y);

   if (bBogen == TRUE)
        y += scF*Size.SehnenHoehe;
   int count = FlParam->GetSize();
   for (i=0; i<count; i++)
   {
     int abstand;
	 CFlParam* FluegelP = (CFlParam*)FlParam->GetAt(i);
     if (FluegelP->Geteilt == TRUE)
	 Rectangle(hdc, x+aSp+i*Fluegel+i*iSp, -(y+ scF*Size.ObtHoehe),
		   x+aSp+(i+1)*Fluegel+i*iSp, -(y+ scF*Size.ObtHoehe+aSp));
     switch(FluegelP->OArt)
     {
       case OzO:
	 Rectangle(hdc, x+aSp+i*Fluegel+i*iSp, -(y+ scF*Size.ObtHoehe-Sp),
		   x+aSp+(i+1)*Fluegel+i*iSp, -(y+ (scF*Size.ObtHoehe)));
	 Rectangle(hdc, x+aSp+i*Fluegel+i*iSp, -(y+ scF*Size.ObtHoehe),
		   x+aSp+(i+1)*Fluegel+i*iSp, -(y+ scF*Size.ObtHoehe+Sp));
	 break;
       case OiF:
	 abstand = rnd(scF*115/20);
	 drawOIF(hdc, x+aSp+i*Fluegel+i*iSp+abstand, y+aSp, Fluegel-2*abstand,
		  scF*Size.ObtHoehe, i==1);
	 break;
       default:
         break;
     }
     switch (FluegelP->FArt)
     {
       case OlF:
       case SF:
       case KF:
       case SF2:
	 Rectangle(hdc, x+aSp+i*Fluegel+i*iSp, -(y+ scF*Size.ObtHoehe),
		   x+aSp+(i+1)*Fluegel+i*iSp, -(y+ (scF*Size.ObtHoehe)+aSp));
	 if (FluegelP->FArt == SF)
	 {
	   MoveToEx(hdc, x+i*Fluegel+Fluegel/2, -(y+ scF*(Size.Hoehe/2-50)), NULL);
	   LineTo(hdc, x+i*Fluegel+Fluegel/2, -(y+ scF*(Size.Hoehe/2+10)));
	   int xy =  scF*SFPFXY;
	   LineTo(hdc, x+i*Fluegel+Fluegel/2-xy, -(y+ scF*(Size.Hoehe/2+10)-xy));
	   MoveToEx(hdc, x+i*Fluegel+Fluegel/2, -(y+ scF*(Size.Hoehe/2+10)), NULL);
	   LineTo(hdc, x+i*Fluegel+Fluegel/2+xy, -(y+ scF*(Size.Hoehe/2+10)-xy));
	 }
	 else
	   if (FluegelP->FArt == SF2)
	   {
	     int versX = 0;
	     Rectangle(hdc, x+i*Fluegel+Fluegel/2, -(y+aSp),
		   x+i*Fluegel+Fluegel/2+aSp, -(y+scF*Size.ObtHoehe));
	     for (int n=1; n<=2; n++)
	     { 
	      MoveToEx(hdc, x+i*Fluegel+Fluegel/4+versX, -(y+ scF*(Size.Hoehe/2-50)), NULL);
	      LineTo(hdc, x+i*Fluegel+Fluegel/4+versX, -(y+ scF*(Size.Hoehe/2+10)));
	      int xy =  scF*SFPFXY;
	      LineTo(hdc, x+i*Fluegel+Fluegel/4-xy+versX, -(y+ scF*(Size.Hoehe/2+10)-xy));
	      MoveToEx(hdc, x+i*Fluegel+Fluegel/4+versX, -(y+ scF*(Size.Hoehe/2+10)), NULL);
	      LineTo(hdc, x+i*Fluegel+Fluegel/4+xy+versX, -(y+ scF*(Size.Hoehe/2+10)-xy));
	      versX += Fluegel/2;
	     }
	   }
	 else
	   if (FluegelP->FArt == KF)
	   {
	     MoveToEx(hdc, x+aSp+i*Fluegel+i*iSp, -(y+ scF*Size.ObtHoehe), NULL);
	     LineTo(hdc, x+i*Fluegel+Fluegel/2, -(y+aSp));
	     LineTo(hdc, x+aSp+(i+1)*Fluegel+i*iSp, -(y+ scF*Size.ObtHoehe));
	   }
       break;
       default: break;
     }
   }
#endif // if 0
   y +=  scF*(Size.Hoehe+BEMABSTAND+3) +  30;
}


void CSchiebeTor::drawBesch(HDC hdc, int x, int y)
{
#if 0
   CFlParam* FluegelP1;
   CFlParam* FluegelP2;
   BOOL hasOzO = FALSE;
   int iStartY = 0;

   if (bBogen == TRUE)
     iStartY = scF*Size.SehnenHoehe;
   else
     iStartY = 0;

   float scFH = scF;
   //scF = 1.0;
   int beschBreite =  scF*BESCHBREITE;
   Rectangle(hdc, x-beschBreite, -(y+iStartY+scFH*BESCHTORY), x,
		  -(y+iStartY+scFH*(BESCHTORY+BESCHHOEHE)));

   int count = FlParam->GetSize();
   if (count > 0)
   {
     FluegelP1 = (CFlParam*)FlParam->GetAt(0);
     FluegelP2 = (CFlParam*)FlParam->GetAt(1);
     if (FluegelP1->OArt == OzO || FluegelP2->OArt == OzO)
     {
         hasOzO = TRUE;
	 Rectangle(hdc, x-beschBreite, -(y+iStartY+scFH*((Size.ObtHoehe-BESCHHOEHE/2))),
		  x, -(y+iStartY+scFH*(Size.ObtHoehe+BESCHHOEHE/2)));
     }
     else
       Rectangle(hdc, x-beschBreite, -(y+iStartY+scFH*((Size.Hoehe-BESCHHOEHE)/2)),
		  x, -(y+iStartY+scFH*(Size.Hoehe+BESCHHOEHE)/2));
   }
   else
     Rectangle(hdc, x-beschBreite, -(y+iStartY+scFH*((Size.Hoehe-BESCHHOEHE)/2)),
		  x, -(y+iStartY+scFH*(Size.Hoehe+BESCHHOEHE)/2));
   Rectangle(hdc, x-beschBreite, -(y+iStartY+scFH*(Size.Hoehe-BESCHTORY-BESCHHOEHE)),
		  x, -(y+iStartY+scFH*(Size.Hoehe-BESCHTORY)));

   Rectangle(hdc, x+Size.ZBreite, -(y+iStartY+scFH*BESCHTORY),
		  x+Size.ZBreite+beschBreite, -(y+iStartY+scFH*(BESCHTORY+BESCHHOEHE)));

   if (hasOzO == TRUE)
      Rectangle(hdc, x+Size.ZBreite, -(y+iStartY+scFH*((Size.ObtHoehe-BESCHHOEHE/2))),
		  x+Size.ZBreite+beschBreite, -(y+iStartY+scFH*(Size.ObtHoehe+BESCHHOEHE/2)));
   else
      Rectangle(hdc, x+Size.ZBreite, -(y+iStartY+scFH*((Size.Hoehe-BESCHHOEHE)/2)),
		  x+Size.ZBreite+beschBreite, -(y+iStartY+scFH*(Size.Hoehe+BESCHHOEHE)/2));
   Rectangle(hdc, x+Size.ZBreite, -(y+iStartY+scFH*(Size.Hoehe-BESCHTORY-BESCHHOEHE)),
		  x+Size.ZBreite+beschBreite, -(y+iStartY+scFH*(Size.Hoehe-BESCHTORY)));
#endif
}

#pragma warning( pop ) 
