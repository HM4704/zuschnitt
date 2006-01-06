#ifndef _DATAMOD
#define _DATAMOD

typedef unsigned char BYTE;

typedef enum {TOR, TUER, FUELLUNG, OBERTEIL, ANDERES}
	     tkindOf;
typedef enum {ATOR, ATUER, FT3, FT4, FT5, STUER, STOR, ART_END} tART;

typedef enum {RZ,RR,RW,RAHMEN_END} tRAHMEN;
typedef enum {L,R} tDIN;
typedef enum {LEER, OzO, OiF, OBERTEIL_END} tOBERTEIL;
typedef enum {KEIN, OlF, SF, KF, SF2} tFENSTER;
typedef enum {Plexi4, Plexi5, SDP10, SDP16, SDP20, ISO,
	      KLAR, DRAHT} tGLAS;
typedef enum {F_NONE, F_HOLZ, F_BETOPLAN, F_BLECH, F_BOHLEN, F_HOLZ_I } tFUELLUNG;

typedef enum { NP=0, RPRR /*Rechteck-Rohr*/, RP1087, RP1093, RP1094, RP1348, RP1070, 
               RP1114, RP1071, RP1163, RP1072,
               RP1191, RP6_8 /* Profil Typ600/800 */  } tProfil;

typedef struct tagTTData{
	int  artNr;
        char sartNr[5];
	char Bezeich[99];
	BYTE kindOf;            //tkindOf
	BYTE Art;		//tART Tür, Tor, 3-flg, 4-flg
	BYTE Typ;		//400, 600, 800
	BYTE Rahmen;		//tRAHMEN Z, W,..
	BYTE DIN;		//tDIN L,R
} TTData;

typedef struct tagTOData {
	int artNr;
	char sartNr[5];
	char Bezeich[60];
	BYTE Art;
} TOData;


typedef struct tagHolzZuschnitt {
    int iLenRiegel;
    int iAnzBretter;
    int iLenAnfaenger;
} HolzZuschnitt;

//class TNSCollection;
class CTorDoor;


class TDataScan
{
   public:
     char* getBezeich(int artNr);
     void  getListArtNr(CStringArray* list, tkindOf kindOf, CString artNr);
     int   getTorArt(char* strArtikel);
     void  getTTDaten(CTorDoor*, CString str );
     void  getAllTT(CStringArray* );
     char* getArtBez(tART art);
     char* getRahmenBez(tRAHMEN rahmen);
     void  getSpecTT(CStringArray*, tART art);
     char* getGlasArt(int Glasart);
     BOOL  getFuellung(int iNrFuellung, tFUELLUNG* pfAussen, tFUELLUNG* pfInnen);
     BOOL  getHolzZuschnitt(int iLenRiegel, HolzZuschnitt** ppHzE);
     char* getNameForProfil(tProfil prfl);
     tProfil getProfilForName(char* szName);
     
   private:
     TTData*  getPtrToArtikel(int iNrArtikel);
};

#endif