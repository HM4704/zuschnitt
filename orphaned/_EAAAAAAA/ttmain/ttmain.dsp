# Microsoft Developer Studio Project File - Name="ttmain" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ttmain - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ttmain.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ttmain.mak" CFG="ttmain - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ttmain - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ttmain - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/ttmain", BAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ttmain - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "D:\User\Hmo\Projekte\vc++\Components\RegMan" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Release/RegMan.lib /nologo /subsystem:windows /machine:I386 /out:"Release/zuschnitt.exe"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=Batch\DllCpRls.bat
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ttmain - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "D:\User\Hmo\Projekte\vc++\Components\RegMan" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Debug\RegMan.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/zuschnitt.exe" /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=Batch\DllCpDbg.bat
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "ttmain - Win32 Release"
# Name "ttmain - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Testsystem"

# PROP Default_Filter "*.h;*.cpp"
# Begin Source File

SOURCE=.\Testsystem\TTTestConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Testsystem\TTTestConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\Testsystem\TTTestManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Testsystem\TTTestManager.h
# End Source File
# End Group
# Begin Source File

SOURCE=AuswDlg.cpp
# End Source File
# Begin Source File

SOURCE=ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=EvaluateCircle.cpp
# End Source File
# Begin Source File

SOURCE=EvaluateDoubleCircle.cpp
# End Source File
# Begin Source File

SOURCE=FTor.cpp
# End Source File
# Begin Source File

SOURCE=HtmlHelper.cpp
# End Source File
# Begin Source File

SOURCE=KreisBerechnungDlg.cpp
# End Source File
# Begin Source File

SOURCE=MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=Node.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfilEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfileElemDlg.cpp
# End Source File
# Begin Source File

SOURCE=StatLink.cpp
# End Source File
# Begin Source File

SOURCE=StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=Tordoor.cpp
# End Source File
# Begin Source File

SOURCE=TorN.cpp
# End Source File
# Begin Source File

SOURCE=TorwahlDlg.cpp
# End Source File
# Begin Source File

SOURCE=Ttdata.cpp
# End Source File
# Begin Source File

SOURCE=ttmain.cpp
# End Source File
# Begin Source File

SOURCE=ttmain.rc
# End Source File
# Begin Source File

SOURCE=ttmainDoc.cpp
# End Source File
# Begin Source File

SOURCE=ttmainView.cpp
# End Source File
# Begin Source File

SOURCE=TuerN.cpp
# End Source File
# Begin Source File

SOURCE=ZRBasePage.cpp
# End Source File
# Begin Source File

SOURCE=ZRCBox.cpp
# End Source File
# Begin Source File

SOURCE=ZRProfilePage.cpp
# End Source File
# Begin Source File

SOURCE=ZRSFPage.cpp
# End Source File
# Begin Source File

SOURCE=ZRSheet.cpp
# End Source File
# Begin Source File

SOURCE=ZRTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=ZRTreeEdit.cpp
# End Source File
# Begin Source File

SOURCE=ZRTreeNewDlg.cpp
# End Source File
# Begin Source File

SOURCE=ZuschnittManager.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=AuswDlg.h
# End Source File
# Begin Source File

SOURCE=ChildFrm.h
# End Source File
# Begin Source File

SOURCE=Datatype.h
# End Source File
# Begin Source File

SOURCE=EvaluateCircle.h
# End Source File
# Begin Source File

SOURCE=EvaluateDoubleCircle.h
# End Source File
# Begin Source File

SOURCE=FTor.h
# End Source File
# Begin Source File

SOURCE=HtmlHelper.h
# End Source File
# Begin Source File

SOURCE=KreisBerechnungDlg.h
# End Source File
# Begin Source File

SOURCE=Macros.h
# End Source File
# Begin Source File

SOURCE=MainFrm.h
# End Source File
# Begin Source File

SOURCE=Node.h
# End Source File
# Begin Source File

SOURCE=.\ProfilEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProfileElemDlg.h
# End Source File
# Begin Source File

SOURCE=Resource.h
# End Source File
# Begin Source File

SOURCE=StatLink.h
# End Source File
# Begin Source File

SOURCE=StdAfx.h
# End Source File
# Begin Source File

SOURCE=Tordoor.h
# End Source File
# Begin Source File

SOURCE=TorN.h
# End Source File
# Begin Source File

SOURCE=TorwahlDlg.h
# End Source File
# Begin Source File

SOURCE=Ttdata.h
# End Source File
# Begin Source File

SOURCE=ttmain.h
# End Source File
# Begin Source File

SOURCE=ttmainDoc.h
# End Source File
# Begin Source File

SOURCE=ttmainView.h
# End Source File
# Begin Source File

SOURCE=TuerN.h
# End Source File
# Begin Source File

SOURCE=version.h
# End Source File
# Begin Source File

SOURCE=ZRBasePage.h
# End Source File
# Begin Source File

SOURCE=ZRCBox.h
# End Source File
# Begin Source File

SOURCE=ZRProfilePage.h
# End Source File
# Begin Source File

SOURCE=ZRSFPage.h
# End Source File
# Begin Source File

SOURCE=ZRSheet.h
# End Source File
# Begin Source File

SOURCE=ZRTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=ZRTreeEdit.h
# End Source File
# Begin Source File

SOURCE=ZRTreeNewDlg.h
# End Source File
# Begin Source File

SOURCE=ZuschnittManager.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00011.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00012.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00013.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00014.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00015.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00016.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00017.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00018.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00019.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00020.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00021.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00022.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00023.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00024.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00025.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00026.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00027.bmp
# End Source File
# Begin Source File

SOURCE=.\res\H_point.cur
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ttmain.ico
# End Source File
# Begin Source File

SOURCE=.\res\ttmain.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ttmainDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Ttprog.ico
# End Source File
# Begin Source File

SOURCE=.\res\ttprog1.ico
# End Source File
# Begin Source File

SOURCE=.\res\w_4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\w_5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\w_ja.bmp
# End Source File
# Begin Source File

SOURCE=.\res\w_nein.bmp
# End Source File
# Begin Source File

SOURCE=.\res\w_oif.bmp
# End Source File
# Begin Source File

SOURCE=.\res\w_ozo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\z_wert.bmp
# End Source File
# End Group
# Begin Group "Hilfe"

# PROP Default_Filter "*.htm;*.html;*.gif"
# Begin Source File

SOURCE=.\Hilfe\aenderungen.htm
# End Source File
# Begin Source File

SOURCE=.\Hilfe\Bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\Hilfe\Bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\Hilfe\Bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\Hilfe\circle.dsf
# End Source File
# Begin Source File

SOURCE=.\Hilfe\circle.gif
# End Source File
# Begin Source File

SOURCE=.\Hilfe\Kontakt.htm
# End Source File
# Begin Source File

SOURCE=.\Hilfe\kreisdlg_hilfe.htm
# End Source File
# Begin Source File

SOURCE=.\Hilfe\Z_wert.bmp
# End Source File
# Begin Source File

SOURCE=.\Hilfe\Zr_dlg.dsf
# End Source File
# Begin Source File

SOURCE=.\Hilfe\zr_dlg.gif
# End Source File
# Begin Source File

SOURCE=.\Hilfe\zr_popupmenu.gif
# End Source File
# Begin Source File

SOURCE=.\Hilfe\Zuschnitt_hilfe.htm
# End Source File
# Begin Source File

SOURCE=.\Hilfe\zuschnittregeln_dlg_hilfe.htm
# End Source File
# End Group
# Begin Group "Batch"

# PROP Default_Filter "bat"
# Begin Source File

SOURCE=.\Batch\DllCpDbg.bat
# End Source File
# Begin Source File

SOURCE=.\Batch\DllCpRls.bat
# End Source File
# Begin Source File

SOURCE=.\Batch\install.bat
# End Source File
# End Group
# Begin Group "Testdateien"

# PROP Default_Filter ""
# Begin Group "Referenz"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t4WNFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t4WNFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t4WNNN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t4WOFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t4WOFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t4WONN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t4ZOFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t6WNFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t6WNFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t6WNFSN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t6WNFSN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t6WNNN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t6WNNN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t6WNOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t6WNOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t6WNOZN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t6WNOZN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t6WOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t6WOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t6WOFSN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t6WOFSN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t6WONN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t6WONN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t6WOOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t6WOOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t6WOOZN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f3t6WOOZN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t4WNFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t4WNFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t4WNNN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t4WOFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t4WOFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t4WONN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t4WONN_HI.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WNFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WNFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WNFN_HH_RP.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WNFSN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WNFSN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WNNN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WNNN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WNOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WNOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WNOZN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WNOZN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WOFSN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WOFSN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WONN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WONN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WOOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WOOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WOOZN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f4t6WOOZN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t4WNFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t4WNFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t4WNNN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t4WOFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t4WOFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t4WONN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t6WNFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t6WNFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t6WNFSN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t6WNFSN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t6WNNN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t6WNNN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t6WNOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t6WNOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t6WNOZN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t6WNOZN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t6WOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t6WOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t6WOFSN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t6WOFSN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t6WONN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t6WONN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t6WOOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t6WOOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t6WOOZN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\f5t6WOOZN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4RWNFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4RWNFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4RWNNN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4RWNNN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4RWOFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4RWOFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4RWONN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4RZNFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4RZNFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4RZNNN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4RZOFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4RZOFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4RZONN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4RZONN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4WNFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4WNFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4WNNN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4wnnn_HI.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4WNOZN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4WOFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4WOFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4WONN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4WOOZN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4ZNFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4ZNFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4ZNNN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4ZNOZN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4ZOFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4ZOFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4zonn_hb.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4ZONN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t4ZOOZN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6WNFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6WNFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6WNFSN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6WNFSN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6WNNN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6WNNN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6WNOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6WNOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6WNOZN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6WNOZN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6WOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6WOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6WOFSN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6WOFSN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6WONN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6WONN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6WOOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6WOOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6WOOZN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6WOOZN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZNFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZNFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZNFN_HH_RP.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZNFSN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZNFSN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZNNB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZNNN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZNNN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZNOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZNOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZNOZN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZNOZN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZOFSN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZOFSN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZONN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZONN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZOOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZOOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZOOZN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\t6ZOOZN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\test1.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tor1.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tor2.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tor3.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tor600.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tor600okf.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4RRNNN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4RWNFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4RWNFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4RWOFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4RWOFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4RWONN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4RZNFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4RZNFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4RZNNN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4RZOFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4RZOFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4RZONN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4WNFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4WNFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4WNNN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4WNOZN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4WOFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4WOFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4WONN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4WOOZN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4ZNFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4ZNFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4ZNNN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4ZNOZN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4ZOFN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4ZOFSN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4ZONN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4ZONN_HI.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt4ZOOZN.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WNFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WNFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WNFSN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WNFSN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WNNB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WNNN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WNNN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WNOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WNOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WNOZN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WNOZN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WOFSN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WOFSN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WONN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WONN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WOOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WOOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WOOZN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6WOOZN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZNFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZNFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZNFN_HH_RP.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZNFSN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZNFSN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZNNB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZNNBG_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZNNN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZNNN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZNOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZNOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZNOIFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZNOZN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZNOZN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZOFSN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZOFSN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZONN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZONN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZOOFN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZOOFN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZOOZN_HB.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tt6ZOOZN_HH.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\tuer600.rtt
# End Source File
# Begin Source File

SOURCE=.\Testdateien\Referenz\vssver.scc
# End Source File
# End Group
# Begin Source File

SOURCE=.\Testdateien\Test1.txt
# End Source File
# End Group
# Begin Group "Install"

# PROP Default_Filter ""
# Begin Group "Component Definitions"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Install\Component Definitions\Default.cdf"
# End Source File
# Begin Source File

SOURCE=".\Install\Component Definitions\Default.fgl"
# End Source File
# End Group
# Begin Group "File Groups"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Install\File Groups\Default.fdf"
# End Source File
# Begin Source File

SOURCE=".\Install\File Groups\Help.fgl"
# End Source File
# Begin Source File

SOURCE=".\Install\File Groups\Program Executables.fgl"
# End Source File
# Begin Source File

SOURCE=".\Install\File Groups\ZuschnittRegeln.fgl"
# End Source File
# End Group
# Begin Group "Registry Entries"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Install\Registry Entries\Default.rge"
# End Source File
# End Group
# Begin Group "Script Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Install\Script Files\Setup.dbg"
# End Source File
# Begin Source File

SOURCE=".\Install\Script Files\Setup.ino"
# End Source File
# Begin Source File

SOURCE=".\Install\Script Files\Setup.ins"
# End Source File
# Begin Source File

SOURCE=".\Install\Script Files\Setup.obs"
# End Source File
# Begin Source File

SOURCE=".\Install\Script Files\setup.rul"
# End Source File
# End Group
# Begin Group "Shell Objects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Install\Shell Objects\Default.shl"
# End Source File
# End Group
# Begin Group "String Tables"

# PROP Default_Filter ""
# Begin Group "0009-English"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Install\String Tables\0009-English\value.shl"
# End Source File
# End Group
# Begin Source File

SOURCE=".\Install\String Tables\Default.shl"
# End Source File
# End Group
# Begin Group "Text Substitutions"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Install\Text Substitutions\Build.tsb"
# End Source File
# Begin Source File

SOURCE=".\Install\Text Substitutions\Setup.tsb"
# End Source File
# End Group
# Begin Source File

SOURCE=.\Install\ttmain.Ipr
# End Source File
# End Group
# Begin Source File

SOURCE=ReadMe.txt
# End Source File
# Begin Source File

SOURCE=".\Zuschnittregeln\Zuschnitt-Regeln.txt"
# End Source File
# End Target
# End Project
