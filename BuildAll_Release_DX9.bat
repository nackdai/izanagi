cd Libs
call BuildLibs_Debug_DX9.bat

cd ..\Tools
call BuildExternal_Debug.bat
call BuildTools_Debug.bat

cd ..\Samples
call BuildSamples_Debug_DX9.bat

cd ..