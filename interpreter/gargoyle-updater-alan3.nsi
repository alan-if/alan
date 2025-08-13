# Alan Windows Gargoyle updater script for NSIS
#

Unicode True

OutFile "gargoyle-updater-alanVERSION.win32.i386.setup.exe"

;-------------------------------------------------------------------------------
; Includes
!include "MUI2.nsh"
!include "LogicLib.nsh"
!include "WinVer.nsh"
!include "x64.nsh"

;-------------------------------------------------------------------------------
; Constants
!define PRODUCT_NAME "Alan V3 Gargoyle Updater"
!define PRODUCT_DESCRIPTION "Alan V3 Gargoyle Updater <BUILD>"
!define PRODUCT_VERSION 3.0.0.8 ; Numeric Status (https://en.wikipedia.org/wiki/Software_versioning)
!define COMPANY "AlanIF Adventure Factories"

;-------------------------------------------------------------------------------
; Attributes
Name "${PRODUCT_NAME}"
InstallDir "$PROGRAMFILES\${PRODUCT_NAME}"

;-------------------------------------------------------------------------------
; Version Info
VIProductVersion "${PRODUCT_VERSION}"
VIAddVersionKey "ProductName" "${PRODUCT_NAME}"
VIAddVersionKey "ProductVersion" "${PRODUCT_VERSION}"
VIAddVersionKey "FileDescription" "${PRODUCT_DESCRIPTION}"
VIAddVersionKey "LegalCopyright" "${COPYRIGHT}"
VIAddVersionKey "FileVersion" "${SETUP_VERSION}"

; NOTE: All BMP's need to be in older Windows 3.x bitmap format!!!
; One converter that generates that is https://www.coolutils.com/online/Image-Converter
;-------------------------------------------------------------------------------
; Modern UI Appearance
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "..\images\nsis_setup_header.bmp" ;;"${NSISDIR}\Contrib\Graphics\Header\orange.bmp"
!define MUI_WELCOMEFINISHPAGE_BITMAP "..\images\nsis_setup_wizard.bmp" ;;"${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp!define MUI_HEADERIMAGE_UNBITMAP "..\images\nsis_setup_header.bmp" ;;"${NSISDIR}\Contrib\Graphics\Header\orange.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "..\images\nsis_setup_wizard.bmp" ;;"${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"
!define MUI_FINISHPAGE_NOAUTOCLOSE

!define MUI_WELCOMEPAGE_TITLE "Updating Gargoyle to Alan VERSION"

!define MUI_WELCOMEPAGE_TEXT "This will update your Gargoyle installation to use the Alan interpreter VERSION.$\r$\n$\r$\nClick Next to continue."

!define MUI_FINISHPAGE_TITLE "Gargoyle Updated"
!define MUI_FINISHPAGE_TEXT "Your Gargoyle installation has been updated."

;-------------------------------------------------------------------------------
; Installer Pages
!insertmacro MUI_PAGE_WELCOME
#!insertmacro MUI_PAGE_LICENSE ..\COPYING.txt
#!insertmacro MUI_PAGE_COMPONENTS
#!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

;-------------------------------------------------------------------------------
#; Uninstaller Pages
#!insertmacro MUI_UNPAGE_WELCOME
#!insertmacro MUI_UNPAGE_CONFIRM
#!insertmacro MUI_UNPAGE_INSTFILES
#!insertmacro MUI_UNPAGE_FINISH

;-------------------------------------------------------------------------------
; Languages
!insertmacro MUI_LANGUAGE "English"

Section "Cinstall"

    # Where is Gargoyle?
    ClearErrors
    EnumRegKey $0 HKLM "SOFTWARE\Tor Andersson\Gargoyle" 0
    IfErrors 0 keyexist
        MessageBox MB_OK|MB_ICONSTOP "Gargoyle's directory is not defined in the registry. Is it installed?$\r$\n$\r$\nAborting." IDOK quit
        Abort
keyexist:
    ReadRegStr $0 HKLM "SOFTWARE\Tor Andersson\Gargoyle" "Directory"

    SetOutPath "$0"

    File alan3.exe
    Goto end

quit:
    Abort    

end:
SectionEnd
