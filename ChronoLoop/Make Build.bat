@echo off
echo Loading Visual Studio dev commands.

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat"
devenv ChronoLoop.sln /build "Release|x64"

if not exist _Build (
  mkdir _Build\bin32
  mkdir _Build\bin64
  mkdir _Build\Logs
  mkdir _Build\Resources
)
xcopy /y /d /e "_Release x64" "_Build\bin64"
xcopy /y /d /e "Resources" "_Build\Resources"
echo D | xcopy /y /d /e "Legal" "_Build\Legal"
del /q /s "_Build\bin64\*.ipdb"
del /q /s "_Build\bin64\*.iobj"
del /q /s "_Build\bin64\*.pdb"
del /q /s "_Build\bin64\PDB Files"
del /q /s "_Build\Resources\*.mtl"


echo ////////////////////////////////////
echo.
echo The build is complete.
echo.
echo ////////////////////////////////////
pause