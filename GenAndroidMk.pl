#!/usr/bin/perl

use strict;
use constant true => 1;
use constant false => 0;

if (@ARGV != 3) {
	# Print Usage.
	print "GenAndroidMK.pl [configure] [src directory] [dst directory]\n";
	print " ex) \$./GenAndroidMK.pl Debug Libs/project/makefile Lib/project/android\n";
	exit();
}

my $baseconfig = $ARGV[0];
my $targetsrc = $ARGV[1];
my $targetdst = $ARGV[2];

unless (-d $targetdst) {
	mkdir($targetdst);
}

opendir(SRC, $targetsrc) or die "Can't open $targetsrc\n";

my @makefiles = readdir(SRC);

closedir(SRC);

my $libs_tag = $baseconfig . "_Libraries";
my $incs_tag = $baseconfig . "_Include_Path";
my $defnition_tag = $baseconfig . "_Preprocessor_Definitions";

my %projects;

foreach my $srcmk (@makefiles) {
	if ($srcmk eq "." || $srcmk eq "..") {
		next;
	}

	if ($srcmk !~ /[a-zA-Z0-9]\.makefile/) {
		next;
	}

	my $name = (split(/\./, $srcmk))[0];

	my $srcmk = $targetsrc . "/" . $srcmk;

	print("$srcmk\n");

	open(IN, $srcmk) or die "Can't open $srcmk\n";

	my @targets = ();
	my @srcfiles = ();
	my @definitions = ();
	my @libraries = ();
	my @includes = ();

	my $is_shared = false;

	while (my $line = <IN>) {
		$line =~ s/\r//;
		$line =~ s/\n//;

		if ($line =~ /\.exe/) {
			#TODO
			$is_shared = true;
		}
		elsif ($line =~ /$baseconfig: /) {
			my @files = split(/ /, $line);
			foreach my $file (@files) {
				unless ($file =~ /\.o/) {
					next;
				}
				push(@targets, $file);
			}
		}
		elsif ($line =~ /$defnition_tag=/) {
			my $defs_tmp = (split(/=/, $line))[1];
			$defs_tmp =~ s/-D //g;
			my @defs = split(/ /, $defs_tmp);

			foreach my $def (@defs) {
				if ($def =~ /GLUT/ || $def =~ /GCC_BUILD/) {
					next;
				}
				push(@definitions, $def);
			}
		}
		elsif ($line =~ /$libs_tag=/) {
			my $libs_tmp = (split(/=/, $line))[1];
			my @libs = split(/ /, $libs_tmp);

			foreach my $lib (@libs) {
				if ($lib =~ /-l([a-zA-Z0-9]*)/) {
					$lib = $1;
					if ($lib ne "glut"
						&& $lib ne "GLU"
						&& $lib ne "GLEW"
						&& $lib ne "GL"
						&& $lib ne "X11"
						&& $lib ne "Xxf86vm"
						&& $lib ne "Xi")
					{
						push(@libraries, $lib);
					}
				}
			}
		}
		elsif ($line =~ /$incs_tag=/) {
			my $inc_tmp = (split(/=/, $line))[1];
			my @incs = split(/ /, $inc_tmp);

			foreach my $inc (@incs) {
				if ($inc =~ /-I\"([\.\/a-zA-Z0-9]*)/) {
					$inc = $1;
					if ($inc !~ /glew/
						&& $inc !~ /freeglut/
						&& $inc !~ /zlib/)
					{
						push(@includes, $inc);
					}
				}
			}
		}
		else {
			foreach my $target (@targets) {
				if ($line =~ /$target: /) {
					my $srcfile = (split(/ /, $line))[1];
					push(@srcfiles, $srcfile);
				}
			}
		}
	}

	close(IN);

	$projects{$name} = $is_shared;

	if ($is_shared) {
		MakeAndroidMk_SharedLib($targetdst, $name, \@libraries, \@includes, \@definitions, \@srcfiles);
	}
	else {
		MakeAndroidMk_StaticLib($targetsrc, $targetdst, $name, \@libraries, \@includes, \@definitions, \@srcfiles);
	}

	MakeApplicationMk($targetdst, $name);
}

MakeMakefile($baseconfig, $targetdst, \%projects);

#==========================

sub GetRelativePath
{
	my $basedir = shift;
	my $path = shift;

	chomp($path);
	chomp($path);

	my @basedir_array = split(/\//, $basedir);
	my @path_array = split(/\//, $path);

	if ($path_array[0] eq ".") {
		splice(@path_array, 0, 1);
	}

	my $basedir_cnt = @basedir_array;
	my $path_cnt = @path_array;

	my $cnt = ($basedir_cnt < $path_cnt ? $basedir_cnt : $path_cnt);
	my $num = 0;

	for (my $i = $cnt - 1; $i >= 0; $i--) {
		if ($basedir_array[$i] eq $path_array[$i]) {
			last;
		}
		$num++;
	}

	my $ret = "";

	if ($num < $cnt) {
		$ret = "..";
		for (my $i = 1; $i < $num; $i++) {
			$ret .= "/..";
		}

		for (my $i = $num - 1; $i < @path_array; $i++) {
			$ret .= "/$path_array[$i]";
		}

		return $ret;
	}

	return $path;
}

sub MakeAndroidMk_StaticLib
{
	my $targetsrc = shift;
	my $targetdst = shift;
	my $name = shift;
	my $libraries_array_ref = shift;
	my $includes_array_ref = shift;
	my $definitions_array_ref = shift;
	my $srcfiles_array_ref = shift;

	my $dstmk = $targetdst . "/Android_" . $name . ".mk";

	open(OUT, ">$dstmk") or die "Can't open $dstmk\n";

	print OUT "LOCAL_PATH:= \$(call my-dir)\n";
	print OUT "\n";

	print OUT "include \$(CLEAR_VARS)\n";
	print OUT "\n";

	print OUT "LOCAL_MODULE     := lib$name\n";

	{
		print OUT "LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__";

		foreach my $def (@$definitions_array_ref) {
			print OUT " -D$def";
		}

		print OUT "\n";
	}

	{
		print OUT "LOCAL_C_INCLUDES :=";

		foreach my $inc (@$includes_array_ref) {
			print OUT " \$(LOCAL_PATH)/" . $inc;
		}

		print OUT "\n";
	}

	{
		print OUT "LOCAL_SRC_FILES  := ";

		my $pos = 0;

		my $srcfiles_cnt = @$srcfiles_array_ref - 1;

		foreach my $src (@$srcfiles_array_ref) {
			if ($src =~ /(glut)/ || $src =~ /(OGL)/) {
				my @tmp_array = split(/\//, $src);
				$src = $tmp_array[@tmp_array - 1];
				$src =~ s/$1/android/;

				my @fullpath = `find . -name $src`;
				if (@fullpath == 1) {
					$src = GetRelativePath($targetsrc, $fullpath[0]);
				}
				else {
					# TODO
					next;
				}
			}

			print OUT "$src";
			if ($pos != $srcfiles_cnt) {
				print OUT " \\";
			}
			print OUT "\n";

			$pos++;
		}

		print OUT "\n";
	}

	print OUT "include \$(BUILD_STATIC_LIBRARY)\n";

	close(OUT);
}

sub WritePrebuildStaticLib
{
	my $OUT = shift;
	my $name = shift;
	my $is_local = shift;
	
	print $OUT "include \$(CLEAR_VARS)\n";
	print $OUT "LOCAL_MODULE := prebuild-lib$name\n";

	if ($is_local) {
		print $OUT "LOCAL_SRC_FILES := \$(LOCAL_PATH)/obj/local/armeabi/lib$name.a\n";
	}
	else {
		print $OUT "LOCAL_SRC_FILES := \$(LOCAL_PATH)/../../../Libs/project/lib/lib$name.a\n";
	}
	print $OUT "include \$(PREBUILT_STATIC_LIBRARY)\n";
	print $OUT "\n";
}

sub WriteArrayElements
{
	my $OUT = shift;
	my $array_ref = shift;
	my $is_prebuild_lib = shift;

	my $pos = 0;

	my $array_cnt = @$array_ref - 1;

	foreach my $elem (@$array_ref) {
		if ($elem =~ /glut/ || $elem =~ /OGL/) {
			next;
		}

		if ($is_prebuild_lib) {
			print $OUT "prebuild-lib" . $elem;
		}
		else {
			print $OUT "$elem";
		}

		if ($pos != $array_cnt) {
			print $OUT " \\";
		}
		print $OUT "\n";

		$pos++;
	}
}

sub MakeAndroidMk_SharedLib
{
	my $targetdst = shift;
	my $name = shift;
	my $libraries_array_ref = shift;
	my $includes_array_ref = shift;
	my $definitions_array_ref = shift;
	my $srcfiles_array_ref = shift;

	my $dstmk = $targetdst . "/Android_" . $name . ".mk";

	open(my $OUT, ">$dstmk") or die "Can't open $dstmk\n";

	print $OUT "LOCAL_PATH:= \$(call my-dir)\n";
	print $OUT "\n";

	foreach my $lib (@$libraries_array_ref) {
		if ($lib =~ /SampleKit/) {
			WritePrebuildStaticLib($OUT, $lib, true);
		}
		else {
			WritePrebuildStaticLib($OUT, $lib, false);
		}
	}

	print $OUT "include \$(CLEAR_VARS)\n";
	print $OUT "\n";

	print $OUT "LOCAL_MODULE     := lib$name\n";

	{
		print $OUT "LOCAL_LDLIBS     := -llog -lGLESv2";
		print $OUT "\n";
	}

	{
		print $OUT "LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__";

		foreach my $def (@$definitions_array_ref) {
			print $OUT " -D$def";
		}

		print $OUT "\n";
	}

	{
		print $OUT "LOCAL_C_INCLUDES :=";

		foreach my $inc (@$includes_array_ref) {
			print $OUT " \$(LOCAL_PATH)/" . $inc;
		}

		print $OUT "\n";
	}

	{
		print $OUT "LOCAL_SRC_FILES  := ";

		WriteArrayElements($OUT, $srcfiles_array_ref, false);

		print $OUT "\n";
	}

	{
		print $OUT "LOCAL_WHOLE_STATIC_LIBRARIES := ";

		WriteArrayElements($OUT, $libraries_array_ref, true);

		print $OUT "\n";
	}

	print $OUT "include \$(BUILD_SHARED_LIBRARY)\n";

	close($OUT);
}

sub MakeApplicationMk
{
	my $targetdst = shift;
	my $name = shift;

	my $appmk = $targetdst . "Application_" . $name .".mk";

	open(OUT, ">$appmk") or die "Can't opne $appmk\n";

	print OUT "APP_PROJECT_PATH := \$(NDK_PROJECT_PATH)\n";
	print OUT "APP_BUILD_SCRIPT := \$(APP_PROJECT_PATH)/Android_" . $name . ".mk\n";
	print OUT "APP_STL := stlport_static\n";
	print OUT "APP_PLATFORM := android-18\n";

	close(OUT);
}

sub FindElement
{
	my $array = shift;
	my $key = shift;

	my $pos = 0;

	foreach my $elem (@$array) {
		if ($elem eq $key) {
			return $pos;
		}
		$pos++;
	}

	return -1;
}

sub MakeMakefile
{
	my $config = shift;
	my $targetdst = shift;
	my $projects_hash = shift;

	my $makefile = $targetdst . "/Makefile";

	open(IN, ">$makefile") or die "Can't opne $makefile\n";

	print IN "$config: create_floders\n";

	my @projects = keys(%$projects_hash);

	my $pos = FindElement(\@projects, "SampleKit");
	if ($pos >= 0) {
		splice(@projects, $pos, 1);
		unshift(@projects, "SampleKit");
	}

	foreach my $name (@projects) {
		my $mk = "Application_" . $name . ".mk";
		print IN "\tndk-build NDK_PROJECT_PATH=\${PWD} NDK_APPLICATION_MK=\"./$mk\"\n";
	}

	print IN "\tcp ./obj/local/armeabi/lib*.a ../lib/\n";
	print IN "\n";

	print IN "create_floders: \n";
	print IN "\tmkdir -p ../lib\n";
	print IN "\n";

	print IN "clean: \n";
	print IN "\trm -rf ./obj\n";
	print IN "\trm -rf ./libs\n";
	print IN "\trm -rf ../lib/lib*.a\n";

	close(IN);
}
