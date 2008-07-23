echo off
copy debug\syshook.dll ..\SysView95\*.*
copy debug\syshook.pdb ..\SysView95\*.*

md ..\SysView95\debug
copy debug\syshook.dll ..\SysView95\debug\*.*
copy debug\syshook.pdb ..\SysView95\debug\*.*
