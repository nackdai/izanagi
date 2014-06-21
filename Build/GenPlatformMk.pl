#!/usr/bin/perl

use GenAndroidMk;

use strict;
use File::Spec;

use constant true => 1;
use constant false => 0;

if (@ARGV != 4) {
	# Print Usage.
	GenAndroidMk::Usage();
	exit();
}

my $isAndroid = ($ARGV[3] eq "android");

my $baseconfig = $ARGV[0];	# Makefile内を参照するコンフィグ
my $targetsrc = $ARGV[1];	# 参照するMakefileがあるディレクトリ
my $targetdst = $ARGV[2];	# 出力ディレクトリ

# 出力ディレクトリが存在しない場合は作る
unless (-d $targetdst) {
	mkdir($targetdst);
}

if ($targetdst !~ /\/$/) {
	$targetdst .= "/";
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

	# Makefile以外は無視する
	if ($srcmk !~ /[a-zA-Z0-9]\.makefile/) {
		next;
	}

	# ファイル名は XXXX.makefile 
	# ここから XXXX にあたる部分を取り出す
	my $name = (split(/\./, $srcmk))[0];

	# フルパスを作成
	my $srcmk = $targetsrc . "/" . $srcmk;

	print("$srcmk\n");

	open(IN, $srcmk) or die "Can't open $srcmk\n";

	my @targets = ();		# 参照するオブジェクトファイル
	my @srcfiles = ();		# ソースファイル
	my @definitions = ();	# -Dオプション
	my @libraries = ();		# 参照ライブラリ
	my @includes = ();		# インクルードパス

	my $is_shared = false;

	while (my $line = <IN>) {
		$line =~ s/\r//;
		$line =~ s/\n//;

		if ($line =~ /\.exe/) {
			# exeファイルはシェアドライブラリとして生成する
			$is_shared = true;
		}
		elsif ($line =~ /$baseconfig: /) {
			# 参照するコンフィグについてmakeのターゲットとして記述された行

			# 参照するオブジェクトファイルを集める
			my @files = split(/ /, $line);
			foreach my $file (@files) {
				unless ($file =~ /\.o/) {
					next;
				}
				push(@targets, $file);
			}
		}
		elsif ($line =~ /$defnition_tag=/) {
			# -Dオプションについて記述された行

			my $defs_tmp = (split(/=/, $line))[1];
			$defs_tmp =~ s/-D //g;
			my @defs = split(/ /, $defs_tmp);

			foreach my $def (@defs) {
				# 関係ないものについては無視する
				if ($def =~ /GLUT/ || $def =~ /GCC_BUILD/) {
					next;
				}
				push(@definitions, $def);
			}
		}
		elsif ($line =~ /$libs_tag=/) {
			# 参照するライブラリ

			my $libs_tmp = (split(/=/, $line))[1];
			my @libs = split(/ /, $libs_tmp);

			foreach my $lib (@libs) {
				if ($lib =~ /-l([a-zA-Z0-9]*)/) {
					$lib = $1;

					# 以下のライブラリは無いので無視する
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
			# インクルードパス

			my $inc_tmp = (split(/=/, $line))[1];
			my @incs = split(/ /, $inc_tmp);

			foreach my $inc (@incs) {
				if ($inc =~ /-I\"([\.\/a-zA-Z0-9]*)/) {
					$inc = $1;

					# 以下の文字列が含まれるインクルードパスは無いので無視する
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
			# 参照するオブジェクトファイルからソースファイルを探す
			foreach my $target (@targets) {
				if ($line =~ /$target: /) {
					my $srcfile = (split(/ /, $line))[1];
					push(@srcfiles, $srcfile);
				}
			}
		}
	}

	close(IN);

	# それぞれのプロジェクトがシェアドライブラリかどうかをハッシュで保持
	$projects{$name} = $is_shared;

	GenAndroidMk::DoLoopAction($is_shared, $targetsrc, $targetdst, $name, \@libraries, \@includes, \@definitions, \@srcfiles);
}

GenAndroidMk::DoEndAction($baseconfig, $targetdst, \%projects);
