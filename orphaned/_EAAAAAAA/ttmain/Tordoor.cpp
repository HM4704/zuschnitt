#include "stdafx.h"
#include <string.h>
#include <stdio.h>

#include "datatype.h"
#include "ttdata.h"
#include "tordoor.h"
#include "Node.h"
#include "ZuschnittManager.h"
#include "ttmain.h"
#include "ttmainDoc.h"
#include "ttmainView.h"
#include "EvaluateDoubleCircle.h"

#pragma warning( push )
#pragma warning( disable : 4244 )


//mathematisch Runden
//#define rnd(x)  floor(x+0.5)
//************************************************
// Klasse CTorDoor
//************************************************

//Kopykonstruktor
CTorDoor::CTorDoor(CTorDoor* ct)
{
   Art       = ct->Art;
   Typ       = ct->Typ;
   RahmenArt = ct->RahmenArt;
   DIN       = ct->DIN;
   Fuellung = ct->Fuellung;
   OKF      = ct->OKF;
   bBogen   = ct->bBogen;
   Size      = ct->Size;
   StueckZahl = ct->StueckZahl;
   FluegelAnz = ct->FluegelAnz;
   FlParam    = ct->FlParam;
   Profile    = ct->Profile;
   GlasProfile = ct->GlasProfile;
   HolzElemente = ct->HolzElemente;
   RiegelElemente = ct->RiegelElemente;
   BetoPlanElemente = ct->BetoPlanElemente;
   strcpy(sArtikel, ct->sArtikel);
   strcpy(Kunde, ct->Kunde);
   strcpy(Kommission, ct->Kommission);
   if (ct->Fuellung == -1)
     strcpy(strFuellung, ct->strFuellung);
   scF = (float)0.7;
   aSp = 2;
   iSp = 1;

}

CTorDoor::CTorDoor()
{
   FlParam = NULL;
   FlParam = new CPtrArray;
   Profile = NULL;
   GlasProfile = NULL;
   HolzElemente = NULL;
   RiegelElemente = NULL;
   BetoPlanElemente = NULL;
   StueckZahl = 0;
   Size.Breite = 0;
   Size.Hoehe = 0;
   OKF = FALSE;
   bBogen = FALSE;
   Art       = 0;
   Typ       = 0;
   RahmenArt = 0;
   DIN       = 0;
   Fuellung =  0;
   FluegelAnz = 0;
   strcpy(sArtikel, " ");
   strcpy(Kunde, " ");
   strcpy(Kommission, " ");

}

CTorDoor::~CTorDoor() 
{ 
	deleteFluegel();
	deleteProfiles();
	deleteGlasProfiles();
    deleteHolzElemente();
    deleteRiegelElemente();
    deleteBetoPlanElemente();
	if (FlParam != NULL) delete FlParam;	 
	if (Profile != NULL) delete Profile;
	if (GlasProfile != NULL) delete GlasProfile;
	if (HolzElemente != NULL) delete HolzElemente;
	if (RiegelElemente != NULL) delete RiegelElemente;
	if (BetoPlanElemente != NULL) delete BetoPlanElemente;
}

void CTorDoor::deleteFluegel()
{
	if (FlParam != NULL)
	{
		int count = FlParam->GetSize();
		for (int i=0; i<count; i++)
		{
			CFlParam* pFl = (CFlParam*)FlParam->GetAt(i);
			delete pFl;
		}
		FlParam->RemoveAll();
	}
}

void CTorDoor::deleteProfiles()
{
	if (Profile != NULL)
	{
		int count = Profile->GetSize();
		for (int i=0; i<count; i++)
		{
			CProfileElem* pFl = (CProfileElem*)Profile->GetAt(i);
			delete pFl;
		}
		Profile->RemoveAll();
	}
}


void CTorDoor::ReleasePtrArray(CPtrArray* pArr)
{
	if (pArr != NULL)
	{
		int count = pArr->GetSize();
		for (int i=0; i<count; i++)
		{
			delete (CHolzElement*)pArr->GetAt(i);
		}
		pArr->RemoveAll();
	}
}

void CTorDoor::deleteGlasProfiles()
{
	if (GlasProfile != NULL)
	{
		int count = GlasProfile->GetSize();
		for (int i=0; i<count; i++)
		{
			CGlasProfileElem* pFl = (CGlasProfileElem*)GlasProfile->GetAt(i);
			delete pFl;
		}
		GlasProfile->RemoveAll();
	}
}

void CTorDoor::deleteHolzElemente()
{
	if (HolzElemente != NULL)
	{
		int count = HolzElemente->GetSize();
		for (int i=0; i<count; i++)
		{
			CHolzElement* pFl = (CHolzElement*)HolzElemente->GetAt(i);
			delete pFl;
		}
		HolzElemente->RemoveAll();
	}
}

void CTorDoor::deleteRiegelElemente()
{
	if (RiegelElemente != NULL)
	{
		int count = RiegelElemente->GetSize();
		for (int i=0; i<count; i++)
		{
			CRiegelElement* pFl = (CRiegelElement*)RiegelElemente->GetAt(i);
			delete pFl;
		}
		RiegelElemente->RemoveAll();
	}
}

void CTorDoor::deleteBetoPlanElemente()
{
	if (BetoPlanElemente != NULL)
	{
		int count = BetoPlanElemente->GetSize();
		for (int i=0; i<count; i++)
		{
			CBetoPlanElement* pFl = (CBetoPlanElement*)BetoPlanElemente->GetAt(i);
			delete pFl;
		}
		BetoPlanElemente->RemoveAll();
	}
}

int CTorDoor::operator== (CTorDoor& td)
{
   return ( Art == td.Art && Typ == td.Typ &&
	    RahmenArt == td.RahmenArt &&
	    DIN == td.DIN && Fuellung == td.Fuellung &&
	    Size == td.Size
	  );
}


HFONT CTorDoor::getFont(int height, int width, int orient)
{
  LOGFONT MainFontRec;
  MainFontRec.lfHeight = height; //*15
  MainFontRec.lfWidth = width;   //*4
  MainFontRec.lfEscapement = orient;
  MainFontRec.lfOrientation = 0;
  MainFontRec.lfWeight = 400;
  MainFontRec.lfItalic = 0;
  MainFontRec.lfUnderline = 0;
  MainFontRec.lfStrikeOut = 0;
  MainFontRec.lfCharSet = ANSI_CHARSET;
  MainFontRec.lfOutPrecision = OUT_DEFAULT_PRECIS;
  MainFontRec.lfClipPrecision = CLIP_DEFAULT_PRECIS;
  MainFontRec.lfQuality = PROOF_QUALITY;
  MainFontRec.lfPitchAndFamily = FIXED_PITCH;
  strcpy(MainFontRec.lfFaceName, "Courier");

  return CreateFontIndirect(&MainFontRec);
}


void CTorDoor::getAddValues(CFlParam* pFl, int count, int &iAddQuerT, int &iAddSenkT, int &iAddOberT, int &iAddUnterT)
{
  if (Typ != 4)
  {
    iAddOberT = 32; iAddUnterT = 30;
    if (Art == TUER)
    {
       iAddQuerT = iAddSenkT = 20;
    }
    else {
      if ((count == 3) && (RahmenArt == RW))
	iAddQuerT = 30;
      else if ((count > 3) && (RahmenArt == RW))
	iAddQuerT = 35;
      else
	iAddQuerT = 32;
      if (OKF == TRUE)   //OKF?
      {
	if (RahmenArt == RZ)   //Z-Rahmen?
        {
	  iAddSenkT = 35;
	  iAddUnterT = 45;
        }
	else if (RahmenArt == RW)  //W-Rahmen
        {
	  iAddSenkT = 25;
	  iAddUnterT = 35;
        }
        else
	  iAddSenkT = 20;
      }
      else
        iAddSenkT = 20;
    }
  }
  else  //Typ 400
  {
    iAddOberT = 22; iAddUnterT = 20;
    iAddQuerT = 22; iAddSenkT = 10;
    if (TRUE == OKF)
    {
	iAddSenkT = 20;
	iAddUnterT = 20;
    }
  }
  if (pFl->OArt == OiF)
  {
      iAddOberT = 10;
  }
}

void CTorDoor::getSFAddValues(CSchiebeFensterZugaben *pSf, CSchiebeFensterZugaben *pSf2)
{
    if (pSf != NULL)
    {
        pSf->m_iQuer = -145;
        pSf->m_iSenkrecht = -30;
        pSf->m_iQuer_400 = -195;
        pSf->m_iSenkrecht_400 = -12;
    }
    if (pSf2 != NULL)
    {
        pSf2->m_iQuer = -206;
        pSf2->m_iSenkrecht = -30;
        pSf->m_iQuer_400 = -206;
        pSf->m_iSenkrecht_400 = -30;
    }
}

void CTorDoor::computeProfile(CFlParam* pFl, int count)
{
  int addQuerT, addSenkT, addOberT, addUnterT;
  int iSaveQuerProf, iSaveSenkProf, iSaveSenkOberProf;
  CSchiebeFensterZugaben Sf, Sf2;

  getAddValues(pFl, count, addQuerT, addSenkT, addOberT, addUnterT);
  getSFAddValues(&Sf, &Sf2);

  CTtmainApp* pApp = (CTtmainApp*)AfxGetApp();
  if (pApp != NULL)
  {
      CZuschnittManager* pMan = pApp->GetZuschnittManager();
      if (pMan != NULL)
      {
        if (pMan->ZugabeWerteHolen(this, pFl, &addSenkT, &addQuerT, &addOberT,
                                    &addUnterT, &Sf, &Sf2) == FALSE)
        {
            // nicht für alle Teile Zugaben gefunden, aber Werte werden nicht überschrieben!!!
        }
      }
  }

  int querProf = iSaveQuerProf = 10*Size.Breite/FluegelAnz+addQuerT;

  if (pFl->Geteilt == TRUE || pFl->OArt == OzO)
  {
    int OberProf = iSaveSenkOberProf = 10*Size.ObtHoehe + addOberT;
    insertProfil(2*StueckZahl, OberProf, SENKRECHT, HOLZUNTEN);
    int UnterProf = iSaveSenkProf = 10*(Size.Hoehe-Size.ObtHoehe)+addUnterT;
    insertProfil(2*StueckZahl, UnterProf, SENKRECHT, HOLZUNTEN);
    insertProfil(2*StueckZahl, querProf, WAAGRECHT, HOLZAUF);
//        insertProfil(2*StueckZahl, querProf, WAAGRECHT, HOLZUNTEN);
//braucht man nicht, da nur HOLZAUF ausgegeben wird
  }
  else
  {  //2 Prof senkr., 2Prof waagrecht
    int senkProf = iSaveSenkProf = 10*Size.Hoehe+addSenkT;
    insertProfil(2*StueckZahl, senkProf, SENKRECHT, HOLZUNTEN);
    insertProfil(1*StueckZahl, querProf, WAAGRECHT, HOLZAUF);
//        insertProfil(1*StueckZahl, querProf, WAAGRECHT, HOLZUNTEN);
//braucht man nicht, da nur HOLZAUF ausgegeben wird
  }
  if (pFl->OArt == OiF)
  {
//    insertProfil(1*StueckZahl, querProf, WAAGRECHT, HOLZAUF);
    int tempQuerProf = querProf;
    int senkProf = 10*Size.ObtHoehe + addOberT;   //????
    tempQuerProf -= 115;
    insertProfil(2*StueckZahl, senkProf, SENKRECHT, HOLZUNTEN);
    insertProfil(1*StueckZahl, tempQuerProf, WAAGRECHT, HOLZAUF);
  }

  // Glas-Profile ermitteln
  if (pFl->FArt == SF2)
  {
    if (Typ == 4)
    {
        int tempQuerProf = querProf;
        int senkProf = 10*Size.ObtHoehe + Sf2.m_iSenkrecht_400; // Add ist negativ!!!
        tempQuerProf = (tempQuerProf + Sf2.m_iQuer_400)/2;
        insertGlasProfil(4*StueckZahl, senkProf, SENKRECHT, HOLZUNTEN);
        insertGlasProfil(2*StueckZahl, tempQuerProf, WAAGRECHT, HOLZAUF);
    }
    else
    {
        int tempQuerProf = querProf;
        int senkProf = 10*Size.ObtHoehe + Sf2.m_iSenkrecht; // Add ist negativ!!!
        tempQuerProf = (tempQuerProf + Sf2.m_iQuer)/2;
        insertGlasProfil(4*StueckZahl, senkProf, SENKRECHT, HOLZUNTEN);
        insertGlasProfil(2*StueckZahl, tempQuerProf, WAAGRECHT, HOLZAUF);
    }
  }
  if (pFl->FArt == SF)
  {
    if (Typ == 4)
    {
        int tempQuerProf = querProf;
        int senkProf = 10*Size.ObtHoehe + Sf.m_iSenkrecht_400;
        tempQuerProf = tempQuerProf + Sf.m_iQuer_400;
        insertGlasProfil(2*StueckZahl, senkProf, SENKRECHT, HOLZUNTEN);
        insertGlasProfil(1*StueckZahl, tempQuerProf, WAAGRECHT, HOLZAUF);
    }
    else
    {
        int tempQuerProf = querProf;
        int senkProf = 10*Size.ObtHoehe + Sf.m_iSenkrecht;
        tempQuerProf = tempQuerProf + Sf.m_iQuer;
        insertGlasProfil(2*StueckZahl, senkProf, SENKRECHT, HOLZUNTEN);
        insertGlasProfil(1*StueckZahl, tempQuerProf, WAAGRECHT, HOLZAUF);
    }
  }

  // Fuellungs-Elemente ermitteln
  if (Fuellung != -1)
  {
      // Fuellung wurda aus Liste ausgewählt
      tFUELLUNG fAussen, fInnen;
      TDataScan ds;
      if (ds.getFuellung(Fuellung, &fAussen, &fInnen) == FALSE)
          return;
      if ((pFl->FArt != KEIN) && (pFl->OArt == LEER))
      {
          // Fenster vorhanden und kein Oberteil
          iSaveSenkProf -= pFl->ObtHoehe*10;
          iSaveSenkOberProf -= pFl->ObtHoehe*10;  //??
      }

      if (fAussen == F_HOLZ || fInnen == F_HOLZ)
      {
          // Holzfuellung
          int iSeitenAnz, iFensterKor;
          if (fAussen == F_HOLZ && fInnen == F_HOLZ)
              iSeitenAnz = 2;
          else
              iSeitenAnz = 1;

          if ((pFl->FArt != KEIN) && (pFl->OArt == LEER))
          {
              // Fenster und kein Oberteil, 5mm abziehen
              iFensterKor = 5;
          }
          else
              iFensterKor = 0;

          if (Typ == 4)
          {
          }
          else
          {
              // Typ 600 + 800
              int iAnzNorm;      // Anzahl der normal breiten Bretter
              int iLaenge;       // Laenge der Bretter
              float fBreiteRest;   // Breite der Rest-Bretter
              int iBreiteRest;     // Breite der Rest-Bretter gerundet
              int iRiegelLaenge, iRiegelAnz;

              // Riegel für jeden angefangenen Meter
              iRiegelLaenge = iSaveQuerProf - 110;

              // Anzahl + Breite
              int iCompBreite = (iSaveQuerProf - 100);  // Breite für Holzfüllung
              iAnzNorm = iCompBreite / HOLZELEM_BREITE;
              iAnzNorm -= 1;
              fBreiteRest = (float)(iCompBreite - (iAnzNorm * HOLZELEM_BREITE)) / (float)2;
              iBreiteRest = (int)fBreiteRest;

#if 0
              //?? das ganze mit Array-Werten ueberschreiben
              HolzZuschnitt* phz;
              if (ds.getHolzZuschnitt(iRiegelLaenge, &phz) == TRUE)
              {
                 iAnzNorm = phz->iAnzBretter;
                 iBreiteRest = phz->iLenAnfaenger;
              }
#endif
              // Laenge
              if ((pFl->Geteilt == TRUE || pFl->OArt == OzO) && (pFl->FArt == KEIN))
              {
                  // Unterteil
                  iLaenge = iSaveSenkProf - 115;
                  insertHolzElement(iAnzNorm*StueckZahl*iSeitenAnz, HOLZELEM_BREITE, iLaenge);
                  insertHolzElement(2*StueckZahl*iSeitenAnz, iBreiteRest, iLaenge);
                  // Oberteil
                  iLaenge = iSaveSenkOberProf - 115;
                  insertHolzElement(iAnzNorm*StueckZahl*iSeitenAnz, HOLZELEM_BREITE, iLaenge);
                  insertHolzElement(2*StueckZahl*iSeitenAnz, iBreiteRest, iLaenge);
              }
              else
              { 
                  iLaenge = iSaveSenkProf - 115 - iFensterKor;

                  insertHolzElement(iAnzNorm*StueckZahl*iSeitenAnz, HOLZELEM_BREITE, iLaenge);
                  insertHolzElement(2*StueckZahl*iSeitenAnz, iBreiteRest, iLaenge);
              }
          
              if (pFl->FArt == KEIN && pFl->OArt != LEER)
              {
                  // Oberteil und kein Fenster
                  iLaenge = Size.Hoehe - Size.ObtHoehe;
                  // Riegel für Unterteil
                  iRiegelAnz = ceil((double)iLaenge/100.0f) + 1;
                  // Riegel für Oberteil
                  iRiegelAnz += ceil((double)Size.ObtHoehe/100.0f) + 1;
              }
              else
              {
                  if (pFl->FArt != KEIN)
                      // Fenster
                     iLaenge = Size.Hoehe - Size.ObtHoehe;
                  else
                     iLaenge = Size.Hoehe;

                  iRiegelAnz = ceil((double)iLaenge/100.0f) + 1;
              }

              insertRiegelElement(iRiegelAnz*StueckZahl, iRiegelLaenge);
          }
      }
      if (fAussen == F_BETOPLAN || fInnen == F_BETOPLAN)
      {
          // Fuellung mit Beto-Plan
          int iSeitenAnz;
          if (fAussen == F_BETOPLAN && fInnen == F_BETOPLAN)
              iSeitenAnz = 2;
          else
              iSeitenAnz = 1;
          if (Typ == 4)
          {
          }
          else
          {
              // Typ 600 + 800
              int iLaenge;       // Laenge der Bretter

              // Laenge
              if ((pFl->Geteilt == TRUE || pFl->OArt == OzO) && (pFl->FArt == KEIN))
              {
                  // mit Oberteil und kein Fenster
                  // Unterreil
                  iLaenge = iSaveSenkProf - 120;
                  insertBetoPlanElement(1*StueckZahl*iSeitenAnz, iSaveQuerProf - 128, iLaenge);
                  // Oberteil
                  iLaenge = iSaveSenkOberProf - 120;
                  insertBetoPlanElement(1*StueckZahl*iSeitenAnz, iSaveQuerProf - 128, iLaenge);
              }
              else
              { 
                  iLaenge = iSaveSenkProf - 120;

                  insertBetoPlanElement(1*StueckZahl*iSeitenAnz, iSaveQuerProf - 128, iLaenge);
              }          
          }
      }
  }
}

void CTorDoor::computeBogenProfile(CFlParam *pFl, int count)
{
    CEvaluateDoubleCircle math;
    int addQuerT, addSenkT, addOberT, addUnterT;

    /* Sicherheitsabfrage: nur Typ 600 bis jetzt unterstützt */
    if (Typ != 6)
        return;
    /* normale Teile */
    getAddValues(pFl, count, addQuerT, addSenkT, addOberT, addUnterT);
    int untenQuerProf = 10*Size.Breite/FluegelAnz;

    /* Bogen Teile */
    double dWinkelRahmen, dWinkelBogen, dWinkelMitte;
    double dProfilDicke = Size.iProfilDicke; //20; /*in mm Typ 600???*/
    double dRadius = math.EvalRadius(10*Size.Breite, 10*Size.SehnenHoehe);
    double dObenQuerBogen = math.EvalArcLength(dRadius, 10*Size.Breite, dProfilDicke);
    double dObenQuerProf = dObenQuerBogen/FluegelAnz;    
    if (FluegelAnz == 1)
    {
        // Tür
        math.EvalAngleWithFrame(dRadius, 10*Size.Breite, 10*Size.SehnenHoehe,
            dProfilDicke, &dWinkelRahmen, &dWinkelBogen);

        // Querteil oben: -2*15 (== Luft links und rechts) + 0 (=Handschw.)
        int obenQuerProf = rnd(dObenQuerProf) - 2*15;
        insertProfil(1*StueckZahl, obenQuerProf, WAAGRECHT, HOLZAUF, rnd(dWinkelBogen),
            rnd(dWinkelBogen));

        // Querteil unten: -2*15 (== Luft links und rechts) + 2*5 (=Maschinenschw.)
        untenQuerProf = untenQuerProf - 2*15 + 2*5;
        insertProfil(1*StueckZahl, untenQuerProf, WAAGRECHT, HOLZAUF);


        if (pFl->Geteilt == TRUE || pFl->OArt == OzO)
        {
            // Querteil mitte: -2*15 (== Luft links und rechts) + 2*5 (=Maschinenschw.)
            insertProfil(1*StueckZahl, untenQuerProf, WAAGRECHT, HOLZAUF);

            // Querteil mitte: -2*15 (== Luft links und rechts) + 2*5 (=Maschinenschw.)
            insertProfil(1*StueckZahl, untenQuerProf, WAAGRECHT, HOLZUNTEN);

            // Senkteil oben Rand: default - 50 +5(=Maschinenschw. unten)
            int OberProf = 10*Size.ObtHoehe + addOberT -50 +5;
            insertProfil(2*StueckZahl, OberProf, SENKRECHT, HOLZUNTEN,
                0, rnd(dWinkelRahmen));

            // Senkteil unten : default - 50 +10(=Maschinenschw. oben+unten)
            int UnterProf = 10*(Size.Hoehe-Size.ObtHoehe)+addUnterT-50+10;
            insertProfil(2*StueckZahl, UnterProf, SENKRECHT, HOLZUNTEN);
            //insertProfil(2*StueckZahl, querProf, WAAGRECHT, HOLZAUF);
            //        insertProfil(2*StueckZahl, querProf, WAAGRECHT, HOLZUNTEN);
            //braucht man nicht, da nur HOLZAUF ausgegeben wird
        }
        else
        {
            // Senkteile
            // : -2*15 (=unten und oben Luft) + 5 (=unten Maschinenschw.)
            int senkProf = 10*Size.Hoehe - 2*15 +5;
            insertProfil(2*StueckZahl, senkProf, SENKRECHT, HOLZUNTEN, 0, rnd(dWinkelRahmen));
        }
    }
    else
    {
        // Tor
        math.EvalAngleWithFrame(dRadius, 10*Size.Breite, 10*Size.SehnenHoehe,
            dProfilDicke, &dWinkelRahmen, &dWinkelBogen);
        dWinkelMitte = math.EvaluateAngleMid(dRadius, dProfilDicke);

        // Querteile
        // unten: -15 (== Luft Rand)- 3 (=Luft mitte) + 2*5 (=Maschinenschw.)
        untenQuerProf = untenQuerProf - 15 - 3 +2*5;
        insertProfil(1*StueckZahl, untenQuerProf, WAAGRECHT, HOLZAUF);

        // oben: -15 (== Luft Rand)-3 (=Luft mitte) + 0 (=Handschw.)
        int obenQuerProf = dObenQuerProf - 15 - 3;
        insertProfil(1*StueckZahl, obenQuerProf, WAAGRECHT, HOLZAUF, rnd(dWinkelBogen),
            rnd(dWinkelMitte));

        if (pFl->Geteilt == TRUE || pFl->OArt == OzO)
        {
            // Querteil mitte: -2*15 (== Luft links und rechts) + 2*5 (=Maschinenschw.)
            insertProfil(1*StueckZahl, untenQuerProf, WAAGRECHT, HOLZAUF);

            // Querteil mitte: -2*15 (== Luft links und rechts) + 2*5 (=Maschinenschw.)
            insertProfil(1*StueckZahl, untenQuerProf, WAAGRECHT, HOLZUNTEN);

            // Senkteil oben Rand: default - 50 +5(=Maschinenschw. unten)
            int OberProf = 10*Size.ObtHoehe + addOberT -50 +5;
            insertProfil(1*StueckZahl, OberProf, SENKRECHT, HOLZUNTEN,
                0, rnd(dWinkelRahmen));

            // Senkteil unten : default - 50 +10(=Maschinenschw. oben+unten)
            int UnterProf = 10*(Size.Hoehe-Size.ObtHoehe)+addUnterT-50+10;
            insertProfil(1*StueckZahl, UnterProf, SENKRECHT, HOLZUNTEN);

            // Senkteil oben mitte: default - 50 +5(=Maschinenschw. unten)
            OberProf = 10*(Size.SehnenHoehe+Size.ObtHoehe) + addOberT -50 +5;
            insertProfil(1*StueckZahl, OberProf, SENKRECHT, HOLZUNTEN,
                0, 90-rnd(dWinkelMitte));

            // Senkteil unten mitte: default - 50 +10(=Maschinenschw. oben+unten)
            UnterProf = 10*(Size.Hoehe-Size.ObtHoehe)+addUnterT-50+10;
            insertProfil(1*StueckZahl, UnterProf, SENKRECHT, HOLZUNTEN);
            //        insertProfil(2*StueckZahl, querProf, WAAGRECHT, HOLZUNTEN);
            //braucht man nicht, da nur HOLZAUF ausgegeben wird
        }
        else
        {
            // Senkteile
            // am Rand: -2*15 (=unten und oben Luft) + 5 (=unten Maschinenschw.)
            int senkProf = 10*Size.Hoehe - 2*15 +5;
            insertProfil(StueckZahl, senkProf, SENKRECHT, HOLZUNTEN, 0, rnd(dWinkelRahmen));

            // in der Mitte: -2*15 (=unten und oben Luft) + 5 (=unten Maschinenschw.)
            senkProf = 10*(Size.Hoehe+Size.SehnenHoehe) - 2*15 +5;
            insertProfil(StueckZahl, senkProf, SENKRECHT, HOLZUNTEN, 0, 90-rnd(dWinkelMitte));
        }
    }

    Size.dRadius = dRadius;  // in cm umrechnen!!!!
}

/*
void CTorDoor::computeProfile(CFlParam* pFl, int count)
{
  int addQuerT, addSenkT, addOberT, addUnterT;

  if (Typ != 4)
  {
    addOberT = 32; addUnterT = 30;
    if (Art == TUER)
    {
       addQuerT = addSenkT = 20;
    }
    else {
      if ((count == 3) && (RahmenArt == RW))
	addQuerT = 30;
      else if ((count > 3) && (RahmenArt == RW))
	addQuerT = 35;
      else
	addQuerT = 32;
      if (OKF == TRUE)   //OKF?
      {
	if (RahmenArt == RZ)   //Z-Rahmen?
        {
	  addSenkT = 35;
	  addUnterT = 45;
        }
	else if (RahmenArt == RW)  //W-Rahmen
        {
	  addSenkT = 25;
	  addUnterT = 35;
        }
        else
	  addSenkT = 20;
      }
      else
        addSenkT = 20;
    }
  }
  else  //Typ 400
  {
    addOberT = 22; addUnterT = 20;
    addQuerT = 22; addSenkT = 10;
    if (TRUE == OKF)
    {
	addSenkT = 20;
	addUnterT = 20;
    }
  }

  int querProf = 10*Size.Breite/FluegelAnz+addQuerT;

  if (pFl->Geteilt == TRUE || pFl->OArt == OzO)
  {
    int OberProf = 10*Size.ObtHoehe + addOberT;
    insertProfil(2*StueckZahl, OberProf, SENKRECHT, HOLZUNTEN);
    int UnterProf = 10*(Size.Hoehe-Size.ObtHoehe)+addUnterT;
    insertProfil(2*StueckZahl, UnterProf, SENKRECHT, HOLZUNTEN);
    insertProfil(2*StueckZahl, querProf, WAAGRECHT, HOLZAUF);
//        insertProfil(2*StueckZahl, querProf, WAAGRECHT, HOLZUNTEN);
//braucht man nicht, da nur HOLZAUF ausgegeben wird
  }
  else
  {  //2 Prof senkr., 2Prof waagrecht
    int senkProf = 10*Size.Hoehe+addSenkT;
    insertProfil(2*StueckZahl, senkProf, SENKRECHT, HOLZUNTEN);
    insertProfil(1*StueckZahl, querProf, WAAGRECHT, HOLZAUF);
//        insertProfil(1*StueckZahl, querProf, WAAGRECHT, HOLZUNTEN);
//braucht man nicht, da nur HOLZAUF ausgegeben wird
  }
  if (pFl->OArt == OiF)
  {
    int senkProf = 10*Size.ObtHoehe + 20;   //????
    querProf -= 115;
    insertProfil(2*StueckZahl, senkProf, SENKRECHT, HOLZUNTEN);
    insertProfil(1*StueckZahl, querProf, WAAGRECHT, HOLZAUF);
  }
}
*/

//berechnet den Skalierungsfaktor für eine Tür, um in
//vorgegebenes Maß zu passen 
float CTorDoor::calculateScale(int breite, int hoehe)
{
  float scaleB;
  float scaleH;
  float scaleRet;
  float fbreite = (float)breite;
  float fhoehe = (float)hoehe;
  float fthoehe = (float)Size.Hoehe;
  if (bBogen == TRUE)
  {
      fthoehe += Size.SehnenHoehe; //SEHNENHOEHE; //Size.SehnenHoehe;
  }
   
  float ftbreite = (float)Size.Breite;

  scaleB = (float)fbreite/ftbreite;
  scaleH = (float)fhoehe/fthoehe;
  scaleRet = ((scaleB < scaleH) ? scaleB : scaleH);
  Size.ZGesamtHoehe = scaleRet*Size.Hoehe;
  if (bBogen == TRUE)
  {
      Size.ZGesamtHoehe += scF*Size.SehnenHoehe;
  }
  return scaleRet;
}
    

BOOL CTorDoor::insertProfil(int Anzahl, int Laenge, tDirect dir,
                              tTypWaagrecht uTyp, int iWinkel1, 
                              int iWinkel2)
{
  BOOL found = FALSE;
  int count = Profile->GetSize();
  for (int i=0; i<count&&!found; i++)
  {
    CProfileElem *tempElem = (CProfileElem*) Profile->GetAt(i);
    if (tempElem->Laenge == Laenge &&
	    tempElem->Direction == dir && tempElem->uTyp == uTyp &&
        tempElem->iWinkel1 == iWinkel1 && tempElem->iWinkel2 == iWinkel2)
    {
      found = TRUE;
      tempElem->Anzahl += Anzahl;
    }
  }
  if (!found)
  {
     CProfileElem* pP = new CProfileElem(Anzahl, Laenge, dir,
     				         uTyp, iWinkel1, iWinkel2);
     Profile->Add(pP);
  }
  return found;
}
 
BOOL CTorDoor::insertGlasProfil(int Anzahl, int Laenge, tDirect dir,
                              tTypWaagrecht uTyp)
{
  BOOL found = FALSE;
  int count = GlasProfile->GetSize();
  for (int i=0; i<count&&!found; i++)
  {
    CGlasProfileElem *tempElem = (CGlasProfileElem*) GlasProfile->GetAt(i);
    if (tempElem->Laenge == Laenge &&
	tempElem->Direction == dir
	&& tempElem->uTyp == uTyp)
    {
      found = TRUE;
      tempElem->Anzahl += Anzahl;
    }
  }
  if (!found)
  {
     CGlasProfileElem* pP = new CGlasProfileElem(Anzahl, Laenge, dir,
     				         uTyp);
     GlasProfile->Add(pP);
  }
  return found;
}

BOOL CTorDoor::insertHolzElement(int iAnzahl, int iBreite, int iLaenge)
{
  BOOL found = FALSE;
  int count = HolzElemente->GetSize();
  for (int i=0; i<count&&!found; i++)
  {
    CHolzElement *tempElem = (CHolzElement*) HolzElemente->GetAt(i);
    if (tempElem->m_iLaenge == iLaenge &&
	tempElem->m_iBreite == iBreite)
    {
      found = TRUE;
      tempElem->m_iAnzahl += iAnzahl;
    }
  }
  if (!found)
  {
     CHolzElement* pP = new CHolzElement(iAnzahl, iBreite, iLaenge);
     HolzElemente->Add(pP);
  }
  return found;
}

BOOL CTorDoor::insertRiegelElement(int iAnzahl, int iLaenge)
{
  BOOL found = FALSE;
  int count = RiegelElemente->GetSize();
  for (int i=0; i<count&&!found; i++)
  {
    CRiegelElement *tempElem = (CRiegelElement*) RiegelElemente->GetAt(i);
    if (tempElem->m_iLaenge == iLaenge)
    {
      found = TRUE;
      tempElem->m_iAnzahl += iAnzahl;
    }
  }
  if (!found)
  {
     CRiegelElement* pP = new CRiegelElement(iAnzahl, iLaenge);
     RiegelElemente->Add(pP);
  }
  return found;
}

BOOL CTorDoor::insertBetoPlanElement(int iAnzahl, int iBreite, int iLaenge)
{
  BOOL found = FALSE;
  int count = BetoPlanElemente->GetSize();
  for (int i=0; i<count&&!found; i++)
  {
    CBetoPlanElement *tempElem = (CBetoPlanElement*) BetoPlanElemente->GetAt(i);
    if (tempElem->m_iLaenge == iLaenge &&
	tempElem->m_iBreite == iBreite)
    {
      found = TRUE;
      tempElem->m_iAnzahl += iAnzahl;
    }
  }
  if (!found)
  {
     CBetoPlanElement* pP = new CBetoPlanElement(iAnzahl, iBreite, iLaenge);
     BetoPlanElemente->Add(pP);
  }
  return found;
}

void CTorDoor::drawTT(HDC hdc, int x, int y, int b, int h)
{
}

void CTorDoor::draw(HDC hdc)
{
  drawTT(hdc, dx, dy, db, dh);
}

void CTorDoor::drawDIN(HDC hdc, int x, int y)
{
  POINT point;
  MoveToEx(hdc, x, -y, &point);
  Rectangle(hdc, x, -y, x+ scF*DINBREITE, -(y+ scF*DINHOEHE));
}


//Bemaßung zeichnen; x,y linke obere Ecke des Tores
void CTorDoor::drawBem(HDC hdc, int x, int y)
{
  int bx = x+Size.ZBreite+ scF*BLINIEAB;
  MoveToEx(hdc, bx, y, NULL);
  LineTo(hdc, bx+ scF*BLINIEL, y);  
  bx = x+Size.ZBreite+ scF*BEMABSTAND;
  int Ueber =  scF*BLINIEO;
  MoveToEx(hdc, bx, y-Ueber, NULL);
  LineTo(hdc, bx, y+ scF*Size.Hoehe+Ueber);
  bx = x+Size.ZBreite+ scF*BLINIEAB;
  MoveToEx(hdc, bx, y+ scF*Size.Hoehe, NULL);
  LineTo(hdc, bx+ scF*BLINIEL, y+ scF*Size.Hoehe);

  char zahl[10];
  float fHoehe = Size.Hoehe;
  fHoehe = fHoehe/100;
  sprintf(zahl, "%5.2f", fHoehe);
  bx = x+Size.ZBreite+ scF*(BEMABSTAND+5);
  TextOut(hdc, bx, y+ scF*(Size.Hoehe/2-5), zahl, strlen(zahl));

  bx = x;
  int by = y+ scF*(Size.Hoehe+BLINIEAB);
  MoveToEx(hdc, bx, by, NULL);
  LineTo(hdc, bx, by+ scF*BLINIEL);
  by = y+ scF*(Size.Hoehe+BEMABSTAND);
  MoveToEx(hdc, bx-Ueber, by, NULL);
  bx = bx+Size.ZBreite;
  LineTo(hdc, bx+Ueber, by);
  by = y+ scF*(Size.Hoehe+BLINIEAB);
  MoveToEx(hdc, bx, by, NULL);
  LineTo(hdc, bx, by+ scF*BLINIEL);

  float fBreite = Size.Breite;
  fBreite /= 100;
  sprintf(zahl, "%5.2f", fBreite);
  TextOut(hdc, x+Size.ZBreite/2- scF*30, y+ scF*(Size.Hoehe+BEMABSTAND+10), zahl, strlen(zahl));
}

void CTorDoor::drawOIF(HDC hdc, int x, int y, int breite, int hoehe, int seite)
{
  int Sp =  rnd(scF*2);

  Rectangle(hdc, x, -y, x+breite, -(y+hoehe));
  Rectangle(hdc, x+Sp, -(y+Sp), x+breite-Sp, -(y+hoehe-Sp));
  int versatz;
  if (seite == 0) //links
    versatz = -scF* OIFBANDBREITE;
  else
    versatz = breite;

  int xAbstand = scF* 10;//hoehe-scF* 2*OIFBANDHOEHE/6;
  Rectangle(hdc, x+versatz, -(y+xAbstand), x+versatz+scF* OIFBANDBREITE, -(y+xAbstand+scF* OIFBANDHOEHE));
  Rectangle(hdc, x+versatz, -(y+hoehe-xAbstand), x+versatz+scF* OIFBANDBREITE, -(y+hoehe-xAbstand-scF* OIFBANDHOEHE));
}


void CTorDoor::drawInfo(HDC hdc, int x, int y)
{
  char infoStr[100];
  char styp[20];
  char sdin;
  TDataScan datascan;
  int iTyp = Typ*100;
  itoa(iTyp, styp, 10);
  if (DIN == L)
      sdin = 'L';
  else
      sdin = 'R';
  sprintf(infoStr, "%s Typ %s m. %s, verz. DIN %c",
	  datascan.getArtBez((tART)Art),styp,datascan.getRahmenBez((tRAHMEN)RahmenArt),sdin);
   
  TextOut(hdc, x, y, infoStr, strlen(infoStr));
  if (Fuellung != 0)
  {
    y +=  15;
    strcpy(infoStr, datascan.getBezeich(Fuellung));
    TextOut(hdc, x, y, infoStr, strlen(infoStr));
  }
}
 
void CTorDoor::drawBesch(HDC hdc, int x, int y)
{
}

BOOL CTorDoor::drawConnectedPoints(HDC hdc, LPPOINT pPoints, int iCount)
{
    POINT point;
    int i = 0;

    if (!iCount || pPoints == NULL)
        return FALSE;

    MoveToEx(hdc, pPoints[i].x, pPoints[i].y, &point);

    i++;
    while (i < iCount)
    {
        LineTo(hdc, pPoints[i].x, pPoints[i].y);
        i++;
    }

    return TRUE;
}

BOOL CTorDoor::inBounds(int x, int y, int mx, int my)
{
  return (mx <= x+Size.ZBreite && my <= y+scF*Size.Hoehe);
}


int CTorDoor::isInOnBounds(int x, int y)
//0 : außerhalb, 1 : auf, 2 : innerhalb
{
   if (((x<dx) || (x>(dx+Size.ZBreite))) ||
      ((y<dy) || (y>(dy+scF*Size.Hoehe))))
      return 0;
   else
   if ((dx<x && x<(dx+Size.ZBreite)) &&
       (dy<y && y<(dy+scF*Size.Hoehe)))
       return 2;
   else
       return 1;
}


int CTorDoor::printKundKomm(HDC hdc, int x, int y, int maxX, HFONT bFont)
{
  char szBuf[100];
  HFONT orgFont = (HFONT)SelectObject(hdc, bFont);
  TextOut(hdc, x, -y, Kunde, strlen(Kunde));
  SelectObject(hdc, orgFont);
  TextOut(hdc, x, -(y+rowH), Kommission, strlen(Kommission));

  y += getLineBegin(CA_Y_AFTER_ROFILES);
  maxX -= 20;

  if (GlasProfile->GetSize() > 0)
  {
      y += rowH;
      CFlParam* FluegelP = (CFlParam*)FlParam->GetAt(0);
      if (FluegelP != NULL)
      {
          switch(FluegelP->FArt)
          {
          case SF:
          case SF2:
              strcpy(szBuf, "Schiebef.");
              break;
          case KF:
              strcpy(szBuf, "Kippf.");
              break;
          default:
              strcpy(szBuf, "Fenster");
              break;
          }
      }
      else
        strcpy(szBuf, "Fenster");
      OutputAligned(hdc, E_ALIGN_RIGHT, szBuf, maxX, -y);
  }
  y += GlasProfile->GetSize()*rowH;
  
  y += rowH;

  if (HolzElemente->GetSize() > 0)
  {
      strcpy(szBuf, "Bretter");
      OutputAligned(hdc, E_ALIGN_RIGHT, szBuf, maxX, -y);
  }
  y += HolzElemente->GetSize()*rowH;

  if (RiegelElemente->GetSize() > 0)
  {
      strcpy(szBuf, "Riegel");
      OutputAligned(hdc, E_ALIGN_RIGHT, szBuf, maxX, -y);
  }
  y += RiegelElemente->GetSize()*rowH;

  if (BetoPlanElemente->GetSize() > 0)
  {
      strcpy(szBuf, "Beto");
      OutputAligned(hdc, E_ALIGN_RIGHT, szBuf, maxX, -y);
  }
  return y+2* rowH;
}

int CTorDoor::printStueck(HDC hdc, int x, int y, int maxX, HFONT bFont)
{
  char temp[50];
  char buf[200];
  TDataScan dataScan;

  HFONT orgFont = (HFONT)SelectObject(hdc, bFont);
  sprintf(temp, "%d x %s", StueckZahl, dataScan.getArtBez((tART)Art));
  TextOut(hdc, x, -y, temp, strlen(temp));
  SelectObject(hdc, orgFont);

  if (bBogen == TRUE)
    y += rowH/2;

  y += rowH;
  if (TRUE == OKF)
  {
//     y += rowH/2;
     y +=  rowH/2;
  }
  else
  {
      // weiter von Profil weg
      y += rowH/2;
  }


  for (int i=0; i<Profile->GetSize(); i++)
  {
     CProfileElem* pF = (CProfileElem*)Profile->GetAt(i);
     if (pF->Direction == SENKRECHT)
     {
        if ((Typ == 4) && (Art != ATUER))
        {
            // Typ 400: 2 unterschiedliche Profile bei Senkrecht
            // Innenstreben jeweils eines anders
            y += rowH;
	        sprintf(buf, "%d St.", pF->Anzahl-StueckZahl*(FlParam->GetSize()-1));
	        TextOut(hdc, x, -(y), buf, strlen(buf));
            y += rowH;
	        sprintf(buf, "%d St.", StueckZahl*(FlParam->GetSize()-1));
	        TextOut(hdc, x, -(y), buf, strlen(buf));
        }
        else
        {
            y += rowH;
	        sprintf(buf, "%d St.", pF->Anzahl);
	        TextOut(hdc, x, -(y), buf, strlen(buf));
        }
     }
  }
  for (i=0; i<Profile->GetSize(); i++)
  {
     CProfileElem* pF = (CProfileElem*)Profile->GetAt(i);
     if (pF->Direction == WAAGRECHT)
     {
        y += rowH;
        if (bBogen == TRUE)
	        sprintf(buf, "%d St.", pF->Anzahl);
        else
	        sprintf(buf, "je %d St.", pF->Anzahl);
	    TextOut(hdc, x, -(y), buf, strlen(buf));
     }
  }

  y += rowH;
  for (i=0; i<GlasProfile->GetSize(); i++)
  {
     CGlasProfileElem* pF = (CGlasProfileElem*)GlasProfile->GetAt(i);
     if (pF->Direction == SENKRECHT)
     {
        y += rowH;
		sprintf(buf, "%d St.", pF->Anzahl);
		TextOut(hdc, x, -(y), buf, strlen(buf));
     }
  }
  for (i=0; i<GlasProfile->GetSize(); i++)
  {
     CGlasProfileElem* pF = (CGlasProfileElem*)GlasProfile->GetAt(i);
     if (pF->Direction == WAAGRECHT)
     {
        y += rowH;
		sprintf(buf, "je %d St.", pF->Anzahl);
		TextOut(hdc, x, -(y), buf, strlen(buf));
     }
  }

  if (i > 0)
     y += rowH;

  if (bBogen == TRUE)
  {
//      y += rowH;
      if (Typ == 6)
        strcpy(buf, "Achtung: Kastenmaße !");
      else
        strcpy(buf, "Berechnung nur für Typ 600 !");

      TextOut(hdc, x, -(y), buf, strlen(buf));
  }


  // Ausgabe der Holz-Elemente
  // Ausgabe:           Stück       |       Breite         |   Höhe             | DIN
  //            --------------------|----------------------|--------------------|------
  //         Anzahl ganze Bretter   |                      |  Länge der Bretter |   |
  //         Anzahl Rest-Bretter    |                      |      Breite        |  |-|
  for (i=0; i<HolzElemente->GetSize(); i++)
  {
     CHolzElement* pF = (CHolzElement*)HolzElemente->GetAt(i);
     y += rowH;
     sprintf(buf, "%d St.", pF->m_iAnzahl);
	 TextOut(hdc, x, -(y), buf, strlen(buf));
  }
  for (i=0; i<RiegelElemente->GetSize(); i++)
  {
     CRiegelElement* pF = (CRiegelElement*)RiegelElemente->GetAt(i);
     y += rowH;
     sprintf(buf, "%d St.", pF->m_iAnzahl);
	 TextOut(hdc, x, -(y), buf, strlen(buf));
  }
  for (i=0; i<BetoPlanElemente->GetSize(); i++)
  {
     CBetoPlanElement* pF = (CBetoPlanElement*)BetoPlanElemente->GetAt(i);
     y += rowH;
     sprintf(buf, "%d St.", pF->m_iAnzahl);
	 TextOut(hdc, x, -(y), buf, strlen(buf));
  }

  return 0; //temp., später richtig!!!
}


// Function name	: CTorDoor::getLineBegin
// Description	    : gibt denAbstand in y zurück, an der ein Abschnitt beginnt
// Return type		: int Abstand in Pixel von oben herab
// Argument         : int iKind   Art des Abschnitts:
//                                  0 : ganz oben
//                                  1 : nach den Profilen
//                                  2 : nach den Glas-Profilen
//                                  3 : nach den Holz-Elementen
int CTorDoor::getLineBegin(int iKind)
{
  int iLineCount = 0;

  if (iKind == CA_Y_TOP)
      return iLineCount;

  if (bBogen == TRUE)
    iLineCount += rowH/2;

  iLineCount += rowH;
  if (TRUE == OKF)
  {
//     y += rowH/2;
     iLineCount +=  rowH/2;
  }
  else
  {
      // weiter von Profil weg
      iLineCount += rowH/2;
  }


  for (int i=0; i<Profile->GetSize(); i++)
  {
     CProfileElem* pF = (CProfileElem*)Profile->GetAt(i);
     if (pF->Direction == SENKRECHT)
     {
        if ((Typ == 4) && (Art != ATUER))
        {
            // Typ 400: 2 unterschiedliche Profile bei Senkrecht
            // Innenstreben jeweils eines anders
            iLineCount += rowH;
            iLineCount += rowH;
        }
        else
        {
            iLineCount += rowH;
        }
     }
  }
  for (i=0; i<Profile->GetSize(); i++)
  {
     CProfileElem* pF = (CProfileElem*)Profile->GetAt(i);
     if (pF->Direction == WAAGRECHT)
     {
        iLineCount += rowH;
     }
  }

  iLineCount += rowH;

  if (iKind == CA_Y_AFTER_ROFILES)
      return iLineCount;

  for (i=0; i<GlasProfile->GetSize(); i++)
  {
     CGlasProfileElem* pF = (CGlasProfileElem*)GlasProfile->GetAt(i);
     if (pF->Direction == SENKRECHT)
     {
        iLineCount += rowH;
     }
  }
  for (i=0; i<GlasProfile->GetSize(); i++)
  {
     CGlasProfileElem* pF = (CGlasProfileElem*)GlasProfile->GetAt(i);
     if (pF->Direction == WAAGRECHT)
     {
        iLineCount += rowH;
     }
  }

  // Abstand für nachfolgende Einträge
  if (i > 0)
      iLineCount += rowH;
  if (bBogen == TRUE)
  {
//      iLineCount += rowH;
  }

  if (iKind == CA_Y_AFTER_GLASPROFILES)
      return iLineCount;

  // Ausgabe der Holz-Elemente
  // Ausgabe:           Stück       |       Breite         |   Höhe             | DIN
  //            --------------------|----------------------|--------------------|------
  //         Anzahl ganze Bretter   |                      |  Länge der Bretter |   |
  //         Anzahl Rest-Bretter    |                      |      Breite        |  |-|
  for (i=0; i<HolzElemente->GetSize(); i++)
  {
     iLineCount += rowH;
  }
  for (i=0; i<RiegelElemente->GetSize(); i++)
  {
     iLineCount += rowH;
  }
  for (i=0; i<BetoPlanElemente->GetSize(); i++)
  {
     iLineCount += rowH;
  }

  iLineCount += rowH;
  return iLineCount;
}

int CTorDoor::printBreite(HDC hdc, int x, int y, int maxX, HFONT bFont,
                          HFONT bThinFont)
{
  char temp[50];
  int iYOrg = y;
  char buf[100];

  HFONT orgFont = (HFONT)SelectObject(hdc, bFont);
  sprintf(temp, "%d", 10*Size.Breite);
  TextOut(hdc, x, -y, temp, strlen(temp));
  SelectObject(hdc, orgFont);

  drawProfil(hdc, x, y+rowH+15, Typ);

  if (bBogen == TRUE)
    y += rowH/2;
  
  y +=  rowH;
  if (TRUE == OKF)
  {
//     y += rowH/2;
     y += rowH/2;
  }
  else
  {
     // weiter von Profil weg
     y += rowH/2;
  }

  if (Typ == 4 && Art != ATUER)
  {
      // zwei Profile zeichnen
      y += rowH;
      profilRP1093(hdc, x+15, y-10);
//      profilRP1615(hdc, x+15, y);
      y += rowH;
//      profilRP1614(hdc, x+15, y);
      profilRP1094(hdc, x+15, y+5);
  }
  // Bogenberechnung nur für Typ 600!!!
  if (bBogen == TRUE)
  {
      orgFont = (HFONT)SelectObject(hdc, bThinFont);
      for (int i=0; i<Profile->GetSize(); i++)
      {
         char buf[100];
         CProfileElem* pF = (CProfileElem*)Profile->GetAt(i);
         if (pF->Direction == SENKRECHT)
         {
           y += rowH;
           if (pF->iWinkel2 != 0)
           {
              sprintf(buf, "%d°", pF->iWinkel2);
              TextOut(hdc, x, -(y), buf, strlen(buf));
           }
         }
      }
      for (i=0; i<Profile->GetSize(); i++)
      {
         CProfileElem* pF = (CProfileElem*)Profile->GetAt(i);
         if (pF->Direction == WAAGRECHT && (pF->iWinkel1 || pF->iWinkel2))
         {
           y += rowH;
           sprintf(buf, "%d°,%d°", pF->iWinkel1, pF->iWinkel2);
           TextOut(hdc, x, -(y), buf, strlen(buf));
         }
         else
             if (pF->Direction == WAAGRECHT)
                y += rowH;
      }
      SelectObject(hdc, orgFont);
  }

  if (GlasProfile->GetSize() > 0)
  {
      y = iYOrg + getLineBegin(CA_Y_AFTER_ROFILES);
//      sprintf(temp, " %s", "Glas");
//      TextOut(hdc, x, -(y), temp, strlen(temp));
  }

  // Ausgabe der Holz-Elemente
  y = iYOrg + getLineBegin(CA_Y_AFTER_GLASPROFILES);
  for (int i=0; i<HolzElemente->GetSize(); i++)
  {
     CHolzElement* pF = (CHolzElement*)HolzElemente->GetAt(i);
     y += rowH;
     if (pF->m_iBreite != HOLZELEM_BREITE)
     {
         // nur Ausgabe der Anfänger-Breiten
         sprintf(buf, "%d", pF->m_iBreite);
	     TextOut(hdc, x, -(y), buf, strlen(buf));
     }
  }
  y += rowH*RiegelElemente->GetSize();
  for (i=0; i<BetoPlanElemente->GetSize(); i++)
  {
     CBetoPlanElement* pF = (CBetoPlanElement*)BetoPlanElemente->GetAt(i);
     y += rowH;
     sprintf(buf, "%d", pF->m_iBreite);
	 TextOut(hdc, x, -(y), buf, strlen(buf));
  }

  return 0; //temp., später richtig!!!
}

void CTorDoor::profilRP1093(HDC hdc, int x, int y)
{
    x += 15;
    y += 18;
    MoveToEx(hdc, x, -y, NULL);
    y += 18;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 20;   //15
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y += 8;   //4 
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x += 63;  //33
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x += 8;   //4
    y += 8;   //4
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x += 36;   //36
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y -= 8;  //4
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 23;   //18
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y -= 26;  //22
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 64;  //40
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);    
}

void CTorDoor::profilRP1094(HDC hdc, int x, int y)
{
    x += 15; y += 4;
    MoveToEx(hdc, x, -y, NULL);
    y += 18;    //36
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 20;  //15
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y += 8;   //4
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x += 85;   //55
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y -= 26;    //40
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x += 23;  //18
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y -= 8;   //4
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 36;  //36
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 8; y += 8;  //4
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 44;  //18
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
}

//____-----
//  |    |
//  |____|
// 
void CTorDoor::profilRP1615(HDC hdc, int x, int y)
{
    x += 15;
    MoveToEx(hdc, x, -y, NULL);
    y += 36;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 15;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y += 4;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x += 33;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x += 4;
    y += 4;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x += 36;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y -= 4;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 18;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y -= 40;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 40;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);    
}

//   __---
// _|    |
//
// 
void CTorDoor::profilRP1614(HDC hdc, int x, int y)
{
    x += 15; y += 4;
    MoveToEx(hdc, x, -y, NULL);
    y += 36;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 15;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y += 4;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x += 55;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y -= 40;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x += 18;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y -= 4;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 36;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 4; y += 4;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 18;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
}

void CTorDoor::drawProfil(HDC hdc, int x, int y, int typ)
{
  if (typ == 6 || typ == 8)
  {
    x += 30;
    MoveToEx(hdc, x, -y, NULL);
    LineTo(hdc, x+60, -y);   //--
    MoveToEx(hdc, x, -y, NULL);
    LineTo(hdc, x, -(y+60));   // |
    MoveToEx(hdc, x+30, -y, NULL);
    LineTo(hdc, x+30, -(y+60));
    MoveToEx(hdc, x+60, -(y+60), NULL);
    LineTo(hdc, x-30, -(y+60));
  }
  else
  {
    x += 20;
    MoveToEx(hdc, x, -y, NULL);
    LineTo(hdc, x+46, -y);
    MoveToEx(hdc, x, -y, NULL);
    LineTo(hdc, x, -(y+40));   // |
    MoveToEx(hdc, x, -(y+40), NULL);
    LineTo(hdc, x+70, -(y+40));
    MoveToEx(hdc, x+70, -(y+40), NULL);
    LineTo(hdc, x+70, -(y));
    MoveToEx(hdc, x+46, -y, NULL);
    LineTo(hdc, x+50, -(y-4));
    MoveToEx(hdc, x+50, -(y-4), NULL);
    LineTo(hdc, x+88, -(y-4));
    MoveToEx(hdc, x+88, -(y-4), NULL);
    LineTo(hdc, x+88, -(y));
    MoveToEx(hdc, x+88, -y, NULL);
    LineTo(hdc, x+70, -y);
  }
}


int CTorDoor::printHoehe(HDC hdc, int x, int y, int posDir, int maxX,
    HFONT bFont)
{
  char temp[50];
  char buf[100];
  SIZE size;
  POINT points[2] = {0,0};

  HFONT orgFont = (HFONT)SelectObject(hdc, bFont);
  sprintf(temp, "%d", 10*Size.Hoehe);
  TextOut(hdc, x, -y, temp, strlen(temp));
    
  if (bBogen == TRUE)
  {
     sprintf(temp, "%d", 10*Size.SehnenHoehe);
     GetTextExtentPoint(hdc, temp, strlen(temp), &size);

//     points[1].x = size.cx; points[1].y = size.cy;
//     LPtoDP(hdc, points, 2);

     int iY = size.cy+10;
     TextOut(hdc, x, -(y+iY), temp, strlen(temp));
     y += rowH/2;
  }
  SelectObject(hdc, orgFont);

 y += rowH;
 if (TRUE == OKF)
  {
     TextOut(hdc, x+ 10, -y, "OKF", strlen("OKF"));
//     y += rowH/2;
     y += rowH/2;
  }
  else
  {
     // weiter von Profil weg
     y += rowH/2;
  }

  for (int i=0; i<Profile->GetSize(); i++)
  {
     char buf[100];
     CProfileElem* pF = (CProfileElem*)Profile->GetAt(i);
     if (pF->Direction == SENKRECHT)
     {
       if ((Typ == 4) && (Art != ATUER))
       {
            // Typ 400: 2 unterschiedliche Profile bei Senkrecht
            // Innenstreben jeweils eines anders
           y += rowH;
           sprintf(buf, "%d", pF->Laenge);
           TextOut(hdc, x, -(y), buf, strlen(buf));
           drawDirection(hdc, posDir, y,
		       pF->Direction);
           y += rowH;
           sprintf(buf, "%d", pF->Laenge);
           TextOut(hdc, x, -(y), buf, strlen(buf));
           drawDirection(hdc, posDir, y,
		       pF->Direction);
       }
       else
       {
           y += rowH;
           sprintf(buf, "%d", pF->Laenge);
           TextOut(hdc, x, -(y), buf, strlen(buf));
           drawDirection(hdc, posDir, y,
		       pF->Direction);
       }
     }
  }
  for (i=0; i<Profile->GetSize(); i++)
  {
     CProfileElem* pF = (CProfileElem*)Profile->GetAt(i);
     if (pF->Direction == WAAGRECHT)
     {
       y += rowH;
       sprintf(buf, "%d", pF->Laenge);
       TextOut(hdc, x, -(y), buf, strlen(buf));
       drawDirection(hdc, posDir, y,
		   pF->Direction);
     }
  }

  y += rowH;
  for (i=0; i<GlasProfile->GetSize(); i++)
  {
     char buf[100];
     CGlasProfileElem* pF = (CGlasProfileElem*)GlasProfile->GetAt(i);
     if (pF->Direction == SENKRECHT)
     {
       y += rowH;
       sprintf(buf, "%d", pF->Laenge);
       TextOut(hdc, x, -(y), buf, strlen(buf));
       drawDirection(hdc, posDir, y,
		   pF->Direction);
     }
  }
  for (i=0; i<GlasProfile->GetSize(); i++)
  {
     char buf[100];
     CGlasProfileElem* pF = (CGlasProfileElem*)GlasProfile->GetAt(i);
     if (pF->Direction == WAAGRECHT)
     {
       y += rowH;
       sprintf(buf, "%d", pF->Laenge);
       TextOut(hdc, x, -(y), buf, strlen(buf));
       drawDirection(hdc, posDir, y,
		   pF->Direction);
     }
  }

  // Abstand für nachfolgende Einträge
  if (i > 0)
      y += rowH;

  // Ausgabe der Fuellungen
  for (i=0; i<HolzElemente->GetSize(); i++)
  {
     CHolzElement* pF = (CHolzElement*)HolzElemente->GetAt(i);
     y += rowH;
     sprintf(buf, "%d", pF->m_iLaenge);
	 TextOut(hdc, x, -(y), buf, strlen(buf));
     drawDirection(hdc, posDir, y,
	   SENKRECHT);
  }
  for (i=0; i<RiegelElemente->GetSize(); i++)
  {
     CRiegelElement* pF = (CRiegelElement*)RiegelElemente->GetAt(i);
     y += rowH;
     sprintf(buf, "%d", pF->m_iLaenge);
	 TextOut(hdc, x, -(y), buf, strlen(buf));
     drawDirection(hdc, posDir, y, WAAGRECHT);
  }
  for (i=0; i<BetoPlanElemente->GetSize(); i++)
  {
     CBetoPlanElement* pF = (CBetoPlanElement*)BetoPlanElemente->GetAt(i);
     y += rowH;
     sprintf(buf, "%d", pF->m_iLaenge);
	 TextOut(hdc, x, -(y), buf, strlen(buf));
  }

  return 0; //temp., später richtig!!!
}

void CTorDoor::drawDirection(HDC hdc, int x, int y, tDirect dir)
{
  if (dir == SENKRECHT)
  {
    MoveToEx(hdc, x, -y, NULL);
    LineTo(hdc, x+30, -(y+15));
    MoveToEx(hdc, x+30/2, -(y+7), NULL);
    LineTo(hdc, x+30/2, -(y+37));
    MoveToEx(hdc, x, -(y+45), NULL);
    LineTo(hdc, x+30, -(y+30));
  }
  else
  {
    MoveToEx(hdc, x, -(y+10), NULL);
    LineTo(hdc, x+15, -(y+38));   // 
    MoveToEx(hdc, x+7, -(y+24), NULL);
    LineTo(hdc, x+40, -(y+24));   // --
    MoveToEx(hdc, x+50, -(y+10), NULL);
    LineTo(hdc, x+30, -(y+38));   //  /
  }
}

int CTorDoor::printDIN(HDC hdc, int x, int y, int maxX, HFONT bFont)
{
  char cDIN[2];
  if (DIN == L)
     sprintf(cDIN,"%c",'L');
  else
     sprintf(cDIN,"%c",'R');
  HFONT orgFont = (HFONT)SelectObject(hdc, bFont);
  TextOut(hdc, x, -y, cDIN, strlen(cDIN));
  SelectObject(hdc, orgFont);
  return y;
}

int CTorDoor::printObert(HDC hdc, int x, int y, int maxX, HFONT bFont)
{
  char Buf[30];
  int actY = y;

  int count = FlParam->GetSize();
  for (int i=0; i<count; i++)
  {
    CFlParam* pFl = (CFlParam*)FlParam->GetAt(i);
    if (pFl->OArt == OzO || pFl->OArt == OiF)
    {
       sprintf(Buf, "%d", Size.ObtHoehe*10);
//       HFONT orgFont = (HFONT)SelectObject(hdc, bFont);
       TextOut(hdc, x, -actY, Buf, strlen(Buf));
//       SelectObject(hdc, orgFont);

       actY += rowH;
       if (pFl->OArt == OzO)
         sprintf(Buf, "%s", "z. Öffnen");
       else if (pFl->OArt == OiF)
	 sprintf(Buf, "%s", "i. Flügel");
       TextOut(hdc, x, -actY, Buf, strlen(Buf));
       actY += rowH;
    }
  }
  return actY; //temp., später richtig!!!
}

int CTorDoor::printFenster(HDC hdc, int y,
	      int posStarr, int posKipp, int posSchieb,
	      int maxX, HFONT bFont)
{
  char Buf[30];
  int actX;
  int Ykipp = y;
  int Yschieb = y;
  int Ystarr = y;
  int actY;
  TDataScan dataScan;
  BOOL Fenster;


  int count = FlParam->GetSize();
  for (int i=0; i<count; i++)
  {
    Fenster = FALSE; 
    CFlParam* pFl = (CFlParam*)FlParam->GetAt(i);
    if (pFl->FArt == OlF)
    {
       actX = posStarr;
       Fenster = TRUE;
       actY = Ystarr;
       Ystarr += 2*rowH;
    }
    else if (pFl->FArt == SF || pFl->FArt == SF2)
    {
       actX = posSchieb;
       Fenster = TRUE;
       actY = Yschieb;
       Yschieb += 2*rowH;
    }
    else if (pFl->FArt == KF)
    {
       actX = posKipp;
       Fenster = TRUE;
       actY = Ykipp;
       Ykipp += 2*rowH;
    }
    if (Fenster == TRUE)
    {
      itoa(pFl->ObtHoehe*10, Buf, 10);

//      HFONT orgFont = (HFONT)SelectObject(hdc, bFont);
      TextOut(hdc, actX, -actY, Buf, strlen(Buf));
//      SelectObject(hdc, orgFont);

      actY +=  rowH;
      if (pFl->GArt != -1)
      {
        sprintf(Buf, "%s", dataScan.getGlasArt(pFl->GArt));
        TextOut(hdc, actX, -actY, Buf, strlen(Buf));
	actY +=  rowH;
      }
    }
  } 
  return actY;
}

int CTorDoor::printFuellung(HDC hdc, int x, int y, int maxX,
    HFONT bFont)
{
  char temp[150];
  TDataScan dataScan;

  if (Fuellung != -1)
  {
    sprintf(temp, "%s", dataScan.getBezeich(Fuellung));

    //  HFONT orgFont = (HFONT)SelectObject(hdc, bFont);
    TextOut(hdc, x, -y, temp, strlen(temp));
//  SelectObject(hdc, orgFont);
  }
  else
  {
    TextOut(hdc, x, -y, strFuellung, strlen(strFuellung));
  }

  y +=  rowH;
  if (bBogen == TRUE)
  {
      sprintf(temp, "%s, Radius: %0.0f mm", dataScan.getRahmenBez((tRAHMEN)RahmenArt)
          , Size.dRadius);
  }
  else
  {
      sprintf(temp, "%s", dataScan.getRahmenBez((tRAHMEN)RahmenArt));
  }
  TextOut(hdc, x, -y, temp, strlen(temp));

  return 0; //temp., später richtig!!!
}

//x, y: Koordinaten des Tores links oben
void CTorDoor::drawSquares(HDC hdc, int x, int y)
{
  int abstandX = rnd((Size.ZBreite-squareW)/2);
  Rectangle(hdc, x+abstandX, -(y-2*spaceSquare-squareH), x+abstandX+squareW,
	    -(y-2*spaceSquare));
  Rectangle(hdc, x+abstandX, -(y+Size.ZGesamtHoehe+2*spaceSquare),
      x+abstandX+squareW, -(y+Size.ZGesamtHoehe+2*spaceSquare+squareH));

  int abstandY = rnd((Size.ZGesamtHoehe-squareH)/2);
  Rectangle(hdc, x-spaceSquare-squareW, -(y+abstandY),
      x-spaceSquare, -(y+abstandY+squareH));
  Rectangle(hdc, x+Size.ZBreite+spaceSquare, -(y+abstandY),
      x+Size.ZBreite+spaceSquare+squareW, -(y+abstandY+squareH));

}

void CTorDoor::updateValues()
{
   //Profilemaße löschen
   if (Profile == NULL)
      Profile = new CPtrArray;
   else
      deleteProfiles();

   //Glas-Profilemaße löschen
   if (GlasProfile == NULL)
      GlasProfile = new CPtrArray;
   else
      deleteGlasProfiles();

   if (HolzElemente == NULL)
      HolzElemente = new CPtrArray;
   else
      deleteHolzElemente();
   if (RiegelElemente == NULL)
      RiegelElemente = new CPtrArray;
   else
      deleteRiegelElemente();
   if (BetoPlanElemente == NULL)
      BetoPlanElemente = new CPtrArray;
   else
      deleteBetoPlanElemente();

   int count = FlParam->GetSize();
   for (int i=0; i<count; i++)
   {
     CFlParam* FluegelP = (CFlParam*)FlParam->GetAt(i);
     if (bBogen == TRUE)
     {
        computeBogenProfile(FluegelP, count);
     }
     else
     {
        computeProfile(FluegelP, count);
     }
   }
}


// Function name	: CTorDoor::OutputAligned
// Description	    : gibt Text aligned wie durch Übergabe spezifiziert aus
// Return type		: void 
// Argument         : eAlignType eAlign
// Argument         : char* szBuf   
// Argument         : int iXPos     X-Position(links: links, rechts: rechts, mitte: mitte)
// Argument         : int iYPos
void CTorDoor::OutputAligned(HDC hdc, eAlignType eAlign, char* szBuf, int iXPos, int iYPos)
{
    UINT dwOrgAlign;
    if (eAlign == E_ALIGN_LEFT)
    {
        // Standard-Ausgabe verwenden
    }
    else
    {
        if (eAlign == E_ALIGN_RIGHT)
        {
            dwOrgAlign = SetTextAlign(hdc, TA_RIGHT);
        }
        else
        {
            dwOrgAlign = SetTextAlign(hdc, TA_CENTER);
        }
    }

    TextOut(hdc, iXPos, iYPos, szBuf, strlen(szBuf));
    SetTextAlign(hdc, dwOrgAlign);
}

#pragma warning( pop ) 
