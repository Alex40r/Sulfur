@echo off
:loop
timeout 1 > nul
for %%i in (Shaders.hlsl) do echo %%~ai|find "a">nul || goto :loop
echo file was changed
"%VULKAN_SDK%\Bin\dxc.exe" -Zi -T lib_6_7 -Fo Shaders.spv -spirv .\Shaders.hlsl
echo "Shader compilation finished..."
attrib -a Shaders.hlsl
goto :loop