[Setup]
AppName=FileSend
AppVersion=1.0
DefaultDirName={autopf}\FileSend
DefaultGroupName=FileSend
OutputDir=output
OutputBaseFilename=FileSendSetup
Compression=lzma
SolidCompression=yes

[Files]
Source: "..\..\dist\windows\filesend\*"; DestDir: "{app}"; Flags: recursesubdirs ignoreversion

[Icons]
Name: "{group}\FileSend"; Filename: "{app}\filesend.exe"
Name: "{commondesktop}\FileSend"; Filename: "{app}\filesend.exe"

[Run]
Filename: "{app}\filesend.exe"; Description: "Launch FileSend"; Flags: nowait postinstall skipifsilent
