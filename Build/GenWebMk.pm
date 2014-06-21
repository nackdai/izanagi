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

	print OUT "emcc ";

	# -D�I�v�V����
	{
		print OUT "-DEMSCRIPTEN -D__IZ_GLES2__";

		foreach my $def (@$definitions_array_ref) {
			print OUT " -D$def";
		}
	}

	# �C���N���[�h�p�X
	foreach my $inc (@$includes_array_ref) {
		print OUT "-I" . $inc;
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

			print OUT "$src ";

			$pos++;
		}
	}

	print OUT "-o lib" . $name . ".bc";

	close(OUT);
}

# static library�������N����R�[�h���o��
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

# �z��̗v�f���o��
sub WriteArrayElements
{
	my $OUT = shift;				# �o�̓t�@�C���n���h��
	my $array_ref = shift;			# �z��̎Q��
	my $is_prebuild_lib = shift;	# �Q�Ƃ���X�^�e�B�b�N���C�u�����ɂ��ď������邩�ǂ���

	my $pos = 0;

	my $array_cnt = @$array_ref - 1;

	foreach my $elem (@$array_ref) {
		# �v�f�� 'glut' or 'OGL' ���܂܂��ꍇ�͖�������
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
	my $dstmk = $targetdst . "/Android_" . $name . ".mk";

	open(my $OUT, ">$dstmk") or die "Can't open $dstmk\n";

	print $OUT "LOCAL_PATH:= \$(call my-dir)\n";
	print $OUT "\n";

	# �X�^�e�B�b�N���C�u�����������N����R�[�h���o��
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

	# -D�I�v�V����
	{
		print $OUT "LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__";

		foreach my $def (@$definitions_array_ref) {
			print $OUT " -D$def";
		}

		print $OUT "\n";
	}

	# �C���N���[�h�p�X
	{
		print $OUT "LOCAL_C_INCLUDES :=";

		foreach my $inc (@$includes_array_ref) {
			print $OUT " \$(LOCAL_PATH)/" . $inc;
		}

		print $OUT "\n";
	}

	# �\�[�X�t�@�C��
	{
		print $OUT "LOCAL_SRC_FILES  := ";

		WriteArrayElements($OUT, $srcfiles_array_ref, false);

		print $OUT "\n";
	}

	# �Q�Ƃ��鎩���ŗp�ӂ����X�^�e�B�b�N���C�u����
	{
		print $OUT "LOCAL_WHOLE_STATIC_LIBRARIES := ";

		WriteArrayElements($OUT, $libraries_array_ref, true);

		print $OUT "\n";
	}

	print $OUT "include \$(BUILD_SHARED_LIBRARY)\n";

	close($OUT);
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
