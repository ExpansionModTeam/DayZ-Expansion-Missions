@echo off

taskkill /F /IM DayZ_x64.exe /T

RD /s /q "storage_-1" > nul 2>&1

cd ../../

start DayZ_x64.exe -mission=.\Missions\ExpansionCOM.TakistanPlus -nosplash -noPause -noBenchmark -filePatching -doLogs -scriptDebug=true "-mod=P:/Mods/@Community-Framework;P:/Mods/@Community-Online-Tools;P:/Mods/@Dabs-Framework;P:/Mods/@DayZ-Expansion-Core;P:/Mods/@DayZ-Expansion;P:/Mods/@DayZ-Expansion-Vehicles;P:/Mods/@DayZ-Expansion-Licensed;P:/Mods/@DayZ-Expansion-Book;P:/Mods/@DayZ-Expansion-Market;P:/Mods/@TakistanPlus"