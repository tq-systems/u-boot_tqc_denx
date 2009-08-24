/**
 * (C) Copyright 2008
 * Thomas Waehner, TQ Systems, thomas.waehner@tqs.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
#include <common.h>
#include <ioports.h>
#include <mpc83xx.h>
#include <i2c.h>
#include <miiphy.h>

#if defined(CONFIG_PCI)
#include <pci.h>
#include <netdev.h>
#endif
#include <asm/mmu.h>
#if defined(CONFIG_OF_LIBFDT)
#include <libfdt.h>
#endif
#include <asm/io.h>	/* for sync() */
#ifdef CONFIG_NAND
#include <nand.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/fsl_upm.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

#if defined(CONFIG_NAND) && defined(CONFIG_HAVE_NAND_RB)
extern void qe_config_iopin (u8 port, u8 pin, int dir,
			     int open_drain, int assign);
#endif

/* defines */
#define DDR_MAX_SIZE_PER_CS	0x20000000
#define INITIAL_DDR_CS_CONFIG	0x80044102

ulong flash_get_size (ulong base, int banknum);

#ifdef CONFIG_PS2MULT
void ps2mult_early_init (void);
#endif

/* global variable */
const qe_iop_conf_t qe_iop_conf_tab[] = {
/* GETH in RGMII mode */
#ifdef CONFIG_UEC_ETH1
	/* GETH1 */
	{0,  3, 1, 0, 1}, /* TxD0 */
	{0,  4, 1, 0, 1}, /* TxD1 */
	{0,  5, 1, 0, 1}, /* TxD2 */
	{0,  6, 1, 0, 1}, /* TxD3 */
	{0,  9, 2, 0, 1}, /* RxD0 */
	{0, 10, 2, 0, 1}, /* RxD1 */
	{0, 11, 2, 0, 1}, /* RxD2 */
	{0, 12, 2, 0, 1}, /* RxD3 */
	{0,  7, 1, 0, 1}, /* TX_EN */
	{0, 15, 2, 0, 1}, /* RX_DV */
	{0,  0, 2, 0, 1}, /* RX_CLK */
	{2,  9, 1, 0, 3}, /* GTX_CLK - CLK10 */
	{2,  8, 2, 0, 1}, /* GTX125 - CLK9 */
#endif
#ifdef CONFIG_UEC_ETH2
	/* GETH2 */
	{0, 17, 1, 0, 1}, /* TxD0 */
	{0, 18, 1, 0, 1}, /* TxD1 */
	{0, 19, 1, 0, 1}, /* TxD2 */
	{0, 20, 1, 0, 1}, /* TxD3 */
	{0, 23, 2, 0, 1}, /* RxD0 */
	{0, 24, 2, 0, 1}, /* RxD1 */
	{0, 25, 2, 0, 1}, /* RxD2 */
	{0, 26, 2, 0, 1}, /* RxD3 */
	{0, 21, 1, 0, 1}, /* TX_EN */
	{0, 29, 2, 0, 1}, /* RX_DV */
	{0, 31, 2, 0, 1}, /* RX_CLK */
	{2,  2, 1, 0, 2}, /* GTX_CLK = CLK10 */
	{2,  3, 2, 0, 1}, /* GTX125 - CLK4 */
#endif
#ifdef CONFIG_QUAD_ETH
#ifdef CONFIG_UEC_ETH3
	/* UEC3 = FETH3 MII */
	{1,  0, 1, 0, 1}, /* TxD0 */
	{1,  1, 1, 0, 1}, /* TxD1 */
	{1,  2, 1, 0, 2}, /* TxD2 */
	{1,  3, 1, 0, 1}, /* TxD3 */
	{1,  6, 2, 0, 1}, /* RxD0 */
	{1,  7, 2, 0, 1}, /* RxD1 */
	{1,  8, 2, 0, 1}, /* RxD2 */
	{1,  9, 2, 0, 1}, /* RxD3 */
	{1,  4, 1, 0, 1}, /* TX_EN */
	{1,  5, 1, 0, 1}, /* TX_ER */
	{1, 12, 2, 0, 1}, /* RX_DV */
	{1, 13, 2, 0, 1}, /* RX_ER */
	{1, 10, 2, 0, 1}, /* COL */
	{1, 11, 2, 0, 1}, /* CRS */
	{2, 11, 2, 0, 1}, /* TX_CLK = CLK12 */
	{2, 10, 2, 0, 1}, /* RX_CLK = CLK11 */
#endif
#ifdef CONFIG_UEC_ETH4
	/* UEC4 = FETH4 MII */
	{1, 14, 1, 0, 1}, /* TxD0 */
	{1, 15, 1, 0, 1}, /* TxD1 */
	{1, 16, 1, 0, 1}, /* TxD2 */
	{1, 17, 1, 0, 1}, /* TxD3 */
	{1, 20, 2, 0, 1}, /* RxD0 */
	{1, 21, 2, 0, 1}, /* RxD1 */
	{1, 22, 2, 0, 1}, /* RxD2 */
	{1, 23, 2, 0, 1}, /* RxD3 */
	{1, 18, 1, 0, 1}, /* TX_EN */
	{1, 19, 1, 0, 2}, /* TX_ER */
	{1, 26, 2, 0, 1}, /* RX_DV */
	{1, 27, 2, 0, 1}, /* RX_ER */
	{1, 24, 2, 0, 1}, /* COL */
	{1, 25, 2, 0, 1}, /* CRS */
	{2,  7, 2, 0, 1}, /* TX_CLK = CLK8 */
	{2,  6, 2, 0, 1}, /* RX_CLK = CLK7 */
#endif
#ifdef CONFIG_UEC_ETH5
	/* UEC5 = FETH5 MII */
	{3,  0, 1, 0, 1}, /* TxD0 */
	{3,  1, 1, 0, 1}, /* TxD1 */
	{3,  2, 1, 0, 1}, /* TxD2 */
	{3,  3, 1, 0, 1}, /* TxD3 */
	{3,  6, 2, 0, 1}, /* RxD0 */
	{3,  7, 2, 0, 1}, /* RxD1 */
	{3,  8, 2, 0, 1}, /* RxD2 */
	{3,  9, 2, 0, 1}, /* RxD3 */
	{3,  4, 1, 0, 1}, /* TX_EN */
	{3,  5, 1, 0, 1}, /* TX_ER */
	{3, 12, 2, 0, 1}, /* RX_DV */
	{3, 13, 2, 0, 1}, /* RX_ER */
	{3, 10, 2, 0, 1}, /* COL */
	{3, 11, 2, 0, 1}, /* CRS */
	{2, 13, 2, 0, 1}, /* TX_CLK = CLK14 */
	{2, 12, 2, 0, 1}, /* RX_CLK = CLK13 */
#endif
#ifdef CONFIG_UEC_ETH6
	/* UEC6 = FETH6 MII */
	{3, 14, 1, 0, 1}, /* TxD0 */
	{3, 15, 1, 0, 1}, /* TxD1 */
	{3, 16, 1, 0, 1}, /* TxD2 */
	{3, 17, 1, 0, 1}, /* TxD3 */
	{3, 20, 2, 0, 1}, /* RxD0 */
	{3, 21, 2, 0, 1}, /* RxD1 */
	{3, 22, 2, 0, 1}, /* RxD2 */
	{3, 23, 2, 0, 1}, /* RxD3 */
	{3, 18, 1, 0, 1}, /* TX_EN */
	{3, 19, 1, 0, 1}, /* TX_ER */
	{3, 26, 2, 0, 1}, /* RX_DV */
	{3, 27, 2, 0, 1}, /* RX_ER */
	{3, 24, 2, 0, 1}, /* COL */
	{3, 25, 2, 0, 1}, /* CRS */
	{2,  5, 2, 0, 1}, /* TX_CLK = CLK6 */
	{2,  4, 2, 0, 1}, /* RX_CLK = CLK5 */
#endif
#endif

	{0,  1, 3, 0, 2}, /* MDIO */
	{0,  2, 1, 0, 1}, /* MDC */

	/* UART2 */
	{5,  0, 1, 0, 2}, /* UART2_SOUT */
	{5,  1, 2, 0, 3}, /* UART2_CTS */
	{5,  2, 1, 0, 1}, /* UART2_RTS */
	{5,  3, 2, 0, 2}, /* UART2_SIN */

#ifdef CONFIG_HAVE_NAND_RB
	{CONFIG_NAND_RB_PORT, CONFIG_NAND_RB_PIN, 2, 0, 0},
#endif /* CONFIG_HAVE_NAND_RB */

	{0,  0, 0, 0, QE_IOP_TAB_END}, /* END of table */
};

#ifdef CONFIG_NAND
/* NAND-FLASH UPM tables for TQM8360 */

/* UPM pattern for bus clock = 25 MHz */
static const ulong upmTable25[] = {
	/* Offset *//* UPM Read Single RAM array entry -> NAND Read Data */
	/* 0x00 */ 0x0ff32000, 0x0fa32000, 0x3fb32005, 0xfffffc00,
	/* 0x04 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Read Burst RAM array entry -> NAND Write CMD */
	/* 0x08 */ 0x00ff2c30, 0x00ff2c30, 0x0fff2c35, 0xfffffc00,
	/* 0x0C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Read Burst RAM array entry -> NAND Write ADDR */
	/* 0x10 */ 0x00f3ec30, 0x00f3ec30, 0x0ff3ec35, 0xfffffc00,
	/* 0x14 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Write Single RAM array entry -> NAND Write Data */
	/* 0x18 */ 0x00f32c00, 0x00f32c00, 0x0ff32c05, 0xfffffc00,
	/* 0x1C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Write Burst RAM array entry -> unused */
	/* 0x20 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x24 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x28 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x2C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,

	/* UPM Refresh Timer RAM array entry -> unused */
	/* 0x30 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x34 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x38 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,

	/* UPM Exception RAM array entry -> unsused */
	/* 0x3C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,
};

/* UPM pattern for bus clock = 33.3 MHz */
static const ulong upmTable33[] = {
	/* Offset *//* UPM Read Single RAM array entry -> NAND Read Data */
	/* 0x00 */ 0x0ff32000, 0x0fa32100, 0x3fb32005, 0xfffffc00,
	/* 0x04 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Read Burst RAM array entry -> NAND Write CMD */
	/* 0x08 */ 0x00ff2c30, 0x00ff2c30, 0x0fff2c35, 0xfffffc00,
	/* 0x0C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Read Burst RAM array entry -> NAND Write ADDR */
	/* 0x10 */ 0x00f3ec30, 0x00f3ec30, 0x0ff3ec35, 0xfffffc00,
	/* 0x14 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Write Single RAM array entry -> NAND Write Data */
	/* 0x18 */ 0x00f32c00, 0x00f32c00, 0x0ff32c05, 0xfffffc00,
	/* 0x1C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Write Burst RAM array entry -> unused */
	/* 0x20 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x24 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x28 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x2C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,

	/* UPM Refresh Timer RAM array entry -> unused */
	/* 0x30 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x34 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x38 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,

	/* UPM Exception RAM array entry -> unsused */
	/* 0x3C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,
};

/* UPM pattern for bus clock = 41.7 MHz */
static const ulong upmTable42[] = {
	/* Offset *//* UPM Read Single RAM array entry -> NAND Read Data */
	/* 0x00 */ 0x0ff32000, 0x0fa32100, 0x3fb32005, 0xfffffc00,
	/* 0x04 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Read Burst RAM array entry -> NAND Write CMD */
	/* 0x08 */ 0x00ff2c30, 0x00ff2c30, 0x0fff2c35, 0xfffffc00,
	/* 0x0C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Read Burst RAM array entry -> NAND Write ADDR */
	/* 0x10 */ 0x00f3ec30, 0x00f3ec30, 0x0ff3ec35, 0xfffffc00,
	/* 0x14 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Write Single RAM array entry -> NAND Write Data */
	/* 0x18 */ 0x00f32c00, 0x00f32c00, 0x0ff32c05, 0xfffffc00,
	/* 0x1C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Write Burst RAM array entry -> unused */
	/* 0x20 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x24 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x28 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x2C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,

	/* UPM Refresh Timer RAM array entry -> unused */
	/* 0x30 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x34 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x38 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,

	/* UPM Exception RAM array entry -> unsused */
	/* 0x3C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,
};

/* UPM pattern for bus clock = 50 MHz */
static const ulong upmTable50[] = {
	/* Offset *//* UPM Read Single RAM array entry -> NAND Read Data */
	/* 0x00 */ 0x0ff33000, 0x0fa33100, 0x0fa33005, 0xfffffc00,
	/* 0x04 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Read Burst RAM array entry -> NAND Write CMD */
	/* 0x08 */ 0x00ff3d30, 0x00ff3c30, 0x0fff3c35, 0xfffffc00,
	/* 0x0C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Read Burst RAM array entry -> NAND Write ADDR */
	/* 0x10 */ 0x00f3fd30, 0x00f3fc30, 0x0ff3fc35, 0xfffffc00,
	/* 0x14 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Write Single RAM array entry -> NAND Write Data */
	/* 0x18 */ 0x00f33d00, 0x00f33c00, 0x0ff33c05, 0xfffffc00,
	/* 0x1C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Write Burst RAM array entry -> unused */
	/* 0x20 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x24 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x28 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x2C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,

	/* UPM Refresh Timer RAM array entry -> unused */
	/* 0x30 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x34 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x38 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,

	/* UPM Exception RAM array entry -> unsused */
	/* 0x3C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,
};

/* UPM pattern for bus clock = 66.7 MHz */
static const ulong upmTable67[] = {
	/* Offset *//* UPM Read Single RAM array entry -> NAND Read Data */
	/* 0x00 */ 0x0ff33000, 0x0fe33000, 0x0fa33100, 0x0fa33000,
	/* 0x04 */ 0x0fa33005, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Read Burst RAM array entry -> NAND Write CMD */
	/* 0x08 */ 0x00ff3d30, 0x00ff3c30, 0x0fff3c30, 0x0fff3c35,
	/* 0x0C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Read Burst RAM array entry -> NAND Write ADDR */
	/* 0x10 */ 0x00f3fd30, 0x00f3fc30, 0x0ff3fc30, 0x0ff3fc35,
	/* 0x14 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Write Single RAM array entry -> NAND Write Data */
	/* 0x18 */ 0x00f33d00, 0x00f33c00, 0x0ff33c00, 0x0ff33c05,
	/* 0x1C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Write Burst RAM array entry -> unused */
	/* 0x20 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x24 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x28 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x2C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,

	/* UPM Refresh Timer RAM array entry -> unused */
	/* 0x30 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x34 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x38 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,

	/* UPM Exception RAM array entry -> unsused */
	/* 0x3C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,
};

/* UPM pattern for bus clock = 83.3 MHz */
static const ulong upmTable83[] = {
	/* Offset *//* UPM Read Single RAM array entry -> NAND Read Data */
	/* 0x00 */ 0x0ff33000, 0x0fe33000, 0x0fa33100, 0x0fa33000,
	/* 0x04 */ 0x0fa33005, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Read Burst RAM array entry -> NAND Write CMD */
	/* 0x08 */ 0x00ff3e30, 0x00ff3c30, 0x0fff3c30, 0x0fff3c35,
	/* 0x0C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Read Burst RAM array entry -> NAND Write ADDR */
	/* 0x10 */ 0x00f3fe30, 0x00f3fc30, 0x0ff3fc30, 0x0ff3fc35,
	/* 0x14 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Write Single RAM array entry -> NAND Write Data */
	/* 0x18 */ 0x00f33e00, 0x00f33c00, 0x0ff33c00, 0x0ff33c05,
	/* 0x1C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Write Burst RAM array entry -> unused */
	/* 0x20 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x24 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x28 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x2C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,

	/* UPM Refresh Timer RAM array entry -> unused */
	/* 0x30 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x34 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x38 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,

	/* UPM Exception RAM array entry -> unsused */
	/* 0x3C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,
};

/* UPM pattern for bus clock = 100 MHz */
static const ulong upmTable100[] = {
	/* Offset *//* UPM Read Single RAM array entry -> NAND Read Data */
	/* 0x00 */ 0x0ff33100, 0x0fe33000, 0x0fa33200, 0x0fa33000,
	/* 0x04 */ 0x0fa33005, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Read Burst RAM array entry -> NAND Write CMD */
	/* 0x08 */ 0x00ff3f30, 0x00ff3c30, 0x0fff3c30, 0x0fff3c35,
	/* 0x0C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Read Burst RAM array entry -> NAND Write ADDR */
	/* 0x10 */ 0x00f3ff30, 0x00f3fc30, 0x0ff3fc30, 0x0ff3fc35,
	/* 0x14 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Write Single RAM array entry -> NAND Write Data */
	/* 0x18 */ 0x00f33f00, 0x00f33c00, 0x0ff33c00, 0x0ff33c05,
	/* 0x1C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Write Burst RAM array entry -> unused */
	/* 0x20 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x24 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x28 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x2C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,

	/* UPM Refresh Timer RAM array entry -> unused */
	/* 0x30 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x34 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x38 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,

	/* UPM Exception RAM array entry -> unsused */
	/* 0x3C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,
};

/* UPM pattern for bus clock = 133.3 MHz */
static const ulong upmTable133[] = {
	/* Offset *//* UPM Read Single RAM array entry -> NAND Read Data */
	/* 0x00 */ 0x0ff33100, 0x0fe33000, 0x0fa33300, 0x0fa33000,
	/* 0x04 */ 0x0fa33000, 0x0fa33005, 0xfffffc00, 0xfffffc00,

	/* UPM Read Burst RAM array entry -> NAND Write CMD */
	/* 0x08 */ 0x00ff3f30, 0x00ff3d30, 0x0fff3d30, 0x0fff3c35,
	/* 0x0C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Read Burst RAM array entry -> NAND Write ADDR */
	/* 0x10 */ 0x00f3ff30, 0x00f3fd30, 0x0ff3fd30, 0x0ff3fc35,
	/* 0x14 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Write Single RAM array entry -> NAND Write Data */
	/* 0x18 */ 0x00f33f00, 0x00f33d00, 0x0ff33d00, 0x0ff33c05,
	/* 0x1C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Write Burst RAM array entry -> unused */
	/* 0x20 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x24 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x28 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x2C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,

	/* UPM Refresh Timer RAM array entry -> unused */
	/* 0x30 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x34 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x38 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,

	/* UPM Exception RAM array entry -> unsused */
	/* 0x3C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,
};

/* UPM pattern for bus clock = 166.7 MHz */
static const ulong upmTable167[] = {
	/* Offset *//* UPM Read Single RAM array entry -> NAND Read Data */
	/* 0x00 */ 0x0ff33200, 0x0fe33000, 0x0fa33300, 0x0fa33300,
	/* 0x04 */ 0x0fa33005, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Read Burst RAM array entry -> NAND Write CMD */
	/* 0x08 */ 0x00ff3f30, 0x00ff3f30, 0x0fff3e30, 0xffff3c35,
	/* 0x0C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Read Burst RAM array entry -> NAND Write ADDR */
	/* 0x10 */ 0x00f3ff30, 0x00f3ff30, 0x0ff3fe30, 0x0ff3fc35,
	/* 0x14 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Write Single RAM array entry -> NAND Write Data */
	/* 0x18 */ 0x00f33f00, 0x00f33f00, 0x0ff33e00, 0x0ff33c05,
	/* 0x1C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,

	/* UPM Write Burst RAM array entry -> unused */
	/* 0x20 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x24 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x28 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x2C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,

	/* UPM Refresh Timer RAM array entry -> unused */
	/* 0x30 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x34 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc00,
	/* 0x38 */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,

	/* UPM Exception RAM array entry -> unsused */
	/* 0x3C */ 0xfffffc00, 0xfffffc00, 0xfffffc00, 0xfffffc01,
};

/* Structure for table with supported UPM timings */
typedef struct {
	ulong frequency;
	const ulong *pUpmTable;
	ulong mxmr;
	ulong or;
} NAND_UPM_FREQ_TABLE_T;

/* Supported UPM timings */
NAND_UPM_FREQ_TABLE_T nandUpmFreqTable[] = {
	/* nominal freq. | ptr to table | GPL4 dis. | ORx */
	{25000000,  upmTable25,  MxMR_GPL_x4DIS,  0},
	{33333333,  upmTable33,  MxMR_GPL_x4DIS,  0},
	{41666667,  upmTable42,  MxMR_GPL_x4DIS,  0},
	{50000000,  upmTable50,  0, 0},
	{66666667,  upmTable67,  0, 0},
	{83333333,  upmTable83,  0, 0},
	{100000000, upmTable100, 0, (3 << OR_UPM_EHTR_SHIFT) | OR_UPM_EAD},
	{133333333, upmTable133, 0, (3 << OR_UPM_EHTR_SHIFT) | OR_UPM_EAD},
	{166666667, upmTable167, 0, (3 << OR_UPM_EHTR_SHIFT) | OR_UPM_EAD},
};

#define N_NAND_UPM_FREQS (sizeof(nandUpmFreqTable) \
			  / sizeof(NAND_UPM_FREQ_TABLE_T))

#endif

/* DDR controller configuration */
struct {
	unsigned long timing_cfg_3;
	unsigned long timing_cfg_0;
	unsigned long timing_cfg_1;
	unsigned long timing_cfg_2;
	unsigned long sdram_cfg;
	unsigned long sdram_cfg2;
	unsigned long sdram_mode;
	unsigned long sdram_mode2;
	unsigned long sdram_interval;
	unsigned long sdram_clk_cntl;
} ddr_conf[] = {
	{	/* 533 MHZ DDR configuration */
		0x00000000,
		0x00110105,
		0x26259222,
		0x131028C7,
		0x43000008,
		0x00401000,
		0x440E0232,
		0x8000C000,
		((1040 << 16) | 256),
		0x02000000,
	},
};

/*
* This array lists all valid DDR RAM configurations, with
* Bank sizes in bytes. The last entry has to to have size equal 0
* and is ignored during autodection. Bank sizes must be in increasing
* order of size
*/
struct {
	long row;
	long col;
	unsigned long size;
} ddr_bank_conf[] = {
	{CSCONFIG_ROW_BIT_12,	CSCONFIG_COL_BIT_8,	32 << 20},
	{CSCONFIG_ROW_BIT_12,	CSCONFIG_COL_BIT_9,	64 << 20},
	{CSCONFIG_ROW_BIT_12,	CSCONFIG_COL_BIT_10,	128 << 20},
	{CSCONFIG_ROW_BIT_13,	CSCONFIG_COL_BIT_9,	128 << 20},
	{CSCONFIG_ROW_BIT_13,	CSCONFIG_COL_BIT_10,	256 << 20},
	{CSCONFIG_ROW_BIT_13,	CSCONFIG_COL_BIT_11,	512 << 20},
	{CSCONFIG_ROW_BIT_14,	CSCONFIG_COL_BIT_10,	512 << 20},
	{CSCONFIG_ROW_BIT_14,	CSCONFIG_COL_BIT_11,	1024 << 20},
	{0,			0,			0}
};


/* functions */

int board_early_init_f (void)
{
	return 0;
}

int board_early_init_r (void)
{
#ifdef CONFIG_PS2MULT
	ps2mult_early_init ();
#endif

	return 0;
}

int board_eth_init (bd_t * bis)
{
	cpu_eth_init (bis);
	return pci_eth_init (bis);
}

/**
 * Sets DDR bank CS bounds.
 *
 * @param cs chip select
 * @param base base address for cs
 * @param size memory size for cs
 */
static void set_cs_bounds (volatile ddr83xx_t * ddr,
			   short cs, long base, long size)
{
	debug ("Setting bounds %08x, %08x for cs %d\n", base, size, cs);
	if (size == 0) {
		ddr->csbnds[cs].csbnds = 0x00000000;
	} else {
		ddr->csbnds[cs].csbnds =
		    ((base >> CSBNDS_SA_SHIFT) & CSBNDS_SA) |
		    (((base + size - 1) >> CSBNDS_EA_SHIFT) & CSBNDS_EA);
	}
	sync ();
}

/**
 * Sets DDR banks CS configuration.
 * config == 0x00000000 disables the CS.
 *
 * @param cs chip select
 * @param config configuration word for cs
 */
static void set_cs_config (volatile ddr83xx_t * ddr, short cs, long config)
{
	debug ("Setting config %08x for cs %d\n", config, cs);
	ddr->cs_config[cs] = config;

	sync ();
}

/**
 * Detect the size of a ddr bank. Sets CS bounds and CS config accordingly.
 *
 * @param cs chip select
 * @param base base address of chip select
 */
static long int get_ddr_bank_size (volatile ddr83xx_t * ddr,
				   short cs, volatile long *base)
{
	int i;
	int detected;
	unsigned long size;
	int ddr64;		/* 0 = 32 bit, 1 = 64 bit */
	volatile immap_t *im = (volatile immap_t *)CONFIG_SYS_IMMR;

	/* check ddr bus size 64 or 32 bit */
	ddr64 = im->reset.rcwh & HRCWH_SECONDARY_DDR_ENABLE ? 0 : 1;

	detected = -1;
	for (i = 0; ddr_bank_conf[i].size != 0; ++i) {

		/* set sdram bank configuration */
		set_cs_config (ddr,
			       cs,
			       0x80044000
			       | ddr_bank_conf[i].col | ddr_bank_conf[i].row);

		debug ("Getting RAM size...\n");
		size = get_ram_size (base, DDR_MAX_SIZE_PER_CS);

		if ((size == ddr_bank_conf[i].size << ddr64) &&
		    (i == detected + 1))
			detected = i;

		debug ("Trying %ld x %ld (%lu MiB) at addr %p,"
		       "detected: %lu MiB\n",
		       ddr_bank_conf[i].row,
		       ddr_bank_conf[i].col,
		       (ddr_bank_conf[i].size >> 20) << ddr64,
		       base, size >> 20);
	}

	if (detected == -1) {
		/* disable empty cs */
		debug ("\nNo valid configurations for CS%d, disabling...\n",
		       cs);
		set_cs_config (ddr, cs, 0);
		return 0;
	}

	debug ("\nDetected configuration %ld x %ld (%lu MiB) at addr %p\n",
	       ddr_bank_conf[detected].row,
	       ddr_bank_conf[detected].col,
	       (ddr_bank_conf[detected].size >> 20) << ddr64, base);

	/* configure cs ro detected params */
	set_cs_config (ddr, cs,
		       0x80044000
		       | ddr_bank_conf[detected].row
		       | ddr_bank_conf[detected].col);

	set_cs_bounds (ddr,
		       cs, (long)base, ddr_bank_conf[detected].size << ddr64);

	return (ddr_bank_conf[detected].size << ddr64);
}

phys_size_t initdram (int board_type)
{
	volatile immap_t *im = (volatile immap_t *)CONFIG_SYS_IMMR;
	volatile ddr83xx_t *ddr = &im->ddr;
	int cs;
	long bank_size;
	long msize = 0;
	int ddr_speed_idx = 0;
	char buf[32];

	if ((im->sysconf.immrbar & IMMRBAR_BASE_ADDR) != (u32) im)
		return -1;

	/* DDR SDRAM */
	/* check if secondary DDR memory controller is disabled */
	if (!(im->reset.rcwh & HRCWH_SECONDARY_DDR_ENABLE)) {
		/*
		 * only primary DDR memory controller is enabled
		 * (64-bit data bus)
		 */
		debug ("1 * 64 bit DDR memory configuration \n");

		debug ("Configuring primary DDR memory controller\n");

		/* during size detection, set up the max DDRLAW size */
		im->sysconf.ddrlaw[0].bar = CONFIG_SYS_DDR_BASE;
		im->sysconf.ddrlaw[0].ar = (LAWAR_EN | LAWAR_SIZE_2G);

		/* set CS bounds to maximum size */
		for (cs = 0; cs < CONFIG_SYS_DDR_BANKS; ++cs) {
			set_cs_bounds (ddr,
				       cs,
				       CONFIG_SYS_DDR_BASE
				       + (cs * DDR_MAX_SIZE_PER_CS),
				       DDR_MAX_SIZE_PER_CS);

			set_cs_config (ddr, cs, INITIAL_DDR_CS_CONFIG);
		}

		/* configure DDR timing */
		ddr->timing_cfg_0 = ddr_conf[ddr_speed_idx].timing_cfg_0;
		ddr->timing_cfg_1 = ddr_conf[ddr_speed_idx].timing_cfg_1;
		ddr->timing_cfg_2 = ddr_conf[ddr_speed_idx].timing_cfg_2;
		ddr->timing_cfg_3 = ddr_conf[ddr_speed_idx].timing_cfg_3;
		ddr->sdram_cfg = ddr_conf[ddr_speed_idx].sdram_cfg;
		ddr->sdram_cfg2 = ddr_conf[ddr_speed_idx].sdram_cfg2;
		ddr->sdram_mode = ddr_conf[ddr_speed_idx].sdram_mode;
		ddr->sdram_mode2 = ddr_conf[ddr_speed_idx].sdram_mode2;
		ddr->sdram_interval = ddr_conf[ddr_speed_idx].sdram_interval;
		ddr->sdram_clk_cntl = ddr_conf[ddr_speed_idx].sdram_clk_cntl;

		udelay (200);
		ddr->sdram_cfg |= SDRAM_CFG_MEM_EN;

		/* size detection */
		debug ("\n");
		for (cs = 0; cs < CONFIG_SYS_DDR_BANKS; ++cs) {
			debug ("\nDetecting Bank %d\n", cs);

			bank_size = get_ddr_bank_size (ddr,
					cs,
					(volatile long *) \
					(CONFIG_SYS_DDR_BASE + msize));
			debug ("Bank%d size: %d MiB\n\n", cs, bank_size >> 20);

			msize += bank_size;

			/* exit if less than one bank */
			if (msize < DDR_MAX_SIZE_PER_CS)
				break;
		}

		im->sysconf.ddrlaw[0].ar = (LAWAR_EN | (__ilog2 (msize) - 1));
	} else {
		/*
		 * secondary DDR memory controller is enabled
		 * (2 * 32-bit data bus)
		 */
		debug ("2 * 32 bit DDR memory configuration \n");

		/* during size detection, set up the max DDRLAW size */
		im->sysconf.ddrlaw[0].bar = CONFIG_SYS_DDR_BASE;
		im->sysconf.ddrlaw[0].ar = (LAWAR_EN | LAWAR_SIZE_1G);
		im->sysconf.sddrlaw[0].bar = CONFIG_SYS_DDR_BASE +
		    DDR_MAX_SIZE_PER_CS * CONFIG_SYS_DDR_BANKS;
		im->sysconf.sddrlaw[0].ar = (LAWAR_EN | LAWAR_SIZE_1G);

		while (ddr) {
			/* configure DDR controller */

			/* set CS bounds to maximum size */
			for (cs = 0; cs < CONFIG_SYS_DDR_BANKS; ++cs) {
				set_cs_bounds (ddr, cs,
					       CONFIG_SYS_DDR_BASE
					       + (cs * DDR_MAX_SIZE_PER_CS),
					       DDR_MAX_SIZE_PER_CS);

				set_cs_config (ddr, cs, INITIAL_DDR_CS_CONFIG);
			}

			/* configure DDR timing */
			ddr->timing_cfg_0 =
			    ddr_conf[ddr_speed_idx].timing_cfg_0;
			ddr->timing_cfg_1 =
			    ddr_conf[ddr_speed_idx].timing_cfg_1;
			ddr->timing_cfg_2 =
			    ddr_conf[ddr_speed_idx].timing_cfg_2;
			ddr->timing_cfg_3 =
			    ddr_conf[ddr_speed_idx].timing_cfg_3;
			ddr->sdram_cfg =
			    ddr_conf[ddr_speed_idx].sdram_cfg | \
			    SDRAM_CFG_32_BE;
			ddr->sdram_cfg2 = ddr_conf[ddr_speed_idx].sdram_cfg2;
			ddr->sdram_mode = ddr_conf[ddr_speed_idx].sdram_mode;
			ddr->sdram_mode2 = ddr_conf[ddr_speed_idx].sdram_mode2;
			ddr->sdram_interval =
			    ddr_conf[ddr_speed_idx].sdram_interval;
			ddr->sdram_clk_cntl =
			    ddr_conf[ddr_speed_idx].sdram_clk_cntl;

			udelay (200);
			ddr->sdram_cfg |= SDRAM_CFG_MEM_EN;

			/* size detection */
			debug ("\n");
			for (cs = 0; cs < CONFIG_SYS_DDR_BANKS; ++cs) {
				debug ("\nDetecting Bank %d\n", cs);

				bank_size = get_ddr_bank_size (ddr,
						cs,
						(volatile long *) \
						(CONFIG_SYS_DDR_BASE + msize));

				debug ("DDR Bank%d size: %d MiB\n\n",
				       cs, bank_size >> 20);

				msize += bank_size;

				/* exit if less than one bank */
				if (msize < DDR_MAX_SIZE_PER_CS)
					break;
			}

			if (ddr == &im->ddr)
				ddr = &im->ddr_secondary;
			else if (ddr == &im->ddr_secondary)
				ddr = NULL;	/* finished */
		}		/* while */

		im->sysconf.ddrlaw[0].ar = (LAWAR_EN
					    | (__ilog2 (msize / 2) - 1));
		im->sysconf.sddrlaw[0].bar = CONFIG_SYS_DDR_BASE + msize / 2;
		im->sysconf.sddrlaw[0].ar = (LAWAR_EN
					     | (__ilog2 (msize / 2) - 1));
	}

	printf (" (DDR%d", ((im->ddr.sdram_cfg & SDRAM_CFG_SDRAM_TYPE_MASK)
			    >> SDRAM_CFG_SDRAM_TYPE_SHIFT) - 1);

	if (im->ddr.sdram_cfg & SDRAM_CFG_32_BE)
		puts (", 32-bit");
	else
		puts (", 64-bit");

	printf (", %s MHz) ", strmhz (buf, gd->mem_clk));

	/* return total bus SDRAM size(bytes)  -- DDR */
	return msize;
}

int checkboard (void)
{
	puts ("Board: TQM8360\n");
	return 0;
}

/*
 * initialize TQM8360 flash bank
 */
int misc_init_r (void)
{
	return (0);
}

#if defined(CONFIG_NAND)
/*
 * Initialize UPM for NAND flash access.
 */
void nand_upm_init (void)
{
	volatile immap_t *im = (volatile immap_t *)CONFIG_SYS_IMMR;
	uint i;

	/* set up LBC law 3 */
	im->sysconf.lblaw[3].bar = CONFIG_SYS_NAND0_BASE;
	im->sysconf.lblaw[3].ar = (LAWAR_EN | LAWAR_SIZE_32K);

	/* set up lbc3 */
	im->lbus.bank[3].br = CONFIG_SYS_NAND_BR;

	/*
	 * Search appropriate UPM table for bus clock.
	 * If the bus clock exceeds a tolerated value, take the UPM timing for
	 * the next higher supported frequency to ensure that access works
	 * (even the access may be slower then).
	 */
	for (i = 0;
	     (i < N_NAND_UPM_FREQS) &&
	     (gd->lclk_clk > nandUpmFreqTable[i].frequency); i++) ;

	if (i >= N_NAND_UPM_FREQS) {
		/*
		 * no valid entry found
		 * take last entry with configuration for max. bus clock
		 */
		i--;
	}

	/* set LBC3 options according to TQM8360 timing specification */
	im->lbus.bank[3].or = CONFIG_SYS_NAND_OR | nandUpmFreqTable[i].or;

	/* configure UPM pattern */
	upmconfig (1, (uint *) nandUpmFreqTable[i].pUpmTable, 64);

	im->lbus.mbmr |= nandUpmFreqTable[i].mxmr;

	return;
}

#ifdef CONFIG_HAVE_NAND_RB
static int tqm_nand_dev_ready (void)
{
	volatile immap_t *im = (volatile immap_t *)CONFIG_SYS_IMMR;

	return (in_be32 (&im->qepio.ioport[CONFIG_NAND_RB_PORT].pdat) >>
		(31 - CONFIG_NAND_RB_PIN)) & 1;
}
#endif

static struct fsl_upm_nand fun = {
	.width = 8,
	.upm_cmd_offset = 0x08,
	.upm_addr_offset = 0x10,
	.chip_delay = NAND_BIG_DELAY_US,
};

void board_nand_select_device (struct nand_chip *nand, int chip)
{
}

int board_nand_init (struct nand_chip *nand)
{
	static int UpmInit = 0;

	volatile immap_t *im = (volatile immap_t *)CONFIG_SYS_IMMR;

	if (UpmInit == 0) {
		nand_upm_init ();
		UpmInit = 1;
	}

	fun.upm.io_addr = nand->IO_ADDR_R;
	fun.upm.mxmr = (void __iomem *)&im->lbus.mbmr;
	fun.upm.mdr = (void __iomem *)&im->lbus.mdr;
	fun.upm.mar = (void __iomem *)&im->lbus.mar;

#ifdef CONFIG_HAVE_NAND_RB
	fun.dev_ready = tqm_nand_dev_ready;
#endif

	return fsl_upm_nand_init (nand, &fun);
}
#endif

#if defined(CONFIG_OF_BOARD_SETUP)
void ft_board_setup (void *blob, bd_t * bd)
{
	const immap_t *immr = (immap_t *) CONFIG_SYS_IMMR;
	int nodeoffset;
	const char *prop;
	int path;

	ft_cpu_setup (blob, bd);
#ifdef CONFIG_PCI
	ft_pci_setup (blob, bd);
#endif
	/*
	 * mpc8360ea pb mds errata 2: RGMII timing
	 * if on mpc8360ea rev. 2.1,
	 * change both ucc phy-connection-types from rgmii-id to rgmii-rxid
	 */
	if ((REVID_MAJOR (immr->sysconf.spridr) == 2) &&
	    (REVID_MINOR (immr->sysconf.spridr) == 1)) {
		nodeoffset = fdt_path_offset (blob, "/aliases");
		if (nodeoffset >= 0) {
#if defined(CONFIG_HAS_ETH0)
			/* fixup UCC 1 if using rgmii-id mode */
			prop = fdt_getprop (blob,
					    nodeoffset, "ethernet0", NULL);
			if (prop) {
				path = fdt_path_offset (blob, prop);
				prop = fdt_getprop (blob, path,
						    "phy-connection-type", 0);
				if (prop && (strcmp (prop, "rgmii-id") == 0))
					fdt_setprop (blob, path,
						     "phy-connection-type",
						     "rgmii-rxid",
						     sizeof ("rgmii-rxid"));
			}
#endif
#if defined(CONFIG_HAS_ETH1)
			/* fixup UCC 2 if using rgmii-id mode */
			prop = fdt_getprop (blob,
					    nodeoffset, "ethernet1", NULL);
			if (prop) {
				path = fdt_path_offset (blob, prop);
				prop = fdt_getprop (blob, path,
						    "phy-connection-type", 0);
				if (prop && (strcmp (prop, "rgmii-id") == 0))
					fdt_setprop (blob, path,
						     "phy-connection-type",
						     "rgmii-rxid",
						     sizeof ("rgmii-rxid"));
			}
#endif
		}
	}
}
#endif
