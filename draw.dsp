# Microsoft Developer Studio Project File - Name="Draw" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Draw - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "draw.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "draw.mak" CFG="Draw - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Draw - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Draw - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Draw - Win32 Release"

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
F90=df.exe
# ADD BASE F90 /include:"Release/"
# ADD F90 /include:"Release/"
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 gdiplus.lib htmlhelp.lib sgl.lib libmx.lib /nologo /subsystem:windows /incremental:yes /machine:I386
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "Draw - Win32 Debug"

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
F90=df.exe
# ADD BASE F90 /include:"Debug/"
# ADD F90 /include:"Debug/"
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 gdiplus.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Draw - Win32 Release"
# Name "Draw - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=".\undo manager\Action.cpp"
# End Source File
# Begin Source File

SOURCE=.\ArrowPicker.cpp
# End Source File
# Begin Source File

SOURCE=.\ArrowWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BEZIER.CPP
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\CntrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\COLORLIS.CPP
# End Source File
# Begin Source File

SOURCE=.\ColourPicker.cpp
# End Source File
# Begin Source File

SOURCE=.\ColourPopupWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Draw.CPP
# End Source File
# Begin Source File

SOURCE=.\Draw.rc
# End Source File
# Begin Source File

SOURCE=.\DRAWDOC.CPP
# End Source File
# Begin Source File

SOURCE=.\DRAWOBJ.CPP
# End Source File
# Begin Source File

SOURCE=.\DrawProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DRAWTOOL.CPP
# End Source File
# Begin Source File

SOURCE=.\EFFECTDLG.CPP
# End Source File
# Begin Source File

SOURCE=.\FillGradientDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FillPatternDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FillPictureDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\formatba.cpp
# End Source File
# Begin Source File

SOURCE=.\HatchWellWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\IpFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\LINE.CPP
# End Source File
# Begin Source File

SOURCE=.\LinePicker.cpp
# End Source File
# Begin Source File

SOURCE=.\LineProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\LineWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Maction.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\NetworkView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TextFormatPage.cpp
# End Source File
# Begin Source File

SOURCE=.\TextProperty.cpp
# End Source File
# Begin Source File

SOURCE=".\undo manager\UndoManager.cpp"
# End Source File
# Begin Source File

SOURCE=.\Zoomview.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=".\undo manager\Action.h"
# End Source File
# Begin Source File

SOURCE=.\ArrowPicker.h
# End Source File
# Begin Source File

SOURCE=.\ArrowWnd.h
# End Source File
# Begin Source File

SOURCE=.\BEZIER.H
# End Source File
# Begin Source File

SOURCE=.\CntrItem.h
# End Source File
# Begin Source File

SOURCE=.\COLORLIS.H
# End Source File
# Begin Source File

SOURCE=.\ColourPicker.h
# End Source File
# Begin Source File

SOURCE=.\ColourPopupWnd.h
# End Source File
# Begin Source File

SOURCE=.\draw.H
# End Source File
# Begin Source File

SOURCE=.\DRAWDOC.H
# End Source File
# Begin Source File

SOURCE=.\DRAWOBJ.H
# End Source File
# Begin Source File

SOURCE=.\DrawProperty.h
# End Source File
# Begin Source File

SOURCE=.\DRAWTOOL.H
# End Source File
# Begin Source File

SOURCE=.\EffectDlg.h
# End Source File
# Begin Source File

SOURCE=.\FillGradientDlg.h
# End Source File
# Begin Source File

SOURCE=.\FillPatternDlg.h
# End Source File
# Begin Source File

SOURCE=.\FillPictureDlg.h
# End Source File
# Begin Source File

SOURCE=.\formatba.h
# End Source File
# Begin Source File

SOURCE=.\HatchWellWnd.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\IpFrame.h
# End Source File
# Begin Source File

SOURCE=.\LINE.H
# End Source File
# Begin Source File

SOURCE=.\LinePicker.h
# End Source File
# Begin Source File

SOURCE=.\LineProperty.h
# End Source File
# Begin Source File

SOURCE=.\LineWnd.h
# End Source File
# Begin Source File

SOURCE=.\MAction.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl1\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\MyTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\NetworkView.h
# End Source File
# Begin Source File

SOURCE=".\undo manager\ObjectData.h"
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\spline.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TextFormatPage.h
# End Source File
# Begin Source File

SOURCE=.\TextProperty.h
# End Source File
# Begin Source File

SOURCE=".\undo manager\UMResources.h"
# End Source File
# Begin Source File

SOURCE=".\undo manager\UMString.h"
# End Source File
# Begin Source File

SOURCE=".\undo manager\UndoManager.h"
# End Source File
# Begin Source File

SOURCE=.\Zoomview.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\32x32x16_OkBor.ico
# End Source File
# Begin Source File

SOURCE=.\res\Arw02rt.ico
# End Source File
# Begin Source File

SOURCE=.\res\b1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
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

SOURCE=.\res\classview.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00003.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00004.cur
# End Source File
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\Draw.rc2
# End Source File
# Begin Source File

SOURCE=.\res\DrawDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\fileview.bmp
# End Source File
# Begin Source File

SOURCE=.\res\format_t.bmp
# End Source File
# Begin Source File

SOURCE=.\res\hand.cur
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

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\IToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Pencil.cur
# End Source File
# Begin Source File

SOURCE=.\res\resource.bmp
# End Source File
# Begin Source File

SOURCE=.\res\smallico.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\workspc.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\do.txt
# End Source File
# Begin Source File

SOURCE=.\element.txt
# End Source File
# Begin Source File

SOURCE=.\nmr.ini
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
