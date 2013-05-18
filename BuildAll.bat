set CONFIG=%1
set GFX=%2

cd Libs
call BuildLibs_%CONFIG%_%GFX%.bat
cd ..

cd Tools
call BuildExternal_%CONFIG%.bat
call BuildTools_%CONFIG%.bat
cd ..

cd Samples
call BuildSamples_%CONFIG%_%GFX%.bat
cd ..