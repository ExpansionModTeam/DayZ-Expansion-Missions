@echo off

taskkill /F /IM DayZ_x64.exe /T

RD /s /q "storage_-1" > nul 2>&1

cd ../../

start DayZ_x64.exe -mission=.\Missions\ExpansionCOM.Namalsk -nosplash -noPause -noBenchmark -filePatching -doLogs -scriptDebug=true "-mod=!Workshop\@CF;!Workshop\@Community-Online-Tools;!Workshop\@Dabs Framework;!Workshop\@DayZ-Expansion-Bundle;!Workshop\@DayZ-Expansion-Licensed;!Workshop\@Namalsk Survival;!Workshop\@Namalsk Island;"