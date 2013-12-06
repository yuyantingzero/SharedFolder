#!/usr/bin/perl

#################################################################
# Nathan Clark
# 10/29/02
#################################################################

#################################################################
# this program takes benchmark and runs some tests to help narrow down what's
# wrong with the program. it assumes that the standard trimaran environment variables
# are set up and that the benchmark is properly setup in one of the benchmark
# directories.
#################################################################


#################################################################
# setup the environment

my $usage = "\nUsage: $0 -bench benchmark [-region b|s|h ] [other options passed to tcc]\n\n";

my $root = $ENV{TRIMARAN_ROOT};
($root eq "") and die "TRIMARAN_ROOT environment variable not set!\n";

my $stars = "*****************************************************************\n";

#################################################################
# parse the command line.

@src_files = ();
my $src_dir;
my $bench = "", $region = "b";
$#ARGV == -1 and die $usage;
while($ARGV[0] =~ /.+/o) {
  my $flag = shift @ARGV;
  if($flag eq '-bench') {
    ($#ARGV == -1) and die $usage;
    $bench = shift @ARGV;
    # find the directory and setup source listing.
    foreach("specint92", "specfp92", "specint95", "specint2000", "utils", "simple",
	    "perfect", "mediabench", "mybench/automotive", "mybench/consumer",
	    "mybench/network", "mybench/office", "mybench/security", "mybench/telecom"){
      $dir = "$root" . "/benchmarks/" . "$_" . "/" . "$bench";
      unless(-d $dir) { next; }
      $src_dir = "$dir" . "/src";
      opendir SRC, $src_dir or die "can't find source directory: $src_dir!\n";
      @src_files = grep /.+\.c$/, readdir SRC;
      closedir SRC;
      last;
    }
    $#src_files == -1 and die "can't find benchmark: $bench!\n";
    next;
  }
  elsif($flag eq '-region') {
    ($#ARGV == -1) and die $usage;
    $region = shift @ARGV;
    $region eq "b" or $region eq "s" or $region eq "h" or die $usage;
  }
  # add new flags here as 'elsif's
  else {
    $bench eq "" and die $usage;
    # warning: this doesn't work yet.
    $tcc_flags = join ' ', @ARGV;
  }
}

#################################################################
# set up some variables

my $tcc_dir = "$bench" . "_";
my $suffix = "";
if($region eq "b") {
  $suffix .= "O";
} elsif ($region eq "s") {
  $suffix .= "S";
} elsif ($region eq "h") {
  $suffix .= "HS";
} else {
  die "WTF!\n";
}
$tcc_dir .= "$suffix";

$impact_dir = "$tcc_dir" . "/impact_intermediate/";
$elcor_dir = "$tcc_dir" . "/elcor_intermediate/";
$simu_dir = "$tcc_dir" . "/simu_intermediate/";

my $setup, $args, $check, $cleanup;
my $benchdir = "$src_dir" . "/../";
open EXEC, ("$benchdir" . "/exec_info_input1") or
  die "couldn't open exec_info file: $!\n";
my @line = <EXEC>;
close EXEC;

foreach(@line) {
  my $asdf = $_;
  my $other_line = $_;
  $asdf =~ s/\$\{BENCH_DIR}/$benchdir/go;
  $asdf =~ s/\$\{BASE_DIR}/$benchdir/go;
  $asdf =~ s/\$\{RESULT_FILE}/result.out.input1/go;
  $other_line =~ s/\$\{BENCH_DIR}/$benchdir/go;
  $other_line =~ s/\$\{BASE_DIR}/$benchdir/go;
  $other_line =~ s/\$\{RESULT_FILE}/result.out.input1/go;
  $asdf =~ /\s*SETUP\s*=\s*"(.*)"/ and $setup = $1;
  $asdf =~ /\s*ARGS\s*=\s*"(.*)"/ and $args = $1;
  $asdf =~ /\s*CHECK\s*=\s*"(.*)"/ and $check = $1;
  $other_line =~ /\s*CHECK\s*=\s*"(.*)"/ and $impact_check = $1;
  $asdf =~ /\s*CLEANUP\s*=\s*"(.*)"/ and $cleanup = $1;
}

$check eq "" and die "no check command setup up in the exec_input1 file\n";

#################################################################
# do a basic run to set up the directories and intermediate files

print "doing a basic run to set up the directories...\n";
system "tcc -bench $bench -region $region >& test_bench.out";

# check to make sure everything went ok.
open OUT, "<test_bench.out" or die "couldn't open test_bench.out: $!";
@line = <OUT>;
close OUT;
grep /.*RESULT CHECK.*PASSED.*/, @line and $pcf_worked = 1;
grep /Saving \..+_tr files/, @line and $impact_ran = 1;
grep /gen_elcor successful/, @line and $elcor_ran = 1;
grep /gen_CtoO successful/, @line and $simu_ran = 1;
grep /\*SUCCESSFUL\*/, @line and $whole_thing_ran = 1;

unless($pcf_worked) {
  print "$stars";
  print "WARNING: the pcf run of impact failed: probably have a source code or setup error.\n";
  print "$stars\n";
}
unless($impact_ran) {
  die "impact didn't run!\n";
}
unless($elcor_ran) {
  die "elcor didn't run!\n";
}
unless($simu_ran) {
  die "simu didn't run!\n";
}
if($whole_thing_ran) {
  die "$stars$bench w/ $region blocks works!\n$stars";
}

#################################################################
# try the *.O/ *.S/ *.HS files with Lemulate.

print "checking Lemulate...\n";
if(do_Lemulate($suffix)) {
  print "$stars Lemulate on the *.$suffix files passed\n$stars\n";
} else {
  # Lemulate failed on the highest level files, so try it on lower level ones.
  if($suffix eq "S") {
    if(do_Lemulate("O")) {
      # it passed on the .O's, so figure out which file screwed up.
      Lemul_divide_and_conquer("O", "S");
    } else {
      die "$stars The *.O files failed to Lemulate correctly!\n$stars";
    }
  } elsif($suffix eq "H") {
    # hyper blocks
    if(do_Lemulate("H")) {
      # it passed on the .H's
      Lemul_divide_and_conquer("H", "HS");
    } else {
      if(do_Lemulate("O")) {
	# it passed on the .O's
	Lemul_divide_and_conquer("O", "H");
      } else {
	die "$stars The *.O files failed to Lemulate correctly!\n$stars";
      }
    }
  } else {
    die "$stars The *.O files failed to Lemulate correctly!\n$stars";
  }
}
#################################################################
# impact seems to work. try elcor stuff.

print "impact successful, trying elcor switches...\n";

my @good_files = ();
my @bad_files = ();

# expand the o_tr files
system "cd $elcor_dir; tar xzf *tr.tgz";

# create .o files with do_null_processing.
opendir DIR, $elcor_dir or die "couldn't oper source dir: $!\n";
my @good_files = sort(grep /.*_tr$/, readdir DIR);
closedir DIR;
($#good_files == -1) and die "couldn't find any *_tr files in $elcor_dir\n";

for (my $j = 0; $j <= $#good_files; $j++) {
  my $tr = $good_files[$j];
  $tr =~ /(.*)\.([A-Z]+)_tr/;
  my $el = "$1" . ".$2" . "_el";
  my $dot_c = "$1" . ".good.c";
  $good_files[$j] = "$1" . ".good.o";
  system "elcor -i $elcor_dir/$tr -o $simu_dir/$el -Finput_format=lcode -Foutput_format=rebel -Fdo_null_processing=yes >& /dev/null";
  system "cd $simu_dir; codegen -el $el -o $dot_c -Finput_format=rebel -Femulate_virtual_regs=yes -Femulate_unscheduled=yes >& /dev/null";
}
# the bad files don't compile when the good ones are using virtual registers, so we have
# to remake them with vrs.
system "cd $simu_dir; gen_EtoC . -Femulate_virtual_regs=yes < codegen_input_list >& /dev/null";
system "cd $simu_dir; gcc -c -w *.c -I" . "$root/simu/src/Emulib";

opendir DIR, $simu_dir or die "couldn't oper simu dir: $!\n";
my @bad_files = sort(grep /.*$suffix.*simu\.o$/, readdir DIR);
closedir DIR;
($#bad_files == -1) and die "couldn't find any *.o files in $simu_dir\n";

my $top = $#good_files;
my $bottom = 0;
my $middle = int(($top + $bottom)/2);
my $gcc_opts = " benchmark_data_init.simu.o -w -I" . "$root/simu/src/Emulib -L$root/simu/lib -lequals -lm ";
my $cmd = "gcc ";

# make sure the null_processed files work.
chdir $simu_dir;

for ($i = 0; $i <= $#good_files; $i++) {
  $cmd .= "$good_files[$i] ";
}
$cmd .= " $gcc_opts";
system "$cmd";

unless (-e "a.out") {
  die "$stars error compiling in elcor divide and conquer\n";
}
system "$setup";
if (system "$check >& /dev/null") {
  print "$stars elcor null processing passed.\n$stars\n";
} else {
  die "$stars NULL PROCESSING FAILED!\n$stars";
}
system "$cleanup";


# start mixing good and bad files.
while ($bottom != $top) {
  system "rm -f a.out";

  $cmd = "gcc ";
  for (my $i = $bottom; $i <= $middle; $i++) {
    $cmd .= "$bad_files[$i] ";
  }
  for ($i = 0; $i < $bottom; $i++) {
    $cmd .= "$good_files[$i] ";
  }
  for ($i = $middle + 1; $i <= $#good_files; $i++) {
    $cmd .= "$good_files[$i] ";
  }
  $cmd .= " $gcc_opts";
  system "$cmd";

  unless (-e "a.out") {
    die "$stars error compiling in elcor divide and conquer\n$stars\n";
  }
  system "$setup";
  system "./a.out $args >& result.out.input1";
  if (system "$check >& /dev/null") {
    $simu_failed = 1;
  } else {
    $simu_failed = 0;
  }
  system "$cleanup";

  if ($simu_failed) {
    $top = $middle;
    $middle = int(($top + $bottom)/2);
  } else {
    $bottom = $middle + 1;
    $middle = int(($top + $bottom)/2);
  }
}

# there's a problem in compiling this file: $bad_files[$top];
# try and determine what part of elcor breaks it.

$_ = $bad_files[$top];
/(.+\.)[A-Z]+_el\..*/;
my $o_tr = $1;
my $o_el = $o_tr;
$o_tr .= "$suffix" . "_tr";
$o_tr = "../../" . "$elcor_dir/" . "$o_tr";
$o_el .= "$suffix" . "_el";
my $c_file = "$o_el" . ".simu.c";


#################################################################
# test for modulo scheduling -the bane of my existence.
system "elcor -i $o_tr -o $o_el -Finput_format=lcode -Foutput_format=rebel -Fdo_modulo_scheduling=no >& /dev/null";

system "codegen -el $o_el -o $c_file -Finput_format=rebel -Femulate_virtual_regs=yes >& /dev/null";

system "gcc -c -w $c_file -I" . "$root/simu/src/Emulib";

# recompile with the new .c file
system "rm -f a.out";
my $cmd = "gcc ";
for ($i = 0; $i <= $#good_files; $i++) {
  if ($i == $top) {
    $cmd .= "$bad_files[$i] ";
  } else {
    $cmd .= "$good_files[$i] ";
  }
}

$cmd .= " $gcc_opts";
system "$cmd";

unless (-e "a.out") {
  die "$stars error compiling with no modulo scheduling\n$stars\n";
}
system "$setup";
system "./a.out $args >& result.out.input1";
#system "$check >& out";
if (system "$check >& /dev/null") {
  $simu_failed = 1;
} else {
  $simu_failed = 0;
}
system "$cleanup";

unless($simu_failed) {
  die "$stars Modulo scheduling bug in $bad_files[$top] \n$stars";
} else {
  print "Modulo scheduling ok...\n";
}

#################################################################
# test for register allocation
system "elcor -i $o_tr -o $o_el -Finput_format=lcode -Foutput_format=rebel -Fdo_scalar_regalloc=no >& /dev/null";
system "codegen -el $o_el -o $c_file -Finput_format=rebel -Femulate_virtual_regs=yes >& /dev/null";
system "gcc -c -w $c_file -I" . "$root/simu/src/Emulib";

system "rm -f a.out";
my $cmd = "gcc ";
for ($i = 0; $i <= $#good_files; $i++) {
  if ($i == $top) {
    $cmd .= "$bad_files[$i] ";
  } else {
    $cmd .= "$good_files[$i] ";
  }
}

$cmd .= " $gcc_opts";
system "$cmd";

unless (-e "a.out") {
  die "$stars error compiling with no register allocation\n";
}
system "$setup";
system "./a.out $args >& result.out.input1";
#system "cd $simu_dir; $check >& out";
if (system "$check >& /dev/null") {
  $simu_failed = 1;
} else {
  $simu_failed = 0;
}
system "$cleanup";

unless($simu_failed) {
  die "$stars Register allocation bug in $bad_files[$top] \n$stars";
} else {
  die "$stars unknown elcor bug in $bad_files[$top] \n$stars";
}

#################################################################
# add more elcor tests here.


# this function takes two types of file suffixes, the first of which works with
# Lemulate and the second of which doesn't, and then determines which file has
# the problem in it. for example, if i passed "O" and "S", it would repeatedly
# compile different combinations of the .O files with the .S files until it found
# the .S file that was causing an error. it assumes that $impact_dir is set up.
sub Lemul_divide_and_conquer
{
  my $good_suf = shift @_;
  my $bad_suf = shift @_;

  # remove *.o files from previous runs
  system "cd $impact_dir; rm -f *.o list";

  # create all the *.o files.
  system "cd $impact_dir; tar xzf $bench.$good_suf.tgz; tar xzf $bench.$bad_suf.tgz";
  opendir DIR, $impact_dir or die "couldn't oper impact dir: $!\n";
  my @good_files = grep /.*\.$good_suf$/, readdir DIR;
  closedir DIR;
  ($#good_files == -1) and die "couldn't find any *.$good_suf files in $impact_dir\n";
  opendir DIR, $impact_dir or die "couldn't oper impact dir: $!\n";
  my @bad_files = grep /.*\.$bad_suf$/, readdir DIR;
  closedir DIR;
  ($#bad_files == -1) and die "couldn't find any *.$bad_suf files in $impact_dir\n";

  # make a list of the files for Lemulate
  open LIST, ">$impact_dir/list" or die "couldn't create list file: $!\n";
  foreach (@good_files) {
    print LIST "$_\n";
  }
  foreach (@bad_files) {
    print LIST "$_\n";
  }
  close LIST;
  system "cd $impact_dir; Lemulate -i list; gcc -c -w *.c";

  # append '.o' to the file names
  ($#good_files != $#bad_files) and die "come again?!\n";
  for(my $i = 0; $i <= $#good_files; $i++) {
    $good_files[$i] .= ".o";
    $bad_files[$i] .= ".o";
  }

  # start mixing and matching.
  my $top = $#good_files;
  my $bottom = 0;
  my $middle = int(($top + $bottom)/2);
  my $lib = "$ENV{IMPACT_REL_PATH}" . "/platform/x86lin_gcc/impact_lemul_lib.o";
  while($bottom != $top) {
    system "cd $impact_dir; rm -f a.out";
    my $cmd = "gcc $lib ";
    for(my $i = $bottom; $i <= $middle; $i++) {
      $cmd .= "$bad_files[$i] ";
    }
    for($i = 0; $i < $bottom; $i++) {
      $cmd .= "$good_files[$i] ";
    }
    for($i = $middle + 1; $i <= $#good_files; $i++) {
      $cmd .= "$good_files[$i] ";
    }
    $cmd .= " -lm";
    system "cd $impact_dir; $cmd";

    unless (-e "$impact_dir/a.out") { die "$stars compiling in divide and conquer\n"; }
    system "cd $impact_dir; $setup";
    system "cd $impact_dir; ./a.out $args >& result.out.input1";
    if(system "$impact_check") {
      $Lemulate_failed = 1;
    }
    system "cd $impact_dir; $cleanup";

    if($Lemulate_failed) {
      $top = $middle;
      $middle = int(($top + $bottom)/2);
    } else {
      $bottom = $middle + 1;
      $middle = int(($top + $bottom)/2);
    }
  }

  # FIX: we could start turning off impact switches here to narrow down the problem.
  chop($good_files[$top]);
  chop($good_files[$top]);
  chop($bad_files[$top]);
  chop($bad_files[$top]);
  my $ok_file = $good_files[$top];
  my $not_ok_file = $bad_files[$top];
  die "$stars Impact error translating from $ok_file to $not_ok_file!\n$stars";
}


# this function will run Lemulate. it assumes that $impact_dir is setup and it takes
# the suffix of the files to compile as an argument. this returns 1 if it passes
# and 0 if it fails.

sub do_Lemulate
{
  my $suf = shift @_;

  # remove *.o files from previous runs
  system "cd $impact_dir; rm -f *.o list";

  # untar the files and get a list
  system "cd $impact_dir; tar xzf $bench.$suf.tgz";
  opendir DIR, $impact_dir or die "couldn't oper impact dir: $!\n";
  my @dot_O_files = grep /.*\.$suf$/, readdir DIR;
  closedir DIR;
  ($#dot_O_files == -1) and die "couldn't find any *.$suf files in $impact_dir\n";

  # make a list of the files for Lemulate
  open LIST, ">$impact_dir/list" or die "couldn't create list file: $!\n";
  foreach (@dot_O_files) {
    print LIST "$_\n";
  }
  close LIST;

  # run Lemulate
  my $lib = "$ENV{IMPACT_REL_PATH}" . "/platform/x86lin_gcc/impact_lemul_lib.o";
  system "cd $impact_dir; Lemulate -i list; gcc -c -w *.c; gcc *.$suf.o _EM_*.o $lib -lm";
  unless (-e "$impact_dir/a.out") { die "$stars compiling *.$suf files failed!\n$stars"; }

  system "cd $impact_dir; $setup";
  system "cd $impact_dir; ./a.out $args >& result.out.input1";
  if(system "cd $impact_dir; $impact_check >& /dev/null") {
    $Lemulate_failed = 1;
  }
  system "cd $impact_dir; $cleanup";

  if($Lemulate_failed) { return 0; }
  else { return 1; }
}


