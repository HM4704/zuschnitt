# Microsoft Developer Studio Project File - Name="SysView95" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SysView95 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SysView95.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SysView95.mak" CFG="SysView95 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SysView95 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SysView95 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SysView/SysView95", LDAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SysView95 - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "NO_REGMAN_DLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\SysHook\Release\SysHook.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "SysView95 - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "NO_REGMAN_DLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\SysHook\Debug\SysHook.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SysView95 - Win32 Release"
# Name "SysView95 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ActiveXUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\ApplicationList.cpp
# End Source File
# Begin Source File

SOURCE=.\ApplicationPg.cpp
# End Source File
# Begin Source File

SOURCE=.\chart.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MemUsageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PerformancePg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessList.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessPg.cpp
# End Source File
# Begin Source File

SOURCE=.\RegistryItem.cpp
# End Source File
# Begin Source File

SOURCE=.\RegistryManager.cpp
# End Source File
# Begin Source File

SOURCE=.\RegKeyEx.cpp
# End Source File
# Begin Source File

SOURCE=.\SpawnProcessDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StatisticData.cpp
# End Source File
# Begin Source File

SOURCE=.\StatLink.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SysBasePage.cpp
# End Source File
# Begin Source File

SOURCE=.\SysIntegrationDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SysSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\SysView95.cpp
# End Source File
# Begin Source File

SOURCE=.\SysView95.rc
# End Source File
# Begin Source File

SOURCE=.\ToolHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\WinTop.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ActiveXUtils.h
# End Source File
# Begin Source File

SOURCE=.\ApplicationList.h
# End Source File
# Begin Source File

SOURCE=.\ApplicationPg.h
# End Source File
# Begin Source File

SOURCE=.\chart.h
# End Source File
# Begin Source File

SOURCE=.\ColumnList.h
# End Source File
# Begin Source File

SOURCE=.\Datatype.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MemUsageDlg.h
# End Source File
# Begin Source File

SOURCE=.\PerformancePg.h
# End Source File
# Begin Source File

SOURCE=.\ProcessList.h
# End Source File
# Begin Source File

SOURCE=.\ProcessPg.h
# End Source File
# Begin Source File

SOURCE=.\RegistryItem.h
# End Source File
# Begin Source File

SOURCE=.\RegistryManager.h
# End Source File
# Begin Source File

SOURCE=.\RegKeyEx.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SpawnProcessDlg.h
# End Source File
# Begin Source File

SOURCE=.\StatisticData.h
# End Source File
# Begin Source File

SOURCE=.\StatLink.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SysBasePage.h
# End Source File
# Begin Source File

SOURCE=.\SysIntegrationDlg.h
# End Source File
# Begin Source File

SOURCE=.\SysSheet.h
# End Source File
# Begin Source File

SOURCE=.\SysView95.h
# End Source File
# Begin Source File

SOURCE=.\ToolHelp.h
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.h
# End Source File
# Begin Source File

SOURCE=.\WinTop.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\app.ico
# End Source File
# Begin Source File

SOURCE=.\res\H_point.cur
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00005.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00007.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00008.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\msdos.ico
# End Source File
# Begin Source File

SOURCE=.\res\part_.ico
# End Source File
# Begin Source File

SOURCE=.\res\part_7.ico
# End Source File
# Begin Source File

SOURCE=.\res\SysView95.ico
# End Source File
# Begin Source File

SOURCE=.\res\SysView95.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\KnowHow\WindowList.txt
# End Source File
# End Target
# End Project
# Section SysView95 : {7186232C-CEAC-11D4-8E7B-C3BDF0D38D3C}
# 	2:5:Class:CChart
# 	2:10:HeaderFile:chart.h
# 	2:8:ImplFile:chart.cpp
# End Section
# Section SysView95 : {7186232E-CEAC-11D4-8E7B-C3BDF0D38D3C}
# 	2:21:DefaultSinkHeaderFile:chart.h
# 	2:16:DefaultSinkClass:CChart
# End Section
