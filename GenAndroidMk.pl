#!/usr/bin/perl

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

my $defnition_tag = $baseconfig . "_Preprocessor_Definitions";

foreach my $srcmk (@makefiles) {
	if ($srcmk eq "." || $srcmk eq "..") {
		next;
	}

	my $name = (split(/\./, $srcmk))[0];

	my $srcmk = $targetsrc . "/" . $srcmk;
	my $dstmk = $targetdst . "/Android_" . $name . ".mk";

	print("$srcmk -> $dstmk\n");

	open(IN, $srcmk) or die "Can't open $srcmk\n";

	my @targets = ();
	my @srcfiles = ();
	my @definitions = ();

	while (my $line = <IN>) {
		$line =~ s/\r//;
		$line =~ s/\n//;

		if ($line =~ /$baseconfig: /) {
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

	open(IN, ">$dstmk") or die "Can't open $dstmk\n";

	print IN "LOCAL_PATH:= \$(call my-dir)\n";
	print IN "\n";

	print IN "include \$(CLEAR_VARS)\n";
	print IN "\n";

	print IN "LOCAL_MODULE     := lib$name\n";

	print IN "LOCAL_LDLIBS    := -llog -lGLESv2\n";

#	print IN "LOCAL_CFLAGS     := -Wno-error=format-security -DANDROID";
#	print IN "LOCAL_CFLAGS     := -Werror -DANDROID";
	print IN "LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__";

	foreach my $def (@definitions) {
		print IN " -D$def";
	}

	print IN "\n";

#	print IN "LOCAL_LDLIBS     := -llog -lGLESv2\n";
	print IN "LOCAL_C_INCLUDES := \$(LOCAL_PATH)/../../include\n";
	print IN "LOCAL_SRC_FILES  := ";

	my $pos = 0;

	foreach my $src (@srcfiles) {
		if ($src =~ /glut/ || $src =~ /OGL/) {
			next;
		}

		print IN "$src";
		if ($pos != $#srcfiles) {
			print IN " \\";
		}
		print IN "\n";

		$pos++;
	}

	print IN "\n";

	# TODO
	print IN "include \$(BUILD_STATIC_LIBRARY)\n";

	close(IN);

	my $appmk = $targetdst . "Application_" . $name .".mk";

	open(IN, ">$appmk") or die "Can't opne $appmk\n";

	print IN "APP_PROJECT_PATH := \$(NDK_PROJECT_PATH)\n";
	print IN "APP_BUILD_SCRIPT := \$(APP_PROJECT_PATH)/Android_" . $name . ".mk\n";
	print IN "APP_STL := stlport_static\n";
	print IN "APP_PLATFORM := android-18\n";

	close(IN);
}
