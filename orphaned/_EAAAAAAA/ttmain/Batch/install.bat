REM
REM     Install für Zuschnitt V1.4
REM


REM Verzeichnisse anlegen
md a:\Zuschnitt
md a:\Zuschnitt\Hilfe
md a:\Zuschnitt\Zuschnittregeln

REM Programm-Dateien kopieren
xcopy /D .\Release\*.exe   a:\Zuschnitt\*.*
xcopy /D .\Release\*.dll      a:\Zuschnitt\*.*

REM Hilfe-Dateien kopieren
xcopy /D .\Hilfe\*.htm   a:\Zuschnitt\Hilfe\*.*
xcopy /D .\Hilfe\*.gif   a:\Zuschnitt\Hilfe\*.*
xcopy /D .\Hilfe\*.bmp   a:\Zuschnitt\Hilfe\*.*

REM Zuschnittregeln-Datei kopieren
xcopy /D .\Zuschnittregeln\*.txt   a:\Zuschnitt\Zuschnittregeln\*.*
