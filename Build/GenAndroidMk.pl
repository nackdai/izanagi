#!/usr/bin/perl

use strict;
use File::Spec;

use constant true => 1;
use constant false => 0;

if (@ARGV != 3) {
	# Print Usage.
	print "GenAndroidMK.pl [configure] [src directory] [dst directory]\n";
	print " ex) \$./GenAndroidMK.pl Debug Libs/project/makefile Lib/project/android\n";
	exit();
}

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

	# Android.mkを生成
	if ($is_shared) {
		MakeAndroidMk_SharedLib($targetdst, $name, \@libraries, \@includes, \@definitions, \@srcfiles);
	}
	else {
		MakeAndroidMk_StaticLib($targetsrc, $targetdst, $name, \@libraries, \@includes, \@definitions, \@srcfiles);
	}

	# Application.mkを生成
	MakeApplicationMk($targetdst, $name);
}

# 全体をビルドするためのMakefileを生成
MakeMakefile($baseconfig, $targetdst, \%projects);

#==========================

# 相対パス取得
sub GetRelativePath
{
	my $basedir = shift;	# ベースとなるディレクトリのパス
	my $path = shift;		# 相対パスにしたいパス

	chomp($path);
	chomp($path);

	# パスを分解する
	my @basedir_array = split(/\//, $basedir);
	my @path_array = split(/\//, $path);

	if ($path_array[0] eq ".") {
		splice(@path_array, 0, 1);
	}

	my $basedir_cnt = @basedir_array;
	my $path_cnt = @path_array;

	my $cnt = ($basedir_cnt < $path_cnt ? $basedir_cnt : $path_cnt);
	my $num = 0;

	# パスを後ろから見ていき、一致する位置を探す
	for (my $i = $cnt - 1; $i >= 0; $i--) {
		if ($basedir_array[$i] eq $path_array[$i]) {
			last;
		}
		$num++;
	}

	my $ret = "";

	# 一致しない分だけが相対になる
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

# 相対パス取得
sub RemoveRelativePath
{
	my $src = shift;

	my @tmp_array = split(/\//, $src);

	my @dir_array = ();
	foreach my $t (@tmp_array) {
		if ($t eq "..") {
			pop(@dir_array);
		}
		else {
			push(@dir_array, $t);
		}
	}

	$src = "";
	foreach my $d (@dir_array) {
		$src .= $d . "/";
	}

	if ($src =~ /\/$/) {
		chop($src);
	}

	return $src;
}

# スタティックライブラリを生成するMakefileを出力する
sub MakeAndroidMk_StaticLib
{
	my $targetsrc = shift;
	my $targetdst = shift;	# 出力ディレクトリ
	my $name = shift;
	my $libraries_array_ref = shift;	# 参照ライブラリの配列への参照
	my $includes_array_ref = shift;		# インクルードパスの配列への参照
	my $definitions_array_ref = shift;	# -Dオプションの配列への参照
	my $srcfiles_array_ref = shift;

	# 出力Makefileのパス
	my $dstmk = $targetdst . "/Android_" . $name . ".mk";

	open(OUT, ">$dstmk") or die "Can't open $dstmk\n";

	print OUT "LOCAL_PATH:= \$(call my-dir)\n";
	print OUT "\n";

	print OUT "include \$(CLEAR_VARS)\n";
	print OUT "\n";

	print OUT "LOCAL_MODULE     := lib$name\n";

	# -Dオプション
	{
		print OUT "LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__";

		foreach my $def (@$definitions_array_ref) {
			print OUT " -D$def";
		}

		print OUT "\n";
	}

	# インクルードパス
	{
		print OUT "LOCAL_C_INCLUDES :=";

		foreach my $inc (@$includes_array_ref) {
			print OUT " \$(LOCAL_PATH)/" . $inc;
		}

		print OUT "\n";
	}

	# 
	{
		print OUT "LOCAL_SRC_FILES  := ";

		my $pos = 0;

		my $srcfiles_cnt = @$srcfiles_array_ref - 1;

		foreach my $src (@$srcfiles_array_ref) {
			# ファイル名に 'glut' or 'OGL' or 'linux' が含まれている場合は 'android' に置換する
			if ($src =~ /(glut)/ || $src =~ /(OGL)/ || $src =~ /linux/) {
				# 元のパスを残しておく
				my $org = $src;

				# パスを分解
				my @tmp_array = split(/\//, $src);

				# 末尾 = ファイル名 を取り出す
				$src = $tmp_array[@tmp_array - 1];

				# 'glut' or 'OGL' を 'android' に置換する
				$src =~ s/$1/android/;

				my $find_dir = "./" . $targetsrc;
				for (my $i = 0; $i < @tmp_array - 2; $i++) {
					$find_dir .= "/" . $tmp_array[$i];
				}

				# 本当にファイルが存在するか探す
				my @fullpath = `find $find_dir -name $src`;

				if (@fullpath == 1) {
					# 見つかった
					# 相対パスにする
					chomp($fullpath[0]);
					$src = File::Spec->rel2abs($fullpath[0]);
					$src = RemoveRelativePath($src);

					my $targetsrc = File::Spec->rel2abs($targetsrc);
					$targetsrc = RemoveRelativePath($targetsrc);

					#$src = GetRelativePath($targetsrc, $src);
					$src = File::Spec->abs2rel($src, $targetsrc);
				}
				elsif (@fullpath > 1) {
					# TODO
					# 複数ファイルが存在した場合
					next;
				}
				else {
					# 見つからなかった場合は元のパスに戻す
					$src = $org;
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

# static libraryをリンクするコードを出力
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

# 配列の要素を出力
sub WriteArrayElements
{
	my $OUT = shift;				# 出力ファイルハンドル
	my $array_ref = shift;			# 配列の参照
	my $is_prebuild_lib = shift;	# 参照するスタティックライブラリについて処理するかどうか

	my $pos = 0;

	my $array_cnt = @$array_ref - 1;

	foreach my $elem (@$array_ref) {
		# 要素に 'glut' or 'OGL' が含まれる場合は無視する
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

# シェアドライブラリを生成するMakefileを出力する
sub MakeAndroidMk_SharedLib
{
	my $targetdst = shift;	# 出力ディレクトリ
	my $name = shift;
	my $libraries_array_ref = shift;	# 参照ライブラリの配列への参照
	my $includes_array_ref = shift;     # インクルードパスの配列への参照
	my $definitions_array_ref = shift;  # -Dオプションの配列への参照
	my $srcfiles_array_ref = shift;		# ソースファイルの配列への参照

	# 出力Makefileのパス
	my $dstmk = $targetdst . "/Android_" . $name . ".mk";

	open(my $OUT, ">$dstmk") or die "Can't open $dstmk\n";

	print $OUT "LOCAL_PATH:= \$(call my-dir)\n";
	print $OUT "\n";

	# スタティックライブラリをリンクするコードを出力
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
		print $OUT "LOCAL_LDLIBS     := -llog -lGLESv2 -landroid";
		print $OUT "\n";
	}

	# -Dオプション
	{
		print $OUT "LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__";

		foreach my $def (@$definitions_array_ref) {
			print $OUT " -D$def";
		}

		print $OUT "\n";
	}

	# インクルードパス
	{
		print $OUT "LOCAL_C_INCLUDES :=";

		foreach my $inc (@$includes_array_ref) {
			print $OUT " \$(LOCAL_PATH)/" . $inc;
		}

		print $OUT "\n";
	}

	# ソースファイル
	{
		print $OUT "LOCAL_SRC_FILES  := ";

		WriteArrayElements($OUT, $srcfiles_array_ref, false);

		print $OUT "\n";
	}

	# 参照する自分で用意したスタティックライブラリ
	{
		print $OUT "LOCAL_WHOLE_STATIC_LIBRARIES := ";

		WriteArrayElements($OUT, $libraries_array_ref, true);

		print $OUT "\n";
	}

	print $OUT "include \$(BUILD_SHARED_LIBRARY)\n";

	close($OUT);
}

# Application.mkを生成する
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

# 指定された要素が配列内のどの位置にあるのか探す
sub FindElement
{
	my $array = shift;	# 配列の参照
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

# 全体をビルドするためのMakefileを生成
sub MakeMakefile
{
	my $config = shift;			# 対象のコンフィグ
	my $targetdst = shift;
	my $projects_hash = shift;	# それぞれのプロジェクトがシェアドライブラリかどうかのハッシュの参照

	my $makefile = $targetdst . "/Makefile";

	open(IN, ">$makefile") or die "Can't opne $makefile\n";

	print IN "$config: create_floders\n";

	# ハッシュのキーの配列を取得
	my @projects = keys(%$projects_hash);

	# 'SampleKit'の文字列が配列内のどの位置にあるのか探す
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
