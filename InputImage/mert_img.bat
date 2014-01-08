@echo off
cd %~dp0

set MagicTool=%CD%\..\Tools\ImageMagic
set DATAPATH=%CD%\..\bin

del temp.txt>NUL 2>&1
del merge_image.bat>nul 2>&1
del baseTitleData>nul 2>&1
del define.h>nul 2>&1

echo %MagicTool%\montage ^^>>merge_image.bat

set /a fileline=0
set /a defineNum=0
for /f "delims=" %%i in (input.txt) do (
	echo dummy^ dummy^ changeLine>>temp.txt
	CALL:DealImg "%%i" fileline defineNum
)
echo %fileline%
echo -tile x%fileline% -geometry +1+0 -bordercolor none -background none baseTitle.png>>merge_image.bat
call merge_image

set /a lastW=0
set /a xpos=0
set /a ypos=0
set /a maxH=0
set /a xTimes=0
for /f "tokens=3 delims= " %%i in (temp.txt) do (
	CALL:DealSize %%i xpos ypos lastW maxH xTimes
)

del merge_image.bat> nul 2>&1
del temp.txt>NUL 2>&1
%MagicTool%\convert -page 256x256+0+0 baseTitle.png -background none -flatten baseTitle.png
rem del temp.txt>NUL 2>&1
rem del merge_image.bat> nul 2>&1

move /b baseTitle.png %DATAPATH%
move /b baseTitleData %DATAPATH%
EXIT /b

:DealImg
SETLOCAL
	set str=%1
	set /a line=%2
	set /a defNum=%3
	
	set /a line+=1
	set str=%str:"=%
	for %%i in (%str%) do (
		identify %%i>>temp.txt
		CALL:PrintDefine %%i defNum
	)
	echo %str% ^^>>merge_image.bat
ENDLOCAL && SET %2=%line% && SET %3=%defNum%	
EXIT /b

:PrintDefine
SETLOCAL
	set str=%1
	set str=%str:~0,-4%
	set /a num=%2
	set "code=A B C D E F G H I J K L N M O P Q R S T U V W X Y Z"
	for %%i in (%code%) do call,set "str=%%str:%%i=%%i%%"
	echo ^#define^ %str%^ %num% >> define.h
	set /a num+=1
ENDLOCAL && set %2=%num%
EXIT /b

:DealSize
SETLOCAL
	set readLine=%1
	set /a xpos=%2
	set /a ypos=%3
	set /a lastW=%4
	set /a maxH=%5
    set /a xTimes=%6
	set /a width=0
	set /a heigh=0
	
	set isCL="false"
	if "%readLine%" EQU "changeLine" (
		set /a ypos+=maxH
		set /a maxH=0
        set /a xTimes=0
		set isCL="true"
	)
	
	if %isCL% EQU "true" (
		::echo changeLine		
		ENDLOCAL && SET %2=0 && SET %3=%ypos% && SET %4=0 && SET %5=0 && SET %6=0
		EXIT /b	
	)
	
	for /f "tokens=1,2 delims=x" %%a in ("%readLine%") do (
		set /a width=%%a
		set /a heigh=%%b
	)
    set /a xTimes+=1
	set /a xpos+=lastW
    set /a xpos+=1
    set /a xpos_patch=xpos
	set /a lastW=width
	if %maxH% LEQ %heigh% (
		set /a maxH=heigh
	)

    if %xTimes% EQU 3 (
        set /a xpos_patch+=1
    )
	::echo ==============
	::echo x^:%xpos% ?y^:%ypos%
	::echo w^:%width% h^:%heigh%
	::echo ==============
	echo %xpos_patch%^ %ypos%^ %width%^ %heigh%>>baseTitleData
ENDLOCAL && SET %2=%xpos% && SET %3=%ypos% && SET %4=%lastW% && SET %5=%maxH% && SET %6=%xTimes%	
EXIT /b