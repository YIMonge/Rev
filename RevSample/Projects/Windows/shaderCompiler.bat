chcp 65001
chcp

set HLSL_EXT=.hlsl
set SPV_EXT=.spv

setlocal enabledelayedexpansion
for /f "usebackq tokens=*" %%i in (`dir /s /b ..\..\Resources\*_vert.hlsl`) do (
	set fileName=%%i
	set outName=!fileName:%HLSL_EXT%=%SPV_EXT%!
	rem glslangValidator.exe -S vert -e main -o !outName! -V -D !fileName!
	glslc.exe -fshader-stage=vertex !fileName! -o !outName!
)

for /f "usebackq tokens=*" %%i in (`dir /s /b ..\..\Resources\*_frag.hlsl`) do (
	set fileName=%%i
	set outName=!fileName:%HLSL_EXT%=%SPV_EXT%!
	rem glslangValidator.exe -S frag -e main -o !outName! -V -D !fileName!
	glslc.exe -fshader-stage=fragment !fileName! -o !outName!
)

for /f "usebackq tokens=*" %%i in (`dir /s /b ..\..\Resources\*.hlsl.meta`) do (
	set fileName=%%i
	set outName=!fileName:%HLSL_EXT%=%SPV_EXT%!
	copy !fileName! !outName!
)

endlocal


rem pause