/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2011 Advanced Micro Devices, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <lib.h>
#include <stdint.h>
#include <string.h>
#include <device/pci_def.h>
#include <device/pci_ids.h>
#include <arch/io.h>
#include <arch/stages.h>
#include <device/pnp_def.h>
#include <arch/cpu.h>
#include <cpu/x86/lapic.h>
#include <console/console.h>
#include <console/loglevel.h>
#include <cpu/x86/mtrr.h>
#include "cpu/amd/car.h"
#include "agesawrapper.h"
#include "cpu/x86/bist.h"
#include "superio/smsc/kbc1100/kbc1100_early_init.c"
#include "cpu/x86/lapic.h"
#include "drivers/pc80/i8254.c"
#include "drivers/pc80/i8259.c"
#include <sb_cimx.h>
#include "SBPLATFORM.h"

void cache_as_ram_main(unsigned long bist, unsigned long cpu_init_detectedx)
{
	u32 val;

	/* all cores: allow caching of flash chip code and data
	 * (there are no cache-as-ram reliability concerns with family 14h)
	 */
	__writemsr (0x20c, (0x0100000000ull - CACHE_ROM_SIZE) | 5);
	__writemsr (0x20d, (0x1000000000ull - CACHE_ROM_SIZE) | 0x800);

	/* all cores: set pstate 0 (1600 MHz) early to save a few ms of boot time
	 */
	__writemsr (0xc0010062, 0);

	if (!cpu_init_detectedx && boot_cpu()) {
		post_code(0x30);
		sb_Poweron_Init();

		post_code(0x31);
		kbc1100_early_init(CONFIG_SIO_PORT);
		console_init();
	}

	/* Halt if there was a built in self test failure */
	post_code(0x34);
	report_bist_failure(bist);

	/* Load MPB */
	val = cpuid_eax(1);
	printk(BIOS_DEBUG, "BSP Family_Model: %08x \n", val);
	printk(BIOS_DEBUG, "cpu_init_detectedx = %08lx \n", cpu_init_detectedx);

	post_code(0x35);
	printk(BIOS_DEBUG, "agesawrapper_amdinitmmio ");
	val = agesawrapper_amdinitmmio();
	if (val)
		printk(BIOS_DEBUG, "error level: %x \n", val);
	else
		printk(BIOS_DEBUG, "passed.\n");

	post_code(0x37);
	printk(BIOS_DEBUG, "agesawrapper_amdinitreset ");
	val = agesawrapper_amdinitreset();
	if (val)
		printk(BIOS_DEBUG, "error level: %x \n", val);
	else
		printk(BIOS_DEBUG, "passed.\n");

	post_code(0x39);
	printk(BIOS_DEBUG, "agesawrapper_amdinitearly ");
	val = agesawrapper_amdinitearly();
	if (val)
		printk(BIOS_DEBUG, "error level: %x \n", val);
	else
		printk(BIOS_DEBUG, "passed.\n");

	post_code(0x40);
	printk(BIOS_DEBUG, "agesawrapper_amdinitpost ");
	val = agesawrapper_amdinitpost();
	if (val)
		printk(BIOS_DEBUG, "error level: %x \n", val);
	else
		printk(BIOS_DEBUG, "passed.\n");

	post_code(0x41);
	printk(BIOS_DEBUG, "agesawrapper_amdinitenv ");
	val = agesawrapper_amdinitenv();
	if (val)
		printk(BIOS_DEBUG, "error level: %x \n", val);
	else
		printk(BIOS_DEBUG, "passed.\n");

	/* Initialize i8259 pic */
	post_code(0x41);
	setup_i8259();

	/* Initialize i8254 timers */
	post_code(0x42);
	setup_i8254();

	post_code(0x50);
	copy_and_run();
	printk(BIOS_ERR, "Error: copy_and_run() returned!\n");

	post_code(0x54);	/* Should never see this post code. */
}
