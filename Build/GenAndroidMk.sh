#!/bin/sh

cur_dir=`pwd`
echo $cur_dir

my_path=`readlink -f $0`
dir=`dirname $my_path`
echo $dir

cd $dir

chmod +x GenAndroidMk.pl

./GenAndroidMk.pl Debug_GLUT ../Libs/project/makefile ../Libs/project/android
./GenAndroidMk.pl Debug_GLUT ../Samples/projects/makefile ../Samples/projects/android

cd $cur_dir