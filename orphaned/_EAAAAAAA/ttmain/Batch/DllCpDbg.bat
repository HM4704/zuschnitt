rem //////////////////////////////////////////////////
rem // Batch-Datei, um alle Dlls vor dem Linken zu Kopieren

xcopy /D /Y d:\User\HMO\projekte\vc++\Components\RegMan\Debug\RegMan.lib .\Debug\*.*
xcopy /D /Y d:\user\hmo\projekte\vc++\Components\RegMan\Debug\RegMan.dll .\Debug\*.*
xcopy /D /Y d:\user\hmo\projekte\vc++\Components\RegMan\Debug\RegMan.pdb .\Debug\*.*

xcopy /D /Y D:\User\Hmo\Projekte\vc++\glpk-4.28\w32\glpk_4_28.lib .\Debug\*.*
xcopy /D /Y D:\User\Hmo\Projekte\vc++\glpk-4.28\w32\glpk_4_28.dll .\Debug\*.*
