#include "stdafx.h"
#include "datatype.h"
#include "ttdata.h"
#include "tordoor.h"
#include <string.h>
#include <stdio.h>
//#include <math.h>
#include "ttmainDoc.h"
#include "ttmainView.h"
#include "EvaluateDoubleCircle.h"

#include "TuerN.h"

#pragma warning( push )
#pragma warning( disable : 4244 )


//************************************************
// Klasse CTuerN
//************************************************
float CTuerN::calculateScale(int breite, int hoehe)
{
  if (bBogen == FALSE)
  {
      return CTorDoor::calculateScale(breite, hoehe);
  }
  else
  {
      float scaleB;
      float scaleH;
      float scaleRet;
      float fbreite = (float)breite;
      float fhoehe = (float)hoehe;
      float fthoehe = (float)Size.Hoehe;

      fthoehe += SEHNENHOEHE; //Size.SehnenHoehe;
   
      float ftbreite = (float)Size.Breite;

      scaleB = (float)fbreite/ftbreite;
      scaleH = (float)fhoehe/fthoehe;
      scaleRet = ((scaleB < scaleH) ? scaleB : scaleH);
      Size.ZGesamtHoehe = scaleRet*Size.Hoehe;
      if (bBogen == TRUE)
      {
          Size.ZGesamtHoehe += scF*SEHNENHOEHE;
      }
      return scaleRet;
  }
}


void CTuerN::drawTT(HDC hdc, int x, int y, int b, int h, HFONT fntSmall)
{
   scF = calculateScale(b, h);

   aSp =  rnd(scF*3);
   iSp =  rnd(scF*3);
   int Sp = aSp;


   Size.ZBreite =  rnd(scF*Size.Breite);

   /* Bogen */
   if (bBogen == TRUE)
   {
       double dRadius;

       POINT points[4];
     
       //double SehnenHoehe = 11.4*Size.Hoehe/Size.Breite;
       double SehnenHoehe = scF*SEHNENHOEHE; //scF*Size.SehnenHoehe; //SEHNENHOEHE; // fest, da Probleme beim Zeichnen!!!!

       points[0].x = x-Sp; points[0].y = -(y+SehnenHoehe-Sp);
       points[1].x = x-Sp; points[1].y = -(y+SehnenHoehe+ scF*Size.Hoehe+aSp);
       points[2].x = x+Size.ZBreite+Sp; points[2].y = -(y+SehnenHoehe+ scF*Size.Hoehe+aSp);
       points[3].x = x+Size.ZBreite+Sp; points[3].y = -(y+SehnenHoehe-Sp);//-(y-Sp);
       drawConnectedPoints(hdc, points, 4);

       CEvaluateDoubleCircle circMath;
       dRadius = circMath.EvalRadius(Size.ZBreite+2*Sp, SehnenHoehe+Sp);
       dRadius = rnd(dRadius+1);
       Arc(hdc, x+Size.ZBreite/2-dRadius-Sp, -(y-Sp), 
           x+Size.ZBreite/2+dRadius+Sp, -(y-Sp+2*dRadius),
           x+Size.ZBreite+Sp, -(y+SehnenHoehe-Sp), x-Sp, -(y+SehnenHoehe-Sp));

       points[0].x = x; points[0].y = -(y+SehnenHoehe);
       points[1].x = x; points[1].y = -(y+SehnenHoehe+ scF*Size.Hoehe);
       points[2].x = x+Size.ZBreite; points[2].y = -(y+SehnenHoehe+ scF*Size.Hoehe);
       points[3].x = x+Size.ZBreite; points[3].y = -(y+SehnenHoehe);
       drawConnectedPoints(hdc, points, 4);
//       dRadius = circMath.EvalRadius(Size.ZBreite, scF*Size.SehnenHoehe);
       Arc(hdc, x+Size.ZBreite/2-dRadius, -(y), 
           x+Size.ZBreite/2+dRadius, -(y+2*dRadius),
           x+Size.ZBreite, -(y+SehnenHoehe), x, -(y+SehnenHoehe));


       points[0].x = x+Sp; points[0].y = -(y+SehnenHoehe+Sp);
       points[1].x = x+Sp; points[1].y = -(y+SehnenHoehe+ scF*Size.Hoehe-aSp);
       points[2].x = x+Size.ZBreite-Sp; points[2].y = -(y+SehnenHoehe+ scF*Size.Hoehe-aSp);
       points[3].x = x+Size.ZBreite-Sp; points[3].y = -(y+SehnenHoehe+Sp);
       drawConnectedPoints(hdc, points, 4);
//       dRadius = circMath.EvalRadius(Size.ZBreite-2*Sp, scF*Size.SehnenHoehe-Sp);
       Arc(hdc, x+Size.ZBreite/2-dRadius+Sp, -(y+Sp), 
           x+Size.ZBreite/2+dRadius-Sp, -(y+Sp+2*dRadius),
           x+Size.ZBreite-Sp, -(y+SehnenHoehe+Sp), x+Sp, -(y+SehnenHoehe+Sp));

   }
   else
   {
        Rectangle(hdc, x-Sp, -(y-Sp), x+Size.ZBreite+Sp, -(y+ scF*Size.Hoehe+aSp));
        Rectangle(hdc, x, -y, x+Size.ZBreite, -(y+ scF*Size.Hoehe));
        Rectangle(hdc, x+Sp, -(y+Sp), x+Size.ZBreite-Sp, -(y+ scF*Size.Hoehe-aSp));
   }

   //ab hier nicht angepaßt
   if (FlParam->GetSize() == 1)
   {
        int DINy;
        int iSehnenHoehe;
        if (bBogen == TRUE)
            iSehnenHoehe = Size.SehnenHoehe;
        else
            iSehnenHoehe = 0;
        CFlParam* FluegelP = (CFlParam*)FlParam->GetAt(0);
        if (FluegelP->FArt != KEIN || FluegelP->OArt != LEER
            || FluegelP->Geteilt == TRUE)
        {
           DINy = y + scF*(iSehnenHoehe+Size.ObtHoehe+20);
        }
        else
        {
           DINy = y+ scF*(iSehnenHoehe+Size.Hoehe-OEFFNERPOSY);
        }
        DINy = __max(DINy, y+ scF*(iSehnenHoehe+Size.Hoehe-OEFFNERPOSY));
        int DINx;
        if (DIN == L)
         DINx = x+ scF*(Size.Breite-DINBREITE-3)-Sp;
        else
         DINx = x+ scF*3+Sp;
        drawDIN(hdc, DINx, DINy);
   }
   drawBesch(hdc, x, y);
   //drawBem(hdc, x, y);

   //Profilemaße löschen
/*   if (Profile == NULL)
      Profile = new TNSCollection(sizeof(CProfileElem), sizeof(CProfileElem));
   else
      Profile->freeAll();
*/
   if (FlParam == NULL)
	   return;

   if (bBogen == TRUE)
      y += scF*SEHNENHOEHE;

   int FHoehe;
   if (FlParam->GetSize() == 1)
   {
     CFlParam* FluegelP = (CFlParam*)FlParam->GetAt(0);
     if (FluegelP->Geteilt == TRUE)
	    Rectangle(hdc, x+Sp, -(y+ scF*Size.ObtHoehe), x+Size.ZBreite-Sp,
		  -(y+ scF*Size.ObtHoehe+Sp));
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

            int posY = -(y+scF*Size.ObtHoehe+aSp);
//            if (FluegelP->FArt == SF)
//                posY += 50;

            if (FluegelP->FArt == SF)
                OutputAligned(hdc, E_ALIGN_MID, szTemp, 
                  x+Size.ZBreite/2+scF*15, posY);
            else
                OutputAligned(hdc, E_ALIGN_MID, szTemp, 
                  x+Size.ZBreite/2, posY);
         }

	     FHoehe = scF* Size.ObtHoehe;
	     Rectangle(hdc, x+Sp, -(y+FHoehe), x+Size.ZBreite-Sp,
			-(y+FHoehe+Sp));

	     if (FluegelP->FArt == SF)
	     {
	       MoveToEx(hdc, x+Size.ZBreite/2, -(y+ scF*(Size.Hoehe/2-50)), NULL);
	       LineTo(hdc, x+Size.ZBreite/2, -(y+ scF*(Size.Hoehe/2+10)));
	       int xy =  scF*SFPFXY;
	       LineTo(hdc, x+Size.ZBreite/2-xy, -(y+ scF*(Size.Hoehe/2+10)-xy));
	       MoveToEx(hdc, x+Size.ZBreite/2, -(y+ scF*(Size.Hoehe/2+10)), NULL);
	       LineTo(hdc, x+Size.ZBreite/2+xy, -(y+ scF*(Size.Hoehe/2+10)-xy));

	     }
	     else
	       if (FluegelP->FArt == SF2)
	       {
	         int versX = 0;
	         Rectangle(hdc, x+Size.ZBreite/2, -(y+aSp),
		       x+Size.ZBreite/2+aSp, -(y+scF*Size.ObtHoehe));
	         for (int n=1; n<=2; n++)
	         { 
	          MoveToEx(hdc, x+Size.ZBreite/4+versX, -(y+ scF*(Size.Hoehe/2-50)), NULL);
	          LineTo(hdc, x+Size.ZBreite/4+versX, -(y+ scF*(Size.Hoehe/2+10)));
	          int xy =  scF*SFPFXY;
	          LineTo(hdc, x+Size.ZBreite/4-xy+versX, -(y+ scF*(Size.Hoehe/2+10)-xy));
	          MoveToEx(hdc, x+Size.ZBreite/4+versX, -(y+ scF*(Size.Hoehe/2+10)), NULL);
	          LineTo(hdc, x+Size.ZBreite/4+xy+versX, -(y+ scF*(Size.Hoehe/2+10)-xy));
	          versX += Size.ZBreite/2;
	         }
	       }
	     else
	       if (FluegelP->FArt == KF)
	       {
	         MoveToEx(hdc, x+Sp, -(y+ FHoehe), NULL);
	         LineTo(hdc, x+Size.ZBreite/2, -(y+Sp));
	         LineTo(hdc, x+Size.ZBreite-Sp, -(y+ FHoehe));
	       }
       break;
       default:
	 break;
     }
	 int abstand;
     switch (FluegelP->OArt)
     {
       case OzO:
	 Rectangle(hdc, x, -(y+ scF*Size.ObtHoehe-Sp), x+Size.ZBreite,
			   -(y+ scF*Size.ObtHoehe));
	 Rectangle(hdc, x, -(y+ scF*Size.ObtHoehe), x+Size.ZBreite,
			   -(y+ scF*Size.ObtHoehe+Sp));
       break;
       case OiF:
			abstand = rnd(scF*115/20);
			drawOIF(hdc, x+abstand+Sp, y+Sp, Size.ZBreite-2*(abstand+Sp),
				scF*Size.ObtHoehe, DIN==R);
	 break;
       default: break;
     }
   }

   y +=  scF*(Size.Hoehe+BEMABSTAND+3) +  30;
}


void CTuerN::drawBesch(HDC hdc, int x, int y)
{
  int count = 0;
  int iStartY;
  if (FlParam != NULL)
	count = FlParam->GetSize();

  if (bBogen == TRUE)
      iStartY = scF*SEHNENHOEHE;//scF*Size.SehnenHoehe;
  else
      iStartY = 0;


  if (DIN == L)
  {
    Rectangle(hdc, x- scF*BESCHBREITE, -(y+ iStartY+scF*BESCHTUERY), x,
		  -(y+iStartY+ scF*(BESCHTUERY+BESCHHOEHE)));
    if (count > 0)
    {
       CFlParam* FluegelP = (CFlParam*)FlParam->GetAt(0);
       if (FluegelP->OArt == OzO)
	 Rectangle(hdc, x- scF*BESCHBREITE, -(y+iStartY+scF*(Size.ObtHoehe-BESCHHOEHE/2)), x,
		  -(y+iStartY+scF*(Size.ObtHoehe+BESCHHOEHE/2)));
    }
    Rectangle(hdc, x- scF*BESCHBREITE, -(y+iStartY+scF*(Size.Hoehe-BESCHTUERY-BESCHHOEHE)),
		  x, -(y+iStartY+scF*(Size.Hoehe-BESCHTUERY)));
  }
  else
  {
    Rectangle(hdc, x+ scF*Size.Breite, -(y+iStartY+scF*BESCHTUERY),
		  x+ scF*(Size.Breite+BESCHBREITE), -(y+iStartY+scF*(BESCHTUERY+BESCHHOEHE)));
    if (count > 0)
    {
       CFlParam* FluegelP = (CFlParam*)FlParam->GetAt(0);
       if (FluegelP->OArt == OzO)
	 Rectangle(hdc, x+Size.ZBreite, -(y+iStartY+scF*(Size.ObtHoehe-BESCHHOEHE/2)),
	 x+ scF*(Size.Breite+BESCHBREITE), -(y+iStartY+scF*(Size.ObtHoehe+BESCHHOEHE/2)));
    }

    Rectangle(hdc, x+ scF*Size.Breite, -(y+iStartY+scF*(Size.Hoehe-BESCHTUERY-BESCHHOEHE)),
		  x+ scF*(Size.Breite+BESCHBREITE), -(y+iStartY+scF*(Size.Hoehe-BESCHTUERY)));
  }
}

//************************************************
// Klasse CSchiebeTuer
//************************************************

void CSchiebeTuer::drawTT(HDC hdc, int x, int y, int b, int h, HFONT fntSmall)
{
   scF = calculateScale(b, h);

   aSp =  rnd(scF*3);
   iSp =  rnd(scF*3);
   int Sp = aSp;


   Size.ZBreite =  rnd(scF*Size.Breite);

   /* Bogen */
   if (bBogen == TRUE)
   {
       double dRadius;

       POINT points[4];
     
       //double SehnenHoehe = 11.4*Size.Hoehe/Size.Breite;
       double SehnenHoehe = scF*SEHNENHOEHE; //scF*Size.SehnenHoehe; //SEHNENHOEHE; // fest, da Probleme beim Zeichnen!!!!

       points[0].x = x-Sp; points[0].y = -(y+SehnenHoehe-Sp);
       points[1].x = x-Sp; points[1].y = -(y+SehnenHoehe+ scF*Size.Hoehe+aSp);
       points[2].x = x+Size.ZBreite+Sp; points[2].y = -(y+SehnenHoehe+ scF*Size.Hoehe+aSp);
       points[3].x = x+Size.ZBreite+Sp; points[3].y = -(y+SehnenHoehe-Sp);//-(y-Sp);
       drawConnectedPoints(hdc, points, 4);

       CEvaluateDoubleCircle circMath;
       dRadius = circMath.EvalRadius(Size.ZBreite+2*Sp, SehnenHoehe+Sp);
       dRadius = rnd(dRadius+1);
       Arc(hdc, x+Size.ZBreite/2-dRadius-Sp, -(y-Sp), 
           x+Size.ZBreite/2+dRadius+Sp, -(y-Sp+2*dRadius),
           x+Size.ZBreite+Sp, -(y+SehnenHoehe-Sp), x-Sp, -(y+SehnenHoehe-Sp));

       points[0].x = x; points[0].y = -(y+SehnenHoehe);
       points[1].x = x; points[1].y = -(y+SehnenHoehe+ scF*Size.Hoehe);
       points[2].x = x+Size.ZBreite; points[2].y = -(y+SehnenHoehe+ scF*Size.Hoehe);
       points[3].x = x+Size.ZBreite; points[3].y = -(y+SehnenHoehe);
       drawConnectedPoints(hdc, points, 4);
//       dRadius = circMath.EvalRadius(Size.ZBreite, scF*Size.SehnenHoehe);
       Arc(hdc, x+Size.ZBreite/2-dRadius, -(y), 
           x+Size.ZBreite/2+dRadius, -(y+2*dRadius),
           x+Size.ZBreite, -(y+SehnenHoehe), x, -(y+SehnenHoehe));


       points[0].x = x+Sp; points[0].y = -(y+SehnenHoehe+Sp);
       points[1].x = x+Sp; points[1].y = -(y+SehnenHoehe+ scF*Size.Hoehe-aSp);
       points[2].x = x+Size.ZBreite-Sp; points[2].y = -(y+SehnenHoehe+ scF*Size.Hoehe-aSp);
       points[3].x = x+Size.ZBreite-Sp; points[3].y = -(y+SehnenHoehe+Sp);
       drawConnectedPoints(hdc, points, 4);
//       dRadius = circMath.EvalRadius(Size.ZBreite-2*Sp, scF*Size.SehnenHoehe-Sp);
       Arc(hdc, x+Size.ZBreite/2-dRadius+Sp, -(y+Sp), 
           x+Size.ZBreite/2+dRadius-Sp, -(y+Sp+2*dRadius),
           x+Size.ZBreite-Sp, -(y+SehnenHoehe+Sp), x+Sp, -(y+SehnenHoehe+Sp));

   }
   else
   {
        Rectangle(hdc, x-Sp, -(y-Sp), x+Size.ZBreite+Sp, -(y+ scF*Size.Hoehe+aSp));
        Rectangle(hdc, x, -y, x+Size.ZBreite, -(y+ scF*Size.Hoehe));
        Rectangle(hdc, x+Sp, -(y+Sp), x+Size.ZBreite-Sp, -(y+ scF*Size.Hoehe-aSp));
   }

   //ab hier nicht angepaßt
   if (FlParam->GetSize() == 1)
   {
        int DINy;
        int iSehnenHoehe;
        if (bBogen == TRUE)
            iSehnenHoehe = Size.SehnenHoehe;
        else
            iSehnenHoehe = 0;
        CFlParam* FluegelP = (CFlParam*)FlParam->GetAt(0);
        if (FluegelP->FArt != KEIN || FluegelP->OArt != LEER
            || FluegelP->Geteilt == TRUE)
        {
           DINy = y + scF*(iSehnenHoehe+Size.ObtHoehe+20);
        }
        else
        {
           DINy = y+ scF*(iSehnenHoehe+Size.Hoehe-OEFFNERPOSY);
        }
        DINy = __max(DINy, y+ scF*(iSehnenHoehe+Size.Hoehe-OEFFNERPOSY));
        int DINx;
        if (DIN == L)
         DINx = x+ scF*(Size.Breite-DINBREITE-3)-Sp;
        else
         DINx = x+ scF*3+Sp;
        drawDIN(hdc, DINx, DINy);
   }
   drawBesch(hdc, x, y);
   //drawBem(hdc, x, y);

   //Profilemaße löschen
/*   if (Profile == NULL)
      Profile = new TNSCollection(sizeof(CProfileElem), sizeof(CProfileElem));
   else
      Profile->freeAll();
*/
   if (FlParam == NULL)
	   return;

   if (bBogen == TRUE)
      y += scF*SEHNENHOEHE;

   int FHoehe;
   if (FlParam->GetSize() == 1)
   {
     CFlParam* FluegelP = (CFlParam*)FlParam->GetAt(0);
     if (FluegelP->Geteilt == TRUE)
	Rectangle(hdc, x+Sp, -(y+ scF*Size.ObtHoehe), x+Size.ZBreite-Sp,
		  -(y+ scF*Size.ObtHoehe+Sp));
     switch (FluegelP->FArt)
     {
       case OlF:
       case SF:
       case KF:
       case SF2:
	    FHoehe = scF* Size.ObtHoehe;
	    Rectangle(hdc, x+Sp, -(y+FHoehe), x+Size.ZBreite-Sp,
			-(y+FHoehe+Sp));

	 if (FluegelP->FArt == SF)
	 {
	   MoveToEx(hdc, x+Size.ZBreite/2, -(y+ scF*(Size.Hoehe/2-50)), NULL);
	   LineTo(hdc, x+Size.ZBreite/2, -(y+ scF*(Size.Hoehe/2+10)));
	   int xy =  scF*SFPFXY;
	   LineTo(hdc, x+Size.ZBreite/2-xy, -(y+ scF*(Size.Hoehe/2+10)-xy));
	   MoveToEx(hdc, x+Size.ZBreite/2, -(y+ scF*(Size.Hoehe/2+10)), NULL);
	   LineTo(hdc, x+Size.ZBreite/2+xy, -(y+ scF*(Size.Hoehe/2+10)-xy));

	 }
	 else
	   if (FluegelP->FArt == SF2)
	   {
	     int versX = 0;
	     Rectangle(hdc, x+Size.ZBreite/2, -(y+aSp),
		   x+Size.ZBreite/2+aSp, -(y+scF*Size.ObtHoehe));
	     for (int n=1; n<=2; n++)
	     { 
	      MoveToEx(hdc, x+Size.ZBreite/4+versX, -(y+ scF*(Size.Hoehe/2-50)), NULL);
	      LineTo(hdc, x+Size.ZBreite/4+versX, -(y+ scF*(Size.Hoehe/2+10)));
	      int xy =  scF*SFPFXY;
	      LineTo(hdc, x+Size.ZBreite/4-xy+versX, -(y+ scF*(Size.Hoehe/2+10)-xy));
	      MoveToEx(hdc, x+Size.ZBreite/4+versX, -(y+ scF*(Size.Hoehe/2+10)), NULL);
	      LineTo(hdc, x+Size.ZBreite/4+xy+versX, -(y+ scF*(Size.Hoehe/2+10)-xy));
	      versX += Size.ZBreite/2;
	     }
	   }
	 else
	   if (FluegelP->FArt == KF)
	   {
	     MoveToEx(hdc, x+Sp, -(y+ FHoehe), NULL);
	     LineTo(hdc, x+Size.ZBreite/2, -(y+Sp));
	     LineTo(hdc, x+Size.ZBreite-Sp, -(y+ FHoehe));
	   }
       break;
       default:
	 break;
     }
	 int abstand;
     switch (FluegelP->OArt)
     {
       case OzO:
	 Rectangle(hdc, x, -(y+ scF*Size.ObtHoehe-Sp), x+Size.ZBreite,
			   -(y+ scF*Size.ObtHoehe));
	 Rectangle(hdc, x, -(y+ scF*Size.ObtHoehe), x+Size.ZBreite,
			   -(y+ scF*Size.ObtHoehe+Sp));
       break;
       case OiF:
			abstand = rnd(scF*115/20);
			drawOIF(hdc, x+abstand+Sp, y+Sp, Size.ZBreite-2*(abstand+Sp),
				scF*Size.ObtHoehe, DIN==R);
	 break;
       default: break;
     }
   }

   y +=  scF*(Size.Hoehe+BEMABSTAND+3) +  30;
}


void CSchiebeTuer::drawBesch(HDC hdc, int x, int y)
{
#if 0
  int count = 0;
  int iStartY;
  if (FlParam != NULL)
	count = FlParam->GetSize();

  if (bBogen == TRUE)
      iStartY = scF*SEHNENHOEHE;//scF*Size.SehnenHoehe;
  else
      iStartY = 0;


  if (DIN == L)
  {
    Rectangle(hdc, x- scF*BESCHBREITE, -(y+ iStartY+scF*BESCHTUERY), x,
		  -(y+iStartY+ scF*(BESCHTUERY+BESCHHOEHE)));
    if (count > 0)
    {
       CFlParam* FluegelP = (CFlParam*)FlParam->GetAt(0);
       if (FluegelP->OArt == OzO)
	 Rectangle(hdc, x- scF*BESCHBREITE, -(y+iStartY+scF*(Size.ObtHoehe-BESCHHOEHE/2)), x,
		  -(y+iStartY+scF*(Size.ObtHoehe+BESCHHOEHE/2)));
    }
    Rectangle(hdc, x- scF*BESCHBREITE, -(y+iStartY+scF*(Size.Hoehe-BESCHTUERY-BESCHHOEHE)),
		  x, -(y+iStartY+scF*(Size.Hoehe-BESCHTUERY)));
  }
  else
  {
    Rectangle(hdc, x+ scF*Size.Breite, -(y+iStartY+scF*BESCHTUERY),
		  x+ scF*(Size.Breite+BESCHBREITE), -(y+iStartY+scF*(BESCHTUERY+BESCHHOEHE)));
    if (count > 0)
    {
       CFlParam* FluegelP = (CFlParam*)FlParam->GetAt(0);
       if (FluegelP->OArt == OzO)
	 Rectangle(hdc, x+Size.ZBreite, -(y+iStartY+scF*(Size.ObtHoehe-BESCHHOEHE/2)),
	 x+ scF*(Size.Breite+BESCHBREITE), -(y+iStartY+scF*(Size.ObtHoehe+BESCHHOEHE/2)));
    }

    Rectangle(hdc, x+ scF*Size.Breite, -(y+iStartY+scF*(Size.Hoehe-BESCHTUERY-BESCHHOEHE)),
		  x+ scF*(Size.Breite+BESCHBREITE), -(y+iStartY+scF*(Size.Hoehe-BESCHTUERY)));
  }
#endif  // if 0
}

#pragma warning( pop ) 
