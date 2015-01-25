#!/bin/sh

cur_dir=`pwd`
echo $cur_dir

my_path=`readlink -f $0`
dir=`dirname $my_path`
echo $dir

cd $dir

chmod +x GenPlarformMk.pl

./GenPlatformMk.pl Debug_GLUT ../Libs/project/makefile ../Libs/project/android android
./GenPlatformMk.pl Debug_GLUT ../Samples/projects/makefile ../Samples/projects/android android

cd $cur_dir