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
		MakeAndroidMk_SharedLib($targetdst, $name, $libraries, $includes, $definitions, $srcfiles);
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

	# 共通処理を呼び出すコードを出力
	print OUT "call PreBuild.bat\n\n";

	print OUT "emcc ";

	# -Dオプション
	{
		print OUT "-DEMSCRIPTEN -D__IZ_GLUT__ ";

		foreach my $def (@$definitions_array_ref) {
			print OUT "-D$def ";
		}
	}

	# インクルードパス
	foreach my $inc (@$includes_array_ref) {
		print OUT "-I$inc ";
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

			if ($src =~ /pthread/) {
				# pthreadはサポートされていないため無視する
				next;
			}

			print OUT "$src ";

			$pos++;
		}
	}

	print OUT "-o ../lib/lib" . $name . ".bc";

	close(OUT);
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
	my $dstmk = $targetdst . "/Web_" . $name . ".bat";

	open(OUT, ">$dstmk") or die "Can't open $dstmk\n";

	# 共通処理を呼び出すコードを出力
	print OUT "if exist ../lib/SampleKit.bc (\n";
	print OUT "    call PreBuild.bat\n";
	print OUT ") else (\n";
	print OUT "    call Web_SampleKit.bat\n";
	print OUT ")\n\n";

	print OUT "if exist ../../$name/pre.js (\n";
	print OUT "    set PREJS=--pre-js ../../Render2D/pre.js\n";
	print OUT ") else (\n";
	print OUT "    set PREJS=\n";
	print OUT ")\n\n";

	print OUT "emcc ";

	# -Dオプション
	{
		print OUT "-DEMSCRIPTEN -D__IZ_GLUT__ ";

		foreach my $def (@$definitions_array_ref) {
			print OUT "-D$def ";
		}
	}

	# インクルードパス
	foreach my $inc (@$includes_array_ref) {
		print OUT " -I$inc ";
	}

	# スタティックライブラリをリンクするコードを出力
	foreach my $lib (@$libraries_array_ref) {
		if ($lib =~ /SampleKit/) {
			print OUT "../lib/lib" . $lib . ".bc ";
		}
		else {
			print OUT "%LIB%lib" . $lib . ".bc ";
		}
	}

	# ソースファイル
	foreach my $src (@$srcfiles_array_ref) {
		print OUT "$src ";
	}

	print OUT "-o $name" . ".html %PREJS%";

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


1;
__END__
