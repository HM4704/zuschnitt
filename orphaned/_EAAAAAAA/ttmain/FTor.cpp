#include "stdafx.h"
#include "ttdata.h"
#include "tordoor.h"
#include <string.h>
#include <stdio.h>
//#include <math.h>
#include "datatype.h"
#include "ttmainDoc.h"
#include "ttmainView.h"
#include "EvaluateDoubleCircle.h"

#include "FTor.h"

#pragma warning( push )
#pragma warning( disable : 4244 )


//************************************************
// Klasse CFT3
//************************************************
CFTor::CFTor(CTorDoor* ct) : CTorDoor(ct)
{
}

void CFTor::drawTT(HDC hdc, int x, int y, int b, int h, HFONT fntSmall)
{
   scF = calculateScale(b, h);

   aSp =  rnd(scF*3);
   iSp =  rnd(scF*3);
   int Sp = aSp;


   int Fluegel = ( scF*Size.Breite-2*aSp-(FluegelAnz-1)*iSp)/FluegelAnz;
   Size.ZBreite = FluegelAnz*Fluegel+2*aSp+(FluegelAnz-1)*iSp;
   Rectangle(hdc, x-aSp, -(y-aSp), x+Size.ZBreite+aSp, -(y+ scF*Size.Hoehe+aSp));
   Rectangle(hdc, x, -y, x+Size.ZBreite, -(y+ scF*Size.Hoehe));
   for (int i = 0; i<FluegelAnz; i++)
     Rectangle(hdc, x+aSp+i*Fluegel+i*iSp, -(y+aSp), x+aSp+(i+1)*Fluegel+i*iSp, -(y+ scF*Size.Hoehe-aSp));

   int DINy = y+ scF*(Size.Hoehe-OEFFNERPOSY);
   int DINx;
   if (FluegelAnz < 5)
   {
       if (DIN == L)
         DINx = x+aSp+Fluegel- scF*(DINBREITE+3);
       else
         DINx = x+aSp+(FluegelAnz-1)*(Fluegel+iSp)+ scF*3;
   }
   else
   {
       // bei 5-FT Drücker im Mittelflügel
       if (DIN == L)
         DINx = x+aSp+(3)*(Fluegel+iSp)- scF*(DINBREITE+3);
       else
         DINx = x+aSp+(2)*(Fluegel+iSp)+ scF*3;
   }
   drawDIN(hdc, DINx, DINy);
   drawBesch(hdc, x, y);
   //drawBem(hdc, x, y);

   //Profilemaße löschen
/*   if (Profile == NULL)
      Profile = new TNSCollection(sizeof(CProfileElem), sizeof(CProfileElem));
   else
      Profile->freeAll();
*/
   int count = FlParam->GetSize();
   for (i=0; i<count; i++)
   {
     CFlParam* FluegelP = (CFlParam*)FlParam->GetAt(i);
     if (FluegelP->Geteilt == TRUE)
	 Rectangle(hdc, x+aSp+i*Fluegel+i*iSp, -(y+ scF*Size.ObtHoehe),
		   x+aSp+(i+1)*Fluegel+i*iSp, -(y+ scF*Size.ObtHoehe+aSp));
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
//            if (FluegelP->FArt == SF)
//                posY -= 40;
            if (FluegelP->FArt == SF)
                OutputAlignedFont(hdc, E_ALIGN_MID, szTemp, 
                  x+i*(Fluegel+iSp)+Fluegel/2+iSp+scF*15, posY, fntSmall);
            else
                OutputAlignedFont(hdc, E_ALIGN_MID, szTemp, 
                  x+i*(Fluegel+iSp)+Fluegel/2+iSp, posY, fntSmall);
         }

	 Rectangle(hdc, x+aSp+i*Fluegel+i*iSp, -(y+ scF*Size.ObtHoehe),
		   x+aSp+(i+1)*Fluegel+i*iSp, -(y+ (scF*Size.ObtHoehe)+aSp));
	 if (FluegelP->FArt == SF)
	 {
	   MoveToEx(hdc, x+i*(Fluegel+iSp)+Fluegel/2, -(y+ scF*(Size.ObtHoehe/2)), NULL);
	   LineTo(hdc, x+i*(Fluegel+iSp)+Fluegel/2, -(y+ scF*(Size.ObtHoehe+40)));
	   int xy =  scF*SFPFXY;
	   LineTo(hdc, x+i*(Fluegel+iSp)+Fluegel/2-xy, -(y+ scF*(Size.ObtHoehe+40)-xy));
	   MoveToEx(hdc, x+i*(Fluegel+iSp)+Fluegel/2, -(y+ scF*(Size.ObtHoehe+40)), NULL);
	   LineTo(hdc, x+i*(Fluegel+iSp)+Fluegel/2+xy, -(y+ scF*(Size.ObtHoehe+40)-xy));
	 }
	 else
	   if (FluegelP->FArt == SF2)
	   {
	     int versX = 0;
	     Rectangle(hdc, x+i*(Fluegel+iSp)+Fluegel/2, -(y+aSp),
		   x+i*(Fluegel+iSp)+Fluegel/2+aSp, -(y+scF*Size.ObtHoehe));
	     for (int n=1; n<=2; n++)
	     { 
	      MoveToEx(hdc, x+i*(Fluegel+iSp)+Fluegel/4+versX, -(y+ scF*(Size.Hoehe/2-50)), NULL);
	      LineTo(hdc, x+i*(Fluegel+iSp)+Fluegel/4+versX, -(y+ scF*(Size.Hoehe/2+10)));
	      int xy =  scF*SFPFXY;
	      LineTo(hdc, x+i*(Fluegel+iSp)+Fluegel/4-xy+versX, -(y+ scF*(Size.Hoehe/2+10)-xy));
	      MoveToEx(hdc, x+i*(Fluegel+iSp)+Fluegel/4+versX, -(y+ scF*(Size.Hoehe/2+10)), NULL);
	      LineTo(hdc, x+i*(Fluegel+iSp)+Fluegel/4+xy+versX, -(y+ scF*(Size.Hoehe/2+10)-xy));
	      versX += Fluegel/2;
	     }
	   }
	 else
	   if (FluegelP->FArt == KF)
	   {
	     MoveToEx(hdc, x+aSp+i*Fluegel+i*iSp, -(y+ scF*Size.ObtHoehe), NULL);
	     LineTo(hdc, x+i*(Fluegel+iSp)+Fluegel/2, -(y+aSp));
	     LineTo(hdc, x+aSp+(i+1)*Fluegel+i*iSp, -(y+ scF*Size.ObtHoehe));
	   }
       break;
       default: break;
     }
//     computeProfile(FluegelP);
   }
   y +=  scF*(Size.Hoehe+BEMABSTAND+3) +  25;
}

void CFTor::drawBesch(HDC hdc, int x, int y)
{
   float scFtmp = scF;
//   scF = 1.0;
   float scFH =  scFtmp;
   int beschBreite =  scF*BESCHBREITE;
   Rectangle(hdc, x-beschBreite, -(y+scFH*BESCHTORY), x,
		  -(y+scFH*(BESCHTORY+BESCHHOEHE)));
   Rectangle(hdc, x-beschBreite, -(y+scFH*((Size.Hoehe-BESCHHOEHE)/2)),
		  x, -(y+scFH*(Size.Hoehe+BESCHHOEHE)/2));
   Rectangle(hdc, x-beschBreite, -(y+scFH*(Size.Hoehe-BESCHTORY-BESCHHOEHE)),
		  x, -(y+scFH*(Size.Hoehe-BESCHTORY)));

   Rectangle(hdc, x+Size.ZBreite, -(y+scFH*BESCHTORY),
		  x+Size.ZBreite+beschBreite, -(y+scFH*(BESCHTORY+BESCHHOEHE)));
   Rectangle(hdc, x+Size.ZBreite, -(y+scFH*((Size.Hoehe-BESCHHOEHE)/2)),
		  x+Size.ZBreite+beschBreite, -(y+scFH*(Size.Hoehe+BESCHHOEHE)/2));
   Rectangle(hdc, x+Size.ZBreite, -(y+scFH*(Size.Hoehe-BESCHTORY-BESCHHOEHE)),
		  x+Size.ZBreite+beschBreite, -(y+scFH*(Size.Hoehe-BESCHTORY)));
   scF = scFtmp;

}

#pragma warning( pop ) 
