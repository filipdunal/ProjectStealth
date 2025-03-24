::@RD /S /Q "%~dp0\Build"
@RD /S /Q "%~dp0\Binaries"

@RD /S /Q "%~dp0\Intermediate\Build"
@RD /S /Q "%~dp0\Intermediate\Staging"

@RD /S /Q "%~dp0\Saved\Cooked"
@RD /S /Q "%~dp0\Saved\StagedBuilds"
@RD /S /Q "%~dp0\Saved\Temp"
@RD /S /Q "%~dp0\Saved\Diff"
@RD /S /Q "%~dp0\Saved\SaveGames"

::Remove Binaries and Intermediate folders in plugins
@echo off
set back=%cd%
FOR /D %%i IN ("%~dp0Plugins\*") DO (
cd "%%i"
@RD /S /Q "%%i\Binaries"
@RD /S /Q "%%i\Intermediate"
cd
)
cd %back%
