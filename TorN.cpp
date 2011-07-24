#include "stdafx.h"
#include <string.h>
#include <stdio.h>
//#include <math.h>
#include "datatype.h"
#include "ttdata.h"
#include "resource.h"
#include "ttmainDoc.h"
#include "ttmainView.h"
#include "EvaluateDoubleCircle.h"
#include "TorN.h"


#pragma warning( push )
#pragma warning( disable : 4244 )

//************************************************
// Klasse CTorN
//************************************************
void CTorN::drawTT(HDC hdc, int x, int y, int b, int h, HFONT fntSmall)
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
         // Fenster && Typ 600, Sprossentyp schreiben
         if (Typ == 6)
         {
            char szTemp[20];
            if (FluegelP->Sprosse == RP)
                strcpy(szTemp, "RP");
            else
                strcpy(szTemp, "U");

            int posY = -(y+ scF*(Size.ObtHoehe)+aSp);
            if (FluegelP->FArt == SF)
                OutputAligned(hdc, E_ALIGN_MID, szTemp, 
                  x+iPosX+FluegelP->iZBreite/2+scF*25, posY);
            else
                OutputAligned(hdc, E_ALIGN_MID, szTemp, 
                  x+iPosX+FluegelP->iZBreite/2, posY);
         }

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

CSchiebeTor::CSchiebeTor() : CTorN()
{
    m_rollAppBmp.LoadBitmap(IDB_ROLLAPP); 
}

CSchiebeTor::CSchiebeTor(CTorDoor* ct) : CTorN(ct)
{
    m_rollAppBmp.LoadBitmap(IDB_ROLLAPP); 
}


void CSchiebeTor::drawTT(HDC hdc, int x, int y, int b, int h, HFONT fntSmall)
{
//   int i;
   scF = calculateScale(b, h);
   aSp =  max(3, rnd(scF*3));
   iSp =  max(3, rnd(scF*3));
   int Sp = aSp;


   int Fluegel = ( scF*Size.Breite-2*aSp-(FluegelAnz-1)*iSp)/FluegelAnz;
   Size.ZBreite = FluegelAnz*Fluegel+2*aSp+(FluegelAnz-1)*iSp;

   BITMAP bm;
   m_rollAppBmp.GetBitmap(&bm);

   // Rollapparate zeichnen
   // links
   ::StretchBlt(
      hdc,      // handle to destination DC
      x + aSp, // x-coord of destination upper-left corner
      -1*y + aSp, // y-coord of destination upper-left corner
      scF*0.1f*bm.bmWidth,   // width of destination rectangle
      scF*0.1f*bm.bmHeight,  // height of destination rectangle
      m_rollAppBmp.GetCompatibleDC(hdc), // handle to source DC
      0,  // x-coord of source upper-left corner
      0,  // y-coord of source upper-left corner
      bm.bmWidth,    // width of source rectangle
      bm.bmHeight,   // height of source rectangle
      SRCCOPY       // raster operation code
    );
   ::StretchBlt(
      hdc,      // handle to destination DC
      x + Fluegel - 2*iSp, // x-coord of destination upper-left corner
      -1*y + aSp, // y-coord of destination upper-left corner
      scF*0.1f*bm.bmWidth,   // width of destination rectangle
      scF*0.1f*bm.bmHeight,  // height of destination rectangle
      m_rollAppBmp.GetCompatibleDC(hdc), // handle to source DC
      0,  // x-coord of source upper-left corner
      0,  // y-coord of source upper-left corner
      bm.bmWidth,    // width of source rectangle
      bm.bmHeight,   // height of source rectangle
      SRCCOPY       // raster operation code
    );

   // rechts
   ::StretchBlt(
      hdc,      // handle to destination DC
      x + Fluegel + 4*iSp, // x-coord of destination upper-left corner
      -1*y + aSp, // y-coord of destination upper-left corner
      scF*0.1f*bm.bmWidth,   // width of destination rectangle
      scF*0.1f*bm.bmHeight,  // height of destination rectangle
      m_rollAppBmp.GetCompatibleDC(hdc),       // handle to source DC
      0,  // x-coord of source upper-left corner
      0,  // y-coord of source upper-left corner
      bm.bmWidth,    // width of source rectangle
      bm.bmHeight,   // height of source rectangle
      SRCCOPY       // raster operation code
    );

   // Rollapparate zeichnen
   // rechts
   ::StretchBlt(
      hdc,      // handle to destination DC
      x + Size.ZBreite - (scF*0.1f*bm.bmWidth) + 2*iSp, // x-coord of destination upper-left corner
      -1*y + aSp, // y-coord of destination upper-left corner
      scF*0.1f*bm.bmWidth,   // width of destination rectangle
      scF*0.1f*bm.bmHeight,  // height of destination rectangle
      m_rollAppBmp.GetCompatibleDC(hdc),       // handle to source DC
      0,  // x-coord of source upper-left corner
      0,  // y-coord of source upper-left corner
      bm.bmWidth,    // width of source rectangle
      bm.bmHeight,   // height of source rectangle
      SRCCOPY       // raster operation code
    );

   CTorN::drawTT(hdc, x, y, b, h, fntSmall);
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
