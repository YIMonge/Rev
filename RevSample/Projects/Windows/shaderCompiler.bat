chcp 65001
chcp

set HLSL_EXT=.hlsl
set SPV_EXT=.spv

setlocal enabledelayedexpansion
for /f "usebackq tokens=*" %%i in (`dir /s /b ..\..\Resources\*_vert.hlsl`) do (
	set fileName=%%i
	set outName=!fileName:%HLSL_EXT%=%SPV_EXT%!

	rem -binding-base can set offset to binding slot of shader resources, I set texture to 10, sampler to 20, ubo to 0
	glslc.exe -fshader-stage=vertex -ftexture-binding-base vert 10 -fsampler-binding-base vert 20 !fileName! -o !outName!
)

for /f "usebackq tokens=*" %%i in (`dir /s /b ..\..\Resources\*_frag.hlsl`) do (
	set fileName=%%i
	set outName=!fileName:%HLSL_EXT%=%SPV_EXT%!
	rem -binding-base can set offset to binding slot of shader resources, I set texture to 10, sampler to 20, ubo to 0
	glslc.exe -fshader-stage=fragment -ftexture-binding-base frag 10 -fsampler-binding-base frag 20 !fileName! -o !outName!
)

for /f "usebackq tokens=*" %%i in (`dir /s /b ..\..\Resources\*.hlsl.meta`) do (
	set fileName=%%i
	set outName=!fileName:%HLSL_EXT%=%SPV_EXT%!
	copy !fileName! !outName!
)

endlocal

rem pause