#ifndef _CTORDOOR
#define _CTORDOOR


#define DINBREITE   15    //Breite des Türgriffs
#define DINHOEHE    4     //Hoehe des Türgriffs
#define BEMABSTAND  50    //Abstand der Bemaßungslinie
#define BLINIEAB    20    //Abstand der horizontalen Bemaßungslinie
#define BLINIEL     40    //Länge der Hor. Bem.linie
#define BLINIEO     8     //
#define BQUERY      10    //
#define BQUERX      10    //
#define BESCHBREITE 3     //Breite der Beschläge
#define BESCHHOEHE  18    //Hoehe der Beschläge
#define BESCHTORY   18    //Abstand von Oben bei Tor
#define BESCHTUERY  24    //Abstand von Oben bei Tür
#define OEFFNERPOSY 120   //Postion des Öffners in Y-Richtung von unten
   
#define OBERTEILABZUG  14
#define TUERWINHOEHE   70
#define TUERWINPOSY    10
#define TUERWINABX     12   //Abstand in x Richtung zu

//OiF für Falttor und Tor
#define ABOIFFT        7
#define OIFBANDBREITE  2
#define OIFBANDHOEHE   14
#define OIFABSTAND     10

//Pfeil für Schiebefenster
#define SFPFLEN	    80
#define SFPFXY      10          
 
//#define SEHNENHOEHE     16


//class TNSCollection;
class CFlParam;
typedef CArray<int, int&> *tFLParam;

typedef enum { WAAGRECHT, SENKRECHT } tDirect;
typedef enum { HOLZAUF, HOLZUNTEN} tTypWaagrecht;


class CProfileElem : public CObject
{
   public:
    int Anzahl;
    int Laenge;
    tDirect Direction;
    tTypWaagrecht uTyp;
    int iWinkel1, iWinkel2;       // für Bogenprofile

    CProfileElem(int Anz, int Len, tDirect Dir, tTypWaagrecht Typ,
        int iW1 = 0, int iW2 = 0 )
    { Anzahl = Anz; Laenge = Len; Direction = Dir;  uTyp = Typ; 
        iWinkel1 = iW1; iWinkel2 = iW2; };
    int operator== (CProfileElem& p)
    { return (Laenge == p.Laenge && Direction == p.Direction
	     /*&&uTyp == p.uTyp*/ && iWinkel1 == p.iWinkel1 && 
         iWinkel2 == p.iWinkel2); };
    int operator= (CProfileElem& p)
    { Anzahl = p.Anzahl; Laenge = p.Laenge; Direction = p.Direction;
      uTyp = p.uTyp; iWinkel1 = p.iWinkel1; iWinkel2 = p.iWinkel2; }; 
};


class CGlasProfileElem : public CProfileElem
{
   public:
    CGlasProfileElem(int Anz, int Len, tDirect Dir, tTypWaagrecht Typ)
     : CProfileElem(Anz, Len, Dir, Typ)
     {};
//    { Anzahl = Anz; Laenge = Len; Direction = Dir;  uTyp = Typ; };
    int operator== (CGlasProfileElem& p)
    { return (Laenge == p.Laenge && Direction == p.Direction
	     /*&&uTyp == p.uTyp*/); };
    int operator= (CGlasProfileElem& p)
    { Anzahl = p.Anzahl; Laenge = p.Laenge; Direction = p.Direction;
      uTyp = p.uTyp; };
};

class CTTSize : public CObject
{
   public:
    int Breite;
    int Hoehe;
    int BBreite;     //breite für Bearbaitung
    int BHoehe;	     //hoehe für Bearbeitung
    int ZBreite;     //breite für Zeichung
    int ObtHoehe;   //Hoehe des Oberteils
    int SehnenHoehe;  // Hoehe der Sehne bei Bogentor
    int ZGesamtHoehe;  // Hoehe mit Bogen
    int iProfilDicke;  // Dicke des Profils

    CTTSize() { Breite = Hoehe = BBreite = BHoehe = ZBreite = ObtHoehe
        = SehnenHoehe = ZGesamtHoehe = 0; iProfilDicke = 20; };
    int operator== (CTTSize& s)
    { return (Breite == s.Breite && Hoehe == s.Hoehe &&
              ObtHoehe == s.ObtHoehe); }
    void operator=  (CTTSize& s)
    {Breite = s.Breite; Hoehe = s.Hoehe;
     BBreite = s.BBreite; BHoehe = s.BHoehe; ObtHoehe = s.ObtHoehe;
    SehnenHoehe = s.SehnenHoehe; iProfilDicke = s.iProfilDicke; }
};


class CFlParam : public CObject
{
   public:
    int Geteilt;   //geteilt, nicht geteilt
    int OArt;	   //Oberteilart: z. Öffnen, i.Flügel
    int FArt;      //Fensterart
    int ObtHoehe;  //Oberteilhöhe
    int GArt;      //Glasart;

    CFlParam() { ObtHoehe = 70; OArt = 0; FArt = 0;};
    int operator== (CFlParam& s)
    { return (Geteilt == s.Geteilt && FArt == s.FArt &&
              ObtHoehe == s.ObtHoehe && OArt == s.OArt); };
    void operator= (CFlParam& s)
    { Geteilt = s.Geteilt; FArt = s.FArt; ObtHoehe = s.ObtHoehe;
      GArt = s.GArt; OArt = s.OArt; };
};

class CSchiebeFensterZugaben;

class CTorDoor
{
   int dx, dy, db, dh;
 public:
	 void getSFAddValues(CSchiebeFensterZugaben* pSf, CSchiebeFensterZugaben* pSf2);
	 void getAddValues(CFlParam* pFl, int iCount, int &iAddQuerT, int &iAddSenkT, int &iAddOberT, int &iAddUnterT);
	 void computeBogenProfile(CFlParam* pFl, int count);
   char Kunde[100];
   char Kommission[100];
   char sArtikel[5];  //Artikelnr
   int Art;      //TÜR, TOR, FT3FLG, FT4FLG,
   int Typ;      //400,600
   int RahmenArt;
   int DIN;
   int Fuellung;
   char strFuellung[100];
   BOOL OKF;
   BOOL bBogen;
   int FluegelAnz;  //Anzahl Fluegel
   CTTSize Size;
   int StueckZahl;

   CPtrArray* FlParam;
   CPtrArray* Profile;
   CPtrArray* GlasProfile;

   float scF;   //Skalierungsfaktor
   int   aSp;   //Außenspalte
   int   iSp;   //Innenspalte

   CTorDoor();// {scF = 0.7; aSp = 2; iSp = 1; FlParam = NULL;};
   CTorDoor(CTorDoor* );
/*
   ~CTorDoor() { 
	     if (FlParam != NULL) delete FlParam;
		 if (Profile != NULL) delete Profile;
   };
*/
   ~CTorDoor();

   int operator== (CTorDoor&);
   void deleteFluegel();
   void deleteProfiles();
   void deleteGlasProfiles();

   virtual void drawTT(HDC hdc, int x, int y, int breite, int hoehe);
   void draw(HDC hdc);
   void SetMeas(int px, int py, int pb, int ph)
	       { dx=px; dy=py; db=pb; dh=ph;};
   virtual void drawBesch(HDC hdc, int x, int y);
   void drawDIN(HDC hdc, int x, int y);
   void drawBem(HDC hdc, int x, int y);
   void drawOIF(HDC hdc, int x, int y, int breite, int hoehe, int seite);
   void drawInfo(HDC hdc, int x, int y);
   BOOL drawConnectedPoints(HDC hdc, LPPOINT pPoints, int iCount);

   BOOL inBounds(int x, int y, int mx, int my);
   int  isInOnBounds(int x, int y);
   HFONT getFont(int height, int width, int orient);
   BOOL insertProfil(int Anzahl, int Laenge, tDirect dir,
                  tTypWaagrecht uTyp, int Winkel1 = 0, int iWinkel2 = 0);
   BOOL insertGlasProfil(int Anzahl, int Laenge, tDirect dir,
                      tTypWaagrecht uTyp);
   void computeProfile(CFlParam* pFl, int count);
   virtual float calculateScale(int breite, int hoehe);

   int printKundKomm(HDC hdc, int x, int y, int maxX, HFONT font);
   int printStueck(HDC hdc, int x, int y, int maxX, HFONT font);
   int printBreite(HDC hdc, int x, int y, int maxX, HFONT font,
       HFONT thinFont);
   int printHoehe(HDC hdc, int x, int y, int posDir, int maxX, HFONT font);
   int printDIN(HDC hdc, int x, int y, int maxX, HFONT font);
   int printObert(HDC hdc, int x, int y, int maxX, HFONT font);
   int printFenster(HDC hdc, int y, int posStarr,
	int posKipp, int posSchieb, int maxX, HFONT font);
   int printFuellung(HDC hdc, int x, int y, int maxX, HFONT font);
   void drawDirection(HDC hdc, int x, int y, tDirect dir);
   void drawProfil(HDC hdc, int x, int y, int typ);
   void drawSquares(HDC hdc, int x, int y);
   void profilRP1614(HDC hdc, int x, int y);
   void profilRP1615(HDC hdc, int x, int y);
   void profilRP1093(HDC hdc, int x, int y);
   void profilRP1094(HDC hdc, int x, int y);

   void updateValues();
};


#define MAXTOR 10
/*
class CKunde
{
  public:
   char KundName[100];
   TNSCollection* TorListe;   //Liste mit CTorDoor

   int TorCnt;

   CKunde(char* );
   ~CKunde();
   void insertTor(CTorDoor* );
   int operator== (CKunde& );
   char* getName();
   void draw(HDC hdc, int x, int y); 
};


#define MAXKUNDE 10
class CMainList
{
  public:
   TNSCollection* KundListe;
   int KundCnt;

   CKunde* getKunde(char* KundName);
   CMainList();
   ~CMainList();
   CKunde* SeekInsert(char* , CTorDoor* );
};
*/
#endif