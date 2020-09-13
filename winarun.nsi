# WinArun installed script using NSIS
#

OutFile "winarun.setup.exe"

;-------------------------------------------------------------------------------
; Includes
!include "MUI2.nsh"
!include "LogicLib.nsh"
!include "WinVer.nsh"
!include "x64.nsh"

;-------------------------------------------------------------------------------
; Constants
!define PRODUCT_NAME "WinArun VERSION" ; should be <VERSION> and replaced automatically
!define PRODUCT_DESCRIPTION "Alan V3 Interactive Fiction Interpreter for Windows"
!define PRODUCT_VERSION 3.0.7.0 ; should be <WINVER> and replaced automatically
!define COMPANY "AlanIF Adventure Factories"

;-------------------------------------------------------------------------------
; Attributes
Name "${PRODUCT_NAME}"
InstallDir "$PROGRAMFILES\${PRODUCT_NAME}"
InstallDirRegKey HKCU "Software\AlanIF\${PRODUCT_NAME}" ""
;RequestExecutionLevel user ; user|highest|admin

;-------------------------------------------------------------------------------
; Version Info
VIProductVersion "${PRODUCT_VERSION}"
VIAddVersionKey "ProductName" "${PRODUCT_NAME}"
VIAddVersionKey "ProductVersion" "${PRODUCT_VERSION}"
VIAddVersionKey "FileDescription" "${PRODUCT_DESCRIPTION}"
VIAddVersionKey "LegalCopyright" "${COPYRIGHT}"
VIAddVersionKey "FileVersion" "${SETUP_VERSION}"

;-------------------------------------------------------------------------------
; Modern UI Appearance
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP setup_wizard.bmp ;"${NSISDIR}\Contrib\Graphics\Header\orange.bmp"
!define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"
!define MUI_FINISHPAGE_NOAUTOCLOSE

;-------------------------------------------------------------------------------
; Installer Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE COPYING.txt ;"${NSISDIR}\Docs\Modern UI\License.txt"
#!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

;-------------------------------------------------------------------------------
; Uninstaller Pages
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

;-------------------------------------------------------------------------------
; Languages
!insertmacro MUI_LANGUAGE "English"

Section "install"

    SetOutPath "$INSTDIR"

    # Files
    File bin\winarun.exe
    File ..\WindowsGlk-150\Glk.dll
    File ..\WindowsGlk-150\ScaleGfx.dll
    File COPYING.txt
    File regression\saviour.a3c
    File regression\saviour.a3r
    File regression\logo.png
    File games\adventv3\adventv3.a3c
    
    writeUninstaller "$INSTDIR\uninstall.exe"
    
SectionEnd

Section "uninstall"

    Delete "$INSTDIR\uninstall.exe"
    Delete $INSTDIR\winarun.exe
    Delete $INSTDIR\Glk.dll
    Delete $INSTDIR\ScaleGfx.dll
    Delete $INSTDIR\COPYING.txt
    Delete $INSTDIR\saviour.a3c
    Delete $INSTDIR\saviour.a3r
    Delete $INSTDIR\logo.png
    Delete $INSTDIR\adventv3.a3c
    RMDir "$INSTDIR"

SectionEnd
