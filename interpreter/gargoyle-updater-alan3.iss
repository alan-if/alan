[Setup]
AppName=Alan v3.0 Gargoyle slot-in for Windows
AppVerName=Alan interpreter slot-in for Gargoyle vVERSION
AppPublisher=ThoNi Adventure Factories
AppPublisherURL=https://alanif.se
AppSupportURL=https://alanif.se
AppUpdatesURL=https://alanif.se
AllowNoIcons=yes
ChangesAssociations = yes
OutputDir=.
OutputBaseFilename=gargoyle-updater-alanVERSION.win32.i386.setup
WizardImageFile=../setup_wizard.bmp
WizardImageStretch=no
DefaultDirName={code:GargoyleDir}

[Files]
Source: "alan3.exe"; DestDir: "{app}"; Flags: ignoreversion

[Code]
var GargoyleDirectory : String;

function InitializeSetup() : Boolean;
begin
  Result := RegKeyExists(HKEY_LOCAL_MACHINE, 'SOFTWARE\Tor Andersson\Gargoyle')
  if not Result then
    MsgBox('Alan v3 slot-in for Gargoyle requires Gargoyle (http://code.google.com/p/garglk/downloads/list).'#13#13'Could not find Gargoyle in the registry, it seems to not be installed. Installation will now terminate.', mbError, MB_OK);
end;


function GargoyleDir(Param: String): String;
begin
    if RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\Tor Andersson\Gargoyle', 'Directory', GargoyleDirectory) then
      Result := GargoyleDirectory
    else
      begin
        MsgBox('Gargoyle''s directory is not defined in the registry. Aborting.', mbCriticalError, MB_OK);
        Abort();
      end
end;