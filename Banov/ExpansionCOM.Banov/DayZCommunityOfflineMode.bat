@echo off

taskkill /F /IM DayZ_x64.exe /T

RD /s /q "storage_-1" > nul 2>&1

cd ../../

start DayZ_x64.exe -mission=.\Missions\ExpansionCOM.Banov -nosplash -noPause -noBenchmark -filePatching -doLogs -scriptDebug=true "-mod=!Workshop\@CF;!Workshop\@Dabs Framework;!Workshop\@Community-Online-Tools;!Workshop\@DayZ-Expansion-Bundle;!Workshop\@DayZ-Expansion-Licensed;!Workshop\@Banov"