@echo off

taskkill /F /IM DayZ_x64.exe /T

RD /s /q "storage_-1" > nul 2>&1

cd ../../

start DayZ_x64.exe -mission=.\Missions\ExpansionCOM.ChernarusPlusGloom -nosplash -noPause -noBenchmark -filePatching -doLogs -scriptDebug=true "-mod=!Workshop\@CF;!Workshop\@Community-Online-Tools;!Workshop\@DayZ-Expansion-;!Workshop\@DayZ-Expansion-Licensed-;!Workshop\@DayZ-Expansion-Vehicles-;!Workshop\@DayZ-Expansion-Core-;"