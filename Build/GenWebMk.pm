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

	# �S�̂��r���h���邽�߂�Makefile�𐶐�
#	MakeMakefile($baseconfig, $targetdst, $projects);
}

# ���΃p�X�擾
sub GetRelativePath
{
	my $basedir = shift;	# �x�[�X�ƂȂ�f�B���N�g���̃p�X
	my $path = shift;		# ���΃p�X�ɂ������p�X

	chomp($path);
	chomp($path);

	# �p�X�𕪉�����
	my @basedir_array = split(/\//, $basedir);
	my @path_array = split(/\//, $path);

	if ($path_array[0] eq ".") {
		splice(@path_array, 0, 1);
	}

	my $basedir_cnt = @basedir_array;
	my $path_cnt = @path_array;

	my $cnt = ($basedir_cnt < $path_cnt ? $basedir_cnt : $path_cnt);
	my $num = 0;

	# �p�X����납�猩�Ă����A��v����ʒu��T��
	for (my $i = $cnt - 1; $i >= 0; $i--) {
		if ($basedir_array[$i] eq $path_array[$i]) {
			last;
		}
		$num++;
	}

	my $ret = "";

	# ��v���Ȃ������������΂ɂȂ�
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

# ���΃p�X�擾
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

# �X�^�e�B�b�N���C�u�����𐶐�����Makefile���o�͂���
sub MakeWebMk_StaticLib
{
	my $targetsrc = shift;
	my $targetdst = shift;	# �o�̓f�B���N�g��
	my $name = shift;
	my $libraries_array_ref = shift;	# �Q�ƃ��C�u�����̔z��ւ̎Q��
	my $includes_array_ref = shift;		# �C���N���[�h�p�X�̔z��ւ̎Q��
	my $definitions_array_ref = shift;	# -D�I�v�V�����̔z��ւ̎Q��
	my $srcfiles_array_ref = shift;

	# �o��Makefile�̃p�X
	my $dstmk = $targetdst . "/Web_" . $name . ".bat";

	open(OUT, ">$dstmk") or die "Can't open $dstmk\n";

	# ���ʏ������Ăяo���R�[�h���o��
	print OUT "call PreBuild.bat\n\n";

	print OUT "emcc ";

	# -D�I�v�V����
	{
		print OUT "-DEMSCRIPTEN -D__IZ_GLUT__ ";

		foreach my $def (@$definitions_array_ref) {
			print OUT "-D$def ";
		}
	}

	# �C���N���[�h�p�X
	foreach my $inc (@$includes_array_ref) {
		print OUT "-I$inc ";
	}

	# 
	{
		my $pos = 0;

		my $srcfiles_cnt = @$srcfiles_array_ref - 1;

		foreach my $src (@$srcfiles_array_ref) {
			# �t�@�C������ 'glut' or 'OGL' or 'linux' ���܂܂�Ă���ꍇ�� 'web' �ɒu������
			if ($src =~ /(glut)/ || $src =~ /(OGL)/ || $src =~ /linux/) {
				# ���̃p�X���c���Ă���
				my $org = $src;

				# �p�X�𕪉�
				my @tmp_array = split(/\//, $src);

				# ���� = �t�@�C���� �����o��
				$src = $tmp_array[@tmp_array - 1];

				# 'glut' or 'OGL' �� 'web' �ɒu������
				$src =~ s/$1/web/;

				my $find_dir = "./" . $targetsrc;
				for (my $i = 0; $i < @tmp_array - 2; $i++) {
					$find_dir .= "/" . $tmp_array[$i];
				}

				# �{���Ƀt�@�C�������݂��邩�T��
				my @fullpath = `find $find_dir -name $src`;

				if (@fullpath == 1) {
					# ��������
					# ���΃p�X�ɂ���
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
					# �����t�@�C�������݂����ꍇ
					next;
				}
				else {
					# ������Ȃ������ꍇ�͌��̃p�X�ɖ߂�
					$src = $org;
				}
			}

			if ($src =~ /pthread/) {
				# pthread�̓T�|�[�g����Ă��Ȃ����ߖ�������
				next;
			}

			print OUT "$src ";

			$pos++;
		}
	}

	print OUT "-o ../lib/lib" . $name . ".bc";

	close(OUT);
}

# �V�F�A�h���C�u�����𐶐�����Makefile���o�͂���
sub MakeAndroidMk_SharedLib
{
	my $targetdst = shift;	# �o�̓f�B���N�g��
	my $name = shift;
	my $libraries_array_ref = shift;	# �Q�ƃ��C�u�����̔z��ւ̎Q��
	my $includes_array_ref = shift;     # �C���N���[�h�p�X�̔z��ւ̎Q��
	my $definitions_array_ref = shift;  # -D�I�v�V�����̔z��ւ̎Q��
	my $srcfiles_array_ref = shift;		# �\�[�X�t�@�C���̔z��ւ̎Q��

	# �o��Makefile�̃p�X
	my $dstmk = $targetdst . "/Web_" . $name . ".bat";

	open(OUT, ">$dstmk") or die "Can't open $dstmk\n";

	# ���ʏ������Ăяo���R�[�h���o��
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

	# -D�I�v�V����
	{
		print OUT "-DEMSCRIPTEN -D__IZ_GLUT__ ";

		foreach my $def (@$definitions_array_ref) {
			print OUT "-D$def ";
		}
	}

	# �C���N���[�h�p�X
	foreach my $inc (@$includes_array_ref) {
		print OUT " -I$inc ";
	}

	# �X�^�e�B�b�N���C�u�����������N����R�[�h���o��
	foreach my $lib (@$libraries_array_ref) {
		if ($lib =~ /SampleKit/) {
			print OUT "../lib/lib" . $lib . ".bc ";
		}
		else {
			print OUT "%LIB%lib" . $lib . ".bc ";
		}
	}

	# �\�[�X�t�@�C��
	foreach my $src (@$srcfiles_array_ref) {
		print OUT "$src ";
	}

	print OUT "-o $name" . ".html %PREJS%";

	close(OUT);
}

# �w�肳�ꂽ�v�f���z����̂ǂ̈ʒu�ɂ���̂��T��
sub FindElement
{
	my $array = shift;	# �z��̎Q��
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

# �S�̂��r���h���邽�߂�Makefile�𐶐�
sub MakeMakefile
{
	my $config = shift;			# �Ώۂ̃R���t�B�O
	my $targetdst = shift;
	my $projects_hash = shift;	# ���ꂼ��̃v���W�F�N�g���V�F�A�h���C�u�������ǂ����̃n�b�V���̎Q��

	my $makefile = $targetdst . "/Makefile";

	open(IN, ">$makefile") or die "Can't opne $makefile\n";

	print IN "$config: create_floders\n";

	# �n�b�V���̃L�[�̔z����擾
	my @projects = keys(%$projects_hash);

	# 'SampleKit'�̕����񂪔z����̂ǂ̈ʒu�ɂ���̂��T��
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
