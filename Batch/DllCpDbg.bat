rem //////////////////////////////////////////////////
rem // Batch-Datei, um alle Dlls vor dem Linken zu Kopieren

xcopy /D /Y .\RegMan.lib .\Debug\*.*
xcopy /D /Y .\RegMan.dll .\Debug\*.*

rem xcopy /D /Y D:\User\Hmo\Projekte\vc++\glpk-4.28\w32\glpk_4_28.lib .\Debug\*.*
rem xcopy /D /Y D:\User\Hmo\Projekte\vc++\glpk-4.28\w32\glpk_4_28.dll .\Debug\*.*
