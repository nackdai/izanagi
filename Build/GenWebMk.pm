package GenWebMk;

use base Exporter;
#@ISA = qw(Exporter);
@EXPORT = qw(DoLoopAction DoEndAction);

use strict;
use File::Spec;

use constant true => 1;
use constant false => 0;

sub DoLoopAction {
	my $is_shared = shift;
	my $targetsrc = shift;
	my $targetdst = shift;
	my $name = shift;
	my $libraries = shift;
	my $includes = shift;
	my $definitions = shift;
	my $srcfiles = shift;

	if ($is_shared) {
#		MakeAndroidMk_SharedLib($targetdst, $name, $libraries, $includes, $definitions, $srcfiles);
	}
	else {
		MakeWebMk_StaticLib($targetsrc, $targetdst, $name, $libraries, $includes, $definitions, $srcfiles);
	}
}

sub DoEndAction {
	my $baseconfig = shift;
	my $targetdst = shift;
	my $projects = shift;

	# 全体をビルドするためのMakefileを生成
#	MakeMakefile($baseconfig, $targetdst, $projects);
}

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
sub MakeWebMk_StaticLib
{
	my $targetsrc = shift;
	my $targetdst = shift;	# 出力ディレクトリ
	my $name = shift;
	my $libraries_array_ref = shift;	# 参照ライブラリの配列への参照
	my $includes_array_ref = shift;		# インクルードパスの配列への参照
	my $definitions_array_ref = shift;	# -Dオプションの配列への参照
	my $srcfiles_array_ref = shift;

	# 出力Makefileのパス
	my $dstmk = $targetdst . "/Web_" . $name . ".bat";

	open(OUT, ">$dstmk") or die "Can't open $dstmk\n";

	print OUT "emcc ";

	# -Dオプション
	{
		print OUT "-DEMSCRIPTEN -D__IZ_GLES2__";

		foreach my $def (@$definitions_array_ref) {
			print OUT " -D$def";
		}
	}

	# インクルードパス
	foreach my $inc (@$includes_array_ref) {
		print OUT "-I" . $inc;
	}

	# 
	{
		my $pos = 0;

		my $srcfiles_cnt = @$srcfiles_array_ref - 1;

		foreach my $src (@$srcfiles_array_ref) {
			# ファイル名に 'glut' or 'OGL' or 'linux' が含まれている場合は 'web' に置換する
			if ($src =~ /(glut)/ || $src =~ /(OGL)/ || $src =~ /linux/) {
				# 元のパスを残しておく
				my $org = $src;

				# パスを分解
				my @tmp_array = split(/\//, $src);

				# 末尾 = ファイル名 を取り出す
				$src = $tmp_array[@tmp_array - 1];

				# 'glut' or 'OGL' を 'web' に置換する
				$src =~ s/$1/web/;

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

			print OUT "$src ";

			$pos++;
		}
	}

	print OUT "-o lib" . $name . ".bc";

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


1;
__END__
