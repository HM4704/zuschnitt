rem //////////////////////////////////////////////////
rem // Batch-Datei, um alle Dlls vor dem Linken zu Kopieren

xcopy /D /Y d:\User\HMO\projekte\vc++\Components\RegMan\vs2005\Regman\Release\RegMan.lib .\Debug\*.*
xcopy /D /Y d:\user\hmo\projekte\vc++\Components\RegMan\vs2005\Regman\Release\RegMan.dll .\Debug\*.*

rem xcopy /D /Y D:\User\Hmo\Projekte\vc++\glpk-4.28\w32\glpk_4_28.lib .\Debug\*.*
rem xcopy /D /Y D:\User\Hmo\Projekte\vc++\glpk-4.28\w32\glpk_4_28.dll .\Debug\*.*
