; Use the 'Modern' UI System

!include MUI2.nsh



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Global Macro Definitions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

!define COMPANY_NAME "Resultra"
!define PRODUCT_NAME "Chart Pattern Finder"


!define EXE_NAME "Chart Pattern Finder.exe"
!define UNINSTALLER_EXE_NAME "Uninstall Chart Pattern Finder.exe"
!define DESKTOP_LINK "$DESKTOP\${PRODUCT_NAME}.lnk"
!define START_MENU_DIR "$SMPROGRAMS\${COMPANY_NAME}"
!define UNINSTALL_REG_KEY_DISPLAY_NAME "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANY_NAME} ${PRODUCT_NAME}"
!define UNINSTALL_REG_KEY_UNINSTALL_STRING "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANY_NAME} ${PRODUCT_NAME}"


!define QTLIBDIR "C:\Qt\5.3\mingw482_32\bin"
!define QTPLATFORMLIBDIR "C:\Qt\5.3\mingw482_32\plugins\platforms"
!define BOOSTLIBDIR "C:\boost_1_56_0\stage\lib"


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Installer Interface Options
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "Chart Pattern Finder License.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Global Configuration Options
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Name "${PRODUCT_NAME}"

; Disable CRC Checking. The installer will be code-signed.
CRCCheck off

; Configure for Modern UI System


OutFile "ChartPatternFinderInstaller.exe"

; The default installation directory

InstallDir "$PROGRAMFILES\${COMPANY_NAME}\${PRODUCT_NAME}"
ShowInstDetails "nevershow"
ShowUninstDetails "nevershow"

; Request "Admin" privileges for Windows Vista/Win7 (required!)
RequestExecutionLevel admin

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Installation Section
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Section "The Application"

SetOutPath "$INSTDIR"


; Files to be extracted to the installation directory.
File "..\..\build-PatternRecognitionDesktop-Desktop_Qt_5_3_MinGW_32bit-Release\release\${EXE_NAME}"


File "${QTLIBDIR}\Qt5Core.dll"
File "${QTLIBDIR}\Qt5Gui.dll"
File "${QTLIBDIR}\Qt5Widgets.dll"
File "${QTLIBDIR}\Qt5OpenGL.dll"
File "${QTLIBDIR}\Qt5PrintSupport.dll"
File "${QTLIBDIR}\Qt5Svg.dll"
File "${QTLIBDIR}\libgcc_s_dw2-1.dll"
File "${QTLIBDIR}\libstdc++-6.dll"
File "${QTLIBDIR}\libwinpthread-1.dll"
File "${QTLIBDIR}\icuin52.dll"
File "${QTLIBDIR}\icuuc52.dll"
File "${QTLIBDIR}\icudt52.dll"


File "${BOOSTLIBDIR}\libboost_date_time-mgw48-mt-1_56.dll"
File "${BOOSTLIBDIR}\libboost_filesystem-mgw48-mt-1_56.dll"
File "${BOOSTLIBDIR}\libboost_system-mgw48-mt-1_56.dll"
File "${BOOSTLIBDIR}\libboost_thread-mgw48-mt-1_56.dll"
File "C:\qwt-6.1.1\lib\qwt.dll"

; Copy the Windows platform plugin into a sub-directory of the installation directory
CreateDirectory $INSTDIR\platforms
SetOutPath $INSTDIR\platforms
File "${QTPLATFORMLIBDIR}\qwindows.dll"


WriteRegStr HKEY_LOCAL_MACHINE "${UNINSTALL_REG_KEY_DISPLAY_NAME}" "DisplayName" "${PRODUCT_NAME} (remove only)"
WriteRegStr HKEY_LOCAL_MACHINE "${UNINSTALL_REG_KEY_UNINSTALL_STRING}" "UninstallString" "$INSTDIR\${UNINSTALLER_EXE_NAME}"

WriteUninstaller "$INSTDIR\${UNINSTALLER_EXE_NAME}"

SectionEnd ; end Application section

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Optional Sections - Start menu and desktop shortcuts
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Section "Start Menu Shortcuts"

CreateDirectory "${START_MENU_DIR}"
CreateShortCut "${START_MENU_DIR}\Launch ${PRODUCT_NAME}.lnk" "$INSTDIR\${EXE_NAME}"
CreateShortCut "${START_MENU_DIR}\Uninstall ${PRODUCT_NAME}.lnk" "$INSTDIR\${UNINSTALLER_EXE_NAME}"

CreateShortCut "${DESKTOP_LINK}" "$INSTDIR\${EXE_NAME}"

SectionEnd ; end the section

Section "Desktop Shortcut"

CreateShortCut "${DESKTOP_LINK}" "$INSTDIR\${EXE_NAME}"

SectionEnd ; end the section

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Uninstall Section
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Section "Uninstall"
 
;Delete Files 
  RMDir /r "$INSTDIR\*.*"    
 
;Remove the installation directory
  RMDir "$INSTDIR"
 
;Delete Start Menu Shortcuts
  Delete "${DESKTOP_LINK}"
  Delete "${START_MENU_DIR}\*.*"
  RmDir  "${START_MENU_DIR}"
 
;Delete Uninstaller And Unistall Registry Entries

DeleteRegKey HKEY_LOCAL_MACHINE "${UNINSTALL_REG_KEY_DISPLAY_NAME}"
DeleteRegKey HKEY_LOCAL_MACHINE "${UNINSTALL_REG_KEY_UNINSTALL_STRING}"  
 
SectionEnd ; end of uninstall section

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Post-Installation or Uninstallation
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Function that calls a messagebox when installation finished correctly
Function .onInstSuccess
  MessageBox MB_OK "You have successfully installed ${PRODUCT_NAME}. Use the start menu or desktop link to launch the program."
FunctionEnd

Function un.onUninstSuccess
  MessageBox MB_OK "You have successfully uninstalled ${PRODUCT_NAME}."
FunctionEnd

