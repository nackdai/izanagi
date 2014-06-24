SET Emscripten=..\..\..\External\emscripten\emscripten\
SET Version=1.16.0

PATH=%PATH%;%Emscripten%%Version%

IF NOT EXIST ../lib (
    mkdir ../lib
)
