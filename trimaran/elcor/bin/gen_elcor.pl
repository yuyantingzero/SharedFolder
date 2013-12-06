#!/usr/bin/perl

#	Script to run elcor through multiple input files. Updated to
#	run files in parallel if your machine has multiple CPUs.
#
#	Usage: gen_elcor.pl procs dir [file_ext [extra args]] < "list"
#
#       Nate Clark
#       2/17/07


if($#ARGV == 1) {
  $num_cpus = shift @ARGV;
  $directory = shift @ARGV;
} elsif($#ARGV == 2) {
  $num_cpus = shift @ARGV;
  $directory = shift @ARGV;
  $file_ext = shift @ARGV;
} elsif($#ARGV == 3) {
  $num_cpus = shift @ARGV;
  $directory = shift @ARGV;
  $file_ext = shift @ARGV;
  $extra_args = shift @ARGV;
} else {
  print "> Usage gen_elcor.pl #procs \"dir\" [\"file_ext\" [\"extra_args\"]] < \"list\"\n";
  print "> \n";
  print ">   #procs is the # of jobs to run in parallel\n";
  print ">   \"dir\" is location of input files\n";
  print ">   \"file_ext\" is the suffix for compiled files\n";
  print ">   \"extra_args\" are optional additional -P and -F arguments to pass\n";
  print ">                the compiler (Put quote around a list of args, ie\n";
  print ">                '-Fnachos=rule -Fdonuts=mmmm -Fmeetings=long')\n";
  print ">   \"list\" is list of files to process\n";
  exit 200;
}

if(-e 'ELCOR_STATS') {
  system "rm -f ELCOR_STATS";
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
print "> Extra elcor arguments : $extra_args\n";

while(<>) {
  $file_list .= $_;
}

@files = split /\s+/, $file_list;

foreach $file (@files) {
  $file =~ /^([^\.]+)\./;
  $out_file_name = $1;
  system "rm -f $directory/$out_file_name.$file_ext";
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
      $file =~ /^([^\.]+)\./;
      $out_file_name = $1;

      unless(-e "$directory/$out_file_name.$file_ext") {
	print "> processing $directory/$file\n";
	$cmd = "elcor $extra_args -i $directory/$file -o $directory/$out_file_name.$file_ext";

	# If there's multiple compilations going on at a time, the
	# output is impossible to read.
	if($num_cpus > 1) {
	  $cmd .= " >& /dev/null";
	}

	if(system "$cmd") {
	  # There was an error in compilation.
	  die "> elcor failed on input file: $file\n> Using this command: $cmd\n\n";
	}
      }
    }
    exit 0;
  } else {
    die "Error using fork in gen_elcor.pl!\n";
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
