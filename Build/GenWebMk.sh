#!/bin/sh

cur_dir=`pwd`
echo $cur_dir

my_path=`readlink -f $0`
dir=`dirname $my_path`
echo $dir

cd $dir

chmod +x GenPlarformMk.pl

./GenPlarformMk.pl Debug_GLUT ../Libs/project/makefile ../Libs/project/web web
./GenPlarformMk.pl Debug_GLUT ../Samples/projects/makefile ../Samples/projects/web web

cd $cur_dir