@echo OFF 
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86
devenv ".\SDL2WithLLVM\SDL2WithLLVM.sln" /build Debug 
 