cd gyp
call ..\..\tools\gyp\src\gyp.bat --depth=. bunnylod.gyp --generator-output=../test_build -G msvs_version=2013
pause