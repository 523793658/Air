cd gyp
call ..\..\tools\gyp\src\gyp.bat -I basic.gypi -I engine.gypi --depth=. engine.gyp --generator-output=../gyp_build -G msvs_version=2013
pause