#ifndef _CTORDOOR
#define _CTORDOOR


// Klassen-Version
#define TORDOOR_VERSION 1

#define DINBREITE   15    //Breite des T�rgriffs
#define DINHOEHE    4     //Hoehe des T�rgriffs
#define BEMABSTAND  50    //Abstand der Bema�ungslinie
#define BLINIEAB    20    //Abstand der horizontalen Bema�ungslinie
#define BLINIEL     40    //L�nge der Hor. Bem.linie
#define BLINIEO     8     //
#define BQUERY      10    //
#define BQUERX      10    //
#define BESCHBREITE 3     //Breite der Beschl�ge
#define BESCHHOEHE  18    //Hoehe der Beschl�ge
#define BESCHTORY   18    //Abstand von Oben bei Tor
#define BESCHTUERY  24    //Abstand von Oben bei T�r
#define OEFFNERPOSY 120   //Postion des �ffners in Y-Richtung von unten
   
#define OBERTEILABZUG  14
#define TUERWINHOEHE   70
#define TUERWINPOSY    10
#define TUERWINABX     12   //Abstand in x Richtung zu

//OiF f�r Falttor und Tor
#define ABOIFFT        7
#define OIFBANDBREITE  2
#define OIFBANDHOEHE   14
#define OIFABSTAND     10

//Pfeil f�r Schiebefenster
#define SFPFLEN	    80
#define SFPFXY      10          
 
//#define SEHNENHOEHE     16


//////////////////////////////
// Defines f�r Holzbretter
#define  HOLZELEM_BREITE     112     // 112 mm

/////////////////////////////////
// Defines f�r Custom Areas in y
#define  CA_Y_TOP                  0
#define  CA_Y_AFTER_ROFILES        1
#define  CA_Y_AFTER_GLASPROFILES   2
#define  CA_Y_AFTER_FUELLUNG       3
#define  CA_Y_BOTTOMLINE           4      // unterste Zeile



//class TNSCollection;
class CFlParam;
typedef CArray<int, int&> *tFLParam;

typedef enum { WAAGRECHT, SENKRECHT } tDirect;
typedef enum { HOLZAUF, HOLZUNTEN} tTypWaagrecht;

//////////////////////////////////////////////////////////////////
// Basis-Klasse f�r typsicheres Serialisieren von PtrArrays
//
#define TTOBJ_TYPE_FLPARAM        0
#define TTOBJ_TYPE_PROFILE_ELEM   1
#define TTOBJ_TYPE_RIEGEL_ELEM    2
#define TTOBJ_TYPE_HOLZ_ELEM      3


class CTTObject : public CObject
{
public:
    virtual void MySerialize(CArchive& archive) = 0;
    virtual int  GetObjType(void) = 0;
};

// Klasse f�r Zolz-Riegel (Querteile, auf die genagelt wird)
//
class CRiegelElement : public CTTObject
{

public:
    int m_iAnzahl;          // Anzahl der Riegel
    int m_iLaenge;          // Laenge der Riegel
    
    CRiegelElement() { m_iAnzahl = m_iLaenge = 0; };
    CRiegelElement(int iAnz, int iLaenge)
    { m_iAnzahl = iAnz; m_iLaenge = iLaenge; };

    int operator!= (CRiegelElement& p)
    { return (m_iLaenge != p.m_iLaenge || m_iAnzahl != p.m_iAnzahl); };
    int operator== (CRiegelElement& p)
    { return (m_iLaenge == p.m_iLaenge); };
    int operator= (CRiegelElement& p)
    { m_iAnzahl = p.m_iAnzahl; m_iLaenge = p.m_iLaenge; }; 

    virtual void MySerialize(CArchive& archive);
    virtual int  GetObjType(void) { return TTOBJ_TYPE_RIEGEL_ELEM; };
};

// Klasse enth�lt Informationen zu Holzzuschnitt-Brettern
//
class CHolzElement : public CTTObject
{

public:
    int m_iAnzahl;          // Anzahl der Bretter
    int m_iBreite;          // Breite der Bretter
    int m_iLaenge;          // Laenge der Bretter
    
    CHolzElement() { m_iAnzahl = m_iBreite = m_iLaenge = 0; };
    CHolzElement(int iAnz, int iBreite, int iLaenge)
    { m_iAnzahl = iAnz; m_iBreite = iBreite; m_iLaenge = iLaenge; };

    int operator!= (CHolzElement& p)
    { return (m_iBreite != p.m_iBreite || m_iLaenge != p.m_iLaenge || m_iAnzahl != p.m_iAnzahl); };
    int operator== (CHolzElement& p)
    { return (m_iBreite == p.m_iBreite && m_iLaenge == p.m_iLaenge); };
    int operator= (CHolzElement& p)
    { m_iAnzahl = p.m_iAnzahl; m_iBreite = p.m_iBreite; m_iLaenge = p.m_iLaenge; }; 

    virtual void MySerialize(CArchive& archive);
    virtual int  GetObjType(void) { return TTOBJ_TYPE_HOLZ_ELEM; };
};

class CBetoPlanElement : public CHolzElement
{

public:
    CBetoPlanElement(int iAnz, int iBreite, int iLaenge)
    { m_iAnzahl = iAnz; m_iBreite = iBreite; m_iLaenge = iLaenge; };
};

class CProfileElem : public CTTObject
{

   public:
    int Anzahl;
    int Laenge;
    tDirect Direction;
    tTypWaagrecht uTyp;
    int iWinkel1, iWinkel2;       // f�r Bogenprofile

    CProfileElem() { Anzahl = 0; Laenge = 0; Direction = WAAGRECHT;  uTyp = HOLZAUF; 
        iWinkel1 = 0; iWinkel2 = 0; };
    CProfileElem(int Anz, int Len, tDirect Dir, tTypWaagrecht Typ,
        int iW1 = 0, int iW2 = 0 )
    { Anzahl = Anz; Laenge = Len; Direction = Dir;  uTyp = Typ; 
        iWinkel1 = iW1; iWinkel2 = iW2; };
    int operator!= (CProfileElem& p)
    { return (Laenge != p.Laenge || Direction != p.Direction
	     /*&&uTyp == p.uTyp*/ || iWinkel1 != p.iWinkel1 || 
         iWinkel2 != p.iWinkel2 || Anzahl != p.Anzahl); };
    int operator== (CProfileElem& p)
    { return (Laenge == p.Laenge && Direction == p.Direction
	     /*&&uTyp == p.uTyp*/ && iWinkel1 == p.iWinkel1 && 
         iWinkel2 == p.iWinkel2); };
    int operator= (CProfileElem& p)
    { Anzahl = p.Anzahl; Laenge = p.Laenge; Direction = p.Direction;
      uTyp = p.uTyp; iWinkel1 = p.iWinkel1; iWinkel2 = p.iWinkel2; }; 

    virtual void MySerialize(CArchive& archive);
    virtual int  GetObjType(void) { return TTOBJ_TYPE_PROFILE_ELEM; };
};


class CGlasProfileElem : public CProfileElem
{
    
   public:
    CGlasProfileElem(int Anz, int Len, tDirect Dir, tTypWaagrecht Typ)
     : CProfileElem(Anz, Len, Dir, Typ)
     {};
//    { Anzahl = Anz; Laenge = Len; Direction = Dir;  uTyp = Typ; };
    int operator!= (CGlasProfileElem& p)
    { return (Laenge != p.Laenge || Direction != p.Direction || Anzahl != p.Anzahl
	     /*&&uTyp == p.uTyp*/); };
    int operator== (CGlasProfileElem& p)
    { return (Laenge == p.Laenge && Direction == p.Direction
	     /*&&uTyp == p.uTyp*/); };
    int operator= (CGlasProfileElem& p)
    { Anzahl = p.Anzahl; Laenge = p.Laenge; Direction = p.Direction;
      uTyp = p.uTyp; };
};

class CTTSize : public CObject
{
   DECLARE_SERIAL( CTTSize )

   public:
    int Breite;
    int Hoehe;
    int BBreite;       //breite f�r Bearbaitung
    int BHoehe;	       //hoehe f�r Bearbeitung
    int ZBreite;       //breite f�r Zeichung
    int ObtHoehe;      //Hoehe des Oberteils
    int SehnenHoehe;   // Hoehe der Sehne bei Bogentor
    int ZGesamtHoehe;  // Hoehe mit Bogen
    int iProfilDicke;  // Dicke des Profils
    double dRadius;       // Radius bei Bogen

    CTTSize() { Breite = Hoehe = BBreite = BHoehe = ZBreite = ObtHoehe
        = SehnenHoehe = ZGesamtHoehe = 0; iProfilDicke = 20; 
        dRadius = 0; };
    int operator== (CTTSize& s)
    { return (Breite == s.Breite && Hoehe == s.Hoehe &&
              ObtHoehe == s.ObtHoehe); }
    void operator=  (CTTSize& s)
    {Breite = s.Breite; Hoehe = s.Hoehe;
     BBreite = s.BBreite; BHoehe = s.BHoehe; ObtHoehe = s.ObtHoehe;
    SehnenHoehe = s.SehnenHoehe; iProfilDicke = s.iProfilDicke; 
    dRadius = s.dRadius; };

    virtual void Serialize( CArchive& archive );
};


class CFlParam : public CTTObject
{

   public:
    int Geteilt;   //geteilt, nicht geteilt
    int OArt;	   //Oberteilart: z. �ffnen, i.Fl�gel
    int FArt;      //Fensterart
    int ObtHoehe;  //Oberteilh�he
    int GArt;      //Glasart;
    int iBreite;   // Breite eines Fluegels
    int iZBreite;  // Zeichen-Breite

    CFlParam() { ObtHoehe = 70; OArt = 0; FArt = 0; iBreite = 0;};
    int operator!= (CFlParam& s)
    { return (Geteilt != s.Geteilt || FArt != s.FArt ||
              ObtHoehe != s.ObtHoehe || OArt != s.OArt || iBreite != s.iBreite); };
    int operator== (CFlParam& s)
    { return (Geteilt == s.Geteilt && FArt == s.FArt &&
              ObtHoehe == s.ObtHoehe && OArt == s.OArt && iBreite == s.iBreite); };
    void operator= (CFlParam& s)
    { Geteilt = s.Geteilt; FArt = s.FArt; ObtHoehe = s.ObtHoehe;
      GArt = s.GArt; OArt = s.OArt; iBreite = s.iBreite; };

    virtual void MySerialize(CArchive& archive);
    virtual int  GetObjType(void) { return TTOBJ_TYPE_FLPARAM; };
};

class CSchiebeFensterZugaben;

typedef enum { E_ALIGN_LEFT, E_ALIGN_RIGHT, E_ALIGN_MID } eAlignType;

class CTorDoor : public CObject
{
   DECLARE_SERIAL( CTorDoor )

   int dx, dy, db, dh;
 public:
   char Kunde[100];
   char Kommission[100];
   char sArtikel[5];  //Artikelnr
   int Art;      //T�R, TOR, FT3FLG, FT4FLG,
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
   CPtrArray* HolzElemente;
   CPtrArray* RiegelElemente;
   CPtrArray* BetoPlanElemente;

   float scF;   //Skalierungsfaktor
   int   aSp;   //Au�enspalte
   int   iSp;   //Innenspalte

   CTorDoor();// {scF = 0.7; aSp = 2; iSp = 1; FlParam = NULL;};
   CTorDoor(CTorDoor* );
   ~CTorDoor();

   void getSFAddValues(CSchiebeFensterZugaben* pSf, CSchiebeFensterZugaben* pSf2);
   void getAddValues(CFlParam* pFl, int iCount, int &iAddQuerT, int &iAddSenkT, int &iAddOberT, int &iAddUnterT);
   void computeBogenProfile(CFlParam* pFl, int count);
   int operator== (CTorDoor&);
   void deleteFluegel();
   void deleteProfiles();
   void deleteGlasProfiles();
   void deleteHolzElemente();
   void deleteRiegelElemente();
   void deleteBetoPlanElemente();

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
   BOOL insertHolzElement(int iAnzahl, int iBreite, int iLaenge);
   BOOL insertRiegelElement(int iAnzahl, int iLaenge);
   BOOL insertBetoPlanElement(int iAnzahl, int iBreite, int iLaenge);

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

   int getLineBegin(int iKind);


   ///////////////
   // Testsystem
   void SaveToFile(char* strFileName);
   BOOL LoadFromFile(char* strFileName);
   virtual void Serialize( CArchive& archive );
   void SerializePtrArray(CArchive& archive, CPtrArray** ppPAry, int iObjType); 

 private:
    void ReleasePtrArray(CPtrArray* pArr);

 protected:
    void OutputAligned(HDC hdc, eAlignType eAlign, char* szBuf, int iXPos, int iYPos);
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