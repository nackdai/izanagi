SET Emscripten=..\..\..\External\emscripten\emscripten\
SEt Version=1.16.0

PATH=%PATH%;%Emscripten%%Version%

IF NOT EXIST ../lib (
    mkdir ../lib
)
