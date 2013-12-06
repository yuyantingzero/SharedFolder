
/*=============================================================================*/
/*	Descript: Sum up stats produced by elcor                               */
/*                                                                             */
/*	Written by: Scott Mahlke                                               */
/*	Date: Feb 1996                                                         */
/*=============================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "library/dynamic_symbol.h"


#define DEFAULT_INPUT_FILE	"ELCOR_STATS"
#define DEFAULT_OUTPUT_FILE	"stdout"
#define DEFAULT_DO_TOTAL	0
#define DEFAULT_DO_CYCLE_RANK	0
#define DEFAULT_TOP		0
#define DEFAULT_ZERO		0
#define DEFAULT_NONZERO		0

#define PERCENT_DIVISION	1
#define DEFAULT_CUM_PERCENT	99.0

#undef DEBUG

FILE *INPUT_FILE=NULL, *OUTPUT_FILE=NULL;
double target_cum_percent = 0.0;

static void punt(char *msg)
{
    fprintf(stderr, "Sumstat failed\n");
    fprintf(stderr, "%s\n", msg);
    fflush(stderr);
    exit(0);
}

void open_files(char *input_file_name, char *output_file_name)
{
    if (! strcmp(input_file_name, "stdin"))
	INPUT_FILE = stdin;
    else if (! strcmp(input_file_name, "cin"))
	INPUT_FILE = stdin;
    else
	INPUT_FILE = fopen(input_file_name, "r");
    if (INPUT_FILE == NULL)
	punt("open_files: cannot open input file");

    if (! strcmp(output_file_name, "stdout"))
	OUTPUT_FILE = stdout;
    else if (! strcmp(output_file_name, "cout"))
	OUTPUT_FILE = stdout;
    else if (! strcmp(output_file_name, "stderr"))
	OUTPUT_FILE = stderr;
    else if (! strcmp(output_file_name, "cerr"))
	OUTPUT_FILE = stderr;
    else
	OUTPUT_FILE = fopen(output_file_name, "w");
    if (OUTPUT_FILE == NULL)
	punt("open_files: cannot open output file");
}

void close_files(void)
{
}

char **func_name = NULL;
char **file_name = NULL;

double *compute_cycles=NULL;
double *s_compute_cycles=NULL;
double *l_compute_cycles=NULL;
double *dcache_stall_cycles=NULL;
double *bmpred_stall_cycles=NULL;
double *total_cycles=NULL;

double sum_compute_cycles=0.0;
double sum_s_compute_cycles=0.0;
double sum_l_compute_cycles=0.0;
double sum_dcache_stall_cycles=0.0;
double sum_bmpred_stall_cycles=0.0;
double sum_total_cycles=0.0;

double *total_ops=NULL;
double *branch_ops=NULL;
double *load_ops=NULL;
double *store_ops=NULL;
double *ialu_ops=NULL;
double *falu_ops=NULL;
double *icmpp_ops=NULL;
double *fcmpp_ops=NULL;
double *pclear_ops=NULL;
double *pbr_ops=NULL;
double *regalloc_ops=NULL;
double *spill_ops=NULL;
double *caller_ops=NULL;
double *callee_ops=NULL;
double *iw_move_ops=NULL;
double *win_swap_ops=NULL;
double *icmove_ops=NULL;
double *bcast_ops=NULL;
double *wake_ops=NULL;

double sum_total_ops=0.0;
double sum_branch_ops=0.0;
double sum_load_ops=0.0;
double sum_store_ops=0.0;
double sum_ialu_ops=0.0;
double sum_falu_ops=0.0;
double sum_icmpp_ops=0.0;
double sum_fcmpp_ops=0.0;
double sum_pclear_ops=0.0;
double sum_pbr_ops=0.0;
double sum_regalloc_ops=0.0;
double sum_spill_ops=0.0;
double sum_caller_ops=0.0;
double sum_callee_ops=0.0;
double sum_iw_move_ops=0.0;
double sum_win_swap_ops=0.0;
double sum_icmove_ops=0.0;
double sum_bcast_ops=0.0;
double sum_wake_ops=0.0;

double *static_total_ops=NULL;
double *static_branch_ops=NULL;
double *static_load_ops=NULL;
double *static_store_ops=NULL;
double *static_ialu_ops=NULL;
double *static_falu_ops=NULL;
double *static_icmpp_ops=NULL;
double *static_fcmpp_ops=NULL;
double *static_pclear_ops=NULL;
double *static_pbr_ops=NULL;
double *static_regalloc_ops=NULL;
double *static_spill_ops=NULL;
double *static_caller_ops=NULL;
double *static_callee_ops=NULL;
double *static_iw_move_ops=NULL;
double *static_win_swap_ops=NULL;
double *static_icmove_ops=NULL;
double *static_bcast_ops=NULL;
double *static_wake_ops=NULL;

double sum_static_total_ops=0.0;
double sum_static_branch_ops=0.0;
double sum_static_load_ops=0.0;
double sum_static_store_ops=0.0;
double sum_static_ialu_ops=0.0;
double sum_static_falu_ops=0.0;
double sum_static_icmpp_ops=0.0;
double sum_static_fcmpp_ops=0.0;
double sum_static_pclear_ops=0.0;
double sum_static_pbr_ops=0.0;
double sum_static_regalloc_ops=0.0;
double sum_static_spill_ops=0.0;
double sum_static_caller_ops=0.0;
double sum_static_callee_ops=0.0;
double sum_static_iw_move_ops=0.0;
double sum_static_win_swap_ops=0.0;
double sum_static_icmove_ops=0.0;
double sum_static_bcast_ops=0.0;
double sum_static_wake_ops=0.0;


void alloc_arrays(int size)
{
    func_name = (char **) malloc(sizeof(char *)*size);
    file_name = (char **) malloc(sizeof(char *)*size);

    compute_cycles = (double *) calloc(size, sizeof(double));
    s_compute_cycles = (double *) calloc(size, sizeof(double));
    l_compute_cycles = (double *) calloc(size, sizeof(double));
    dcache_stall_cycles = (double *) calloc(size, sizeof(double));
    bmpred_stall_cycles = (double *) calloc(size, sizeof(double));
    total_cycles = (double *) calloc(size, sizeof(double));

    total_ops = (double *) calloc(size, sizeof(double));
    branch_ops = (double *) calloc(size, sizeof(double));
    load_ops = (double *) calloc(size, sizeof(double));
    store_ops = (double *) calloc(size, sizeof(double));
    ialu_ops = (double *) calloc(size, sizeof(double));
    falu_ops = (double *) calloc(size, sizeof(double));
    icmpp_ops = (double *) calloc(size, sizeof(double));
    fcmpp_ops = (double *) calloc(size, sizeof(double));
    pclear_ops = (double *) calloc(size, sizeof(double));
    pbr_ops = (double *) calloc(size, sizeof(double));
    regalloc_ops = (double *) calloc(size, sizeof(double));
    spill_ops = (double *) calloc(size, sizeof(double));
    caller_ops = (double *) calloc(size, sizeof(double));
    callee_ops = (double *) calloc(size, sizeof(double));
    iw_move_ops = (double *) calloc(size, sizeof(double));
    win_swap_ops = (double *) calloc(size, sizeof(double));
    icmove_ops = (double *) calloc(size, sizeof(double));
    bcast_ops = (double *) calloc(size, sizeof(double));
    wake_ops = (double *) calloc(size, sizeof(double));

    static_total_ops = (double *) calloc(size, sizeof(double));
    static_branch_ops = (double *) calloc(size, sizeof(double));
    static_load_ops = (double *) calloc(size, sizeof(double));
    static_store_ops = (double *) calloc(size, sizeof(double));
    static_ialu_ops = (double *) calloc(size, sizeof(double));
    static_falu_ops = (double *) calloc(size, sizeof(double));
    static_icmpp_ops = (double *) calloc(size, sizeof(double));
    static_fcmpp_ops = (double *) calloc(size, sizeof(double));
    static_pclear_ops = (double *) calloc(size, sizeof(double));
    static_pbr_ops = (double *) calloc(size, sizeof(double));
    static_regalloc_ops = (double *) calloc(size, sizeof(double));
    static_spill_ops = (double *) calloc(size, sizeof(double));
    static_caller_ops = (double *) calloc(size, sizeof(double));
    static_callee_ops = (double *) calloc(size, sizeof(double));
    static_iw_move_ops = (double *) calloc(size, sizeof(double));
    static_win_swap_ops = (double *) calloc(size, sizeof(double));
    static_icmove_ops = (double *) calloc(size, sizeof(double));
    static_bcast_ops = (double *) calloc(size, sizeof(double));
    static_wake_ops = (double *) calloc(size, sizeof(double));

}

int read_cycle_data(int index, char *buf)
{
    char tmp1[4096];
    double val;

    sscanf(buf, "%s %lf", tmp1, &val);

    if (! strcmp(tmp1, "Dynamic_total_cycles:")) {
	total_cycles[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_total_compute_cycles:")) {
	compute_cycles[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_scalar_compute_cycles:")) {
	s_compute_cycles[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_loop_compute_cycles:")) {
	l_compute_cycles[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_bmpred_stall_cycles:")) {
	bmpred_stall_cycles[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_dcache_stall_cycles:")) {
	dcache_stall_cycles[index] = val;
	return (1);
    }

    return (0);
}

int read_op_data(int index, char *buf)
{
    char tmp1[4096];
    double val;

    sscanf(buf, "%s %lf", tmp1, &val);

    if (! strcmp(tmp1, "Dynamic_total_operations:")) {
	total_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_branch:")) {
	branch_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_load:")) {
	load_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_store:")) {
	store_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_ialu:")) {
	ialu_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_falu:")) {
	falu_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_icmpp:")) {
	icmpp_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_fcmpp:")) {
	fcmpp_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_pclear:")) {
	pclear_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_pbr:")) {
	pbr_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_regalloc_op_overhead:")) {
	regalloc_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_spill_code:")) {
	spill_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_caller_save:")) {
	caller_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_callee_save:")) {
	callee_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_iw_moves:")) {
	iw_move_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_win_swaps:")) {
	win_swap_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_icmove:")) {
	icmove_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_bcast:")) {
	bcast_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Dynamic_wake:")) {
	wake_ops[index] = val;
	return (1);
    }

    return (0);
}

int read_static_op_data(int index, char *buf)
{
    char tmp1[4096];
    double val;

    sscanf(buf, "%s %lf", tmp1, &val);

    if (! strcmp(tmp1, "Static_total_operations:")) {
	static_total_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Static_branch:")) {
	static_branch_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Static_load:")) {
	static_load_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Static_store:")) {
	static_store_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Static_ialu:")) {
	static_ialu_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Static_falu:")) {
	static_falu_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Static_icmpp:")) {
	static_icmpp_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Static_fcmpp:")) {
	static_fcmpp_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Static_pclear:")) {
	static_pclear_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Static_pbr:")) {
	static_pbr_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Static_regalloc_op_overhead:")) {
	static_regalloc_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Static_spill_code:")) {
	static_spill_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Static_caller_save:")) {
	static_caller_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Static_callee_save:")) {
	static_callee_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Static_iw_move:")) {
	static_iw_move_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Static_win_swap:")) {
	static_win_swap_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Static_icmove:")) {
	static_icmove_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Static_bcast:")) {
	static_bcast_ops[index] = val;
	return (1);
    }

    if (! strcmp(tmp1, "Static_wake:")) {
	static_wake_ops[index] = val;
	return (1);
    }

    return (0);
}

void compute_cycle_totals(int count)
{
    int i;
    for (i=0; i<count; i++) {
	sum_compute_cycles += compute_cycles[i];
	sum_s_compute_cycles += s_compute_cycles[i];
	sum_l_compute_cycles += l_compute_cycles[i];
	sum_dcache_stall_cycles += dcache_stall_cycles[i];
	sum_bmpred_stall_cycles += bmpred_stall_cycles[i];
	sum_total_cycles += total_cycles[i];
    }
}

void compute_op_totals(int count)
{
    int i;
    for (i=0; i<count; i++) {
	sum_total_ops += total_ops[i];
	sum_branch_ops += branch_ops[i];
	sum_load_ops += load_ops[i];
	sum_store_ops += store_ops[i];
	sum_ialu_ops += ialu_ops[i];
	sum_falu_ops += falu_ops[i];
	sum_icmpp_ops += icmpp_ops[i];
	sum_fcmpp_ops += fcmpp_ops[i];
	sum_pclear_ops += pclear_ops[i];
	sum_pbr_ops += pbr_ops[i];
	sum_regalloc_ops += regalloc_ops[i];
	sum_spill_ops += spill_ops[i];
	sum_caller_ops += caller_ops[i];
	sum_callee_ops += callee_ops[i];
        sum_iw_move_ops += iw_move_ops[i];
        sum_win_swap_ops += win_swap_ops[i];
        sum_icmove_ops += icmove_ops[i];
        sum_bcast_ops += bcast_ops[i];
        sum_wake_ops += wake_ops[i];
    }
}

void compute_static_op_totals(int count)
{
    int i;
    for (i=0; i<count; i++) {
        sum_static_total_ops += static_total_ops[i];
        sum_static_branch_ops += static_branch_ops[i];
        sum_static_load_ops += static_load_ops[i];
        sum_static_store_ops += static_store_ops[i];
        sum_static_ialu_ops += static_ialu_ops[i];
        sum_static_falu_ops += static_falu_ops[i];
        sum_static_icmpp_ops += static_icmpp_ops[i];
        sum_static_fcmpp_ops += static_fcmpp_ops[i];
        sum_static_pclear_ops += static_pclear_ops[i];
        sum_static_pbr_ops += static_pbr_ops[i];
        sum_static_regalloc_ops += static_regalloc_ops[i];
        sum_static_spill_ops += static_spill_ops[i];
        sum_static_caller_ops += static_caller_ops[i];
        sum_static_callee_ops += static_callee_ops[i];
        sum_static_iw_move_ops += static_iw_move_ops[i];
        sum_static_win_swap_ops += static_win_swap_ops[i];
        sum_static_icmove_ops += static_icmove_ops[i];
        sum_static_bcast_ops += static_bcast_ops[i];
        sum_static_wake_ops += static_wake_ops[i];
    }
}

void sort_by_compute_cycles(int count)
{
    int i, j;
    double dtmp1, dtmp2, dtmp3, dtmp4, dtmp5, dtmp6, dtmp7;
    char *ctmp1, *ctmp2;

    for (i=0; i<count; i++) {
        for (j=i; j<count; j++) {
	    if (compute_cycles[i] < compute_cycles[j]) {
		dtmp1 = compute_cycles[i];
		dtmp2 = s_compute_cycles[i];
		dtmp3 = l_compute_cycles[i];
		dtmp4 = dcache_stall_cycles[i];
		dtmp5 = total_cycles[i];
		dtmp6 = total_ops[i];
		dtmp7 = bmpred_stall_cycles[i];
		ctmp1 = func_name[i];
		ctmp2 = file_name[i];

		compute_cycles[i] = compute_cycles[j];
		s_compute_cycles[i] = s_compute_cycles[j];
		l_compute_cycles[i] = l_compute_cycles[j];
		dcache_stall_cycles[i] = dcache_stall_cycles[j];
		total_cycles[i] = total_cycles[j];
		total_ops[i] = total_ops[j];
		bmpred_stall_cycles[i] = bmpred_stall_cycles[j];
		func_name[i] = func_name[j];
		file_name[i] = file_name[j];

		compute_cycles[j] = dtmp1;
		s_compute_cycles[j] = dtmp2;
		l_compute_cycles[j] = dtmp3;
		dcache_stall_cycles[j] = dtmp4;
		total_cycles[j] = dtmp5;
		total_ops[j] = dtmp6;
		bmpred_stall_cycles[j] = dtmp7;
		func_name[j] = ctmp1;
		file_name[j] = ctmp2;
            }
        }
    }
}

void print_cycle_totals(void)
{
    double s_ratio, l_ratio, compute_ratio, dcache_ratio, bmpred_ratio;
    if (sum_total_cycles>0) {
	s_ratio = sum_s_compute_cycles/sum_total_cycles;
	l_ratio = sum_l_compute_cycles/sum_total_cycles;
	compute_ratio = sum_compute_cycles/sum_total_cycles;
	dcache_ratio = sum_dcache_stall_cycles/sum_total_cycles;
	bmpred_ratio = sum_bmpred_stall_cycles/sum_total_cycles;
    }
    else {
	s_ratio = l_ratio = compute_ratio = dcache_ratio = bmpred_ratio = 0.0;
    }

    fprintf(OUTPUT_FILE, "    Dynamic_total_cycles:  %8.2f\n",
					sum_total_cycles);
    fprintf(OUTPUT_FILE, "        Dynamic_total_compute_cycles:  %8.2f  (%3.2f)\n",
					sum_compute_cycles, compute_ratio);
    fprintf(OUTPUT_FILE, "            Dynamic_scalar_compute_cycles:  %8.2f  (%3.2f)\n",
					sum_s_compute_cycles, s_ratio);
    fprintf(OUTPUT_FILE, "            Dynamic_loop_compute_cycles:  %8.2f  (%3.2f)\n",
					sum_l_compute_cycles, l_ratio);
    fprintf(OUTPUT_FILE, "        Dynamic_dcache_stall_cycles:  %8.2f  (%3.2f)\n",
					sum_dcache_stall_cycles, dcache_ratio);
    fprintf(OUTPUT_FILE, "        Dynamic_bmpred_stall_cycles:  %8.2f  (%3.2f)\n",
					sum_bmpred_stall_cycles, bmpred_ratio);
}

void print_op_totals(void)
{
    double branch_ratio, load_ratio, store_ratio, ialu_ratio, falu_ratio,
		icmpp_ratio, fcmpp_ratio, pbr_ratio, opc, regalloc_ratio,
		spill_ratio, caller_ratio, callee_ratio, pclear_ratio,
                iw_move_ratio, win_swap_ratio, icmove_ratio, bcast_ratio, wake_ratio;

    if (sum_total_ops>0) {
	branch_ratio = sum_branch_ops / sum_total_ops;
	load_ratio = sum_load_ops / sum_total_ops;
	store_ratio = sum_store_ops / sum_total_ops;
	ialu_ratio = sum_ialu_ops / sum_total_ops;
	falu_ratio = sum_falu_ops / sum_total_ops;
	icmpp_ratio = sum_icmpp_ops / sum_total_ops;
	fcmpp_ratio = sum_fcmpp_ops / sum_total_ops;
	pclear_ratio = sum_pclear_ops / sum_total_ops;
	pbr_ratio = sum_pbr_ops / sum_total_ops;
	regalloc_ratio = sum_regalloc_ops / sum_total_ops;
	spill_ratio = sum_spill_ops / sum_total_ops;
	caller_ratio = sum_caller_ops / sum_total_ops;
	callee_ratio = sum_callee_ops / sum_total_ops;
        iw_move_ratio = sum_iw_move_ops / sum_total_ops;
        win_swap_ratio = sum_win_swap_ops / sum_total_ops;
        icmove_ratio = sum_icmove_ops / sum_total_ops;
        bcast_ratio = sum_bcast_ops / sum_total_ops;
        wake_ratio = sum_wake_ops / sum_total_ops;
    }
    else {
        branch_ratio = load_ratio = store_ratio = ialu_ratio = falu_ratio = 0.0;
	icmpp_ratio = pbr_ratio = regalloc_ratio = spill_ratio = caller_ratio = 0.0;
	fcmpp_ratio = callee_ratio = pclear_ratio = 0.0;
	icmove_ratio = bcast_ratio = wake_ratio = 0.0;
    }

    if (sum_total_cycles>0)
	opc = sum_total_ops / sum_total_cycles;
    else
	opc = 0.0;

    fprintf(OUTPUT_FILE, "    Dynamic_total_operations:  %8.2f\n",
					sum_total_ops);
    fprintf(OUTPUT_FILE, "        Dynamic_branch:  %8.2f  (%3.2f)\n",
					sum_branch_ops, branch_ratio);
    fprintf(OUTPUT_FILE, "        Dynamic_load:  %8.2f  (%3.2f)\n",
					sum_load_ops, load_ratio);
    fprintf(OUTPUT_FILE, "        Dynamic_store:  %8.2f  (%3.2f)\n",
					sum_store_ops, store_ratio);
    fprintf(OUTPUT_FILE, "        Dynamic_ialu:  %8.2f  (%3.2f)\n",
					sum_ialu_ops, ialu_ratio);
    fprintf(OUTPUT_FILE, "        Dynamic_falu:  %8.2f  (%3.2f)\n",
					sum_falu_ops, falu_ratio);
    fprintf(OUTPUT_FILE, "        Dynamic_icmpp:  %8.2f  (%3.2f)\n",
					sum_icmpp_ops, icmpp_ratio);
    fprintf(OUTPUT_FILE, "        Dynamic_fcmpp:  %8.2f  (%3.2f)\n",
					sum_fcmpp_ops, fcmpp_ratio);
    fprintf(OUTPUT_FILE, "        Dynamic_pclear:  %8.2f  (%3.2f)\n",
					sum_pclear_ops, pclear_ratio);
    fprintf(OUTPUT_FILE, "        Dynamic_pbr:  %8.2f  (%3.2f)\n",
					sum_pbr_ops, pbr_ratio);
    fprintf(OUTPUT_FILE, "        Dynamic_regalloc_op_overhead:  %8.2f  (%3.2f)\n",
					sum_regalloc_ops, regalloc_ratio);
    fprintf(OUTPUT_FILE, "            Dynamic_spill_code:  %8.2f  (%3.2f)\n",
					sum_spill_ops, spill_ratio);
    fprintf(OUTPUT_FILE, "            Dynamic_caller_save:  %8.2f  (%3.2f)\n",
					sum_caller_ops, caller_ratio);
    fprintf(OUTPUT_FILE, "            Dynamic_callee_save:  %8.2f  (%3.2f)\n",
					sum_callee_ops, callee_ratio);
    fprintf(OUTPUT_FILE, "            Dynamic_iw_moves:  %8.2f  (%3.2f)\n",
					sum_iw_move_ops, iw_move_ratio);
    fprintf(OUTPUT_FILE, "            Dynamic_win_swaps:  %8.2f  (%3.2f)\n",
					sum_win_swap_ops, win_swap_ratio);
    fprintf(OUTPUT_FILE, "        Dynamic_icmove:  %8.2f  (%3.2f)\n",
					sum_icmove_ops, icmove_ratio);
    fprintf(OUTPUT_FILE, "        Dynamic_bcast:  %8.2f  (%3.2f)\n",
					sum_bcast_ops, bcast_ratio);
    fprintf(OUTPUT_FILE, "        Dynamic_wake:  %8.2f  (%3.2f)\n",
					sum_wake_ops, wake_ratio);
    fprintf(OUTPUT_FILE, "    Dynamic_average_issued_ops/cycle:  %8.2f\n",
					opc);
}

void print_static_op_totals(void)
{
    double branch_ratio, load_ratio, store_ratio, ialu_ratio, falu_ratio,
		icmpp_ratio, pbr_ratio, regalloc_ratio, spill_ratio, caller_ratio,
		fcmpp_ratio, callee_ratio, pclear_ratio, iw_move_ratio, win_swap_ratio,
		icmove_ratio, bcast_ratio, wake_ratio;

    if (sum_static_total_ops>0) {
	branch_ratio = sum_static_branch_ops / sum_static_total_ops;
	load_ratio = sum_static_load_ops / sum_static_total_ops;
	store_ratio = sum_static_store_ops / sum_static_total_ops;
	ialu_ratio = sum_static_ialu_ops / sum_static_total_ops;
	falu_ratio = sum_static_falu_ops / sum_static_total_ops;
	icmpp_ratio = sum_static_icmpp_ops / sum_static_total_ops;
	fcmpp_ratio = sum_static_fcmpp_ops / sum_static_total_ops;
	pclear_ratio = sum_static_pclear_ops / sum_static_total_ops;
	pbr_ratio = sum_static_pbr_ops / sum_static_total_ops;
	regalloc_ratio = sum_static_regalloc_ops / sum_static_total_ops;
	spill_ratio = sum_static_spill_ops / sum_static_total_ops;
	caller_ratio = sum_static_caller_ops / sum_static_total_ops;
	callee_ratio = sum_static_callee_ops / sum_static_total_ops;
        iw_move_ratio = sum_static_iw_move_ops / sum_static_total_ops;
        win_swap_ratio = sum_static_win_swap_ops / sum_static_total_ops;
        icmove_ratio = sum_static_icmove_ops / sum_static_total_ops;
        bcast_ratio = sum_static_bcast_ops / sum_static_total_ops;
        wake_ratio = sum_static_wake_ops / sum_static_total_ops;
    }
    else {
        branch_ratio = load_ratio = store_ratio = ialu_ratio = falu_ratio = 0.0;
	icmpp_ratio = pbr_ratio = regalloc_ratio = spill_ratio = caller_ratio = 0.0;
	fcmpp_ratio = callee_ratio = pclear_ratio = 0.0;
	icmove_ratio = bcast_ratio = wake_ratio = 0.0;
    }

    fprintf(OUTPUT_FILE, "    Static_total_operations:  %8.2f\n",
					sum_static_total_ops);
    fprintf(OUTPUT_FILE, "        Static_branch:  %8.2f  (%3.2f)\n",
					sum_static_branch_ops, branch_ratio);
    fprintf(OUTPUT_FILE, "        Static_load:  %8.2f  (%3.2f)\n",
					sum_static_load_ops, load_ratio);
    fprintf(OUTPUT_FILE, "        Static_store:  %8.2f  (%3.2f)\n",
					sum_static_store_ops, store_ratio);
    fprintf(OUTPUT_FILE, "        Static_ialu:  %8.2f  (%3.2f)\n",
					sum_static_ialu_ops, ialu_ratio);
    fprintf(OUTPUT_FILE, "        Static_falu:  %8.2f  (%3.2f)\n",
					sum_static_falu_ops, falu_ratio);
    fprintf(OUTPUT_FILE, "        Static_icmpp:  %8.2f  (%3.2f)\n",
					sum_static_icmpp_ops, icmpp_ratio);
    fprintf(OUTPUT_FILE, "        Static_fcmpp:  %8.2f  (%3.2f)\n",
					sum_static_fcmpp_ops, fcmpp_ratio);
    fprintf(OUTPUT_FILE, "        Static_pclear:  %8.2f  (%3.2f)\n",
					sum_static_pclear_ops, pclear_ratio);
    fprintf(OUTPUT_FILE, "        Static_pbr:  %8.2f  (%3.2f)\n",
					sum_static_pbr_ops, pbr_ratio);
    fprintf(OUTPUT_FILE, "        Static_regalloc_op_overhead:  %8.2f  (%3.2f)\n",
					sum_static_regalloc_ops, regalloc_ratio);
    fprintf(OUTPUT_FILE, "            Static_spill_code:  %8.2f  (%3.2f)\n",
					sum_static_spill_ops, spill_ratio);
    fprintf(OUTPUT_FILE, "            Static_caller_save:  %8.2f  (%3.2f)\n",
					sum_static_caller_ops, caller_ratio);
    fprintf(OUTPUT_FILE, "            Static_callee_save:  %8.2f  (%3.2f)\n",
					sum_static_callee_ops, callee_ratio);
    fprintf(OUTPUT_FILE, "            Static_iw_move:  %8.2f  (%3.2f)\n",
					sum_static_iw_move_ops, iw_move_ratio);
    fprintf(OUTPUT_FILE, "            Static_win_swap:  %8.2f  (%3.2f)\n",
					sum_static_win_swap_ops, win_swap_ratio);
    fprintf(OUTPUT_FILE, "        Static_icmove:  %8.2f  (%3.2f)\n",
					sum_static_icmove_ops, icmove_ratio);
    fprintf(OUTPUT_FILE, "        Static_bcast:  %8.2f  (%3.2f)\n",
					sum_static_bcast_ops, bcast_ratio);
    fprintf(OUTPUT_FILE, "        Static_wake:  %8.2f  (%3.2f)\n",
					sum_static_wake_ops, wake_ratio);
}

void print_compute_cycle_rank(int count)
{
    int i, flag;
    double cum_percent;

    fprintf(OUTPUT_FILE, "[All]\n");
    fprintf(OUTPUT_FILE, "\tTotal_compute_cycles = \t%15.1f\n", sum_compute_cycles);
    if (sum_compute_cycles>0)
        fprintf(OUTPUT_FILE, "\tOps/cycle = \t\t%15.2f\n",
                        (sum_total_ops/sum_compute_cycles));
    else
        fprintf(OUTPUT_FILE, "\tOps/cycle = \t\t%15.2f\n", 0.0);


    flag = 0;
    cum_percent = 0.0;
    for (i=0; i<count; i++) {
        double percent;
        percent = 100.0*compute_cycles[i]/sum_compute_cycles;
	cum_percent += percent;
        if ((! flag) && (percent < PERCENT_DIVISION)) {
            flag = 1;
            fprintf(OUTPUT_FILE, "*********  Less than %d percent execution below *********\n",
                        PERCENT_DIVISION);
        }
        fprintf(OUTPUT_FILE, "[%3d]\t%s  (%s)\n", i+1, func_name[i], file_name[i]);
        fprintf(OUTPUT_FILE, "\tTotal_compute_cycles = \t%15.1f (%4.1f %%) (%4.1f %%)\n",
                                        compute_cycles[i],
                                        percent,
					cum_percent);
        if (compute_cycles[i]>0.0)
            fprintf(OUTPUT_FILE, "\tOps/cycle = \t\t%15.2f\n",
                        (total_ops[i]/compute_cycles[i]));
        else
            fprintf(OUTPUT_FILE, "\tOps/cycle = \t\t%15.2f\n", 0.0);
    }
}

void print_top_functions(int count)
{
    double percent, cum_percent;
    int i, num_fn;

    num_fn = 0;
    cum_percent = 0.0;
    for (i=0; i<count; i++) {
	percent = 100.0*compute_cycles[i]/sum_compute_cycles;
	cum_percent += percent;
	num_fn++;
	if (cum_percent >= target_cum_percent)
            break;
    }


    fprintf(OUTPUT_FILE, "# %d / %d functions cover %4.2f%% of the dynamic execution\n",
		num_fn, count, cum_percent);

    cum_percent = 0.0;
    for (i=0; i<count; i++) {
	percent = 100.0*compute_cycles[i]/sum_compute_cycles;
	cum_percent += percent;
	fprintf(OUTPUT_FILE, "%s (%4.2f %%)\n",
			func_name[i], cum_percent);
	if (cum_percent >= target_cum_percent)
	    break;
    }
}

void print_zero_functions(int count)
{
    int i, num_fn;

    num_fn = 0;
    for (i=0; i<count; i++) {
	if (compute_cycles[i] > 0.0)
            continue;
	num_fn++;
    }
    fprintf(OUTPUT_FILE, "# %d / %d functions are not executed\n",
		num_fn, count);

    for (i=0; i<count; i++) {
	if (compute_cycles[i] > 0.0)
	    continue;
	fprintf(OUTPUT_FILE, "%s (%s)\n", func_name[i], file_name[i]);
    }
}

void print_nonzero_functions(int count)
{
    int i, num_fn;

    num_fn = 0;
    for (i=0; i<count; i++) {
        if (compute_cycles[i] <= 0.0)
            continue;
        num_fn++;
    }
    fprintf(OUTPUT_FILE, "# %d / %d functions are executed\n",
                num_fn, count);

    for (i=0; i<count; i++) {
        if (compute_cycles[i] <= 0.0)
            continue;
        fprintf(OUTPUT_FILE, "%s (%s)\n", func_name[i], file_name[i]);
    }
}

void print_zero_weight_files(int count)
{
    int i;
    double *cycle_array, *d;
    STRING_Symbol_Table *tbl;
    STRING_Symbol *sym;

    cycle_array = (double *) malloc(sizeof(double)*count);
    tbl = STRING_new_symbol_table("filenames", 200);

    for (i=0; i<count; i++) {
	sym = STRING_find_symbol(tbl, file_name[i]);
	if (sym == NULL) {
	    cycle_array[i] = total_cycles[i];
	    sym = STRING_add_symbol(tbl, file_name[i], (void *)&cycle_array[i]);
	}
	else {
	    d = (double *) sym->data;
	    *d += total_cycles[i];
	}
    }

    for (i=0; i<count; i++) {
	sym = STRING_find_symbol(tbl, file_name[i]);
	d = (double *) sym->data;
	if (*d == 0.0)
	    fprintf(OUTPUT_FILE, "%s\n", file_name[i]);
        *d = -1.0;
    }

    free(cycle_array);
    STRING_delete_symbol_table(tbl, NULL);
}

int main(int argc, char **argv)
{
    char *input_file_name, *output_file_name, buf[4096], tmp1[4096], tmp2[4096];
    int i, len, do_cycle_rank, do_total, do_help, count;
    int do_top, do_nonzero, do_zero;

    input_file_name = DEFAULT_INPUT_FILE;
    output_file_name = DEFAULT_OUTPUT_FILE;
    target_cum_percent = DEFAULT_CUM_PERCENT;
    do_total = DEFAULT_DO_TOTAL;
    do_cycle_rank = DEFAULT_DO_CYCLE_RANK;
    do_top = DEFAULT_TOP;
    do_zero = DEFAULT_ZERO;
    do_nonzero = DEFAULT_NONZERO;
    do_help = 0;

    /*
     *	parse arguments
     */
    for (i=1; i<argc; ) {
        if (! strcmp(argv[i], "-i")) {
            if (argv[i+1]==NULL)
                punt("missing input file name after -i");
            input_file_name = argv[i+1];
            i+=2;
        }
        else if (! strcmp(argv[i], "-o")) {
            if (argv[i+1]==NULL)
                punt("missing output file name after -o");
            output_file_name = argv[i+1];
            i+=2;
        }
	else if (!strcmp(argv[i], "-total")) {
	    do_total = 1;
	    i+=1;
	}
	else if (!strcmp(argv[i], "-rank")) {
	    do_cycle_rank = 1;
	    i+=1;
	}
	else if (!strcmp(argv[i], "-top")) {
            if (argv[i+1]==NULL)
                punt("missing percent value after -top");
	    do_top = 1;
	    target_cum_percent = strtod(argv[i+1], NULL);
	    i+=2;
	}
	else if (!strcmp(argv[i], "-zero")) {
	    do_zero = 1;
	    i+=1;
	}
	else if (!strcmp(argv[i], "-nonzero")) {
	    do_nonzero = 1;
	    i+=1;
	}
	else if (!strcmp(argv[i], "-help")) {
	    do_help = 1;
	    i+=1;
	}
        else {
            punt("unknown argument: Usage Sumstat [-i i_file] [-o o_file] [-total] [-rank] [-name] [-percent val]");
        }
    }

    if (! (do_total || do_cycle_rank || do_top || do_zero || do_nonzero))
	do_help = 1;

    if (do_help) {
	fprintf(stderr, "Usage Sumstat [-i i_file] [-o o_file] [-total] [-rank] [-name][-top val] [-zero] [-nonzero]\n");
	fprintf(stderr, "\ti_file: Elcor stats file (dflt = ELCOR_STATS)\n");
	fprintf(stderr, "\to_file: Any file name you want the output placed into (dflt = stdout)\n");
	fprintf(stderr, "\t-total: Print cycle and instr overall totals (dflt is off)\n");
	fprintf(stderr, "\t-rank: Rank function by cycle count (dflt is off)\n");
	fprintf(stderr, "\t-top val: List top val%% functions (dflt is off)\n");
	fprintf(stderr, "\t-zero: List all unexecuted functions (dflt is off)\n");
	fprintf(stderr, "\t-nonzero: List all executed functions (dflt is off)\n");
    
    }

    open_files(input_file_name, output_file_name);

    /*
     *	count how many sets of data are present
     */
    count = 0;
    for (;;) {
        fgets(buf, 4095, INPUT_FILE);
        if (feof(INPUT_FILE))
            break;
        if (! strncmp(buf, "Function ", 9))
            count++;
    }

    alloc_arrays(count);

    /*
     *	read in the data
     */
    rewind(INPUT_FILE);
    fgets(buf, 4095, INPUT_FILE);
    for (i=0; i<count; i++) {
	sscanf(buf, "Function %s (%s\n", tmp1, tmp2);
	/* Remove the find ')' from tmp2 */
	len = strlen(tmp2);
	tmp2[len-1] = '\0';
	func_name[i] = (char *) strdup(tmp1);
	file_name[i] = (char *) strdup(tmp2);

        fgets(buf, 4095, INPUT_FILE);
	while (1) {
	    sscanf(buf, "%s", tmp1);
	    if (! strcmp(tmp1, "Function"))
		break;
	    if (read_cycle_data(i, buf)) {
#ifdef DEBUG
		fprintf(stderr, "cycle data: %s\n", buf);
#endif
	    }
	    else if (read_op_data(i, buf)) {
#ifdef DEBUG
		fprintf(stderr, "op data: %s\n", buf);
#endif
	    }
	    else if (read_static_op_data(i, buf)) {
#ifdef DEBUG
		fprintf(stderr, "static op data: %s\n", buf);
#endif
	    }
	    else {
#ifdef DEBUG
		fprintf(stderr, "IGNORED data: %s\n", buf);
#endif
	    }
    	    fgets(buf, 4095, INPUT_FILE);
            if (feof(INPUT_FILE))
                break;
	}
    } 

    /*
     *	Compute totals
     */
    compute_cycle_totals(count);
    compute_op_totals(count);
    compute_static_op_totals(count);

    sort_by_compute_cycles(count);
    

    /*
     *	Print out totals
     */
    if (do_total) {
        fprintf(OUTPUT_FILE, "# Totals of %d functions\n", count);
        print_cycle_totals();
        print_op_totals();
        print_static_op_totals();
    }
    if (do_cycle_rank) {
        if (do_total)
	    fprintf(OUTPUT_FILE, "----------------------------------------------------------\n");
        fprintf(OUTPUT_FILE, "# Ranking of %d functions by compute cycles\n", count);
	print_compute_cycle_rank(count);
    }

    /* Print out top N% of the functions */
    if (do_top) {
	if (do_total || do_cycle_rank)
	    fprintf(OUTPUT_FILE, "----------------------------------------------------------\n");
        fprintf(OUTPUT_FILE, "# Top %4.2f%% functions ranked by compute cycles\n",
			target_cum_percent);
        print_top_functions(count);
    }

    /* Print out zero weight functions */
    if (do_zero) {
	if (do_total || do_cycle_rank || do_top)
	    fprintf(OUTPUT_FILE, "----------------------------------------------------------\n");
        print_zero_functions(count);
    }

    /* Print out the non-zero weight functions */
    if (do_nonzero) {
	if (do_total || do_cycle_rank || do_top || do_zero)
	    fprintf(OUTPUT_FILE, "----------------------------------------------------------\n");
        print_nonzero_functions(count);
    }
 
    /* print_zero_weight_files(count); */


    return (0);
}
