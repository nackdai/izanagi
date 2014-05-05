#!/bin/bash

build_freetype()
{
	cd ../External/freetype/
	chmod +x autogen.sh
	./autogen.sh
	sh ./configure
	make

	lib=`find ./ -name libfreetype.a`

	if [ -e "./libs" ]; then
		echo "libs exist"
	else
		mkdir libs
	fi

	cp $lib ./libs/
}

build_freeglut()
{
	cd ../External/freeglut
	sh ./configure
	make

	lib=`find ./ -name "lib*.a"`
	so=`find ./ -name "lib*.so*"`

	if [ -e "./libs" ]; then
		echo "libs exist"
	else
		mkdir libs
	fi

	cp $lib ./libs/
	cp $so ./libs/
}

build_glew()
{
	cd ../External/glew

	export SYSTEM=linux
	export GLEW_DEST=./libs
	make all
	make install
}

build_all()
{
	cd $dir
	build_freetype

	cd $dir
	build_freeglut

	cd $dir
	build_glew
}

cur_dir=`pwd`
echo $cur_dir

my_path=`readlink -f $0`
dir=`dirname $my_path`
echo $dir

cd $dir

if [ $1 = "" ] || [ $1 = "all" ]; then
	echo "build all"
	build_all
elif [ $1 = "freetype" ]; then
	echo "build freetype"
	build_freetype
elif [ $1 = "freeglut" ]; then
	echo "build freeglut"
	build_freeglut
elif [ $1 = "glew" ]; then
	echo "build glew"
	build_glew
else
	echo "build all"
	build_all
fi

cd $cur_dir



