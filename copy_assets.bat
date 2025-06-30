@echo off
echo Copying assets to debug build directory...

REM Create directories in x64/Debug if they don't exist
if not exist "x64\Debug\shaders" mkdir "x64\Debug\shaders"
if not exist "x64\Debug\models" mkdir "x64\Debug\models"
if not exist "x64\Debug\textures" mkdir "x64\Debug\textures"

REM Copy shader files
copy "shaders\*.spv" "x64\Debug\shaders\"

REM Copy model files
copy "models\Plane.obj" "x64\Debug\models\"
copy "models\sphere.obj" "x64\Debug\models\"

REM Copy texture files
copy "textures\viking_room.png" "x64\Debug\textures\"

echo Assets copied successfully!
echo You can now run x64\Debug\AtmosphericScattering.exe directly.
pause 