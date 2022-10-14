/*
 * cmd_ddr.c - just like `ddr` command
 *
 * Copyright (c) 2008 Bitmain Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <config.h>
#include <common.h>
#include <command.h>
#include <mmc.h>
#include <fat.h>
#include <fs.h>
#include <blk.h>
#include <div64.h>
#include <linux/math64.h>
#include <asm/global_data.h>
DECLARE_GLOBAL_DATA_PTR;

int do_savelog(struct cmd_tbl *cmdtp, int flag, int argc, char * const argv[])
{
	const char *filename;
	loff_t bytes;
	unsigned long time;
	loff_t len;
	int ret;
	unsigned long addr = (volatile unsigned long)gd->console_out.start;

	//printf("do_savelog %d  %lu  %lx bytes ", argc, addr, gd->console_out.start);

	if (argc != 4)
		return CMD_RET_USAGE;

	if (fs_set_blk_dev(argv[1], argv[2], FS_TYPE_ANY))
		return 1;

	filename = argv[3];
	bytes = CONFIG_CONSOLE_RECORD_OUT_SIZE;

	time = get_timer(0);
	ret = fs_write(filename, (unsigned long)addr, 0, bytes, &len);
	time = get_timer(time);
	if (ret < 0)
		return 1;

	printf("bm savelog %llu bytes written in %lu ms", len, time);
	if (time > 0) {
		puts(" (");
		print_size(div_u64(len, time) * 1000, "/s");
		puts(")");
	}
	puts("\n");

	membuff_purge(&gd->console_out);
	return 0;
}

U_BOOT_CMD(
	bm_savelog, 4, 0, do_savelog,
	"save log file to a filesystem",
	"<interface> <dev[:part]> <filename> bytes\n"
	"    - Save binary file 'filename' to partition 'part' on device\n"
	"      type 'interface' instance 'dev' from fixed 'addr' in memory.\n"
);
