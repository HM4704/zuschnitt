REM
REM     Install für Zuschnitt V1.93
REM

set Z_DIR=Zuschnitt193
set Z_DRIVE=e:

REM Diskette löschen
REM del /s %Z_DRIVE%\*.*

REM
REM
REM Verzeichnisse anlegen

md %Z_DRIVE%\%Z_DIR%
md %Z_DRIVE%\%Z_DIR%\Hilfe
md %Z_DRIVE%\%Z_DIR%\Zuschnittregeln

REM Programm-Dateien kopieren
xcopy /D ..\Release\*.exe   %Z_DRIVE%\%Z_DIR%\*.*
xcopy /D ..\Release\*.dll   %Z_DRIVE%\%Z_DIR%\*.*

REM Hilfe-Dateien kopieren
xcopy /D ..\Hilfe\*.htm   %Z_DRIVE%\%Z_DIR%\Hilfe\*.*
xcopy /D ..\Hilfe\*.gif   %Z_DRIVE%\%Z_DIR%\Hilfe\*.*
xcopy /D ..\Hilfe\*.bmp   %Z_DRIVE%\%Z_DIR%\Hilfe\*.*

REM Zuschnittregeln-Datei kopieren
xcopy /D ..\Zuschnittregeln\*.txt   %Z_DRIVE%\%Z_DIR%\Zuschnittregeln\*.*
