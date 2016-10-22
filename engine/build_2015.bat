cd gyp
call ..\..\tools\gyp\src\gyp.bat --depth=. engine.gyp --generator-output=../gyp_build -G msvs_version=2015
pause