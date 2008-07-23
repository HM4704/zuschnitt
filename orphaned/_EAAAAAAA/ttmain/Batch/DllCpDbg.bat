rem //////////////////////////////////////////////////
rem // Batch-Datei, um alle Dlls vor dem Linken zu Kopieren

xcopy /D e:\User\HMO\projekte\vc++\Components\RegMan\Debug\RegMan.lib .\Debug\*.*
xcopy /D e:\user\hmo\projekte\vc++\Components\RegMan\Debug\RegMan.dll .\Debug\*.*
xcopy /D e:\user\hmo\projekte\vc++\Components\RegMan\Debug\RegMan.pdb .\Debug\*.*
