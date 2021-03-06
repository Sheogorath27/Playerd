ECHO OFF
%windir%\System32\sc.exe create PlayerdService binpath= %USERPROFILE%\AppData\Local\Temp\bin\playerd.exe start= auto
pause