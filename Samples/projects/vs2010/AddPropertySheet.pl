#!/bin/perl

opendir(DIR, ".");

while ($file = readdir(DIR)) {
    if (-f $file) {
        if ($file =~ /\.vcxproj/ && $file !~ /\.vcxproj([a-b\.]+)/) {
            print "$file\n";
            ReplaceProps($file);
        }
    }
}

closedir(DIR);

#==========================

sub ReplaceProps {
    my $file = shift;

    my $tmp = "tmp.dat";

    open(FILE, "$file") or die "Can't open $file\n";
    open(TMP, ">$tmp") or die "Can't open $tmp\n";

    my $isInImportGroup = 0;

    while (my $line = <FILE>) {
        my $hasCRorLF = chomp($line);
        chomp($line);

        if ($isInImportGroup) {
            if ($line =~ /Import Project/ && $line =~ /Debug_GLES2/) {
                $isInImportGroup = 0;
				$line =~ s/GLES2/GLUT/;
            }
        }
        else {
            if ($line =~ /ImportGroup/ && $line =~ /Debug_GLUT/) {
                $isInImportGroup = 1;
            }
        }

        print TMP "$line";
        if ($hasCRorLF) {
            print TMP "\n";
        }
    }

    close(TMP);
    close(FILE);

    rename($tmp, $file);
}