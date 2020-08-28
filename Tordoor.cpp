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



IMPLEMENT_SERIAL( CTorDoor, CObject, VERSIONABLE_SCHEMA | TORDOOR_VERSION)
IMPLEMENT_SERIAL( CTTSize,  CObject, VERSIONABLE_SCHEMA | TORDOOR_VERSION)



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
   b400R      = ct->b400R;
   ProfilMass = ct->ProfilMass;
   PzGs       = ct->PzGs;
   memcpy(&TorEinbau, &ct->TorEinbau, sizeof(TorEinbau));
   memcpy(&TorFeststellung, &ct->TorFeststellung, sizeof(TorFeststellung));
   FlParam    = ct->FlParam;
   Profile    = ct->Profile;
   RahmenElemente = ct->RahmenElemente;
   Treibriegel = ct->Treibriegel;
   GlasProfile = ct->GlasProfile;
   HolzElemente = ct->HolzElemente;
   RiegelElemente = ct->RiegelElemente;
   BetoPlanElemente = ct->BetoPlanElemente;
   strcpy(sArtikel, ct->sArtikel);
   strcpy(Kunde, ct->Kunde);
   strcpy(Kommission, ct->Kommission);
#if 0   //?? immer Fuellungs-Text merken
   if (ct->Fuellung == -1)
     strcpy(strFuellung, ct->strFuellung);
#else
   strcpy(strFuellung, ct->strFuellung);
#endif  // 0
   scF = (float)0.7;
   aSp = 2;
   iSp = 1;
   ZWidth = 4;   // default 4 mm
   m_iVersion = ct->m_iVersion;
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
   RahmenElemente = NULL;
   Treibriegel = NULL;
   StueckZahl = 0;
   Size.Breite = 0;
   Size.Hoehe = 0;
   OKF = FALSE;
   bBogen = FALSE;
   b400R  = FALSE;
   ProfilMass = PD_70_40;
   Art       = 0;
   Typ       = 0;
   RahmenArt = 0;
   DIN       = 0;
   Fuellung =  0;
   FluegelAnz = 0;
   strcpy(sArtikel, " ");
   strcpy(Kunde, " ");
   strcpy(Kommission, " ");
   m_iVersion = TORDOOR_VERSION;  // immer aktuelle Version
   PzGs = PZ_GS_NONE;
   for (int i = 0; i < TS_MAX; i++)
   {
       TorEinbau[i] = TEB_LEER;
   }
   for (int i = 0; i < 2; i++)
   {
       TorFeststellung[i] = TFS_LEER;
   }   
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
	if (Treibriegel != NULL) delete Treibriegel;
	if (BetoPlanElemente != NULL) delete BetoPlanElemente;
    if (RahmenElemente != NULL) delete RahmenElemente;
    if (Treibriegel != NULL) delete Treibriegel;
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

void CTorDoor::deleteRahmenElemente()
{
	if (RahmenElemente != NULL)
	{
		int count = RahmenElemente->GetSize();
		for (int i=0; i<count; i++)
		{
			CRahmenElem* pFl = (CRahmenElem*)RahmenElemente->GetAt(i);
			delete pFl;
		}
		RahmenElemente->RemoveAll();
	}
}

void CTorDoor::deleteTreibriegel()
{
	if (Treibriegel != NULL)
	{
		int count = Treibriegel->GetSize();
		for (int i=0; i<count; i++)
		{
			CTreibriegelElem* pFl = (CTreibriegelElem*)Treibriegel->GetAt(i);
			delete pFl;
		}
		Treibriegel->RemoveAll();
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
  if ((Art == STUER) || (Art == STOR))
  {
     // bei Schiebe-Türen /-Toren Zugabe über Zuschnittregeln-Datei
     iAddQuerT = iAddSenkT = iAddOberT = iAddUnterT = 0;
     return;
  }
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


// Function name	: CTorDoor::getProfileTypes
// Description	    : 
// Return type		: void 
// Argument         : CFlParam* pFl
// Argument         : int iCount
// Argument         : int iActFl
// Argument         : tProfil& prflLeft    Profiltyp für linkes Profil, von aussen gesehen
// Argument         : tProfil& prflRight
// Argument         : tProfil& prflTop
// Argument         : tProfil& prflBottom
void CTorDoor::getProfileTypes(CFlParam* pFl, int iCount, int iActFl, tProfil& prflLeft, 
                               tProfil& prflRight, tProfil& prflTop, tProfil& prflBottom)
{
    tFUELLUNG fAussen, fInnen;
    TDataScan ds;
    if (Typ == 4)
    {
        CFlParam* pFlNext;
        static tProfil lastProfileRight = NP;

        // nächsten Flügel holen
        if (iActFl < (iCount-1))
            pFlNext = (CFlParam*)FlParam->GetAt(iActFl+1);
        else
            pFlNext = NULL;


        if (iActFl == 0)
            lastProfileRight = NP;   // zurücksetzen

        // Typ 400
        ds.getFuellung(Fuellung, &fAussen, &fInnen);

        if (pFl->FArt == KEIN && !(fAussen == F_BLECH || fInnen == F_BLECH)
            && !(fAussen == F_HOLZ_I || fInnen == F_HOLZ_I))
        {
            // kein Fenster und keine Blech-Füllung und Holzfüllung nicht innenliegend
            if (b400R == FALSE)
                // RP1348
                prflLeft = prflRight = prflTop = prflBottom = RP1348;
            else
            {
                // 400R
                // oben und unten RPRR
                prflTop = prflBottom = RPRR;
                // 400R
                prflLeft = prflRight = RPRR;   // ohne Fenster default Rohrrahmen
                if (getOeffnerFluegel() == iActFl)
                {
                    if (DIN == L)
                    {
                        if (iActFl == (iCount-1))
                            prflRight = RPRR;         // letzter Flügel
                        else
                            prflRight = RP1348;
                    }
                    else
                    {
                        if (iActFl == 0)
                            prflLeft = RPRR;          // erster Flügel
                        else
                            prflLeft = RP1348;
                    }
                }
                else
                if (lastProfileRight == RPRR || lastProfileRight == RP1087)
                    // linkes Profil muss RP1348 sein, damit kein Spalt im Tor
                    prflLeft = RP1348;
                // R-Rahmen?
                if (RahmenArt == RR || RahmenArt == RW)
                {
                    // bei Rohrrahmen nach aussen RP1348, damit kein Spalt
                    prflTop = prflBottom = RP1348;
                    if (iActFl == 0)
                    {
                        // linker Flügel
                        prflLeft = RP1348;
                    }
                    if (iActFl == (iCount-1))
                    {
                        prflRight = RP1348;
                    }
                }
            }

        }
        else
        {
            // Fenster oder Blech-Füllung
            prflTop = RP1093;  
            if (fAussen == F_BLECH || fAussen == F_HOLZ_I || fInnen == F_HOLZ_I)
                prflBottom = RP1093;   // bei Blech Füllung oder Füllung innenliegend auch unten
            else
            if (fInnen == F_BLECH)
            {
                // Innen Blech, beide RP1094
                prflBottom = RP1094;   // ohne BLechfüllung unten normal
                prflTop = RP1094;  
            }
            else
                prflBottom = RP1348;   // bei Fenster unten RP1384

            // Flügel mit Fenster oder Blech oder Holz innenliegend, zuerst Standard bestimmen
            if (iActFl == 0)
            {
                // erster Flügel
                if (iCount == 2)
                {
                    // Tor, erster Flügel und Fenster, RP1093 links und RP1094 rechts
                    if (DIN == R)
                    {
                        prflLeft = RP1093;
                        prflRight = RP1094;
                    }
                    else
                    {
                        prflLeft = RP1093;
                        prflRight = RP1093;
                    }
                }
                else
                {
                    // Tuer oder FT3++, erster Flügel links und rechts RP1093
                    prflLeft = prflRight = RP1093;
                }
            }
            else
            if (iActFl == 1)
            {
                // zweiter Flügel
                if (iCount == 2)
                {
                    // Tor
                    if (DIN == R)
                    {
                        prflLeft = RP1093;
                        prflRight = RP1093;
                    }
                    else
                    {
                        prflLeft = RP1094;
                        prflRight = RP1093;
                    }
                }
                else
                if (iActFl == (iCount-1))
                    // letzter Flügel, immer RP1093 links und rechts
                    prflLeft = prflRight = RP1093;
                else
                if ((iCount == 4) && (DIN == L))
                 {
                    // 4-FT und DIN L
                    prflLeft = RP1094;
                    prflRight = RP1093;
                 }
                 else
                    // FTor, links und rechts immer RP1094
                    prflLeft = prflRight = RP1094;
            }
            else
            if (iActFl == 2)
            {
                // muss 4 oder 5-Flügler sein
                if (iCount == 4)
                {
                    // 4-Flügler
                    if (DIN == L)
                    {
                        prflLeft = RP1094;
                        prflRight = RP1094;
                    }
                    else
                    {
                        prflLeft = RP1093;
                        prflRight = RP1094;
                    }
                }
                else
                {
                    // 5-Flügler, 3.Flügel ist Tür, beide RP1093
                    prflLeft = prflRight =RP1093;
                }
            }
            else
            if (iActFl == 3)
            {
                if (iCount == 4)
                    // 4-FT, links und rechts RP1093
                    prflLeft = prflRight = RP1093;
                else
                    // 5-Flügler, vorletzte Flügel, beide RP1094
                    prflLeft = prflRight = RP1094;
            }
            else
            if (iActFl == 4)
            {
                // 5-FT, beide RP1093
                prflLeft = prflRight = RP1093;
            }
            if (fInnen == F_BLECH)
            {
                // Blechfüllung innen, Anzahl RP194 und 1093 vertauschen
                if (prflLeft == RP1094)
                    prflLeft = RP1093;
                else
                if (prflLeft == RP1093)
                    prflLeft = RP1094;
                if (prflRight == RP1094)
                    prflRight = RP1093;
                else
                if (prflRight == RP1093)
                    prflRight = RP1094;
            }

            if (b400R == TRUE)
            {
                // Fenster, oben RP1087, unten RPRR
                prflTop = RP1087;
                prflBottom = RPRR;
                // Bestimmung linkes Profil
                prflLeft = prflRight = RP1087;    // default auf RP1087
                if (getOeffnerFluegel() == iActFl)
                {
                    if (DIN == L)
                    {
                        if (iActFl == (iCount-1))
                            prflRight = RP1087;    // letzter Flügel
                        else
                            prflRight = RP1093;
                    }
                    else
                    {
                        if (iActFl == 0)
                            prflLeft = RP1087;
                        else
                            prflLeft = RP1093;
                    }
                }
                else
                if (lastProfileRight == RP1087)
                {
                    // entweder RP1093 oder RP1094
                    if (pFlNext != NULL)
                        if (pFlNext->FArt != KEIN)
                            prflLeft = RP1093;
                }
                else
                if (lastProfileRight == RP1348)
                {
                    prflLeft = RP1087;
                }
                else
                    prflLeft = RP1087;
                // R-Rahmen?
                if (RahmenArt == RR || RahmenArt == RW)
                {
                    // bei Rohrrahmen nach aussen RP1348, damit kein Spalt
                    prflTop = RP1093;
                    prflBottom = RP1348;  // bei 400R keine Blechfüllung!!!
                    if (iActFl == 0)
                    {
                        // linker Flügel
                        prflLeft = RP1093;
                    }
                    if (iActFl == (iCount-1))
                    {
                        prflRight = RP1093;
                    }
                }
            }
        }
        lastProfileRight = prflRight;
    }
    else
    {
        // Typ 600 + 800
        prflLeft = prflRight = prflTop = RP6_8;  // erstmal default
        prflBottom = RP1579;
    }
}

void CTorDoor::computeProfile(CFlParam* pFl, int count, int iActFl)
{
  int addQuerT, addSenkT, addOberT, addUnterT;
  int addSenkTL, addSenkTR, addOberTL, addOberTR, addUnterTL, addUnterTR;
  int iSaveQuerProf, iSaveSenkProf, iSaveSenkOberProf;
  CSchiebeFensterZugaben Sf, Sf2;
  tProfil prflLeft, prflRight, prflTop, prflBottom;

  iSaveSenkOberProf = 0; // sonst nicht immer initialisiert!!!

  getAddValues(pFl, count, addQuerT, addSenkT, addOberT, addUnterT);
  getSFAddValues(&Sf, &Sf2);
  getProfileTypes(pFl, count, iActFl, prflLeft, prflRight, prflTop, prflBottom);

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

  addSenkTL = addSenkT;
  addSenkTR = addSenkT;
  addOberTL = addOberT;
  addOberTR = addOberT;
  addUnterTL = addUnterT;
  addUnterTR = addUnterT;
  if (Typ == 4 && b400R == TRUE)
  {
      // Typ 400R, RP1087
      // 35mm abziehen, da kein Lappen
      // bei Tor 1 Lappen, damit man nicht durchschauen kann
      if (prflLeft == RP1087 || prflLeft == RPRR)
      {
        addSenkTL -= 35; 
        addOberTL -= 35;
        addUnterTL -= 35;
      }
      if (prflRight == RP1087 || prflRight == RPRR)
      {
        addSenkTR -= 35; 
        addOberTR -= 35;
        addUnterTR -= 35;
      }
      if (prflTop == RP1087 || prflTop == RPRR || prflBottom == RP1087 || prflBottom == RPRR)
        addQuerT -= 35;
  }

  int querProf = iSaveQuerProf = pFl->iBreite + addQuerT;

  if (pFl->Geteilt == TRUE || pFl->OArt == OzO)
  {
    int OberProfL = iSaveSenkOberProf = 10*Size.ObtHoehe + addOberTL;
    int UnterProfL = iSaveSenkProf = 10*(Size.Hoehe-Size.ObtHoehe)+addUnterTL;
    int OberProfR = iSaveSenkOberProf = 10*Size.ObtHoehe + addOberTR;
    int UnterProfR = iSaveSenkProf = 10*(Size.Hoehe-Size.ObtHoehe)+addUnterTR;
    iSaveSenkProf = __min(UnterProfR, UnterProfL);  // sollte nur bei Typ 400R Bedeutung haben
    iSaveSenkOberProf = __min(OberProfR, OberProfL);  // sollte nur bei Typ 400R Bedeutung haben
    if (b400R == TRUE && prflRight != RP1087 && prflRight != RPRR &&
        prflLeft != RP1087 && prflLeft != RPRR)
    {
        // linkes und rechtes Profil hat Lappen, bei Typ400R wird davon ausgegangen,
        // daß iSaveSenkProf ohne Lappen ist, also Lappen abziehen
        iSaveSenkProf -= 35;
        iSaveSenkOberProf -= 35;
    }

    insertProfil(StueckZahl, OberProfL, SENKRECHT, HOLZUNTEN, prflLeft);     // links
    insertProfil(StueckZahl, OberProfR, SENKRECHT, HOLZUNTEN, prflRight);    // rechts
    insertProfil(StueckZahl, UnterProfL, SENKRECHT, HOLZUNTEN, prflLeft);    // links
    insertProfil(StueckZahl, UnterProfR, SENKRECHT, HOLZUNTEN, prflRight);   // rechts
    if (Typ != 4)
        insertProfil(2*StueckZahl, querProf, WAAGRECHT, HOLZAUF, prflBottom);
    else
    {
        insertProfil(1*StueckZahl, querProf, WAAGRECHT, HOLZAUF, prflBottom);   // unten
        insertProfil(1*StueckZahl, querProf, WAAGRECHT, HOLZAUF, prflTop);      // oben
        if (prflTop == RP1093 && prflBottom == RP1093)
        {
            // Gegenstück von RP1093 ist RP1094
            insertProfil(1*StueckZahl, querProf, WAAGRECHT, HOLZAUF, RP1093);
            insertProfil(1*StueckZahl, querProf, WAAGRECHT, HOLZAUF, RP1094);
        }
        else
        {
            insertProfil(1*StueckZahl, querProf, WAAGRECHT, HOLZAUF, prflBottom);
            insertProfil(1*StueckZahl, querProf, WAAGRECHT, HOLZAUF, prflTop);
        }
    }

//        insertProfil(2*StueckZahl, querProf, WAAGRECHT, HOLZUNTEN);
//braucht man nicht, da nur HOLZAUF ausgegeben wird
  }
  else
  {  //2 Prof senkr., 2Prof waagrecht
    int senkProfL = 10*Size.Hoehe+addSenkTL;
    int senkProfR = 10*Size.Hoehe+addSenkTR;
    iSaveSenkProf = __min(senkProfR, senkProfL);  // sollte nur bei Typ 400R Bedeutung haben
    if (b400R == TRUE && prflRight != RP1087 && prflRight != RPRR &&
        prflLeft != RP1087 && prflLeft != RPRR)
    {
        // linkes und rechtes Profil hat Lappen, bei Typ400R wird davon ausgegangen,
        // daß iSaveSenkProf ohne Lappen ist, also Lappen abziehen
        iSaveSenkProf -= 35;
    }

    insertProfil(StueckZahl, senkProfL, SENKRECHT, HOLZUNTEN, prflLeft);     // links
    insertProfil(StueckZahl, senkProfR, SENKRECHT, HOLZUNTEN, prflRight);    // rechts
    insertProfil(1*StueckZahl, querProf, WAAGRECHT, HOLZAUF, prflTop);
    insertProfil(1*StueckZahl, querProf, WAAGRECHT, HOLZAUF, prflBottom);
//braucht man nicht, da nur HOLZAUF ausgegeben wird
  }
  if (pFl->OArt == OiF)
  {
//    insertProfil(1*StueckZahl, querProf, WAAGRECHT, HOLZAUF);
    int tempQuerProf = querProf;
    int senkProfL = 10*Size.ObtHoehe + addOberTL;   //????
    int senkProfR = 10*Size.ObtHoehe + addOberTR;   //????
    tempQuerProf -= 115;

    insertProfil(StueckZahl, senkProfL, SENKRECHT, HOLZUNTEN, prflLeft);         // links
    insertProfil(StueckZahl, senkProfR, SENKRECHT, HOLZUNTEN, prflRight);        // rechts
    insertProfil(1*StueckZahl, tempQuerProf, WAAGRECHT, HOLZAUF, prflBottom);
  }

  // Glas-Profile ermitteln
  if (pFl->FArt == SF2)
  {
    if (Typ == 4)
    {
        int tempQuerProf = querProf;
        int senkProf = 10*Size.ObtHoehe + Sf2.m_iSenkrecht_400; // Add ist negativ!!!
        tempQuerProf = (tempQuerProf + Sf2.m_iQuer_400)/2;
        if (b400R == TRUE)
            tempQuerProf += 31;
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
        if (b400R == TRUE)
            tempQuerProf += 31;
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

  computeFuellung(pFl, count, iSaveSenkProf, iSaveSenkOberProf,
                              iSaveQuerProf);

}

void CTorDoor::computeFuellung(CFlParam *pFl, int count, int iSaveSenkProf, int iSaveSenkOberProf,
                               int iSaveQuerProf)
{
  if (b400R == TRUE)
  {
      // keine Überlappung von 35 mm, deshalb bei Profilen auch nicht abziehen!!!
      // Abzug kompensieren
      iSaveQuerProf += 35;
      iSaveSenkProf += 35;
      iSaveSenkOberProf += 35;
      if (ProfilMass == PD_60_40)
      {
          // bei 60mm Rohr links,rechts,oben,unten 1 cm mehr für Füllung
          iSaveQuerProf += 20;
          iSaveSenkProf += 20;
          iSaveSenkOberProf += 20;
      }
  }
  if (Fuellung != -1)
  {
      // Fuellung wurde aus Liste ausgewählt
      tFUELLUNG fAussen, fInnen;
      TDataScan ds;
      if (ds.getFuellung(Fuellung, &fAussen, &fInnen) == FALSE)
          return;

      int iFuellungAussen = ds.getFuellungIndex(Fuellung, true);
      int iFuellungInnen = ds.getFuellungIndex(Fuellung, false);

      if ((pFl->FArt != KEIN) && (pFl->OArt == LEER))
      {
          // Fenster vorhanden und kein Oberteil
          iSaveSenkProf -= pFl->ObtHoehe*10;
          iSaveSenkOberProf -= pFl->ObtHoehe*10;  //??
      }

      if (fAussen == F_HOLZ || fInnen == F_HOLZ || fAussen == F_HOLZ_I)
      {
          // Holzfuellung
          int iSeitenAnz, iFensterKor;
          if (fAussen == F_HOLZ && fInnen == F_HOLZ)
              iSeitenAnz = 2;
          else
              iSeitenAnz = 1;

          // Korrekturfaktor für Fenster
          if ((pFl->FArt != KEIN) && (pFl->OArt == LEER))
          {
              // Fenster und kein Oberteil
              if ((Typ == 6) || (Typ == 8))
              {
                  // Typ 600 + 800
                  if ((pFl->Sprosse == RP))
                  {
                      // Typ 600 und RP-Sprosse, 35 mm abziehen
                      iFensterKor = 35;
                  }
                  else
                  {
                      // Fenster und kein Oberteil, 5mm abziehen
                      iFensterKor = 5;
                  }
              }
              else
              {
                  // Typ 400 [R]
                  iFensterKor = 40;   // bei Fenster nochmal 40 mm abziehen wegen Sprosse
              }
          }
          else
              iFensterKor = 0;

          // Sortieren bei unterschiedlich breiten Flügeln;
          static int s_iLastQuerProf = 0;
          static int s_iSort = 0;
          if (iSaveQuerProf != s_iLastQuerProf)
              s_iSort++;
          s_iLastQuerProf = iSaveQuerProf; 

          if (Typ == 4)
          {
              // Typ 400
              int iAnzNorm, iAnzNormInnen;      // Anzahl der normal breiten Bretter
              int iLaenge, iLaengeInnen;       // Laenge der Bretter
              float fBreiteRest, fBreiteRestInnen;   // Breite der Rest-Bretter
              int iBreiteRest, iBreiteRestInnen;     // Breite der Rest-Bretter gerundet
              int iRiegelLaenge, iRiegelAnz;
              int iSenkSub, iQuerSub, iSenkSubInnen, iQuerSubInnen;  // Werte für Subtraktion


              // normal 150mm weg, setzt sich so zusammen:
              // 2*87.5 (Kasteninnenmaß der Profile 1348, 1094, 1093) + 10 (Schweissen) - 35 (Überlappung der Bretter )
              // = 150 mm
              iSenkSub = iQuerSub = iSenkSubInnen = iQuerSubInnen = 150;
              if (fAussen == F_HOLZ_I)
              {
                  iSenkSub += 35 + 10;  // keine 35mm Überlappung bei innenliegend + 10mm Luft
                  iQuerSub += 35;
                  iSenkSubInnen = iSenkSub;
                  iQuerSubInnen = iQuerSub;
              }
              else
              if (pFl->FArt == SF || pFl->FArt == SF2)
              {
                  // Schiebefenster, innenliegende Füllung ohne Überlappung
                  // Laufrahmen für Schiebefenster ist an der Innenseite vom Profil befestigt
                  iSenkSubInnen += 35 + 10;  // keine 35mm Überlappung bei innenliegend + 10mm Luft
                  iQuerSubInnen += 35;                  
              }
              if (b400R == TRUE)
              {
                 //?? aussenm noch 20 mm in der Hoehe dazu, keine Ahnung warum
//??                 iSenkSub -= 20;
                  if (((Art == ATUER) || (Art == STUER)) && ProfilMass == PD_60_40)
                  {
                      // bei 60/40 in der Breite 25mm zugeben. Schild liegt auf Holz?
                      if (fAussen != F_HOLZ_I)
                      {
                          iQuerSub -= 25;
                      }
                      if (pFl->FArt != SF && pFl->FArt != SF2)
                      {
                          iQuerSubInnen -= 25;
                      }
                  }
                  if ((pFl->FArt != KEIN) && (pFl->OArt == LEER))
                  {
                      // Fenster oder SF, Bretter um 20 mm laenger (Bretter außenaufliegend)
                        iSenkSub -= 20;
                  }
              }
              // Riegel für jeden angefangenen Meter
              iRiegelLaenge = iSaveQuerProf - 185;      // -150-35 (35mm: keine Überlappung)
              iRiegelLaenge -= 5;  // noch 5mm abziehen lauf Franz

              // Anzahl + Breite
              int iCompBreite = (iSaveQuerProf - iQuerSub);  // Breite für Holzfüllung
//              if (fAussen != F_HOLZ_I)
                 iCompBreite += 10;    // noch 1cm dazugeben laut Franz
              iAnzNorm = iCompBreite / HOLZELEM_BREITE;
              iAnzNorm -= 1;
              fBreiteRest = (float)(iCompBreite - (iAnzNorm * HOLZELEM_BREITE)) / (float)2;
              iBreiteRest = (int)fBreiteRest;
              // Innen
              iCompBreite = (iSaveQuerProf - iQuerSubInnen);  // Breite für Holzfüllung
//              if (pFl->FArt != SF && pFl->FArt != SF2)
                 iCompBreite += 10;    // noch 1cm dazugeben laut Franz, wenn nicht innenliegend
              iAnzNormInnen = iCompBreite / HOLZELEM_BREITE;
              iAnzNormInnen -= 1;
              fBreiteRestInnen = (float)(iCompBreite - (iAnzNormInnen * HOLZELEM_BREITE)) / (float)2;
              iBreiteRestInnen = (int)fBreiteRestInnen;

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
                  if (fAussen == F_HOLZ || fAussen == F_HOLZ_I)
                  {
                      iLaenge = iSaveSenkProf - iSenkSub;  //??
                      insertHolzElement(iAnzNorm*StueckZahl*1/*iSeitenAnz*/, HOLZELEM_BREITE, iLaenge, s_iSort);
                      insertHolzElement(2*StueckZahl*1/*iSeitenAnz*/, iBreiteRest, iLaenge, s_iSort);
                  }
                  if (fInnen == F_HOLZ || fInnen == F_HOLZ_I)
                  {
                      // Innen
                      iLaengeInnen = iSaveSenkProf - iSenkSubInnen;  //??
                      insertHolzElement(iAnzNormInnen*StueckZahl*1/*iSeitenAnz*/, HOLZELEM_BREITE, iLaengeInnen, s_iSort);
                      insertHolzElement(2*StueckZahl*1/*iSeitenAnz*/, iBreiteRestInnen, iLaengeInnen, s_iSort);
                  }
                  // Oberteil
                  if (fAussen == F_HOLZ || fAussen == F_HOLZ_I)
                  {
                      iLaenge = iSaveSenkOberProf - iSenkSub;  //??
                      insertHolzElement(iAnzNorm*StueckZahl*1/*iSeitenAnz*/, HOLZELEM_BREITE, iLaenge, s_iSort);
                      insertHolzElement(2*StueckZahl*1/*iSeitenAnz*/, iBreiteRest, iLaenge, s_iSort);
                  }
                  if (fInnen == F_HOLZ || fInnen == F_HOLZ_I)
                  {
                      // Innen
                      iLaengeInnen = iSaveSenkOberProf - iSenkSubInnen;  //??
                      insertHolzElement(iAnzNormInnen*StueckZahl*1/*iSeitenAnz*/, HOLZELEM_BREITE, iLaengeInnen, s_iSort);
                      insertHolzElement(2*StueckZahl*1/*iSeitenAnz*/, iBreiteRestInnen, iLaengeInnen, s_iSort);
                  }
              }
              else
              { 
                  if (fAussen == F_HOLZ || fAussen == F_HOLZ_I)
                  {
                      iLaenge = iSaveSenkProf - iSenkSub - iFensterKor;
                      insertHolzElement(iAnzNorm*StueckZahl*1/*iSeitenAnz*/, HOLZELEM_BREITE, iLaenge, s_iSort);
                      insertHolzElement(2*StueckZahl*1/*iSeitenAnz*/, iBreiteRest, iLaenge, s_iSort);
                  }
                  if (fInnen == F_HOLZ || fInnen == F_HOLZ_I)
                  {
                      // Innen
                      iLaengeInnen = iSaveSenkProf - iSenkSubInnen - iFensterKor;
                      insertHolzElement(iAnzNormInnen*StueckZahl*1/*iSeitenAnz*/, HOLZELEM_BREITE, iLaengeInnen, s_iSort);
                      insertHolzElement(2*StueckZahl*1/*iSeitenAnz*/, iBreiteRestInnen, iLaengeInnen, s_iSort);
                  }
              }
          
              // Riegel bestimmen
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
                  insertHolzElement(iAnzNorm*StueckZahl*iSeitenAnz, HOLZELEM_BREITE, iLaenge, s_iSort);
                  insertHolzElement(2*StueckZahl*iSeitenAnz, iBreiteRest, iLaenge, s_iSort);
                  // Oberteil
                  iLaenge = iSaveSenkOberProf - 115;
                  insertHolzElement(iAnzNorm*StueckZahl*iSeitenAnz, HOLZELEM_BREITE, iLaenge, s_iSort);
                  insertHolzElement(2*StueckZahl*iSeitenAnz, iBreiteRest, iLaenge, s_iSort);
              }
              else
              { 
                  iLaenge = iSaveSenkProf - 115 - iFensterKor;

                  insertHolzElement(iAnzNorm*StueckZahl*iSeitenAnz, HOLZELEM_BREITE, iLaenge, s_iSort);
                  insertHolzElement(2*StueckZahl*iSeitenAnz, iBreiteRest, iLaenge, s_iSort);
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
      // Betoplan-Füllung
      if (fAussen == F_BETOPLAN || fInnen == F_BETOPLAN)
      {
          // Fuellung mit Beto-Plan
          int iSeitenAnz;
          int iFensterKor;
          if (fAussen == F_BETOPLAN && fInnen == F_BETOPLAN)
              iSeitenAnz = 2;
          else
              iSeitenAnz = 1;

          if (Typ == 4)
          {
              // Typ 400
              int iLaenge;       // Laenge der Bretter
              int iLaengeInnen;
              int iSubSenk = 150;
              int iSubQuer = 150;
              int iSubSenkInnen = 150;
              int iSubQuerInnen = 150;
              if (b400R == TRUE)
              {
                  // Typ 400R und innen Beto
                  if (TorHasSF() == FALSE)
                  {
                      // kein Fluegel hat Schiebefenster, jeweils um 35mm zu viel
                      // Änderung: innenliegende Füllung liegt jetzt auf
#if 0
                      iSubSenkInnen += 35;
                      iSubQuerInnen += 35;                      
#endif // 0
//??                      iSubSenk -= 20;  //?? um 20mm zu wenig, keine Ahnung warum
                  }
                  else
                  {
                      // mit SF stimmt Breite, aber Hoehe um 35mm zu kurz
                      //iSubSenkInnen;
                      // hat ein anderer Flügel ein SF
                      iSubQuerInnen += 35;                      
                  }
                  if (((Art == ATUER) || (Art == STUER)) && ProfilMass == PD_60_40)
                  {
                      // bei 60/40 in der Breite 25mm zugeben. Schild liegt auf Holz?
                      if (fAussen != F_HOLZ_I)
                      {
                          iSubQuer -= 25;
                      }
                      if (pFl->FArt != SF && pFl->FArt != SF2)
                      {
                          iSubQuerInnen -= 25;
                      }
                  }
              }
              else
              {
                  // 
                  if (pFl->FArt == SF || pFl->FArt == SF2)
                  {
                      // bei Schiebefenster 35 mm Abzug bei Innenfüllung
                      if (fInnen == F_BETOPLAN)
                      {
                          iSubSenkInnen += 35;
                          iSubQuerInnen += 35;
                      }
                  }
              }
              if ((pFl->FArt != KEIN) && (pFl->OArt == LEER))
              {
                  // Fenster und kein Oberteil
                  iFensterKor = 40;   // bei Fenster nochmal 40 mm abziehen wegen Sprosse
              }
              else
                  iFensterKor = 0;

              // Laenge
              if ((pFl->Geteilt == TRUE || pFl->OArt == OzO) && (pFl->FArt == KEIN))
              {
                  // mit Oberteil und kein Fenster
                  // Unterteil
                  iLaenge = iSaveSenkProf - iSubSenk;
                  iLaengeInnen = iSaveSenkProf - iSubSenkInnen;
                  if (fAussen == F_BETOPLAN)
                      insertBetoPlanElement(1*StueckZahl*1, iSaveQuerProf - iSubQuer, iLaenge, iFuellungAussen);
                  if (fInnen == F_BETOPLAN)
                      insertBetoPlanElement(1*StueckZahl*1, iSaveQuerProf - iSubQuerInnen, iLaenge, iFuellungInnen);
                  // Oberteil
                  iLaenge = iSaveSenkOberProf - iSubSenk;
                  iLaengeInnen = iSaveSenkOberProf - iSubSenkInnen;
                  if (fAussen == F_BETOPLAN)
                      insertBetoPlanElement(1*StueckZahl*1, iSaveQuerProf - iSubQuer, iLaenge, iFuellungAussen);
                  if (fInnen == F_BETOPLAN)
                      insertBetoPlanElement(1*StueckZahl*1, iSaveQuerProf - iSubQuerInnen, iLaenge, iFuellungInnen);
              }
              else
              { 
                  iLaenge = iSaveSenkProf - iSubSenk - iFensterKor;
                  iLaengeInnen = iSaveSenkProf - iSubSenkInnen - iFensterKor;

                  if (fAussen == F_BETOPLAN)
                      insertBetoPlanElement(1*StueckZahl*1/*iSeitenAnz*/, iSaveQuerProf - iSubQuer, iLaenge, iFuellungAussen);
                  if (fInnen == F_BETOPLAN)
                      insertBetoPlanElement(1*StueckZahl*1/*iSeitenAnz*/, iSaveQuerProf - iSubQuerInnen, iLaengeInnen, iFuellungInnen);
              }          
          }
          else
          {
              // Typ 600 + 800
              int iLaenge;       // Laenge der Bretter
              iFensterKor = 0;
              if ((pFl->FArt != KEIN) && (pFl->OArt == LEER))
              {
                  // Fenster und kein Oberteil
                  if ((pFl->Sprosse == RP))
                  {
                      // Typ 600 und RP-Sprosse, 30 mm abziehen
                      iFensterKor = 30;
                  }
              }


              // Laenge
              if ((pFl->Geteilt == TRUE || pFl->OArt == OzO) && (pFl->FArt == KEIN))
              {
                  // mit Oberteil und kein Fenster
                  // Unterreil
                  iLaenge = iSaveSenkProf - 120;
                  if (fAussen == F_BETOPLAN)
                    insertBetoPlanElement(1*StueckZahl*1/*iSeitenAnz*/, iSaveQuerProf - 128, iLaenge, iFuellungAussen);
                  if (fInnen == F_BETOPLAN)
                    insertBetoPlanElement(1*StueckZahl*1/*iSeitenAnz*/, iSaveQuerProf - 128, iLaenge, iFuellungInnen);
                  // Oberteil
                  iLaenge = iSaveSenkOberProf - 120;
                  if (fAussen == F_BETOPLAN)
                    insertBetoPlanElement(1*StueckZahl*1/*iSeitenAnz*/, iSaveQuerProf - 128, iLaenge, iFuellungAussen);
                  if (fInnen == F_BETOPLAN)
                    insertBetoPlanElement(1*StueckZahl*1/*iSeitenAnz*/, iSaveQuerProf - 128, iLaenge, iFuellungInnen);
              }
              else
              { 
                  iLaenge = iSaveSenkProf - 120 - iFensterKor;

                  if (fAussen == F_BETOPLAN)
                    insertBetoPlanElement(1*StueckZahl*1/*iSeitenAnz*/, iSaveQuerProf - 128, iLaenge, iFuellungAussen);
                  if (fInnen == F_BETOPLAN)
                    insertBetoPlanElement(1*StueckZahl*1 /*iSeitenAnz*/, iSaveQuerProf - 128, iLaenge, iFuellungInnen);
              }          
          }
      }
  }
}

BOOL CTorDoor::TorHasSF(void)
{
    int i;
    for (i=0; i<FlParam->GetSize(); i++)
    {
        CFlParam* pF = (CFlParam*)FlParam->GetAt(i);
        if (pF->FArt == SF || pF->FArt == SF2)
            return TRUE;
    }
    return FALSE;
}

// durchsucht die Profile und berechnet daraufhin die Füllungen neu
void CTorDoor::updateFuellung(void)
{
    int iSaveSenkProf = 0x7fffffff, iSaveSenkOberProf = 0x7fffffff, 
        iSaveQuerProf  = 0;
    BOOL bOberteil = FALSE;

    // alles löschen
    deleteHolzElemente();
    deleteRiegelElemente();
    deleteBetoPlanElemente();

    // hat ein Flügel ein Oberteil????
    for (int i=0; i<FlParam->GetSize(); i++)
    {
        CFlParam* pFl = (CFlParam*) FlParam->GetAt(i);
        if (pFl && pFl->OArt != KEIN)
        {
            bOberteil = TRUE;
            break;
        }
    }

    // Profile durchsuchen nach kleinstem Wert, bei waagrechten nach längstem Profil
    for (int i=0; i<Profile->GetSize(); i++)
    {
        CProfileElem* pF = (CProfileElem*)Profile->GetAt(i);
        if (pF)
        {
            if (pF->Direction == SENKRECHT)
            {
                // senkrechtes Profile
                if (bOberteil)
                {
                    if (pF->Laenge > Size.Hoehe/2)
                    {
                        // Annahme: wenn Profil länger als die Hälfte, dann Unterteil,
                        // sonst Oberteil
                        if (pF->Laenge < iSaveSenkProf)
                            iSaveSenkProf = pF->Laenge;
                    }
                    else
                    {
                        // Oberteil
                        if (pF->Laenge < iSaveSenkOberProf)
                            iSaveSenkOberProf = pF->Laenge;
                    }
                }
                else
                {
                    // kein Oberteil
                    if (pF->Laenge < iSaveSenkProf)
                        iSaveSenkProf = pF->Laenge;
                }
            }
            else
            {
                // waagrechtes Profile
                if (pF->Laenge > iSaveQuerProf)
                    iSaveQuerProf = pF->Laenge;
            }
        }
    }

    // für jeden Flügel berechnen
    for (int i=0; i<FlParam->GetSize(); i++)
    {
        CFlParam* pFl = (CFlParam*) FlParam->GetAt(i);
        if (pFl)
            computeFuellung(pFl, FlParam->GetSize(), iSaveSenkProf, iSaveSenkOberProf,
                            iSaveQuerProf);
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
    int untenQuerProf = pFl->iBreite;

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
        insertProfil(1*StueckZahl, obenQuerProf, WAAGRECHT, HOLZAUF, RP6_8, rnd(dWinkelBogen),
            rnd(dWinkelBogen));

        // Querteil unten: -2*15 (== Luft links und rechts) + 2*5 (=Maschinenschw.)
        untenQuerProf = untenQuerProf - 2*15 + 2*5;
        insertProfil(1*StueckZahl, untenQuerProf, WAAGRECHT, HOLZAUF, RP6_8);


        if (pFl->Geteilt == TRUE || pFl->OArt == OzO)
        {
            // Querteil mitte: -2*15 (== Luft links und rechts) + 2*5 (=Maschinenschw.)
            insertProfil(1*StueckZahl, untenQuerProf, WAAGRECHT, HOLZAUF, RP6_8);

            // Querteil mitte: -2*15 (== Luft links und rechts) + 2*5 (=Maschinenschw.)
            insertProfil(1*StueckZahl, untenQuerProf, WAAGRECHT, HOLZUNTEN, RP6_8);

            // Senkteil oben Rand: default - 50 +5(=Maschinenschw. unten)
            int OberProf = 10*Size.ObtHoehe + addOberT -50 +5;
            insertProfil(2*StueckZahl, OberProf, SENKRECHT, HOLZUNTEN, RP6_8,
                0, rnd(dWinkelRahmen));

            // Senkteil unten : default - 50 +10(=Maschinenschw. oben+unten)
            int UnterProf = 10*(Size.Hoehe-Size.ObtHoehe)+addUnterT-50+10;
            insertProfil(2*StueckZahl, UnterProf, SENKRECHT, HOLZUNTEN, RP6_8);
            //insertProfil(2*StueckZahl, querProf, WAAGRECHT, HOLZAUF, RP6_8);
            //        insertProfil(2*StueckZahl, querProf, WAAGRECHT, HOLZUNTEN, RP6_8);
            //braucht man nicht, da nur HOLZAUF ausgegeben wird
        }
        else
        {
            // Senkteile
            // : -2*15 (=unten und oben Luft) + 5 (=unten Maschinenschw.)
            int senkProf = 10*Size.Hoehe - 2*15 +5;
            insertProfil(2*StueckZahl, senkProf, SENKRECHT, HOLZUNTEN, RP6_8, 0, rnd(dWinkelRahmen));
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
        insertProfil(1*StueckZahl, untenQuerProf, WAAGRECHT, HOLZAUF, RP6_8);

        // oben: -15 (== Luft Rand)-3 (=Luft mitte) + 0 (=Handschw.)
        int obenQuerProf = dObenQuerProf - 15 - 3;
        insertProfil(1*StueckZahl, obenQuerProf, WAAGRECHT, HOLZAUF, RP6_8, rnd(dWinkelBogen),
            rnd(dWinkelMitte));

        if (pFl->Geteilt == TRUE || pFl->OArt == OzO)
        {
            // Querteil mitte: -2*15 (== Luft links und rechts) + 2*5 (=Maschinenschw.)
            insertProfil(1*StueckZahl, untenQuerProf, WAAGRECHT, HOLZAUF, RP6_8);

            // Querteil mitte: -2*15 (== Luft links und rechts) + 2*5 (=Maschinenschw.)
            insertProfil(1*StueckZahl, untenQuerProf, WAAGRECHT, HOLZUNTEN, RP6_8);

            // Senkteil oben Rand: default - 50 +5(=Maschinenschw. unten)
            int OberProf = 10*Size.ObtHoehe + addOberT -50 +5;
            insertProfil(1*StueckZahl, OberProf, SENKRECHT, HOLZUNTEN, RP6_8,
                0, rnd(dWinkelRahmen));

            // Senkteil unten : default - 50 +10(=Maschinenschw. oben+unten)
            int UnterProf = 10*(Size.Hoehe-Size.ObtHoehe)+addUnterT-50+10;
            insertProfil(1*StueckZahl, UnterProf, SENKRECHT, HOLZUNTEN, RP6_8);

            // Senkteil oben mitte: default - 50 +5(=Maschinenschw. unten)
            OberProf = 10*(Size.SehnenHoehe+Size.ObtHoehe) + addOberT -50 +5;
            insertProfil(1*StueckZahl, OberProf, SENKRECHT, HOLZUNTEN, RP6_8,
                0, 90-rnd(dWinkelMitte));

            // Senkteil unten mitte: default - 50 +10(=Maschinenschw. oben+unten)
            UnterProf = 10*(Size.Hoehe-Size.ObtHoehe)+addUnterT-50+10;
            insertProfil(1*StueckZahl, UnterProf, SENKRECHT, HOLZUNTEN, RP6_8);
            //        insertProfil(2*StueckZahl, querProf, WAAGRECHT, HOLZUNTEN, RP6_8);
            //braucht man nicht, da nur HOLZAUF ausgegeben wird
        }
        else
        {
            // Senkteile
            // am Rand: -2*15 (=unten und oben Luft) + 5 (=unten Maschinenschw.)
            int senkProf = 10*Size.Hoehe - 2*15 +5;
            insertProfil(StueckZahl, senkProf, SENKRECHT, HOLZUNTEN, RP6_8, 0, rnd(dWinkelRahmen));

            // in der Mitte: -2*15 (=unten und oben Luft) + 5 (=unten Maschinenschw.)
            senkProf = 10*(Size.Hoehe+Size.SehnenHoehe) - 2*15 +5;
            insertProfil(StueckZahl, senkProf, SENKRECHT, HOLZUNTEN, RP6_8, 0, 90-rnd(dWinkelMitte));
        }
    }

    Size.dRadius = dRadius;  // in cm umrechnen!!!!
}


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
                              tTypWaagrecht uTyp, tProfil Profil, int iWinkel1, 
                              int iWinkel2)
{
  BOOL found = FALSE;
  int count = Profile->GetSize();
  for (int i=0; i<count&&!found; i++)
  {
    CProfileElem *tempElem = (CProfileElem*) Profile->GetAt(i);
    if (tempElem->Laenge == Laenge &&
	    tempElem->Direction == dir && tempElem->uTyp == uTyp &&
        tempElem->iWinkel1 == iWinkel1 && tempElem->iWinkel2 == iWinkel2
        && tempElem->Profil == Profil)
    {
      found = TRUE;
      tempElem->Anzahl += Anzahl;
    }
  }
  if (!found)
  {
     CProfileElem* pP = new CProfileElem(Anzahl, Laenge, dir,
     				         uTyp, Profil, iWinkel1, iWinkel2);
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

BOOL CTorDoor::insertHolzElement(int iAnzahl, int iBreite, int iLaenge, int iSort)
{
  BOOL found = FALSE;
  int count = HolzElemente->GetSize();
  for (int i=0; i<count&&!found; i++)
  {
    CHolzElement *tempElem = (CHolzElement*) HolzElemente->GetAt(i);
    if (tempElem->m_iLaenge == iLaenge &&
	tempElem->m_iBreite == iBreite && tempElem->m_iSort == iSort)
    {
      found = TRUE;
      tempElem->m_iAnzahl += iAnzahl;
    }
  }
  if (!found)
  {
     CHolzElement* pP = new CHolzElement(iAnzahl, iBreite, iLaenge, iSort);
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

BOOL CTorDoor::insertBetoPlanElement(int iAnzahl, int iBreite, int iLaenge, int iArt)
{
  BOOL found = FALSE;
  int count = BetoPlanElemente->GetSize();
  for (int i=0; i<count&&!found; i++)
  {
    CBetoPlanElement *tempElem = (CBetoPlanElement*) BetoPlanElemente->GetAt(i);
    if (tempElem->m_iLaenge == iLaenge &&
        tempElem->m_iBreite == iBreite && tempElem->m_iSort == iArt)
    {
      found = TRUE;
      tempElem->m_iAnzahl += iAnzahl;
    }
  }
  if (!found)
  {
     CBetoPlanElement* pP = new CBetoPlanElement(iAnzahl, iBreite, iLaenge, iArt);
     BetoPlanElemente->Add(pP);
  }
  return found;
}

BOOL CTorDoor::insertRahmenElement(int Anz, int Len, tDirect Dir, tRAHMEN rahmen)
{
  BOOL found = FALSE;
  int count = RahmenElemente->GetSize();
  for (int i=0; i<count&&!found; i++)
  {
    CRahmenElem *tempElem = (CRahmenElem*) RahmenElemente->GetAt(i);
    if (tempElem->Laenge == Len &&
	    tempElem->Direction == Dir &&
        tempElem->Rahmen == rahmen)
    {
      found = TRUE;
      tempElem->Anzahl += Anz;
    }
  }
  if (!found)
  {
     CRahmenElem* pP = new CRahmenElem(Anz, Len, Dir, rahmen);
     RahmenElemente->Add(pP);
  }
  return found;
}

BOOL CTorDoor::insertTreibriegel(int Anz, int Len, int Pt)
{
  BOOL found = FALSE;
  int count = Treibriegel->GetSize();
  for (int i=0; i<count&&!found; i++)
  {
    CTreibriegelElem *tempElem = (CTreibriegelElem*) Treibriegel->GetAt(i);
    if (tempElem->Laenge == Len &&
		tempElem->ProfilType == Pt)
	{
      found = TRUE;
      tempElem->Anzahl += Anz;
    }
  }
  if (!found)
  {
     CTreibriegelElem* pP = new CTreibriegelElem(Anz, Len, Pt);
     Treibriegel->Add(pP);
  }
  return found;
}

void CTorDoor::drawTT(HDC hdc, int x, int y, int b, int h, HFONT fntSmall)
{
}

void CTorDoor::draw(HDC hdc)
{
  drawTT(hdc, dx, dy, db, dh, 0);
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

void CTorDoor::GetPositionKommission(char* komm, char* pos)
{
    char sTemp[MAX_PATH];

    strcpy(sTemp, Kommission);

    char* p = strstr(sTemp, POSITION_SIGN);
    if (p != NULL)
    {
        *p = 0;
        strcpy(komm, sTemp);
        p += strlen(POSITION_SIGN);
        strcpy(pos, p);
    }
    else
    {
        strcpy(komm, Kommission);
        pos[0] = '1';
        pos[1] = 0;
    }
}


int CTorDoor::printKundKomm(HDC hdc, int x, int y, int maxX, HFONT bFont)
{
#define POS_STRING  "Pos. "
  char szBuf[100], kommission[100], pos[100];
  HFONT orgFont = (HFONT)SelectObject(hdc, bFont);
  TextOut(hdc, x, -y, Kunde, strlen(Kunde));
  SelectObject(hdc, orgFont);

  GetPositionKommission(kommission, pos);
  // Kommission
  TextOut(hdc, x, -(y + rowH), kommission, strlen(kommission));
  // Position
  TextOut(hdc, x, -(y + 2*rowH), POS_STRING, strlen(POS_STRING));
  TextOut(hdc, x, -(y + 2*rowH), POS_STRING, strlen(POS_STRING));
  SIZE size;
  GetTextExtentPoint(hdc, POS_STRING, strlen(POS_STRING), &size);
  TextOut(hdc, x + size.cx, -(y + 2*rowH), pos, strlen(pos));

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
  if (RahmenElemente != NULL && RahmenElemente->GetSize() > 0)
  {
      strcpy(szBuf, "Rahmen");
      OutputAligned(hdc, E_ALIGN_RIGHT, szBuf, maxX, -y);
	  y += RahmenElemente->GetSize()*rowH;
  }
  if (Treibriegel != NULL && Treibriegel->GetSize() > 0)
  {
      strcpy(szBuf, "Treibriegel");
      OutputAligned(hdc, E_ALIGN_RIGHT, szBuf, maxX, -y);
	  y += Treibriegel->GetSize()*rowH;
  }
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

  //if (BetoPlanElemente->GetSize() > 0)
  //{
  //    strcpy(szBuf, "Beto/PP");
  //    OutputAligned(hdc, E_ALIGN_RIGHT, szBuf, maxX, -y);
  //}
  TDataScan dataScan;
  for (int i=0; i<BetoPlanElemente->GetSize(); i++)
  {
     CBetoPlanElement* pF = (CBetoPlanElement*)BetoPlanElemente->GetAt(i);
     const char* sFuellung = dataScan.getNameForFuellung(pF->m_iSort);
     if (strlen(sFuellung) == 0)
     {
        strcpy(szBuf, "Beto/PP");
     }
     else
     {
         CString s(sFuellung);
         int idx = s.ReverseFind(_T(' '));
         if (idx != -1)
            s = s.Left(idx);
         strcpy(szBuf, (char*)s.GetString());
     }
     OutputAligned(hdc, E_ALIGN_RIGHT, szBuf, maxX, -y);
     y += rowH;     
  }

  return y+2* rowH;
}

int CTorDoor::printStueck(HDC hdc, int x, int y, int maxX, HFONT bFont)
{
  char temp[50];
  char buf[200];
  TDataScan dataScan;
  int i;

  HFONT orgFont = (HFONT)SelectObject(hdc, bFont);
  sprintf(temp, "%d x %s", StueckZahl, dataScan.getArtBez((tART)Art));
  TextOut(hdc, x, -y, temp, strlen(temp));
  SelectObject(hdc, orgFont);

  if (bBogen == TRUE)
    y += rowH/2;

  y += rowH/2;
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

  y += rowH/2;

  for (i=0; i<Profile->GetSize(); i++)
  {
     CProfileElem* pF = (CProfileElem*)Profile->GetAt(i);
     if (pF->Direction == SENKRECHT)
     {
        if (/*(Typ == 4) && (Art != ATUER)*/FALSE)
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
   // alle Profile ausgeben
  for (i=0; i<Profile->GetSize(); i++)
  {
     CProfileElem* pF = (CProfileElem*)Profile->GetAt(i);
     if (pF->Direction == WAAGRECHT)
     {
        y += rowH;
        sprintf(buf, "%d St.", pF->Anzahl);
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

  if (RahmenElemente != NULL)
  {
      for (i=0; i<RahmenElemente->GetSize(); i++)
      {
         CRahmenElem* pF = (CRahmenElem*)RahmenElemente->GetAt(i);
         y += rowH;
         sprintf(buf, "%d St.", pF->Anzahl);
	     TextOut(hdc, x, -(y), buf, strlen(buf));
      }
  }

  if (Treibriegel != NULL)
  {
      for (i=0; i<Treibriegel->GetSize(); i++)
      {
         CTreibriegelElem* pF = (CTreibriegelElem*)Treibriegel->GetAt(i);
         y += rowH;
         sprintf(buf, "%d St.", pF->Anzahl);
	     TextOut(hdc, x, -(y), buf, strlen(buf));
      }
  }

//  y += 2*rowH;
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
  int i;

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

  for (i=0; i<Profile->GetSize(); i++)
  {
     CProfileElem* pF = (CProfileElem*)Profile->GetAt(i);
     if (pF->Direction == SENKRECHT)
     {
         iLineCount += rowH;
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
  if (RahmenElemente != NULL)
  {
      iLineCount += RahmenElemente->GetSize()*rowH;
  }
  if (Treibriegel != NULL)
  {
      iLineCount += Treibriegel->GetSize()*rowH;
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
  int i;

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

  // 
  if (b400R == TRUE)
  {
      if (ProfilMass == PD_70_40)
          TextOut(hdc, x+18, -(y+23), "70/40", strlen("70/40"));
      else
      if (ProfilMass == PD_60_40)
          TextOut(hdc, x+18, -(y+23), "60/40", strlen("60/40"));
  }

  if (Typ == 4/* && Art != ATUER*/)
  {
      for (int i=0; i<Profile->GetSize(); i++)
      {
         CProfileElem* pF = (CProfileElem*)Profile->GetAt(i);
         if (pF->Direction == SENKRECHT)
         {
             y += rowH;
             if (pF->Profil == RP1093)
             {
                  profilRP1093(hdc, x+15, y+8);
             }
             else
             if (pF->Profil == RP1094)
                profilRP1094(hdc, x+15, y+8);
             else
             if (pF->Profil == RP1087)
                profilRP1087(hdc, x+15, y+8);
             else
             if (pF->Profil == RPRR)
                profilRPRR(hdc, x+15, y+8);
             else
             if (pF->Profil == RP1348)
                profilRP1348(hdc, x+15, y+8);
         }
      }  
  }
  else 
  {
      // für Typ 600/800 senkrechte Profile nicht zeichnen
      for (int i=0; i<Profile->GetSize(); i++)
      {
         CProfileElem* pF = (CProfileElem*)Profile->GetAt(i);
         if (pF->Direction == SENKRECHT)
         {
             y += rowH;
         }
      }
  }
  if (/*Typ == 4*/TRUE)
  {
      for (int i=0; i<Profile->GetSize(); i++)
      {
         CProfileElem* pF = (CProfileElem*)Profile->GetAt(i);
         if (pF->Direction == WAAGRECHT)
         {
             y += rowH;
             if (pF->Profil == RP1093)
             {
                  profilRP1093(hdc, x+15, y+8);
             }
             else
             if (pF->Profil == RP1094)
                profilRP1094(hdc, x+15, y+8);
             else
             if (pF->Profil == RP1087)
                profilRP1087(hdc, x+15, y+8);
             else
             if (pF->Profil == RPRR)
                profilRPRR(hdc, x+15, y+8);
             else
             if (pF->Profil == RP1348)
                profilRP1348(hdc, x+15, y+8);
             else
             if (pF->Profil == RP6_8)
                profilRP6_8(hdc, x+15, y+8);
             else
             if (pF->Profil == RP1579)
                profilRP1579(hdc, x+15, y+8);
             if (pF->Profil == RP6_M)
                profilRPM(hdc, x+15, y+8);
        }
      }  
  }
  // Bogenberechnung nur für Typ 600!!!
  if (bBogen == TRUE)
  {
      orgFont = (HFONT)SelectObject(hdc, bThinFont);
      for (i=0; i<Profile->GetSize(); i++)
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

  // Zeichnen der Rahmen
  y = iYOrg + getLineBegin(CA_Y_AFTER_GLASPROFILES);
//  y += 2*rowH;
  if (RahmenElemente != NULL)
  {
      for (i=0; i<RahmenElemente->GetSize(); i++)
      {
         CRahmenElem* pF = (CRahmenElem*)RahmenElemente->GetAt(i);
         if (pF != NULL)
         {
             y += rowH;
             drawRahmen(hdc, x + 10, y, pF->Rahmen);
         }
      }
  }

  if (Treibriegel != NULL)
  {
      for (i=0; i<Treibriegel->GetSize(); i++)
      {
         CTreibriegelElem* pF = (CTreibriegelElem*)Treibriegel->GetAt(i);
         if (pF != NULL)
         {
             y += rowH;
//             drawRahmen(hdc, x + 10, y, pF->Rahmen);
             // TODO: drawProfil
             if (pF->ProfilType == 1)
             {
                 profil30_10(hdc, x+15, y+8);
             }
             else
             {
                 profilQuad(hdc, x+15, y+8);
             }
         }
      }
  }

  // Ausgabe der Holz-Elemente
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
//    x += 15;
//    y += 10;
    MoveToEx(hdc, x, -y, NULL);
    y += 4;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x += 15;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y += 36;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x += 70;
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
    x -= 38;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 4;
    y += 4;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 61;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);    
}

void CTorDoor::profilRP1094(HDC hdc, int x, int y)
{
    x += 15; 
//    y += 4;
    MoveToEx(hdc, x, -y, NULL);
    y += 36;    //36
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 15;  //15
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y += 4;   //4
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x += 85;   //55
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y -= 40;    //40
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x += 18;  //18
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y -= 4;   //4
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 38;  //36
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 4; y += 4;  //4
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 46;  //18
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
}

void CTorDoor::profilRP1087(HDC hdc, int x, int y)
{
//    x += 15;
//    y += 10;
    MoveToEx(hdc, x, -y, NULL);
    y += 40;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x += 70;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y -= 36;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x += 15;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y -= 4;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 85;
    LineTo(hdc, x, -y);
}

void CTorDoor::profilRPRR(HDC hdc, int x, int y)
{
    MoveToEx(hdc, x, -y, NULL);
    y += 40;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x += 70;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y -= 40;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 70;
    LineTo(hdc, x, -y);
}

void CTorDoor::profil30_10(HDC hdc, int x, int y)
{
    MoveToEx(hdc, x, -y, NULL);
    y += 40;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x += 70;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y -= 40;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 70;
    LineTo(hdc, x, -y);

    // Schraegstrich zeichnen
    x -= 3;
    y += 43;
    MoveToEx(hdc, x, -y, NULL);
    x += 79;
    y -= 49;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
}

//
//  quadratisches Profil
//
//
void CTorDoor::profilQuad(HDC hdc, int x, int y)
{
    MoveToEx(hdc, x, -y, NULL);
    y += 40;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x += 40;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    y -= 40;
    LineTo(hdc, x, -y);
    MoveToEx(hdc, x, -y, NULL);
    x -= 40;
    LineTo(hdc, x, -y);
}

void CTorDoor::profilRP1348(HDC hdc, int x, int y)
{
//    x += 20;
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

//
//    |----|---
//    |    |
// ___|____|___
// 
void CTorDoor::profilRP6_8(HDC hdc, int x, int y)
{
    x += 30;
    MoveToEx(hdc, x, -y, NULL);
    LineTo(hdc, x+50, -y);   //--
    MoveToEx(hdc, x, -y, NULL);
    LineTo(hdc, x, -(y+45));   // |
    MoveToEx(hdc, x+25, -y, NULL);
    LineTo(hdc, x+25, -(y+45));
    MoveToEx(hdc, x+50, -(y+45), NULL);
    LineTo(hdc, x-25, -(y+45));
}

//
//
//
//
//
void CTorDoor::profilRP1579(HDC hdc, int x, int y)
{
    x += 20;
    MoveToEx(hdc, x, -y, NULL);
    x += 30;
    LineTo(hdc, x, -y);   //--
    y += 20;
    LineTo(hdc, x, -y);
    x += 15;
    LineTo(hdc, x, -y);
    y += 20;
    LineTo(hdc, x, -y);
    x -= 15;
    LineTo(hdc, x, -y);
    y += 20;
    LineTo(hdc, x, -y);
    x -= 50;
    LineTo(hdc, x, -y);
    y -= 4;
    LineTo(hdc, x, -y);
    x += 20;
    LineTo(hdc, x, -y);
    y -= 56;
    LineTo(hdc, x, -y);
}

void CTorDoor::profilRPM(HDC hdc, int x, int y)
{
    y += 15;
    MoveToEx(hdc, x, -y, NULL);
    x += 60;
    LineTo(hdc, x, -y);   
    y += 30;
    LineTo(hdc, x, -y);   
    x -= 60;
    LineTo(hdc, x, -y);   
    y -= 30;
    LineTo(hdc, x, -y);   
    x += 20;
    MoveToEx(hdc, x, -y, NULL);
    y -= 15;
    LineTo(hdc, x, -y);   
    x += 20;
    LineTo(hdc, x, -y);   
    y += 15;
    LineTo(hdc, x, -y);   
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
      if (b400R == FALSE)
      {
          // Typ 400, RP 1348
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
      else
      {
          // 
          x += 20;
          MoveToEx(hdc, x, -y, NULL);
          LineTo(hdc, x+70, -y);
//        MoveToEx(hdc, x, -y, NULL);
          LineTo(hdc, x+70, -(y+40));
          LineTo(hdc, x, -(y+40));
          LineTo(hdc, x, -y);
//        MoveToEx(hdc, x+46, -y, NULL);
//        LineTo(hdc, x+50, -(y-4));
//        MoveToEx(hdc, x+50, -(y-4), NULL);
//        LineTo(hdc, x+88, -(y-4));
//        MoveToEx(hdc, x+88, -(y-4), NULL);
//        LineTo(hdc, x+88, -(y));
//        MoveToEx(hdc, x+88, -y, NULL);
//        LineTo(hdc, x+70, -y);
      }
  }
}

int CTorDoor::getOeffnerFluegel(void)
{
    switch(Art)
    {
    case ATUER:
    case STUER:
        return 0;  // Flügel 0
    case ATOR:
    case STOR:
        if (DIN == L)
            return 0;
        else
            return 1;
    case FT3:
        if (DIN == L)
            return 0;
        else
            return 2;
    case FT4:
        if (DIN == L)
            return 0;
        else
            return 3;
    case FT5:
        return 3;   // bei 5-flgl in der Mitte
    default:
        break;
    }

    return 0;
}

int CTorDoor::printHoehe(HDC hdc, int x, int y, int posDir, int maxX,
    HFONT bFont)
{
  char temp[50];
  char buf[100];
  SIZE size;
  POINT points[2] = {0,0};
  int i;

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

 if ((4 == Typ) && (TRUE == b400R))
  {
//     TextOut(hdc, x+ 10, -y, "400R", strlen());
//     OutputAligned(hdc, E_ALIGN_CENTER, "400R", x+10, -y);
     if (FALSE == OKF)
        TextOut(hdc, x+ 10, -(y-20), "400R", strlen("400R"));
     else
        TextOut(hdc, x+ 10, -(y+rowH/3), "400R", strlen("400R"));
  }

  for (i=0; i<Profile->GetSize(); i++)
  {
     char buf[100];
     CProfileElem* pF = (CProfileElem*)Profile->GetAt(i);
     if (pF->Direction == SENKRECHT)
     {
       if (/*(Typ == 4) && (Art != ATUER)*/FALSE)
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

  // Ausgabe der Rahmenelemente
  if (RahmenElemente != NULL)
  {
      for (i=0; i<RahmenElemente->GetSize(); i++)
      {
         CRahmenElem* pF = (CRahmenElem*)RahmenElemente->GetAt(i);
         y += rowH;
         sprintf(buf, "%d", pF->Laenge);
	     TextOut(hdc, x, -(y), buf, strlen(buf));
         drawDirection(hdc, posDir, y,
	       pF->Direction);
      }
  }

  // Ausgabe der Treibriegel
  if (Treibriegel != NULL)
  {
      for (i=0; i<Treibriegel->GetSize(); i++)
      {
         CTreibriegelElem* pF = (CTreibriegelElem*)Treibriegel->GetAt(i);
         if (pF != NULL)
         {
             y += rowH;
             sprintf(buf, "%d", pF->Laenge);
	         TextOut(hdc, x, -(y), buf, strlen(buf));
             drawDirection(hdc, posDir, y,
	           SENKRECHT);
         }
      }
  }

  // Ausgabe der Fuellungen
//  y += 2*rowH;
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

void CTorDoor::drawRahmen(HDC hdc, int x, int y, tRAHMEN rahmen)
{
  if (rahmen == RZ)
  {
    x += 5;
    MoveToEx(hdc, x, -y, NULL);
    x += 40;
    LineTo(hdc, x, -y);
    y += 35;
    LineTo(hdc, x, -y);
    x += 35;
    LineTo(hdc, x, -y);
    y += 5;
    LineTo(hdc, x, -y);
    x -= 40;
    LineTo(hdc, x, -y);
    y -= 35;
    LineTo(hdc, x, -y);
    x -= 35;
    LineTo(hdc, x, -y);
    y -= 5;
    LineTo(hdc, x, -y);
  }
  else
  if (rahmen == RW)
  {
    x += 5;
    MoveToEx(hdc, x, -y, NULL);
    x += 5;
    LineTo(hdc, x, -y);
    y += 40;
    LineTo(hdc, x, -y);
    x += 40;
    LineTo(hdc, x, -y);
    y += 5;
    LineTo(hdc, x, -y);
    x -= 45;
    LineTo(hdc, x, -y);
    y -= 45;
    LineTo(hdc, x, -y);
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
int CTorDoor::printPZ_GS(HDC hdc, int x, int y, int maxX, HFONT font)
{
    char strPzGs[10];
    int actY = y + rowH;


    if (PzGs == PZ)
    {
        strcpy(strPzGs, "PZ");
    }
    else
    if (PzGs == PZ_GS)
    {
        strcpy(strPzGs, "PZ/GS");
    }
    else
    {
        strPzGs[0] = 0;
    }
    TextOut(hdc, x, -(actY), strPzGs, strlen(strPzGs));
    return actY; //temp., später richtig!!!
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
       TextOut(hdc, x, -actY, Buf, strlen(Buf));

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

      TextOut(hdc, actX, -actY, Buf, strlen(Buf));

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
  char temp[500];
  TDataScan dataScan;

  int iPrinted = 0;
  int iLen = strlen(strFuellung);
  int iToPrint = iLen;
  while (iPrinted < iLen)
  {
     SIZE sSize;
     strncpy(temp, &strFuellung[iPrinted], iToPrint);
     temp[iToPrint] = 0;
     if (GetTextExtentPoint32(hdc, temp, strlen(temp), &sSize))
     {
         if (sSize.cx < maxX - x)
         {
             TextOut(hdc, x, -y, temp, strlen(temp));
             y += rowH;
             iPrinted += strlen(temp);
             iToPrint = iLen - iPrinted;
         }
         else
         {
            iToPrint--;             
         }
     }
  }

  y +=  rowH/2;
  if (bBogen == TRUE)
  {
      sprintf(temp, "%s, Radius: %0.0f mm", dataScan.getRahmenBez((tRAHMEN)RahmenArt)
          , Size.dRadius);
  }
  else
  {
      sprintf(temp, "%s", dataScan.getRahmenBez((tRAHMEN)RahmenArt));
  }
  if (RahmenArt == RZ && ZWidth > 2) {
      // Z Staerke ausgeben
      char cBuf[100];
      sprintf(cBuf, ", %d mm", ZWidth);
      strcat(temp, cBuf);
  }
  TextOut(hdc, x, -y, temp, strlen(temp));

  return 0; //temp., später richtig!!!
}

//x, y: Koordinaten des Tores links oben
void CTorDoor::drawSquares(HDC hdc, int x, int y)
{
  const char* pTeb;
  TDataScan dataScan;
  int abstandX = rnd((Size.ZBreite-squareW)/2);

  // oben
  Rectangle(hdc, x+2*abstandX, -(y-2*spaceSquare-squareH), x+2*abstandX+squareW,
	    -(y-2*spaceSquare));
  pTeb = dataScan.getTorEinbau(TorEinbau[TS_OBEN]);
  TextOut(hdc, x+abstandX+9, -(y-2*spaceSquare-squareH+6), pTeb, strlen(pTeb));

  // unten
  Rectangle(hdc, x+abstandX, -(y+Size.ZGesamtHoehe+2*spaceSquare),
      x+abstandX+squareW, -(y+Size.ZGesamtHoehe+2*spaceSquare+squareH));
  pTeb = dataScan.getTorEinbau(TorEinbau[TS_UNTEN]);
  TextOut(hdc, x+abstandX+9, -(y+Size.ZGesamtHoehe+2*spaceSquare+6), pTeb, strlen(pTeb));

  // links
  int abstandY = rnd((Size.ZGesamtHoehe-squareH)/2);
  Rectangle(hdc, x-spaceSquare-squareW, -(y+abstandY),
      x-spaceSquare, -(y+abstandY+squareH));
  pTeb = dataScan.getTorEinbau(TorEinbau[TS_LINKS]);
  TextOut(hdc, x-spaceSquare-squareW+9, -(y+abstandY+6), pTeb, strlen(pTeb));

  // Rechts
  Rectangle(hdc, x+Size.ZBreite+spaceSquare, -(y+abstandY),
      x+Size.ZBreite+spaceSquare+squareW, -(y+abstandY+squareH));
  pTeb = dataScan.getTorEinbau(TorEinbau[TS_RECHTS]);
  TextOut(hdc, x+Size.ZBreite+spaceSquare+9, -(y+abstandY+6), pTeb, strlen(pTeb));
}

void CTorDoor::drawFestellung(HDC hdc, int x, int y)
{
  const char* pFst;
  TDataScan dataScan;
  int abstandX = rnd((Size.ZBreite-squareW)/2);

  // unten
  if ((Art == TUER) || (Art == TOR))
  {
     int y_pos = -(y+Size.ZGesamtHoehe+2*spaceSquare+squareH + 10);
     pFst = dataScan.getTorFeststellung(TorFeststellung[0]);
     int len = strlen(pFst);
     if ((DIN == R) || Art == TOR)
     {
        // Beschriftung links
         if (len > 0)
         {
            TextOut(hdc, x-spaceSquare-squareW-5, y_pos, pFst, strlen(pFst));
            MoveToEx(hdc, x-spaceSquare, y_pos - 4, NULL);
            if (Art == TOR)
            {
                LineTo(hdc, x + Size.ZBreite/2 - 10, -(y+Size.ZGesamtHoehe - 30) ); 
            }
            else
            {
                LineTo(hdc, x + 30, -(y+Size.ZGesamtHoehe - 30) ); 
            }
         }
     }
     else
     {
        // Beschriftung rechts für Tür
        if (len > 0)
        {
            TextOut(hdc, x+Size.ZBreite+spaceSquare, y_pos, pFst, strlen(pFst));
            MoveToEx(hdc, x+Size.ZBreite+spaceSquare+20, y_pos - 4, NULL);
            LineTo(hdc, x + Size.ZBreite - 25, -(y+Size.ZGesamtHoehe - 30) ); 
        }
     }
     if (Art == TOR)
     {
        // Beschriftung rechts für Tor
        pFst = dataScan.getTorFeststellung(TorFeststellung[1]);
        len = strlen(pFst);
        if (len > 0)
        {
            CFlParam* pFl = (CFlParam*)FlParam->GetAt(0);
            TextOut(hdc, x+Size.ZBreite-spaceSquare - 40, y_pos, pFst, strlen(pFst));
            MoveToEx(hdc, x+Size.ZBreite+spaceSquare - 40, y_pos - 4, NULL);
            LineTo(hdc, x + pFl->iZBreite + 21, -(y+Size.ZGesamtHoehe - 30) ); 
        }
     }
  }
}

void CTorDoor::updateElemente()
{
   if (RahmenElemente == NULL)
      RahmenElemente = new CPtrArray;
   else
      deleteRahmenElemente();
   computeRahmen();

   if (Treibriegel == NULL)
      Treibriegel = new CPtrArray;
   else
      deleteTreibriegel();
   computeTreibriegel();
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
   if (RahmenElemente == NULL)
      RahmenElemente = new CPtrArray;
   else
      deleteRahmenElemente();
   if (Treibriegel == NULL)
      Treibriegel = new CPtrArray;
   else
      deleteTreibriegel();


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
        computeProfile(FluegelP, count, i);
     }
   }

   computeRahmen();

   computeTreibriegel();
}

void CTorDoor::computeRahmen(void)
{
    int iHoehe, iBreite;

    iBreite = Size.Breite*10;
    iHoehe = Size.Hoehe*10;
    if (Typ == 4)
    {
        // Typ 400
        if (RahmenArt == RW || RahmenArt == RZ)
        {
            // Z- und W-Rahmen
            if (Art == ATUER)
            {
                // Türen ohne Abzuege
            }
            else
            if (Art == ATOR)
            {
                iBreite -= 8;
                iHoehe  -= 8;
            }
            else
            if (Art == FT3 || Art == FT4 || Art == FT5)
            {
                iBreite -= 20;
                iHoehe  -= 8;
            }
            else
            {
                // alles andere nicht
                iBreite = iHoehe = 0;
            }
        }
    }
    else
    if (Typ == 6)
    {
        // Typ 600
        if (RahmenArt == RW || RahmenArt == RZ)
        {
            // Z- und W-Rahmen
            if (Art == ATUER)
            {
                // Türen ohne Abzuege
            }
            else
            if (Art == ATOR)
            {
                iBreite -= 8;
                iHoehe  -= 8;
            }
            else
            if (Art == FT3 || Art == FT4 || Art == FT5)
            {
                iBreite -= 20;
                iHoehe  -= 8;
            }
            else
            {
                // alles andere nicht
                iBreite = iHoehe = 0;
            }
        }        
    }
    if (iBreite != 0 && iHoehe != 0)
    {
        insertRahmenElement(2*StueckZahl, iHoehe, SENKRECHT, (tRAHMEN)RahmenArt);
		if (RahmenArt == RZ)
		{
			insertRahmenElement(1*StueckZahl, iBreite, WAAGRECHT, (tRAHMEN)RahmenArt);
			insertRahmenElement(1*StueckZahl, iBreite, WAAGRECHT, RW);
		}
		else
		{
			insertRahmenElement(2*StueckZahl, iBreite, WAAGRECHT, (tRAHMEN)RahmenArt);
		}
    }
}

void CTorDoor::computeTreibriegel(void)
{
    int profilType;

    if (FluegelAnz == 2)
        profilType = 0;
    else
        profilType = 1;

    if (FluegelAnz > 1)
    {
        // nur bei Toren
        if (FluegelAnz == 2)
        {
            // Treibriegel bei Toren fuer unten
            this->insertTreibriegel((FluegelAnz-1)*StueckZahl, 1100, profilType);
        }
        else
        {
            this->insertTreibriegel((FluegelAnz-1)*StueckZahl, 1110, profilType);
        }
        if (RahmenArt == RZ)
        {
            this->insertTreibriegel((FluegelAnz-1)*StueckZahl, Size.Hoehe*10 - 1240, profilType);
        }
        else
        {
             this->insertTreibriegel((FluegelAnz-1)*StueckZahl, Size.Hoehe*10 - 1220, profilType);
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

// Function name	: CTorDoor::OutputAlignedFont
// Description	    : gibt Text aligned wie durch Übergabe spezifiziert aus mit übergebenem
//                    Font
// Return type		: void 
// Argument         : eAlignType eAlign
// Argument         : char* szBuf   
// Argument         : int iXPos     X-Position(links: links, rechts: rechts, mitte: mitte)
// Argument         : int iYPos
void CTorDoor::OutputAlignedFont(HDC hdc, eAlignType eAlign, char* szBuf, int iXPos, int iYPos,
                                 HFONT font)
{
    UINT dwOrgAlign;
    HFONT orgFont = (HFONT)SelectObject(hdc, font);

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
    SelectObject(hdc, orgFont);
    SetTextAlign(hdc, dwOrgAlign);
}

void CTorDoor::SaveToFile(char* strFileName)
{
}


BOOL CTorDoor::LoadFromFile(char* strFileName)
{
    return TRUE;
}

void CTTSize::Serialize( CArchive& archive, int iVersion )
{
    // call base class function first
    // base class is CObject in this case
    CObject::Serialize( archive );

    // now do the stuff for our specific class
    if( archive.IsStoring() )
    {
         archive << Breite << Hoehe << BBreite << BHoehe << ObtHoehe << SehnenHoehe << iProfilDicke;
         archive << dRadius;
    }
    else 
    {
         archive >> Breite >> Hoehe >> BBreite >> BHoehe >> ObtHoehe >> SehnenHoehe >> iProfilDicke;
         archive >> dRadius;
    }
}

// Return: FALSE  Problem beim Lesen der Version, 
int CTorDoor::Serialize( CArchive& archive, BOOL bReadVersion)
{
    CString strKunde, strKommission, strArtikel, strFuellung;
    
    if (archive.IsLoading())
        m_iVersion = -1;

    // call base class function first
    // base class is CObject in this case
    CObject::Serialize( archive );

    TRY
    {
        if (bReadVersion)
            archive.SerializeClass(RUNTIME_CLASS(CTorDoor));
        else
            m_iVersion = 1;
    }
    CATCH(CArchiveException, pEx)
    {
       // Simply show an error message to the user.
        if (archive.IsLoading())
		{
			if (pEx->m_cause == CArchiveException::badIndex || 
				pEx->m_cause == CArchiveException::badClass || 
				pEx->m_cause == CArchiveException::badSchema)
			{
				return 2;
			}
			else
			if (pEx->m_cause == CArchiveException::endOfFile)
			{
				return 1;
			}
			else
			{
				AfxMessageBox("Fehler beim Öffnen der Datei", MB_OK, 0);
				return 2;
			}
		}
    }
    END_CATCH

    if (m_iVersion == -1)   // Version wurde noch nicht ausgelesen
        m_iVersion = archive.GetObjectSchema();
    Size.Serialize(archive, m_iVersion);

    // now do the stuff for our specific class
    if( archive.IsStoring() )
    {
         strKunde = Kunde;
         strKommission = Kommission;
         strArtikel = sArtikel;
         strFuellung = CTorDoor::strFuellung;

         archive << strKunde << strKommission << strArtikel << strFuellung << Art;
         archive << Typ << RahmenArt << DIN << Fuellung << OKF << bBogen << FluegelAnz;
         archive << StueckZahl;
         // ab hier Version 2
         archive << b400R << (int)ProfilMass;
         // ab hier Version 3
         archive << PzGs;
         for (int i = 0; i < TS_MAX; i++)
         {
            archive << TorEinbau[i];
         }
         for (int i = 0; i < 2; i++)
         {
            archive << TorFeststellung[i];
         }         
    }
    else 
    {
         int iProfilMass;
         archive >> strKunde;
         archive >> strKommission >> strArtikel >> strFuellung >> Art;
         archive >> Typ >> RahmenArt >> DIN >> Fuellung >> OKF >> bBogen >> FluegelAnz;
         archive >> StueckZahl;
         // ab hier Version 2
         iProfilMass = PD_70_40; b400R = FALSE;  // defaults
         // ab hier Version 3
         if (m_iVersion > 1)
         {
            archive >> b400R >> (int)iProfilMass;
         }
         if (m_iVersion > 2)
         {
            int iPzGs = PZ_GS_NONE, iTorEinbau, iTorFeststellung;
            archive >> (int)iPzGs;
            PzGs = (tPzGs)iPzGs;
            for (int i = 0; i < TS_MAX; i++)
            {
                archive >> (int)iTorEinbau;
                TorEinbau[i] = (tTorEinbau)iTorEinbau;
            }
            for (int i = 0; i < 2; i++)
            {
                archive >> (int)iTorFeststellung;
                TorFeststellung[i] = (tTorFeststellung)iTorFeststellung;
            }
         }

         ProfilMass = (tProfilMass)iProfilMass;

         strcpy(Kunde, strKunde);
         strcpy(Kommission, strKommission);
         strcpy(sArtikel, strArtikel);
         strcpy(CTorDoor::strFuellung, strFuellung);
    }

    SerializePtrArray(archive, &FlParam, TTOBJ_TYPE_FLPARAM);
    SerializePtrArray(archive, &Profile, TTOBJ_TYPE_PROFILE_ELEM);
    SerializePtrArray(archive, &GlasProfile, TTOBJ_TYPE_PROFILE_ELEM);
    SerializePtrArray(archive, &RiegelElemente, TTOBJ_TYPE_RIEGEL_ELEM);
    SerializePtrArray(archive, &HolzElemente, TTOBJ_TYPE_HOLZ_ELEM);
    SerializePtrArray(archive, &BetoPlanElemente, TTOBJ_TYPE_HOLZ_ELEM);
    return 0;
}


void CTorDoor::SerializePtrArray(CArchive& archive, CPtrArray** ppPAry, int iObjType)
{
    int iArrSize;
    CTTObject* pObj;

    if ( archive.IsStoring() )
    {
        if (*ppPAry == NULL)
            return;
        archive << iObjType;
        iArrSize = (*ppPAry)->GetSize();
        archive << iArrSize;
        for (int i = 0; i < iArrSize; i++)
        {
            pObj = (CTTObject*)(*ppPAry)->GetAt(i);
            if (pObj)
            {
                pObj->MySerialize(archive, m_iVersion);
            }
        }
    }
    else
    {
        int iObjT;
        archive >> iObjT;
        if (iObjT != iObjType)
            return;
        archive >> iArrSize;
        if (*ppPAry == NULL)
        {
            *ppPAry = new CPtrArray;
            ASSERT(*ppPAry != NULL);
        }
        for (int i = 0; i < iArrSize; i++)
        {
            switch (iObjT)
            {
            case TTOBJ_TYPE_FLPARAM:
                pObj = new CFlParam;
                break;
            case TTOBJ_TYPE_PROFILE_ELEM:
                pObj = new CProfileElem;
                break;
            case TTOBJ_TYPE_RIEGEL_ELEM:
                pObj = new CRiegelElement;
                break;
            case TTOBJ_TYPE_HOLZ_ELEM:
                pObj = new CHolzElement;
                break;
            default:
                ASSERT(TRUE);
                break;
            }
            ASSERT(pObj);
            pObj->MySerialize(archive, m_iVersion);
            (*ppPAry)->Add(pObj);
        }
    }
}

void CFlParam::MySerialize(CArchive& archive, int iVersion)
{
    if ( archive.IsStoring() )
    {
        archive << Geteilt << OArt << FArt << ObtHoehe << GArt << iBreite;
        // ab hier Version 2
        archive << Sprosse << prflOben << prflUnten << prflLinks << prflRechts;
    }
    else
    {
        int iSprosse, iPrflOben, iPrflUnten, iPrflLinks, iPrflRechts;
        archive >> Geteilt >> OArt >> FArt >> ObtHoehe >> GArt >> iBreite;
        if (iVersion > 1)
        {
            archive >> iSprosse >> iPrflOben >> iPrflUnten >> iPrflLinks >> iPrflRechts;
            Sprosse = (tSprosse)iSprosse;
            prflOben = (tProfil)iPrflOben;
            prflUnten = (tProfil)iPrflUnten;
            prflLinks = (tProfil)iPrflLinks;
            prflRechts = (tProfil)iPrflRechts;
        }
        else
        {
            Sprosse = (tSprosse)0;
            prflOben = (tProfil)0;
            prflUnten = (tProfil)0;
            prflLinks = (tProfil)0;
            prflRechts = (tProfil)0;
        }
    }
}

void CProfileElem::MySerialize(CArchive& archive, int iVersion)
{
    int iDir, iTyp;
    if ( archive.IsStoring() )
    {
        archive << Anzahl << Laenge << (int)Direction << (int)uTyp << iWinkel1 << iWinkel2;
        // ab hier Version 2
        archive << Profil;
    }
    else
    {
        int iProfil = RP6_8;  // damit Typ 600 keinen Fehler meldet
        archive >> Anzahl >> Laenge >> iDir >> iTyp >> iWinkel1 >> iWinkel2;
        Direction = (tDirect)iDir; uTyp = (tTypWaagrecht)iTyp;
        if (iVersion > 1)
            archive >> iProfil;
        Profil = (tProfil)iProfil;
    }
}

void CHolzElement::MySerialize(CArchive& archive, int iVersion)
{
    if ( archive.IsStoring() )
    {
        archive << m_iAnzahl << m_iBreite << m_iLaenge;
        // ab hier Version 4
        archive << m_iSort;
    }
    else
    {
        archive >> m_iAnzahl >> m_iBreite >> m_iLaenge;
        if (iVersion > 3)
            archive >> m_iSort;
        else
            m_iSort = 0;
    }
}

void CRiegelElement::MySerialize(CArchive& archive, int iVersion)
{
    if ( archive.IsStoring() )
    {
        archive << m_iAnzahl << m_iLaenge;
    }
    else
    {
        archive >> m_iAnzahl >> m_iLaenge;
    }
}

void CRahmenElem::MySerialize(CArchive& archive, int iVersion)
{
}

void CTreibriegelElem::MySerialize(CArchive& archive, int iVersion)
{
}

#pragma warning( pop ) 
