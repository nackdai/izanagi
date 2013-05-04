#!/bin/perl

my $tag = "SVN_MERGED_TAG";

# fetch svn
DoCmd("git svn fetch");

# pull origin/master
DoCmd("git checkout master");
DoCmd("git pull origin master");

# Get commit hash
print "git log --pretty=oneline --reverse master...$tag\n";
my @logs = `git log --pretty=oneline --reverse master...$tag`;

# merge
DoCmd("git checkout git-svn");

foreach my $log (@logs)
{
	my $hash = (split(/\s/, $log))[0];

	DoCmd("git cherry-pick $hash");
}

# svn commit
DoCmd("git svn dcommit");

# É^ÉO
DoCmd("git checkout master");
DoCmd("git tag -f $tag");
DoCmd("git push -f origin $tag");

#=========================================

sub DoCmd
{
	my $cmd = shift;
	print $cmd . "\n";
	system($cmd);
}
