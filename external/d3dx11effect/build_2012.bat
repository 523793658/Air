cd gyp
call ..\..\tools\gyp\src\gyp.bat -I dxbase.gypi --depth=. dxbase.gyp --generator-output=../gyp_build -G msvs_version=2012