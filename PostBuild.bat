REM This is called after a build to copy the file to the Firefall install and optionaly start up the client
REM Edit this with the path to your Firefall install

REM Dest
SET FIREFALL_INSTALL="F:\Games\Firefall\system\bin"
SET CLIENT_EXE="Firefall TMW Patched.exe"
SET LAUNCH_CLIENT="true"

REM Src
SET SRC_DIR=%1

COPY %SRC_DIR%Version.dll %FIREFALL_INSTALL%\Version.dll
XCOPY /S /E /Y "%SRC_DIR%..\Copy to client bin folder" %FIREFALL_INSTALL%
COPY "%SRC_DIR%..\Copy to client bin folder\Ratchet\RatchetSharp\RatchetSharp.dll" "%FIREFALL_INSTALL%\Ratchet\RatchetSharp\RatchetSharp.dll"
IF %LAUNCH_CLIENT% == "true" ( START "" ""%FIREFALL_INSTALL%\%CLIENT_EXE%"" )

exit 0