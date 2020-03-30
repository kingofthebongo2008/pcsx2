@echo off

echo ^<?xml version="1.0" encoding="utf-8"?^> > build_h.msbuild
echo ^<Project DefaultTargets="Build" ToolsVersion="15.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003"^> >> build_h.msbuild

msbuild_generator.exe --input ..\src\ --mode h >> build_h.msbuild

echo ^</Project^> >> build_h.msbuild

echo ^<?xml version="1.0" encoding="utf-8"?^> > build_cpp.msbuild
echo ^<Project DefaultTargets="Build" ToolsVersion="15.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003"^> >> build_cpp.msbuild

msbuild_generator.exe --input ..\src\ --mode cpp >> build_cpp.msbuild

echo ^</Project^> >> build_cpp.msbuild

echo ^<?xml version="1.0" encoding="utf-8"?^> > build.vcxproj.filters
echo ^<Project DefaultTargets="Build" ToolsVersion="15.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003"^> >> build.vcxproj.filters


msbuild_generator.exe --type filters --input ..\src\ --mode h >> build.vcxproj.filters
msbuild_generator.exe --type filters --input ..\src\ --mode cpp >> build.vcxproj.filters

echo ^</Project^> >> build.vcxproj.filters





