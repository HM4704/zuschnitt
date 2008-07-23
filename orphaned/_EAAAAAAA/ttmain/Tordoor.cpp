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
	if (FlParam != NULL) delete FlParam;	 
	if (Profile != NULL) delete Profile;
	if (GlasProfile != NULL) delete GlasProfile;
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
//    insertProfil(1*StueckZahl, querProf, WAAGRECHT, HOLZAUF);
    int tempQuerProf = querProf;
    int senkProf = 10*Size.ObtHoehe + addOberT;   //????
    tempQuerProf -= 115;
    insertProfil(2*StueckZahl, senkProf, SENKRECHT, HOLZUNTEN);
    insertProfil(1*StueckZahl, tempQuerProf, WAAGRECHT, HOLZAUF);
  }
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
  HFONT orgFont = (HFONT)SelectObject(hdc, bFont);
  TextOut(hdc, x, -y, Kunde, strlen(Kunde));
  SelectObject(hdc, orgFont);
  TextOut(hdc, x, -(y+rowH), Kommission, strlen(Kommission));
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

  if (bBogen == TRUE)
  {
//      y += rowH;
      if (Typ == 6)
        strcpy(buf, "Achtung: Kastenmaße !");
      else
        strcpy(buf, "Berechnung nur für Typ 600 !");

      TextOut(hdc, x, -(y), buf, strlen(buf));
  }
  return 0; //temp., später richtig!!!
}

int CTorDoor::printBreite(HDC hdc, int x, int y, int maxX, HFONT bFont,
                          HFONT bThinFont)
{
  char temp[50];

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
         char buf[100];
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
     char buf[100];
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
  char temp[50];
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
//    TextOut(hdc, x, -y, "123", strlen("123"));
  }

  y +=  rowH;
  sprintf(temp, "%s", dataScan.getRahmenBez((tRAHMEN)RahmenArt));
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

#pragma warning( pop ) 
