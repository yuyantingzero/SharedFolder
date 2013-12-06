#!/usr/bin/perl

#	Script to run elcor through multiple input files. Updated to
#	run files in parallel if your machine has multiple CPUs.
#
#	Usage: gen_CtoO.pl #procs dir [extra args] < "list"
#
#       Nate Clark
#       2/17/07


if($#ARGV == 1) {
  $num_cpus = shift @ARGV;
  $directory = shift @ARGV;
} elsif($#ARGV == 2) {
  $num_cpus = shift @ARGV;
  $directory = shift @ARGV;
  $extra_args = shift @ARGV;
} else {
  print "> Usage gen_CtoO.pl #procs \"dir\" [\"extra_args\"] < \"list\"\n";
  print "> \n";
  print ">   #procs is the # of jobs to run in parallel\n";
  print ">   \"dir\" is location of input files\n";
  print ">   \"extra_args\" are optional additional -P and -F arguments to pass\n";
  print ">                the compiler (Put quote around a list of args, ie\n";
  print ">                '-Fnachos=rule -Fdonuts=mmmm -Fmeetings=long')\n";
  print ">   \"list\" is list of files to process\n";
  exit 200;
}


# Figure out how many CPUs are in this system. If there's a bunch,
# parallelize the compilation.

#$num_cpus = 1;
#if(-e '/proc/cpuinfo') {
#  open FILE, '/proc/cpuinfo';
#  while(<FILE>) {
#    if(/^processor\s+:\s+(\d+)/) {
#      # the CPU ids are zero relative, so add 1.
#      $num_cpus = $1 + 1;
#    }
#  }
#}

print "> Prefixing files with $directory/\n";
print "> Extra $ENV{TRIMARAN_HOST_COMPILER} arguments : $extra_args\n";

while(<>) {
  $file_list .= $_;
}

@files = split /\s+/, $file_list;

foreach $file (@files) {
  system "rm -f $directory/$file.simu.o";
}

# There's a lot of race conditions here, but none of them will cause
# logical errors in the compiler output, so I'm not going to fix them.

# Create a child for each CPU, and have the children do everything in
# the queue until they're all done.
for $id (1..$num_cpus) {
  $pid = fork();
  if($pid) {
    # parent
    push @children, $pid;
  } elsif($pid == 0) {
    # child

    # stagger the children a bit to prevent them from stepping on each
    # others' toes.
    sleep($id - 1);

    foreach $file (@files) {

      unless(-e "$directory/$file.simu.o") {
	# This touch also helps prevent children from stepping on each
	# others' toes.
	system "touch $directory/$file.simu.o";

	print "> compiling $directory/$file.simu.c\n";
	$cmd = "$ENV{TRIMARAN_HOST_COMPILER} $extra_args -c -w $directory/$file.simu.c -I$ENV{SIMU_HOME}/include/ -I$ENV{DINERO_HOME}/include/";

	if(system "$cmd") {
	  # There was an error in compilation.
	  die "> $ENV{TRIMARAN_HOST_COMPILER} failed on input file: $directory/$file.simu.c\n> Using this command: $cmd\n\n";
	}
      }
    }
    exit 0;
  } else {
    die "Error using fork in gen_CtoO.pl!\n";
  }
}

foreach (@children) {
  waitpid($_, 0);
  $exitval = $? >> 8;
  if ($exitval) {
    kill 15, @children;
    exit $exitval;
  }
}

print "> done\n";
