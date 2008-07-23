#include "stdafx.h"
#include "ttdata.h"

//#include <tcollect.h>
#include <string.h>
#include <stdlib.h>
#include "tordoor.h"

//alle Tor-Daten
#define TTMAXDATA 129
#define TOMAXDATA 10
#define GLASMAXDATA 11


TTData tdata[TTMAXDATA] =
{
  { 4101, "4101", "Tür Typ 400 m. Z-Rahmen, verz. DIN R", TUER,
    ATUER, 4, RZ, R},
  { 4102, "4102", "Tür Typ 400 m. W-Rahmen, verz. DIN R", TUER,
    ATUER, 4, RW, R},
  { 4103, "4103", "Tür Typ 400 m. R-Rahmen, verz. DIN R", TUER,
    ATUER, 4, RR, R},
  { 4201, "4201", "Tür Typ 400 m. Z-Rahmen, verz. DIN L", TUER,
    ATUER, 4, RZ, L},
  { 4202, "4202", "Tür Typ 400 m. W-Rahmen, verz. DIN L", TUER,
    ATUER, 4, RW, L},
  { 4203, "4203", "Tür Typ 400 m. R-Rahmen, verz. DIN R", TUER,
    ATUER, 4, RR, L},
  { 4301, "4301", "Tor Typ 400 m. Z-Rahmen, verz. DIN R", TOR,
    ATOR, 4, RZ, R},
  { 4302, "4302", "Tor Typ 400 m. W-Rahmen, verz. DIN R", TOR,
    ATOR, 4, RW, R},
  { 4303, "4303", "Tor Typ 400 m. R-Rahmen, verz. DIN R", TOR,
    ATOR, 4, RR, R},
  { 4401, "4401", "Tor Typ 400 m. Z-Rahmen, verz. DIN L", TOR,
    ATOR, 4, RZ, L},
  { 4402, "4402", "Tor Typ 400 m. W-Rahmen, verz. DIN L", TOR,
    ATOR, 4, RW, L},
  { 4403, "4403", "Tor Typ 400 m. R-Rahmen, verz. DIN L", TOR,
    ATOR, 4, RR, L},
  { 4501, "4501", "m. doppelw. Holzfüllung", FUELLUNG,
    0, 0, 0, 0},
  { 4502, "4502", "m. einwandiger Holzfüllung", FUELLUNG,
    0, 0, 0, 0},
  { 4503, "4503", "ohne Füllung", FUELLUNG,
    0, 0, 0, 0},
  { 4504, "4504", "außen Holz, innen Beto-Plan", FUELLUNG,
    0, 0, 0, 0},
  { 4505, "4505", "m. doppelw. Beto-Plan-Füllung", FUELLUNG,
    0, 0, 0, 0},
  { 4506, "4506", "m. einw. Beto-Plan-Füllung", FUELLUNG,
    0, 0, 0, 0},
  { 4507, "4507", "außen Holz, innen Blech", FUELLUNG,
    0, 0, 0, 0},
  { 4508, "4508", "m. doppelw. Blechfüllung", FUELLUNG,
    0, 0, 0, 0},
  { 4509, "4509", "doppelw. Blech+FH-Isolierung", FUELLUNG,
    0, 0, 0, 0},
  { 4510, "4510", "m. einw. Blechfüllung", FUELLUNG,
    0, 0, 0, 0},
  { 4650, "4650", "einwandig Nord. Fichte, außen aufliegend", FUELLUNG,
    0, 0, 0, 0},
  { 4651, "4651", "einw. Holz m. Fischgrätmuster", FUELLUNG,
    0, 0, 0, 0},
  { 4652, "4652", "doppelw. Holz, außen Fischgrät", FUELLUNG,
    0, 0, 0, 0},
  { 4657, "4657", "außen Beto-Plan, innen Holz", FUELLUNG,
    0, 0, 0, 0},
  { 4658, "4658", "außen Holz, Beto bandseitig", FUELLUNG,
    0, 0, 0, 0},
  { 4653, "4653", "Aufpreis f. Rundbogen", ANDERES,
    0, 0, 0, 0},
  { 4654, "4654", "Aufpreis f. Rundbogen", ANDERES,
    0, 0, 0, 0},
  { 4656, "4656", "Aufpreis f. Korbbogen", ANDERES,
    0, 0, 0, 0},
  { 4701, "4701", "Falttor 3-flgl. Typ 400 m. W-Rahmen, verz. DIN R", TOR,
    FT3, 4, RW, R},
  { 4702, "4702", "Falttor 3-flgl. Typ 400 m. W-Rahmen, verz. DIN L", TOR,
    FT3, 4, RW, L},
  { 4703, "4703", "Falttor 4-flgl. Typ 400 m. W-Rahmen, verz. DIN R", TOR,
    FT4, 4, RW, R},
  { 4704, "4704", "Falttor 4-flgl. Typ 400 m. W-Rahmen, verz. DIN L", TOR,
    FT4, 4, RW, L},
  { 4705, "4705", "Falttor 5-flgl. Typ 400 m. W-Rahmen, verz. DIN R", TOR,
    FT5, 4, RW, R},
  { 4706, "4706", "Falttor 5-flgl. Typ 400 m. W-Rahmen, verz. DIN L", TOR,
    FT5, 4, RW, L},
  { 5800, "5800", "Schiebetor aus R-Rahmen, verz. mit Helmlaufwerk Nr. 400 u. Zubehör", TOR,
    STOR, 4, RR, R},
  { 5801, "5801", "m. einw. Holzfüllung", FUELLUNG,
    0, 0, 0, 0},
  { 5802, "5802", "m. einw. Holzfüllung", FUELLUNG,
    0, 0, 0, 0},
  { 5803, "5803", "m. einw. Holzfüllung", FUELLUNG,
    0, 0, 0, 0},
  { 5804, "5804", "m. einw. Holzfüllung", FUELLUNG,
    0, 0, 0, 0},
  { 5805, "5805", "ohne Füllung", FUELLUNG,
    0, 0, 0, 0},
  { 5806, "5806", "ohne Füllung", FUELLUNG,
    0, 0, 0, 0},
  { 5807, "5807", "ohne Füllung", FUELLUNG,
    0, 0, 0, 0},
  { 5808, "5808", "ohne Füllung", FUELLUNG,
    0, 0, 0, 0},
  { 5809, "5809", "Aufpr für druckimpr Hol.", ANDERES,
    0, 0, 0, 0},
  { 5810, "5810", "Aufpreis f. Fenster-Einbau", ANDERES,
    0, 0, 0, 0},
  { 5811, "5811", "lfm. Wetterschenkel, doppelt", ANDERES,
    0, 0, 0, 0},
  { 5812, "5812", "lfm. Wetterschenkel, einf.", ANDERES,
    0, 0, 0, 0},
  { 5813, "5813", "Aufpreis f. Schlupftür", ANDERES,
    0, 0, 0, 0},
  { 5815, "5815", "Schiebetür aus R-Rahmen, ver. m. Helm-laufwerk u. Zubehör", TUER,
    STUER, 4, RW, R},
  { 5820, "5820", "außen Beto-Plan, innen Holz", FUELLUNG,
    0, 0, 0, 0},
  { 5821, "5821", "außen Holz, Beto bandseitig", FUELLUNG,
    0, 0, 0, 0},
  { 6101, "6101", "Tür Typ 600 m. Z-Rahmen, verz. DIN R", TUER,
    ATUER, 6, RZ, R},
  { 6102, "6102", "Tür Typ 600 m. W-Rahmen, verz. DIN R", TUER,
    ATUER, 6, RW, R},
  { 6103, "6103", "Tür Typ 600 m. R-Rahmen, verz. DIN R", TUER,
    ATUER, 6, RR, R},
  { 6104, "6104", "Tür Typ 600 S m. Z-Rahmen, verz. DIN R", TUER,
    ATUER, 6, RZ, R},
  { 6201, "6201", "Tür Typ 600 m. Z-Rahmen, verz. DIN L", TUER,
    ATUER, 6, RZ, L},
  { 6202, "6202", "Tür Typ 600 m. W-Rahmen, verz. DIN L", TUER,
    ATUER, 6, RW, L},
  { 6203, "6203", "Tür Typ 600 m. R-Rahmen, verz. DIN R", TUER,
    ATUER, 6, RR, L},
  { 6204, "6204", "Tür Typ 600 S m. Z-Rahmen, verz. DIN R", TUER,
    ATUER, 6, RZ, L},
  { 6301, "6301", "Tor Typ 600 m. Z-Rahmen, verz. DIN R", TOR,
    ATOR, 6, RZ, R},
  { 6302, "6302", "Tor Typ 600 m. W-Rahmen, verz. DIN R", TOR,
    ATOR, 6, RW, R},
  { 6303, "6303", "Tor Typ 600 m. R-Rahmen, verz. DIN R", TOR,
    ATOR, 6, RR, R},
  { 6304, "6304", "Tor Typ 600 S m. Z-Rahmen, verz. DIN R", TOR,
    ATOR, 6, RZ, R},
  { 6401, "6401", "Tor Typ 600 m. Z-Rahmen, verz. DIN L", TOR,
    ATOR, 6, RZ, L},
  { 6402, "6402", "Tor Typ 600 m. W-Rahmen, verz. DIN L", TOR,
    ATOR, 6, RW, L},
  { 6403, "6403", "Tor Typ 600 m. R-Rahmen, verz. DIN L", TOR,
    ATOR, 6, RR, L},
  { 6404, "6404", "Tor Typ 600 S m. Z-Rahmen, verz. DIN L", TOR,
    ATOR, 6, RZ, L},
  { 6501, "6501", "doppelw. Holz (2 x 14 mm)", FUELLUNG,
    0, 0, 0, 0},
  { 6502, "6502", "einwandig Holz (1 x 16 mm)", FUELLUNG,
    0, 0, 0, 0},
  { 6503, "6503", "ohne Füllung", FUELLUNG,
    0, 0, 0, 0},
  { 6504, "6504", "außen Holz, innen Beto-Plan", FUELLUNG,
    0, 0, 0, 0},
  { 6505, "6505", "m. doppelw. Beto-Plan-Füllung", FUELLUNG,
    0, 0, 0, 0},
  { 6506, "6506", "m. einw. Beto-Plan-Füllung", FUELLUNG,
    0, 0, 0, 0},
  { 6507, "6507", "außen Holz, innen Blech", FUELLUNG,
    0, 0, 0, 0},
  { 6508, "6508", "m. doppelw. Blechfüllung", FUELLUNG,
    0, 0, 0, 0},
#if 0
  { 6506, "6506", "m. einw. Beto-Plan-Füllung", FUELLUNG,
    0, 0, 0, 0},
  { 6507, "6507", "außen Holz, innen Blech", FUELLUNG,
    0, 0, 0, 0},
  { 6508, "6508", "m. doppelw. Blechfüllung", FUELLUNG,
    0, 0, 0, 0},
#endif
  { 6509, "6509", "doppelw. Blech + und FH-Isolierung", FUELLUNG,
    0, 0, 0, 0},
  { 6510, "6510", "m. einwandiger Blechfüllung", FUELLUNG,
    0, 0, 0, 0},
  { 6511, "6511", "einwandig Sickenblech", FUELLUNG,
    0, 0, 0, 0},
  { 6512, "6512", "doppelw. Holz, außen aufliegend", FUELLUNG,
    0, 0, 0, 0},
  { 6513, "6513", "einw. Nordische Fichte", FUELLUNG,
    0, 0, 0, 0},
  { 6514, "6514", "m. doppelw. Nord. Fichte", FUELLUNG,
    0, 0, 0, 0},
  { 6515, "6515", "außen Nord. Fichte, i. Beto-Plan", FUELLUNG,
    0, 0, 0, 0},
  { 6516, "6516", "außen Holz, innen Bohlen", FUELLUNG,
    0, 0, 0, 0},
  { 6517, "6517", "m. einw. Holz, außen aufliegend", FUELLUNG,
    0, 0, 0, 0},
  { 6518, "6518", "außen Blech, innen Beto-Plan", FUELLUNG,
    0, 0, 0, 0},
  { 6519, "6519", "außen Holz, innen V2A-Blech", FUELLUNG,
    0, 0, 0, 0},
  { 6520, "6520", "außen Holz, innen Blech+FH-Iso", FUELLUNG,
    0, 0, 0, 0},
  { 6530, "6530", "doppelwandig Holz, m. Styropor", FUELLUNG,
    0, 0, 0, 0},
  { 6531, "6531", "außen Holz, innen Beto-Plan", FUELLUNG,
    0, 0, 0, 0},
  { 6532, "6532", "doppelw. Beto-Plan m. Styropor", FUELLUNG,
    0, 0, 0, 0},
  { 6533, "6533", "außen Holz, innen Blech, m. Styropor", FUELLUNG,
    0, 0, 0, 0},
  { 6534, "6534", "doppelwandig Blech, m. Styropor", FUELLUNG,
    0, 0, 0, 0},
  { 6580, "6580", "m. doppelw. Blechfüllung", FUELLUNG,
    0, 0, 0, 0},
  { 6590, "6590", "außen Beto-Plan, innen Holz", FUELLUNG,
    0, 0, 0, 0},
  { 6591, "6591", "außen Holz, Beto bandseitig", FUELLUNG,
    0, 0, 0, 0},

  { 6701, "6701", "Falttor 3-flgl. Typ 600 m. W-Rahmen, verz. DIN R", TOR,
    FT3, 6, RW, R},
  { 6702, "6702", "Falttor 3-flgl. Typ 600 m. W-Rahmen, verz. DIN L", TOR,
    FT3, 6, RW, L},
  { 6703, "6703", "Falttor 4-flgl. Typ 600 m. W-Rahmen, verz. DIN R", TOR,
    FT4, 6, RW, R},
  { 6704, "6704", "Falttor 4-flgl. Typ 600 m. W-Rahmen, verz. DIN L", TOR,
    FT4, 6, RW, L},
  { 6705, "6705", "Falttor 5-flgl. Typ 600 m. W-Rahmen, verz. DIN R", TOR,
    FT5, 6, RW, R},
  { 6706, "6706", "Falttor 5-flgl. Typ 600 m. W-Rahmen, verz. DIN L", TOR,
    FT5, 6, RW, L},

  { 8101, "8101", "Tür Typ 800 m. Z-Rahmen, verz. DIN R", TUER,
    ATUER, 8, RZ, R},
  { 8102, "8102", "Tür Typ 800 m. W-Rahmen, verz. DIN R", TUER,
    ATUER, 8, RW, R},
  { 8103, "8103", "Tür Typ 800 m. R-Rahmen, verz. DIN R", TUER,
    ATUER, 8 /* früher 6 //?? */, RR, R},
  { 8201, "8201", "Tür Typ 800 m. Z-Rahmen, verz. DIN L", TUER,
    ATUER, 8, RZ, L},
  { 8202, "8202", "Tür Typ 800 m. W-Rahmen, verz. DIN L", TUER,
    ATUER, 8, RW, L},
  { 8203, "8203", "Tür Typ 800 m. R-Rahmen, verz. DIN R", TUER,
    ATUER, 8, RR, L},
  { 8301, "8301", "Tor Typ 800 m. Z-Rahmen, verz. DIN R", TOR,
    ATOR, 8, RZ, R},
  { 8302, "8302", "Tor Typ 800 m. W-Rahmen, verz. DIN R", TOR,
    ATOR, 8, RW, R},
  { 8303, "8303", "Tor Typ 800 m. R-Rahmen, verz. DIN R", TOR,
    ATOR, 8, RR, R},
  { 8401, "8401", "Tor Typ 800 m. Z-Rahmen, verz. DIN L", TOR,
    ATOR, 8, RZ, L},
  { 8402, "8402", "Tor Typ 800 m. W-Rahmen, verz. DIN L", TOR,
    ATOR, 8, RW, L},
  { 8403, "8403", "Tor Typ 800 m. R-Rahmen, verz. DIN L", TOR,
    ATOR, 8, RR, L},
  { 8501, "8501", "doppelw. Holz (2 x 16 mm)", FUELLUNG,
    0, 0, 0, 0},
  { 8502, "8502", "doppelw. Holz, außen aufliegend", FUELLUNG,
    0, 0, 0, 0},
  { 8503, "8503", "ohne Füllung", FUELLUNG,
    0, 0, 0, 0},
  { 8504, "8504", "außen Holz, innen Beto-Plan", FUELLUNG,
    0, 0, 0, 0},
  { 8507, "8507", "außen Holz, innen Blech", FUELLUNG,
    0, 0, 0, 0},
  { 8508, "8508", "doppelw. Blech", FUELLUNG,
    0, 0, 0, 0},
  { 8510, "8510", "außen Beto-Plan, innen Holz", FUELLUNG,
    0, 0, 0, 0},
  { 8511, "8511", "außen Holz, Beto bandseitig", FUELLUNG,
    0, 0, 0, 0},

  { 8701, "8701", "Falttor 3-flgl. Typ 800 m. W-Rahmen, verz. DIN R", TOR,
    FT3, 8, RW, R},
  { 8702, "8702", "Falttor 3-flgl. Typ 800 m. W-Rahmen, verz. DIN L", TOR,
    FT3, 8, RW, L},
  { 8703, "8703", "Falttor 4-flgl. Typ 800 m. W-Rahmen, verz. DIN R", TOR,
    FT4, 8, RW, R},
  { 8704, "8704", "Falttor 4-flgl. Typ 800 m. W-Rahmen, verz. DIN L", TOR,
    FT4, 8, RW, L},
  { 8705, "8705", "Falttor 5-flgl. Typ 800 m. W-Rahmen, verz. DIN R", TOR,
    FT5, 8, RW, R},
  { 8706, "8706", "Falttor 5-flgl. Typ 800 m. W-Rahmen, verz. DIN L", TOR,
    FT5, 8, RW, L}

};



TOData OData[TOMAXDATA] =
{
  { 6601, "6601", "Oberteil z. Öffnen", OzO },
  { 6602, "6602", "Oberteile z. Öffnen", OzO },
  { 6603, "6603", "Oberteile z. Öffnen", OzO },
  { 6604, "6604", "Oberteile i. Flügel", OiF },
  { 6605, "6605", "Oberteile i. Flügel", OiF },
  { 6606, "6606", "Oberlichtfenster"   , OlF },
  { 6607, "6607", "Oberlichtfenster", OlF },
  { 6610, "6610", "Schiebefenster"  , SF  }
};

char strTART[7][50] =
{
  "Tor", "Tür", "F-Tor", "F-Tor",
  "F-Tor", "Schiebetor" };

char strTRAHMEN[3][30] = 
{ "Z-Rahmen", "R-Rahmen", "W-Rahmen" };

char GlasArt[GLASMAXDATA][30] =
{ "Plexi4", "Plexi5", "SDP10", "SDP16", "SDP20", "ISO",
  "Klar", "Draht", "SAN4", "SAN5", "" };

char* TDataScan::getArtBez(tART art)
{
  return strTART[art];
}

char* TDataScan::getRahmenBez(tRAHMEN rahmen)
{
  return strTRAHMEN[rahmen];
}

//liefert den Bezeichner für eine Artikel-Nr
char* TDataScan::getBezeich(int aNr)
{
   for (int i = 0; i<TTMAXDATA; i++)
     if (aNr == tdata[i].artNr)
       return tdata[i].Bezeich;
   return NULL;
}

//liefert eine Liste mit den Artikel-Nr für eine bestimmte
//Artikel-Gruppe
void TDataScan::getListArtNr(CStringArray* list, tkindOf kOf, CString aNr)
{
  int idx = 0;
  while ((tdata[idx].sartNr[0] != aNr[0]) && idx < TTMAXDATA)
     idx++;
  while ((idx < TTMAXDATA) && (tdata[idx].sartNr[0] == aNr[0]))
  {
     if (tdata[idx].kindOf == kOf)
     {
//       char* tmp = new char[5];
	   CString str = tdata[idx].sartNr;
//       strcpy(tmp, tdata[idx].sartNr);
       list->Add(str);
     }

     idx++;
  }
}

void TDataScan::getAllTT(CStringArray* list)
{
  for (int i=0; i<TTMAXDATA; i++)
   if (tdata[i].kindOf == TUER || tdata[i].kindOf == TOR)
   {
//      char* tmp = new char[5];
	  CString str = tdata[i].sartNr;
//      strcpy(tmp, tdata[i].sartNr);
      list->Add(str);
   }
}

void TDataScan::getTTDaten(CTorDoor* pTor, CString sArt)
{
  int idx = 0;
  int iArt = atoi(sArt);

  while (idx < TTMAXDATA && tdata[idx].artNr != iArt)
      idx++;

  if (idx < TTMAXDATA)
  {
    pTor->Art = tdata[idx].Art;
    pTor->Typ = tdata[idx].Typ;
    pTor->RahmenArt = tdata[idx].Rahmen;
    pTor->DIN = tdata[idx].DIN;
    switch(pTor->Art)
    {
      case ATOR:
	pTor->FluegelAnz = 2;
	break;
      case FT3:
	pTor->FluegelAnz = 3;
	break;
      case FT4:
	pTor->FluegelAnz = 4;
	break;
      case FT5:
	pTor->FluegelAnz = 5;
	break;
      default:
	pTor->FluegelAnz = 1;
	break;
    }
  }
}

void TDataScan::getSpecTT(CStringArray* list, tART art)
{
  for (int i=0; i<TTMAXDATA; i++)
   if (tdata[i].kindOf == TUER || tdata[i].kindOf == TOR)
   {
     if (tdata[i].Art == art)
     {
       //char* tmp = new char[5];
	   CString str = tdata[i].sartNr;
       //strcpy((char*)str, tdata[i].sartNr);
       list->Add(str);
     }
   }
}

char* TDataScan::getGlasArt(int Glasart)
{
  if (Glasart < GLASMAXDATA)
    return GlasArt[Glasart];
  else
    return NULL;
}