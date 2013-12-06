#!/usr/bin/perl

#############################################################################
# dyn_stats_combiner.pl
#
#     Takes a number 'N' of simu DYN_STATS files, each representing an
#     execution mode for an architecture. Determines the number of cycles
#     to execute the benchmark if we could switch between execution modes
#     at a region-level (basic block/hyperblock) with no cycle penalty. 
#
#     Steps:
#            (1) for each execution mode, for each block,
#                finds the maximum time that mode spends in the block
#                (max time of all the clusters)
#            (2) for each block,
#                finds the minimum time execution mode for the block
#
#############################################################################

sub check_and_clear_flag {
  local ($mode, $func, $block) = @_;

  if ($flag_map[$mode]{$func}{$block} == 1) {
    $flag_map[$mode]{$func}{$block} = 0;
  } else {
    die "function $func, $block exists in mode 0 but was not found in mode $mode";
  }
}

sub is_flag_set {
  local ($mode, $func, $block) = @_;

  return ($flag_map[$mode]{$func}{$block} == 1);
}

sub assert_flag_is_clear {
  local ($mode, $func, $block) = @_;

  if ($flag_map[$mode]{$func}{$block} == 1) {
    die "expected flag at $mode, $func, $block to be clear!";
  }
}

#############################################################################
# begin main:
#############################################################################

$num_modes = $#ARGV + 1;

print "dyn_stats_combiner:\n";
print "    operating on $num_modes file(s), each representing an execution "
. "mode\n\n";

$mode_num = 0;
foreach $simu_dyn_stats_file (@ARGV) {
  print "mode\t$mode_num\tinput file\t$simu_dyn_stats_file\n";
  $mode_num++;
}
print "\n";


# (1) read the input file for each mode, and store results in %cycles_map
$mode_num = 0;
foreach $simu_dyn_stats_file (@ARGV) {

  open(INF, $simu_dyn_stats_file);

  @lines = <INF>;
  $line_num = 1;  # keep line_num for error msgs


  foreach $line (@lines) {
    chomp($line);
    if (substr($line,0,8) eq 'Function') {  
      @spacefields = split(/ +/, $line);
      $func    = $spacefields[1];
      $file    = $spacefields[2];
      $clust   = $spacefields[5];

      $cycles_this_func_this_clust = 0;

    } elsif (substr($line,0,1) eq "\t") {   # line begins with tab
      @tabfields = split(/\t/, $line);
      $possible_total = substr($tabfields[1],0,20);
      $possible_hb_bb = substr($tabfields[2],0,2); 

      if ($possible_hb_bb eq 'hb' or $possible_hb_bb eq 'bb') {
	local $block  = $tabfields[2];
	local $cycles = $tabfields[4];

	if ($tabfields[3] ne 'dyn cyc:') {
	  die "expected line $line_num of $simu_dyn_stats_file to be "
	  . "a cycle count!";
	}

	$cycles_this_func_this_clust += $cycles;

	if ($cycles >= $cycles_map[$mode_num]{$func}{$block}) {
	  $cycles_map[$mode_num]{$func}{$block} = $cycles;
	  $flag_map[$mode_num]{$func}{$block} = 1;
	}
      } elsif ($possible_total eq 'Dynamic_total_cycles') {
	local $total_cycles = $tabfields[2];

	# we've reached the end of the blocks listed in the func,
	# make sure the summary count matches
	if ($cycles_this_func_this_clust != $total_cycles) {
	  die "cycle count mismatch: expecting $cycles_this_func_this_clust"
	  . " as total on line $line_num of $simu_dyn_stats_file!";
	}
      }
    } else {
      #ignore line
    }
    $line_num++; # keep line_num for error msgs
  }

  $total_cycles = 0;

  foreach $func (keys %{$cycles_map[$mode_num]}) {
    local %cycles_by_block = %{$cycles_map[$mode_num]{$func}};
    foreach $block (keys %cycles_by_block) {
      #print "summary: $func, $block, $cycles_by_block{$block}\n";
      $total_cycles += $cycles_by_block{$block};
    }
  }

  print "mode\t$mode_num\ttotal dynamic cycles\t$total_cycles\n";

  close(INF);
  $mode_num++;
}

print "\n";

# (2) go back through %cycles_map and find the best mode to execute each block
#     in, and collect some stats on percentage of time in each mode

$total_cycles = 0;
$conservative_cycles = 0;
for ($mode_num = 0; $mode_num < $num_modes; $mode_num++) {
  foreach $func (keys %{$cycles_map[$mode_num]}) {
    local %cycles_by_block = %{$cycles_map[$mode_num]{$func}};
    foreach $block (keys %cycles_by_block) {

      if ($mode_num == 0) {
	local $min_cycles = $cycles_map[0]{$func}{$block};
	local $min_cycles_mode = 0;

	check_and_clear_flag(0, $func, $block);

	for ($other_mode = 1; $other_mode < $num_modes; $other_mode++) {
	  local $cycles = $cycles_map[$other_mode]{$func}{$block};
	  
	  check_and_clear_flag($other_mode, $func, $block);

	  if ($cycles < $min_cycles) {
	    $min_cycles = $cycles;
	    $min_cycles_mode = $other_mode;
	  }
	}

	$cycles_spent_in_mode[$min_cycles_mode] += $min_cycles;
	$total_cycles += $min_cycles;

      } else {

	if (is_flag_set($mode_num, $func, $block)) {
	  local $cycles = $cycles_map[$mode_num]{$func}{$block};

	  check_and_clear_flag($mode_num, $func, $block);

	  #print "warning: conservatively including $block in total execution "
	  #. "time (appears in mode $mode_num but not mode 0), cycles:\t"
	  #. "$cycles\n";
	  $cycles_spent_in_mode[$mode_num] += $cycles;
	  $total_cycles += $cycles;
	  $conservative_cycles += $cycles
	}
      }
    }
  }
}


print "\n";
for ($mode_num = 0; $mode_num < $num_modes; $mode_num++) {
  local $cycles = $cycles_spent_in_mode[$mode_num];
  print "mode\t$mode_num\tcycles spent in mode\t$cycles\n";
}
print "\n";
print "conservative cycles\t$conservative_cycles\n";
print "total combined mode cycles\t$total_cycles\n";


#
# end of script
#
