#include "stdafx.h"
#include "ttdata.h"

//#include <tcollect.h>
#include <string.h>
#include <stdlib.h>
#include "tordoor.h"

//alle Tor-Daten
#if 0
#define TTMAXDATA 137
#else
#define TTMAXDATA (sizeof(tdata)/sizeof(TTData))
#endif // 0
#define TOMAXDATA 10
#define GLASMAXDATA 13


typedef struct tagProfilData
{
    tProfil ProfilTyp;
    char    strProfilName[10];
} ProfilData;

static ProfilData g_ProfilData[] = 
{
    { RPRR, "RPRR  " }, { RP1087, "RP1087" }, 
    { RP1093, "RP1093" }, { RP1094, "RP1094" }, { RP1348, "RP1348" }, 
    { RP1070, "RP1070" }, { RP1114, "RP1114" }, { RP1071, "RP1071" },
    { RP1163, "RP1163" }, { RP1072, "RP1072" }, { RP1191, "RP1191" },
    { RP1579, "RP1579" }, { RP6_M, "RP_M" },
    { NP, "??????" }, { RP6_8, "RP6_8" }
};

typedef struct tagFuellungsData
{
    tFUELLUNG FuellungTyp;
    char    strFuellungName[100];
} FuellungData;

// diese Reihenfolge nicht ändern!!!!
static FuellungData g_FuellungsArt[] = 
{
    { F_NONE, "", }, {F_HOLZ, "Fichte", }, {F_HOLZ, "Lärche"},
    { F_BETOPLAN, "Beto"}, { F_BETOPLAN, "PP" }, { F_BLECH, "Trapezblech" },
    { F_BETOPLAN, "Isopan Paneele Ral." }, { F_BETOPLAN, "Alpha Paneele Ral." }, { F_BETOPLAN, "Agro grün" }, { F_BETOPLAN, "Agro braun" },
    { F_HOLZ, "Wolfbretter" }, { F_BETOPLAN, "Deko Plan" }
};

TTData tdata[] =
{
  { 4101, "4101", "Tür Typ 400 m. Z-Rahmen, verz. DIN R", TUER,
    ATUER, 4, RZ, R},
  { 4102, "4102", "Tür Typ 400 m. W-Rahmen, verz. DIN R", TUER,
    ATUER, 4, RW, R},
  { 4103, "4103", "Tür Typ 400 m. R-Rahmen, verz. DIN R", TUER,
    ATUER, 4, RR, R},
  { 4104, "4104", "Schiebetür Typ 400, verz. DIN R", TUER,
    STUER, 4, R_OHNE, R},
  { 4201, "4201", "Tür Typ 400 m. Z-Rahmen, verz. DIN L", TUER,
    ATUER, 4, RZ, L},
  { 4202, "4202", "Tür Typ 400 m. W-Rahmen, verz. DIN L", TUER,
    ATUER, 4, RW, L},
  { 4203, "4203", "Tür Typ 400 m. R-Rahmen, verz. DIN L", TUER,
    ATUER, 4, RR, L},
  { 4204, "4204", "Schiebetür Typ 400, verz. DIN L", TUER,
    STUER, 4, R_OHNE, L},
  { 4301, "4301", "Tor Typ 400 m. Z-Rahmen, verz. DIN R", TOR,
    ATOR, 4, RZ, R},
  { 4302, "4302", "Tor Typ 400 m. W-Rahmen, verz. DIN R", TOR,
    ATOR, 4, RW, R},
  { 4303, "4303", "Tor Typ 400 m. R-Rahmen, verz. DIN R", TOR,
    ATOR, 4, RR, R},
  { 4304, "4304", "Schiebetor Typ 400, verz. DIN R", TOR,
    STOR, 4, R_OHNE, R},
  { 4401, "4401", "Tor Typ 400 m. Z-Rahmen, verz. DIN L", TOR,
    ATOR, 4, RZ, L},
  { 4402, "4402", "Tor Typ 400 m. W-Rahmen, verz. DIN L", TOR,
    ATOR, 4, RW, L},
  { 4403, "4403", "Tor Typ 400 m. R-Rahmen, verz. DIN L", TOR,
    ATOR, 4, RR, L},
  { 4404, "4404", "Schiebetor Typ 400, verz. DIN L", TOR,
    STOR, 4, R_OHNE, L},
  { 4501, "4501", "m. doppelw. Holzfüllung", FUELLUNG,
    F_HOLZ, F_HOLZ, 0, 0},
  { 4502, "4502", "m. einw. Holzfüllung", FUELLUNG,
    F_HOLZ, F_NONE, 0, 0},
  { 4503, "4503", "ohne Füllung", FUELLUNG,
    F_NONE, F_NONE, 0, 0},
  { 4504, "4504", "bandseitig Holz, bandgegenseitig Beto-Plan", FUELLUNG,
    F_HOLZ, F_BETOPLAN, 0, 0},
  { 4505, "4505", "m. doppelw. Beto-Plan-Füllung", FUELLUNG,
    F_BETOPLAN, F_BETOPLAN, 0, 0},
  { 4506, "4506", "m. einw. Beto-Plan-Füllung", FUELLUNG,
    F_BETOPLAN, F_NONE, 0, 0},
  { 4507, "4507", "bandseitig Holz, bandgegenseitig Blech", FUELLUNG,
    F_HOLZ, F_BLECH, 0, 0},
  { 4508, "4508", "m. doppelw. Blechfüllung", FUELLUNG,
    F_BLECH, F_BLECH, 0, 0},
  //{ 4509, "4509", "doppelw. Blech+FH-Isolierung", FUELLUNG,
  //  F_BLECH, F_BLECH, 0, 0},
  { 4510, "4510", "m. einw. Blechfüllung", FUELLUNG,
    F_BLECH, F_NONE, 0, 0},
  { 4511, "4511", "m. einw. Holzfüllung bandgegenseitig liegend", FUELLUNG,
    F_HOLZ_I, F_NONE, 0, 0},
  { 4512, "4512", "ohne Füllung, nur Metallrahmen", FUELLUNG,
    F_NONE, F_NONE, 0, 0},
  { 4513, "4513", "bandseitig Holz, bandgegenseitig PP", FUELLUNG,
    F_HOLZ, F_BETOPLAN, 0, 0},
  { 4514, "4514", "m. doppelw. PP-Füllung", FUELLUNG,
    F_BETOPLAN, F_BETOPLAN, 0, 0},
  { 4515, "4515", "m. einw. PP-Füllung", FUELLUNG,
    F_BETOPLAN, F_NONE, 0, 0},

  { 4516, "4516", "doppelw. Holz, bandseitig Wolf", FUELLUNG,
    F_HOLZ, F_HOLZ, 0, 0},
  { 4517, "4517", "bandseitig Holz Wolf, bandgegenseitig Beto", FUELLUNG,
    F_HOLZ, F_BETOPLAN, 0, 0},
  { 4518, "4518", "bandseitig Holz Wolf, bandgegenseitig PP", FUELLUNG,
    F_HOLZ, F_BETOPLAN, 0, 0},

  //{ 4650, "4650", "einwandig Lärche, bandseitig aufliegend", FUELLUNG,
  //  F_HOLZ, F_NONE, 0, 0},
  { 4651, "4651", "einw. Holz m. Fischgrätmuster", FUELLUNG,
    F_HOLZ, F_NONE, 0, 0},
  { 4652, "4652", "doppelw. Holz, bandseitig Fischgrät", FUELLUNG,
    F_HOLZ, F_HOLZ, 0, 0},
  { 4657, "4657", "bandseitig Beto-Plan, bandgegenseitig Holz", FUELLUNG,
    F_BETOPLAN, F_HOLZ, 0, 0},
  { 4658, "4658", "bandseitig Holz, bandgegenseitig Beto", FUELLUNG,
    F_HOLZ, F_BETOPLAN, 0, 0},
  { 4659, "4659", "bandseitig PP, bandgegenseitig Holz", FUELLUNG,
    F_BETOPLAN, F_HOLZ, 0, 0},
  { 4660, "4660", "bandseitig Holz, bandgegenseitig PP", FUELLUNG,
    F_HOLZ, F_BETOPLAN, 0, 0},
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
  { 4703, "4703", "Falttor 3-flgl. Typ 400 m. Z-Rahmen, verz. DIN R", TOR,
    FT3, 4, RZ, R},
  { 4704, "4704", "Falttor 3-flgl. Typ 400 m. Z-Rahmen, verz. DIN L", TOR,
    FT3, 4, RZ, L},
  { 4705, "4705", "Falttor 4-flgl. Typ 400 m. W-Rahmen, verz. DIN R", TOR,
    FT4, 4, RW, R},
  { 4706, "4706", "Falttor 4-flgl. Typ 400 m. W-Rahmen, verz. DIN L", TOR,
    FT4, 4, RW, L},
  { 4707, "4707", "Falttor 4-flgl. Typ 400 m. Z-Rahmen, verz. DIN R", TOR,
    FT4, 4, RZ, R},
  { 4708, "4708", "Falttor 4-flgl. Typ 400 m. Z-Rahmen, verz. DIN L", TOR,
    FT4, 4, RZ, L},
  { 4709, "4709", "Falttor 5-flgl. Typ 400 m. W-Rahmen, verz. DIN R", TOR,
    FT5, 4, RW, R},
  { 4710, "4710", "Falttor 5-flgl. Typ 400 m. W-Rahmen, verz. DIN L", TOR,
    FT5, 4, RW, L},
  { 4711, "4711", "Falttor 5-flgl. Typ 400 m. Z-Rahmen, verz. DIN R", TOR,
    FT5, 4, RZ, R},
  { 4712, "4712", "Falttor 5-flgl. Typ 400 m. Z-Rahmen, verz. DIN L", TOR,
    FT5, 4, RZ, L},
  { 6101, "6101", "Tür Typ 600 m. Z-Rahmen, verz. DIN R", TUER,
    ATUER, 6, RZ, R},
  { 6102, "6102", "Tür Typ 600 m. W-Rahmen, verz. DIN R", TUER,
    ATUER, 6, RW, R},
  { 6103, "6103", "Tür Typ 600 m. R-Rahmen, verz. DIN R", TUER,
    ATUER, 6, RR, R},
  { 6104, "6104", "Tür Typ 600 S m. Z-Rahmen, verz. DIN R", TUER,
    ATUER, 6, RZ, R},
  { 6105, "6105", "Schiebetür Typ 600, verz. DIN R", TUER,
    STUER, 6, R_OHNE, R},
  { 6201, "6201", "Tür Typ 600 m. Z-Rahmen, verz. DIN L", TUER,
    ATUER, 6, RZ, L},
  { 6202, "6202", "Tür Typ 600 m. W-Rahmen, verz. DIN L", TUER,
    ATUER, 6, RW, L},
  { 6203, "6203", "Tür Typ 600 m. R-Rahmen, verz. DIN L", TUER,
    ATUER, 6, RR, L},
  { 6204, "6204", "Tür Typ 600 S m. Z-Rahmen, verz. DIN L", TUER,
    ATUER, 6, RZ, L},
  { 6205, "6205", "Schiebetür Typ 600, verz. DIN L", TUER,
    STUER, 6, R_OHNE, L},
  { 6301, "6301", "Tor Typ 600 m. Z-Rahmen, verz. DIN R", TOR,
    ATOR, 6, RZ, R},
  { 6302, "6302", "Tor Typ 600 m. W-Rahmen, verz. DIN R", TOR,
    ATOR, 6, RW, R},
  { 6303, "6303", "Tor Typ 600 m. R-Rahmen, verz. DIN R", TOR,
    ATOR, 6, RR, R},
  { 6304, "6304", "Tor Typ 600 S m. Z-Rahmen, verz. DIN R", TOR,
    ATOR, 6, RZ, R},
  { 6305, "6305", "Schiebetor Typ 600, verz. DIN R", TOR,
    STOR, 6, R_OHNE, R},
  { 6401, "6401", "Tor Typ 600 m. Z-Rahmen, verz. DIN L", TOR,
    ATOR, 6, RZ, L},
  { 6402, "6402", "Tor Typ 600 m. W-Rahmen, verz. DIN L", TOR,
    ATOR, 6, RW, L},
  { 6403, "6403", "Tor Typ 600 m. R-Rahmen, verz. DIN L", TOR,
    ATOR, 6, RR, L},
  { 6404, "6404", "Tor Typ 600 S m. Z-Rahmen, verz. DIN L", TOR,
    ATOR, 6, RZ, L},
  { 6405, "6405", "Schiebetor Typ 600, verz. DIN L", TOR,
    STOR, 6, R_OHNE, L},
  { 6501, "6501", "doppelw. Holz (2 x 14 mm)", FUELLUNG,
    F_HOLZ, F_HOLZ, 0, 0},
  { 6502, "6502", "einwandig Holz (1 x 16 mm)", FUELLUNG,
    F_HOLZ, F_NONE, 0, 0},
  { 6503, "6503", "ohne Füllung", FUELLUNG,
    F_NONE, F_NONE, 0, 0},
  { 6504, "6504", "bandseitig Holz, bandgegenseitig Beto-Plan", FUELLUNG,
    F_HOLZ, F_BETOPLAN, 0, 0},
  { 6505, "6505", "m. doppelw. Beto-Plan-Füllung", FUELLUNG,
    F_BETOPLAN, F_BETOPLAN, 0, 0},
  { 6506, "6506", "m. einw. Beto-Plan-Füllung", FUELLUNG,
    F_BETOPLAN, F_NONE, 0, 0},
  { 6507, "6507", "bandseitig Holz, bandgegenseitig Blech", FUELLUNG,
    F_HOLZ, F_BLECH, 0, 0},
  { 6508, "6508", "m. doppelw. Blechfüllung", FUELLUNG,
    F_BLECH, F_BLECH, 0, 0},
#if 0
  { 6506, "6506", "m. einw. Beto-Plan-Füllung", FUELLUNG,
    0, 0, 0, 0},
  { 6507, "6507", "bandseitig Holz, bandgegenseitig Blech", FUELLUNG,
    0, 0, 0, 0},
  { 6508, "6508", "m. doppelw. Blechfüllung", FUELLUNG,
    0, 0, 0, 0},
#endif
  //{ 6509, "6509", "doppelw. Blech + und FH-Isolierung", FUELLUNG,
  //  F_BLECH, F_BLECH, 0, 0},
  { 6510, "6510", "m. einwandiger Blechfüllung", FUELLUNG,
    F_BLECH, F_NONE, 0, 0},
  { 6511, "6511", "einwandig Sickenblech", FUELLUNG,
    F_BLECH, F_NONE, 0, 0},
  { 6512, "6512", "doppelw. Holz, bandseitig aufliegend", FUELLUNG,
    F_HOLZ, F_HOLZ, 0, 0},
  //{ 6513, "6513", "einw. Lärche", FUELLUNG,
  //  F_HOLZ, F_NONE, 0, 0},
  //{ 6514, "6514", "m. doppelw. Lärche", FUELLUNG,
  //  F_HOLZ, F_HOLZ, 0, 0},
  { 6515, "6515", "bandseitig Lärche, i. Beto-Plan", FUELLUNG,
    F_HOLZ, F_BETOPLAN, 0, 0},
  { 6516, "6516", "bandseitig Holz, bandgegenseitig Bohlen", FUELLUNG,
    F_HOLZ, F_BOHLEN, 0, 0},
  { 6517, "6517", "m. einw. Holz, bandseitig aufliegend", FUELLUNG,
    F_HOLZ, F_NONE, 0, 0},

  //{ 6518, "6518", "bandseitig Blech, bandgegenseitig Beto-Plan", FUELLUNG,
  //  F_BLECH, F_BETOPLAN, 0, 0},
  { 6519, "6519", "bandseitig Holz, bandgegenseitig V2A-Blech", FUELLUNG,
    F_HOLZ, F_BLECH, 0, 0},
  //{ 6520, "6520", "bandseitig Holz, bandgegenseitig Blech+FH-Iso", FUELLUNG,
  //  F_HOLZ, F_BLECH, 0, 0},

  { 6521, "6521", "bandseitig Holz, bandgegenseitig PP", FUELLUNG,
    F_HOLZ, F_BETOPLAN, 0, 0},
  { 6522, "6522", "m. doppelw. PP-Füllung", FUELLUNG,
    F_BETOPLAN, F_BETOPLAN, 0, 0},
  { 6523, "6523", "m. einw. PP-Füllung", FUELLUNG,
    F_BETOPLAN, F_NONE, 0, 0},
  { 6524, "6524", "bandseitig Blech, bandgegenseitig PP", FUELLUNG,
    F_BLECH, F_BETOPLAN, 0, 0},

  //{ 6525, "6525", "doppelw. Holz, bandseitig Wolf", FUELLUNG,
  //  F_HOLZ, F_HOLZ, 0, 0},
  { 6526, "6526", "bandseitig Holz Wolf, bandgegenseitig Beto", FUELLUNG,
    F_HOLZ, F_BETOPLAN, 0, 0},
  { 6527, "6527", "bandseitig Holz Wolf, bandgegenseitig PP", FUELLUNG,
    F_HOLZ, F_BETOPLAN, 0, 0},

  //{ 6530, "6530", "doppelw. Holz, m. Styropor", FUELLUNG,
  //  F_HOLZ, F_HOLZ, 0, 0},
  //{ 6531, "6531", "bandseitig Holz, bandgegenseitig Beto-Plan + Styrop.", FUELLUNG,
  //  F_HOLZ, F_BETOPLAN, 0, 0},
  //{ 6532, "6532", "doppelw. Beto-Plan m. Styropor", FUELLUNG,
  //  F_BETOPLAN, F_BETOPLAN, 0, 0},
  { 6533, "6533", "bandseitig Holz, bandgegenseitig Blech, m. Styropor", FUELLUNG,
    F_HOLZ, F_BLECH, 0, 0},
  { 6534, "6534", "doppelwandig Blech, m. Styropor", FUELLUNG,
    F_BLECH, F_BLECH, 0, 0},

  //{ 6535, "6535", "bandseitig Holz, bandgegenseitig PP + Styrop.", FUELLUNG,
  //  F_HOLZ, F_BETOPLAN, 0, 0},
  //{ 6536, "6536", "doppelw. PP m. Styropor", FUELLUNG,
  //  F_BETOPLAN, F_BETOPLAN, 0, 0},

  { 6537, "6537", "Stenger-Paneeele-Füllung auß. RAL", FUELLUNG,
    F_NONE, F_NONE, 0, 0},

  //{ 6580, "6580", "m. doppelw. Blechfüllung", FUELLUNG,
  //  F_BLECH, F_BLECH, 0, 0},
  //{ 6590, "6590", "bandseitig Beto-Plan, bandgegenseitig Holz", FUELLUNG,
  //  F_BETOPLAN, F_HOLZ, 0, 0},
  { 6591, "6591", "bandseitig Holz, Beto bandseitig", FUELLUNG,
    F_HOLZ, F_BETOPLAN /* ?? */, 0, 0},

  { 6592, "6592", "bandseitig PP, bandgegenseitig Holz", FUELLUNG,
    F_BETOPLAN, F_HOLZ, 0, 0},
  { 6593, "6593", "bandseitig Holz, PP bandseitig", FUELLUNG,
    F_HOLZ, F_BETOPLAN /* ?? */, 0, 0},

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


////////////////////////////////////////
// Erfahrungswerte für Zuschnitt Holz-Bretter
HolzZuschnitt hzData[] =
{
    {  420,  2, 102 }, {  470,  3,  70 }, {  520,  3, 950 }, 
    {  545,  3, 108 }, {  565,  4,  63 }, {  570,  4,  65 },
    {  620,  4,  90 }, {  670,  5, 580 }, {  710,  5,  80 },
    {  760,  5, 105 }, {  810,  6,  73 }, {  860,  6,  98 },
    {  910,  7,  68 }, {  960,  7,  95 }, {  990,  7, 108 },
    { 1010,  8,  62 }, { 1060,  8,  87 }, { 1110, 10, 111 },
    { 1120,  9,  63 }, { 1135,  9,  68 }, { 1160,  9,  82 },
    { 1210,  9, 107 }, { 1260, 10,  77 }, { 1295, 10,  97 },
    { 1310, 10, 103 }, { 1340, 11,  64 }, { 1370, 11,  77 },
    { 1390, 11,  89 }, { 1420, 11, 102 }, { 1445, 11, 111 },
    { 1470, 12,  70 }, { 1495, 12,  82 }, { 1520, 12,  96 },
    { 1540, 12, 108 }, { 1570, 13,  65 }, { 1595, 13,  77 },
    { 1620, 13,  90 }, { 1645, 13, 102 }, { 1665, 13, 110 },
    { 1670, 13, 111 }, { 1695, 14,  72 }, { 1720, 14,  85 },
    { 1770, 14, 111 }, { 1870, 15, 105 }, { 1920, 16,  74 },
    { 1970, 16,  98 }, { 2050, 17,  83 }, { 2160, 18,  82 }
};

char strTART[7][50] =
{
  "Tor", "Tür", "F-Tor", "F-Tor",
  "F-Tor", "S-Tür", "S-Tor"};

char strTRAHMEN[3][30] = 
{ "Z-Rahmen", "R-Rahmen", "W-Rahmen" };

char GlasArt[GLASMAXDATA][30] =
{ "VSG", "Plexi4", "Plexi5", "Plexi6", "SDP10", "SDP16", "SDP20", "ISO",
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

int TDataScan::getTorArt(char* strArtikel)
{
  int idx = 0;
  int iArt = atoi(strArtikel);

  while (idx < TTMAXDATA && tdata[idx].artNr != iArt)
      idx++;

  if (idx < TTMAXDATA)
  {
    return (int)tdata[idx].Art;
  }

  return -1;
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
      case STOR:
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


// Function name	: TDataScan::getPtrToArtikel
// Description	    : liefert Zeiger auf einen Artikel-Eintrag
// Return type		: TTData* 
// Argument         : int iNrArtikel
TTData* TDataScan::getPtrToArtikel(int iNrArtikel)
{
   for (int i = 0; i<TTMAXDATA; i++)
     if (iNrArtikel == tdata[i].artNr)
       return &tdata[i];
   return NULL;
}


// Function name	: TDataScan::getFuellung
// Description	    : liefert die Fuellung für Innen und Aussen anhand der Artikel-Nr
// Return type		: BOOL                     FALSE, nicht gefunden
// Argument         : int iNrFuellung          Artikel-Nummer
// Argument         : tFUELLUNG* pfAussen
// Argument         : tFUELLUNG* pfInnen
BOOL TDataScan::getFuellung(int iNrFuellung, tFUELLUNG* pfAussen, tFUELLUNG* pfInnen)
{
    *pfAussen = *pfInnen = F_NONE;

    if (iNrFuellung > 0 && iNrFuellung < 4000)
    {
        *pfAussen = (tFUELLUNG)getEnumForFuellung(iNrFuellung&0x0f);
        *pfInnen = (tFUELLUNG)getEnumForFuellung((iNrFuellung&0x0f0)>>4);
        return TRUE;
    }
    else
    {
        TTData* pData = getPtrToArtikel(iNrFuellung);
        if (pData == NULL || (tkindOf)pData->kindOf != FUELLUNG)
            return FALSE;

        if (pData != NULL)
        {
            *pfAussen = (tFUELLUNG)pData->Art;
            *pfInnen = (tFUELLUNG)pData->Typ;
            return TRUE;
        }
        else
        {
        }
    }
    return FALSE;
}

int TDataScan::getFuellungIndex(int iNrFuellung, bool bAussen)
{
    if (iNrFuellung > 0 && iNrFuellung < 4000)
    {
        if (bAussen)
            return iNrFuellung&0x0f;
        else
            return ((iNrFuellung&0x0f0)>>4);
    }
    return 0;
}

BOOL TDataScan::getHolzZuschnitt(int iLenRiegel, HolzZuschnitt** ppHzE)
{
    int iMaxHZ = sizeof(hzData)/sizeof(HolzZuschnitt);

    for (int i=0; i < iMaxHZ; i++)
    {
        if (iLenRiegel == hzData[i].iLenRiegel)
        {
            *ppHzE = &hzData[i];
            return TRUE;
        }
    }
    *ppHzE = NULL;
    return FALSE;
}


char* TDataScan::getNameForProfil(tProfil prfl)
{
    for (int i=0; i<sizeof(g_ProfilData)/sizeof(ProfilData); i++)
    {
        if (g_ProfilData[i].ProfilTyp == prfl)
            return g_ProfilData[i].strProfilName;
    }
    return "??????";
}


const char* TDataScan::getNameForFuellung(int index)
{
    if (index < sizeof(g_FuellungsArt)/ sizeof(FuellungData))
        return g_FuellungsArt[index].strFuellungName;
    return "??????";
}

int TDataScan::getEnumForFuellung(int index)
{
    if (index < sizeof(g_FuellungsArt)/ sizeof(FuellungData))
        return g_FuellungsArt[index].FuellungTyp;
    return F_NONE;
}

tProfil TDataScan::getProfilForName(char* szName)
{
    for (int i=0; i<sizeof(g_ProfilData)/sizeof(ProfilData); i++)
    {
        if (strncmp(g_ProfilData[i].strProfilName, szName, 
            strlen(g_ProfilData[i].strProfilName)) == 0)
            return g_ProfilData[i].ProfilTyp;
    }
    return NP;
}

static const char TorEinbau[TEB_MAX][30] =
{ "", "Mauer", "Beton", "Holz" };

const char* TDataScan::getTorEinbau(int teb)
{
    return TorEinbau[teb];
}

static const char TorFeststellung[TFS_MAX][30] =
{ "", "Torhaken", "Feststeller 40", "Feststeller 60", "Feststeller 80" };

const char* TDataScan::getTorFeststellung(int tfs)
{
    return TorFeststellung[tfs];
}

static const char BandWerte[BAND_MAX][20] = 
{ "", "14", "16", "16-80" };

const char* TDataScan::getBand(int band)
{
    return BandWerte[band];
}

static const char Klappgriffe[KLAPPG_MAX][20] = 
{ "", "innen", "aussen", "beidseitig" };

const char* TDataScan::getKlappgriff(int klapp)
{
    return Klappgriffe[klapp];
}

static const char Schwellen[SCHWELLE_MAX][20] = 
{ "", "100/50/3", "100/50/4" };

const char* TDataScan::getSchwelle(int schwelle)
{
    return Schwellen[schwelle];
}
