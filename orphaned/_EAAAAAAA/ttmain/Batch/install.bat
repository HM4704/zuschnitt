REM
REM     Install für Zuschnitt V1.6
REM

REM Diskette löschen
del /s a:\*.*

REM
REM
REM Verzeichnisse anlegen
set Z_DIR=Zuschnitt16

md a:\%Z_DIR%
md a:\%Z_DIR%\Hilfe
md a:\%Z_DIR%\Zuschnittregeln

REM Programm-Dateien kopieren
xcopy /D .\Release\*.exe   a:\%Z_DIR%\*.*
xcopy /D .\Release\*.dll      a:\%Z_DIR%\*.*

REM Hilfe-Dateien kopieren
xcopy /D .\Hilfe\*.htm   a:\%Z_DIR%\Hilfe\*.*
xcopy /D .\Hilfe\*.gif   a:\%Z_DIR%\Hilfe\*.*
xcopy /D .\Hilfe\*.bmp   a:\%Z_DIR%\Hilfe\*.*

REM Zuschnittregeln-Datei kopieren
xcopy /D .\Zuschnittregeln\*.txt   a:\%Z_DIR%\Zuschnittregeln\*.*
