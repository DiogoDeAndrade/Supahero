# Microsoft Developer Studio Project File - Name="LudiEngine" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LudiEngine - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LudiEngine.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LudiEngine.mak" CFG="LudiEngine - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LudiEngine - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LudiEngine - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LudiEngine - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "d:\code\fmod\api\inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x816 /d "NDEBUG"
# ADD RSC /l 0x816 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "LudiEngine - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LudiEngine___Win32_Debug"
# PROP BASE Intermediate_Dir "LudiEngine___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "D:\Code\fmod\api\inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x816 /d "_DEBUG"
# ADD RSC /l 0x816 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "LudiEngine - Win32 Release"
# Name "LudiEngine - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Source\ldBoundingSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldCamera3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldDrawObj3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldFont.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldFontD3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldGeoFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldGeoFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldGeometry.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldGfx.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldGfxD3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldInputDI.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldKeyboard.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldKeyboardDI.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldLight3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldLight3DD3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldLogger.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldMaterialD3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldMeshD3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldMouse.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldMouseDI.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldObject3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldParticleSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldParticleSystemD3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldSound.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldSoundFMOD.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldSprite2d.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldTextureD3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldTriangulate.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ldUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\LudiEngine.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Headers\ldBoundingSphere.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldCamera3D.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldDrawObj3D.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldFont.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldFontD3D.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldGeoFactory.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldGeoFilter.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldGeometry.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldGfx.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldGfxD3D.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldInputDI.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldKeyboard.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldKeyboardDI.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldLight3d.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldLight3DD3D.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldLogger.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldMaterial.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldMaterialD3D.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldMesh.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldMeshD3D.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldMouse.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldMouseDI.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldObject3d.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldParticleSystem.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldParticleSystemD3D.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldQuaternion.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldSound.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldSoundFMOD.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldSprite2d.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldTexture.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldTextureD3D.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldTriangulate.h
# End Source File
# Begin Source File

SOURCE=.\Headers\ldUtils.h
# End Source File
# Begin Source File

SOURCE=.\Headers\LudiEngine.h
# End Source File
# End Group
# End Target
# End Project
