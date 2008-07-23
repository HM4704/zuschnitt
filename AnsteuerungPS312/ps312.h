#pragma once

////////////////////////////////////////////
// enthält Daten des Steuergeräts PS312


////////////////////////////////////////////
/// Ebenen für Daten Parameter
#define PS312_NUM_LEVEL      2      // 2 Ebenen
#define PS312_LEVEL_ALL      20
#define PS312_LEVEL_AXIS1    21
#define PS312_LEVEL_SOLLW    40     // Sollwert
#define PS312_LEVEL_COUNT    50     // Stückzahl     

////////////////////////////////////////////
// Parameter
#define PS312_MAX_PARAMS    100
#define PS312_MIN_PARAM     0

// Defines für verwendete Parameter
#define PS312_PARAM_COUNT_PROGS      60         // Anzahl der Programm (Parameterebene  ALL)
#define PS312_PARAM_ACTIVE_PROG_NR   62         // Setzen der Programmnummer (Parameterebene  ALL)
#define PS312_PARAM_MASS_STATUS      63         // Sollwertstatus (Absolut-, Kettenmass)

////////////////////////////////////////////
// Defines für Befehle
#define PS312_PARAM_CMD             52

#define PS312_CMD_START_AXIS        0x80        // Start Achse 
#define PS312_CMD_STOP_AXIS         0x81        // Stop Achse 
#define PS312_CMD_START_REF         0x88        // Start Referenzfahrt
#define PS312_CMD_ACTIVATE_DATA     0x89        // Daten aktivieren
#define PS312_CMD_SAVE_2_EEPROM     0x8A        // Daten in EEProm speichern
#define PS312_CMD_START_PROG_LOAD   0x8C        // Programm Download beginnen
#define PS312_CMD_STOP_PROG_LOAD    0x8D        // Programm Download, Abarbeitung beendet
#define PS312_CMD_SET_ABS_MASS      0x8E        // Absolutmaß
#define PS312_CMD_SET_KETT_MASS     0x8F        // Kettenmaß
#define PS312_CMD_START_PROG        0x90        // Programm Starten
#define PS312_CMD_ARM_FWDBCK        0x91        // Arm vor/rück
#define PS312_CMD_ARM_UPDWN         0x92        // Arm auf/ab
#define PS312_CMD_MIN   PS312_CMD_START_AXIS
#define PS312_CMD_MAX   PS312_CMD_ARM_UPDWN

////////////////////////////////////////////
// Defines für Status
#define PS312_PARAM_STATUS            80

#define PS312_STATUS_AXIS_NOT_RDY   0x00        // Achse nicht betriebsbereit
#define PS312_STATUS_AXIS_READY     0x01        // Achse betriebsbereit
#define PS312_STATUS_AXIS_MOVING    0x02        // Achse fährt
#define PS312_STATUS_AXIS_IN_POS    0x04        // Achse in Position
#define PS312_STATUS_REF_ACTIVE     0x08        // Referenzfahrt aktiv
#define PS312_STATUS_CURRENT_EXCD   0x10        // Überstrom
#define PS312_STATUS_KURZSCHLUSS    0x20        // Kurzschluss
#define PS312_STATUS_ERR_MEASURE    0x40        // Fehler am Meßsystem
#define PS312_STATUS_ENDL_UEBERS    0x80        // Endlagenüberschreitung (Bei Referenzfahrt, Hardwareendlagen)
#define PS312_STATUS_PROG_ACTIVE    0x100       // Programm Abarbeitung aktiv

//////////////////////////////////////////////
// Defines für Parameter zur Programmsteuerung
#define PS312_DATA_SET_COUNT        3000        // Gesamtanzahl an Datensätzen
#define PS312_DATA_SET_COUNT_PER_PROG  999      // max. Anzahl nzahl an Datensätzen pro Programm
#define PS312_MAX_PROGRAMS          99          // maximal Anzahl an Programmen

//////////////////////////////////////////////
// Strukturen
typedef enum {MASS_ABSOLUT = 0, MASS_KETTE} eMass;
typedef struct _PS312_DATA_SET
{
    int iLength;
    int iCount;
    eMass mass;
} PS312_DATA_SET;
