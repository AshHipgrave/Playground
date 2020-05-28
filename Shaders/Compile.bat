@ECHO OFF

:Compile
ECHO Compiling shader source code...

C:\VulkanSDK\1.2.135.0\Bin\glslc.exe shader.vert -o vert.spv

IF %ERRORLEVEL% NEQ 0 GOTO Error

C:\VulkanSDK\1.2.135.0\Bin\glslc.exe shader.frag -o frag.spv

IF %ERRORLEVEL% NEQ 0 GOTO Error

:Move
ECHO Copying compiled shaders...

REM Required here when running under Visual Studio
MOVE /Y vert.spv ../PlaygroundGameApp/vert.spv
MOVE /Y frag.spv ../PlaygroundGameApp/frag.spv

IF %ERRORLEVEL% NEQ 0 GOTO Error
GOTO Success

:Error
ECHO Build failed! See output for details.
PAUSE
EXIT

:Success
echo Success!
PAUSE
EXIT