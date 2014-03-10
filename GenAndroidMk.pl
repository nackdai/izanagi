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

	MakeMk($is_shared, $targetdst, $name, \@libraries, \@includes, \@definitions, \@srcfiles);
}

#==========================

sub MakeMk
{
	my $is_shared = shift;
	my $targetdst = shift;
	my $name = shift;
	my $libraries_array_ref = shift;
	my $includes_array_ref = shift;
	my $definitions_array_ref = shift;
	my $srcfiles_array_ref = shift;

	my $dstmk = $targetdst . "/Android_" . $name . ".mk";

	open(IN, ">$dstmk") or die "Can't open $dstmk\n";

	print IN "LOCAL_PATH:= \$(call my-dir)\n";
	print IN "\n";

	print IN "include \$(CLEAR_VARS)\n";
	print IN "\n";

	print IN "LOCAL_MODULE     := lib$name\n";

	{
		print IN "LOCAL_LDLIBS     := -llog -lGLESv2";

		if ($is_shared) {
			foreach my $lib (@$libraries_array_ref) {
				print IN " -l$lib";
			}
		}

		print IN "\n";
	}

	{
		print IN "LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__";

		foreach my $def (@$definitions_array_ref) {
			print IN " -D$def";
		}

		print IN "\n";
	}

	{
		print IN "LOCAL_C_INCLUDES :=";

		foreach my $inc (@$includes_array_ref) {
			print IN " \$(LOCAL_PATH)/" . $inc;
		}

		print IN "\n";
	}

	{
		print IN "LOCAL_SRC_FILES  := ";

		my $pos = 0;

		my $srcfiles_cnt = @$srcfiles_array_ref - 1;

		foreach my $src (@$srcfiles_array_ref) {
			if ($src =~ /glut/ || $src =~ /OGL/) {
				next;
			}

			print IN "$src";
			if ($pos != $srcfiles_cnt) {
				print IN " \\";
			}
			print IN "\n";

			$pos++;
		}

		print IN "\n";
	}

	if ($is_shared) {
		print IN "include \$(BUILD_SHARED_LIBRARY)\n";
	}
	else {
		print IN "include \$(BUILD_STATIC_LIBRARY)\n";
	}

	close(IN);

	my $appmk = $targetdst . "Application_" . $name .".mk";

	open(IN, ">$appmk") or die "Can't opne $appmk\n";

	print IN "APP_PROJECT_PATH := \$(NDK_PROJECT_PATH)\n";
	print IN "APP_BUILD_SCRIPT := \$(APP_PROJECT_PATH)/Android_" . $name . ".mk\n";
	print IN "APP_STL := stlport_static\n";
	print IN "APP_PLATFORM := android-18\n";

	close(IN);
}