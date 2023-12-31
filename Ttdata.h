#ifndef _DATAMOD
#define _DATAMOD

typedef unsigned char BYTE;

typedef enum {TOR, TUER, FUELLUNG, OBERTEIL, ANDERES}
	     tkindOf;
typedef enum {ATOR, ATUER, FT3, FT4, FT5, STUER, STOR, ART_END} tART;

typedef enum {RZ,RR,RW,R_OHNE,RAHMEN_END} tRAHMEN;
typedef enum {L,R} tDIN;
typedef enum {LEER, OzO, OiF, OBERTEIL_END} tOBERTEIL;
typedef enum {KEIN, OlF, SF, KF, SF2} tFENSTER;
typedef enum {Plexi4, Plexi5, SDP10, SDP16, SDP20, ISO,
	      KLAR, DRAHT} tGLAS;
typedef enum {F_NONE, F_HOLZ, F_BETOPLAN, F_BLECH, F_BOHLEN, F_HOLZ_I } tFUELLUNG;

typedef enum { NP=0, RPRR /*Rechteck-Rohr*/, RP1087, RP1093, RP1094, RP1348, RP1070, 
               RP1114, RP1071, RP1163, RP1072,
               RP1191, RP1579, RP6_M, RP6_8 /* Profil Typ600/800 */  } tProfil;

typedef enum { TEB_LEER = 0, TEB_MAUERWERK, TEB_BETON, TEB_HOLZ, TEB_MAX } tTorEinbau;

typedef enum { TFS_LEER = 0, TFS_HAKEN, TFS_STELLER_40, TFS_STELLER_60, TFS_STELLER_80, TFS_MAX } tTorFeststellung;

typedef enum { BAND_LEER = 0, BAND_14, BAN_16, BAND_16_80, BAND_MAX } tBand;

typedef enum { KLAPPG_LEER = 0, KLAPPG_INNEN, KLAPPG_AUSSEN, KLAPPG_BEIDS, KLAPPG_MAX } tKlappgriff;

typedef enum { SCHWELLE_LEER = 0, SCHWELLE_3, SCHWELLE_4, SCHWELLE_MAX } tSchwelle;

typedef enum { SF_RHMN_LEER = 0, SF_RHMN_T, SF_RHMN_P, SF_RHMN_MAX } tSfRahmen;

typedef enum { FLIEGENGITTER_LEER = 0, FLIEGENGITTER_ALU, FLIEGENGITTER_VA, FLIEGENGITTER_MAX } tFliegengitter;


typedef struct tagTTData{
	int  artNr;
        char sartNr[5];
	char Bezeich[99];
	BYTE kindOf;            //tkindOf
	BYTE Art;		//tART T�r, Tor, 3-flg, 4-flg
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
     const char* getTorEinbau(int teb);
     const char* getTorFeststellung(int tfs);
     const char* getBand(int band);
     BOOL  getFuellung(int iNrFuellung, tFUELLUNG* pfAussen, tFUELLUNG* pfInnen);
     BOOL  getHolzZuschnitt(int iLenRiegel, HolzZuschnitt** ppHzE);
     char* getNameForProfil(tProfil prfl);
     const char* getNameForFuellung(int index);
     int getEnumForFuellung(int index);
     int getFuellungIndex(int iNrFuellung, bool bAussen);
     tProfil getProfilForName(char* szName);
     const char* getKlappgriff(int klapp);
     const char* getSchwelle(int schwelle);
     const char* getSfRahmen(int sfr);
     const char* getFliegengitter(int fg);

   private:
     TTData*  getPtrToArtikel(int iNrArtikel);
};

#endif