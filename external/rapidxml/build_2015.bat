cd gyp
call ..\..\..\tools\gyp\src\gyp.bat -D target_arch=x64 --depth=. rapidxml.gyp --generator-output=../gyp_build -G msvs_version=2015
pause