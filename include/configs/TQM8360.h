/**
 * (C) Copyright 2008
 * Thomas Waehner, TQ Systems, thomas.waehner@tqs.de.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * TQM8360 board configuration file
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * High Level Configuration Options
 */
#define CONFIG_E300		1	/* E300 family */
#define CONFIG_QE		1	/* Has QE */
#define CONFIG_MPC83XX		1	/* MPC83XX family */
#define CONFIG_MPC8360		1	/* MPC8360 CPU specific */
#define CONFIG_TQM8360		1	/* TQM8360 board specific */
#undef  CONFIG_TQM8358			/* TQM8358 board specific */

#define CONFIG_MISC_INIT_R	1	/* Call misc_init_r		*/

/*
 * System Clock Setup
 */
#define CONFIG_83XX_CLKIN	66666667 /* in Hz */

/*
 * NAND flash support (disabled by default)
 *
 * Warning: NAND support will likely increase the U-Boot image size
 * to more than 256 KB. Please adjust TEXT_BASE if necessary.
 */
#undef CONFIG_NAND
#define CONFIG_NAND

/*
 * Hardware Reset Configuration Word
 */
#define CONFIG_SYS_HRCW_LOW (\
			HRCWL_LCL_BUS_TO_SCB_CLK_1X1 |\
			HRCWL_DDR_TO_SCB_CLK_1X1 |\
			HRCWL_CSB_TO_CLKIN_4X1 |\
			HRCWL_VCO_1X2 |\
			HRCWL_CE_PLL_VCO_DIV_4 |\
			HRCWL_CE_PLL_DIV_1X1 |\
			HRCWL_CE_TO_PLL_1X6 |\
			HRCWL_CORE_TO_CSB_2X1)

#define CONFIG_SYS_HRCW_HIGH (\
			HRCWH_PCI_HOST |\
			HRCWH_PCI1_ARBITER_ENABLE |\
			HRCWH_PCICKDRV_ENABLE |\
			HRCWH_CORE_ENABLE |\
			HRCWH_FROM_0X00000100 |\
			HRCWH_BOOTSEQ_DISABLE |\
			HRCWH_SW_WATCHDOG_DISABLE |\
			HRCWH_ROM_LOC_LOCAL_32BIT)

/*
 * System IO Config
 */
#define CONFIG_SYS_SICRH	0x00000000
#define CONFIG_SYS_SICRL	0x40000000

#define CONFIG_BOARD_EARLY_INIT_F /* call board_pre_init */
#define CONFIG_BOARD_EARLY_INIT_R

/* IMMR Base Addres Register */
#define CONFIG_SYS_IMMR		0xE0000000

/*
 * DDR Setup
 */
#define CONFIG_SYS_DDR_BASE		0x00000000
#define CONFIG_SYS_SDRAM_BASE		CONFIG_SYS_DDR_BASE
#define CONFIG_SYS_DDR_BANKS		1
#define CONFIG_SYS_DDRCDR		0x00080001

/*
 * Memory test
 */
#undef CONFIG_SYS_DRAM_TEST		/* memory test, takes time */
#define CONFIG_SYS_MEMTEST_START	0x00000000 /* memtest region */
#define CONFIG_SYS_MEMTEST_END		0x00100000

/*
 * The reserved memory
 */

#define CONFIG_SYS_MONITOR_BASE	TEXT_BASE /* start of monitor */

#if (CONFIG_SYS_MONITOR_BASE < CONFIG_SYS_FLASH_BASE)
#define CONFIG_SYS_RAMBOOT
#else
#undef  CONFIG_SYS_RAMBOOT
#endif

#define CONFIG_SYS_MONITOR_LEN		(512 * 1024) /* 512 kB for Mon */
#define CONFIG_SYS_MALLOC_LEN		(256 * 1024) /* 256 kB for malloc */

/*
 * Initial RAM Base Address Setup
 */
#define CONFIG_SYS_INIT_RAM_LOCK	1
#define CONFIG_SYS_INIT_RAM_ADDR	0xF0000000
#define CONFIG_SYS_INIT_RAM_END		0x1000

#define CONFIG_SYS_GBL_DATA_SIZE	0x100
#define CONFIG_SYS_GBL_DATA_OFFSET	(CONFIG_SYS_INIT_RAM_END \
					- CONFIG_SYS_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_OFFSET	CONFIG_SYS_GBL_DATA_OFFSET


/*
 * Local Bus Configuration & Clock Setup
 */
#define CONFIG_SYS_LCRR		(LCRR_DBYP | LCRR_CLKDIV_4)
#define CONFIG_SYS_LBC_LBCR	0x00000000

/*
 * FLASH on the Local Bus
 */
#define CONFIG_FLASH_CFI_DRIVER
#define CONFIG_SYS_FLASH_CFI
#define CONFIG_SYS_FLASH_PROTECTION	1
#define CONFIG_SYS_FLASH_BASE		0x80000000
#define CONFIG_SYS_FLASH_SIZE		128

#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE

#define CONFIG_SYS_MAX_FLASH_BANKS	2

/* flash has dual die */
#define CONFIG_SYS_FLASH_BANKS_LIST	{CONFIG_SYS_FLASH_BASE, \
					(CONFIG_SYS_FLASH_BASE + \
					((CONFIG_SYS_FLASH_SIZE / 2) << 20))}
#define CONFIG_SYS_MAX_FLASH_SECT	260

#undef  CONFIG_SYS_FLASH_CHECKSUM

#define CONFIG_SYS_LBLAWBAR0_PRELIM	CONFIG_SYS_FLASH_BASE
#define CONFIG_SYS_LBLAWAR0_PRELIM	0x8000001D /* 512MiB window size */

/* 32 bit device at 0x80000000 via GPCM (0x8000_1801) */
#define CONFIG_SYS_BR0_PRELIM		((CONFIG_SYS_FLASH_BASE & BR_BA) | \
					BR_MS_GPCM | BR_PS_32 | BR_V)

/* FLASH timing */
/* OR0 = 0x.....FA2 */
#define CONFIG_SYS_OR_TIMING_FLASH	(OR_GPCM_CSNT \
					| OR_GPCM_ACS_DIV2 \
					| OR_GPCM_XACS \
					| OR_GPCM_SCY_10 \
					| OR_GPCM_EHTR)

#define CONFIG_SYS_PRELIM_OR_AM		0xC0000000  /* OR addr mask: 1 GiB */

#define CONFIG_SYS_OR0_PRELIM		(CONFIG_SYS_PRELIM_OR_AM  \
					| CONFIG_SYS_OR_TIMING_FLASH)

/* disable remaining mappings */
#define CONFIG_SYS_BR1_PRELIM		0x00000000
#define CONFIG_SYS_OR1_PRELIM		0x00000000
#define CONFIG_SYS_LBLAWBAR1_PRELIM	0x00000000
#define CONFIG_SYS_LBLAWAR1_PRELIM	0x00000000

#define CONFIG_SYS_BR2_PRELIM		0x00000000
#define CONFIG_SYS_OR2_PRELIM		0x00000000
#define CONFIG_SYS_LBLAWBAR2_PRELIM	0x00000000
#define CONFIG_SYS_LBLAWAR2_PRELIM	0x00000000

#define CONFIG_SYS_BR3_PRELIM		0x00000000
#define CONFIG_SYS_OR3_PRELIM		0x00000000
#define CONFIG_SYS_LBLAWBAR3_PRELIM	0x00000000
#define CONFIG_SYS_LBLAWAR3_PRELIM	0x00000000

#define CONFIG_SYS_BR4_PRELIM		0x00000000
#define CONFIG_SYS_OR4_PRELIM		0x00000000
#define CONFIG_SYS_LBLAWBAR4_PRELIM	0x00000000
#define CONFIG_SYS_LBLAWAR4_PRELIM	0x00000000

#define CONFIG_SYS_BR5_PRELIM		0x00000000
#define CONFIG_SYS_OR5_PRELIM		0x00000000
#define CONFIG_SYS_LBLAWBAR5_PRELIM	0x00000000
#define CONFIG_SYS_LBLAWAR5_PRELIM	0x00000000

#define CONFIG_SYS_BR6_PRELIM		0x00000000
#define CONFIG_SYS_OR6_PRELIM		0x00000000
#define CONFIG_SYS_LBLAWBAR6_PRELIM	0x00000000
#define CONFIG_SYS_LBLAWAR6_PRELIM	0x00000000

#define CONFIG_SYS_BR7_PRELIM		0x00000000
#define CONFIG_SYS_OR7_PRELIM		0x00000000
#define CONFIG_SYS_LBLAWBAR7_PRELIM	0x00000000
#define CONFIG_SYS_LBLAWAR7_PRELIM	0x00000000


/*
 * Serial Port
 */
#define CONFIG_CONS_INDEX		1
#undef  CONFIG_SERIAL_SOFTWARE_FIFO
#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	1
#define CONFIG_SYS_NS16550_CLK		get_bus_freq(0)

#define CONFIG_SYS_BAUDRATE_TABLE  \
	{300, 600, 1200, 2400, 4800, 9600, 19200, 38400,115200}

#define CONFIG_SYS_NS16550_COM1		(CONFIG_SYS_IMMR+0x4500)
#define CONFIG_SYS_NS16550_COM2		(CONFIG_SYS_IMMR+0x4600)

#define CONFIG_CMDLINE_EDITING		1
/* Use the HUSH parser */
#define CONFIG_SYS_HUSH_PARSER
#ifdef  CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#endif

/* pass open firmware flat tree */
#define CONFIG_OF_LIBFDT		1
#define CONFIG_OF_BOARD_SETUP		1
#define CONFIG_OF_STDOUT_VIA_ALIAS	1

#define CONFIG_SYS_64BIT_VSPRINTF	1
#define CONFIG_SYS_64BIT_STRTOUL	1

/* I2C */
#define CONFIG_HARD_I2C
#undef  CONFIG_SOFT_I2C
#define CONFIG_FSL_I2C
#define CONFIG_SYS_I2C_SPEED		100000
#define CONFIG_SYS_I2C_SLAVE		0x7F
#define CONFIG_SYS_I2C_NOPROBES		{0x50}
#define CONFIG_SYS_I2C_OFFSET		0x3000

/* I2C EEPROM, configuration for onboard EEPROMs 24C256 and 24C32 */
#define CONFIG_SYS_I2C_EEPROM_ADDR		0x50
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN		2 /* 16 bit           */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	5 /* 32 bytes per write */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	12 /* 10ms +/- 20% */
#define CONFIG_SYS_I2C_MULTI_EEPROMS		1 /* more than one eeprom */

/* I2C SYSMON (LM75) */
#define CONFIG_DTT_LM75			1	/* ON Semi's LM75 */
#define CONFIG_DTT_SENSORS		{0}	/* Sensor addresses  */
#define CONFIG_SYS_DTT_MAX_TEMP		70
#define CONFIG_SYS_DTT_LOW_TEMP		-30
#define CONFIG_SYS_DTT_HYSTERESIS	3

/* I2C RTC */
#define CONFIG_RTC_DS1337
#define CONFIG_SYS_I2C_RTC_ADDR		0x68    /* at address 0x68      */

/*
 * General PCI
 * Addresses are mapped 1-1.
 */

#define CONFIG_PCI

#ifdef CONFIG_PCI

/* PCI1 host bridge */
#define CONFIG_SYS_PCI1_MEM_BASE	0xC0000000
#define CONFIG_SYS_PCI1_MEM_PHYS	CONFIG_SYS_PCI1_MEM_BASE
#define CONFIG_SYS_PCI1_MEM_SIZE	0x10000000      /* 256M */
#define CONFIG_SYS_PCI1_IO_BASE		(CONFIG_SYS_IMMR + 0x02000000)
#define CONFIG_SYS_PCI1_IO_PHYS		CONFIG_SYS_PCI1_IO_BASE
#define CONFIG_SYS_PCI1_IO_SIZE		0x01000000 /* 16M */
#define CONFIG_SYS_PCI1_MMIO_BASE	0xD0000000
#define CONFIG_SYS_PCI1_MMIO_PHYS	CONFIG_SYS_PCI1_MMIO_BASE
#define CONFIG_SYS_PCI1_MMIO_SIZE	0x10000000 /* 256M */

#define CONFIG_SYS_PCI1_SLV_MEM_LOCAL	CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_PCI1_SLV_MEM_BUS	0x00000000
#define CONFIG_SYS_PCI1_SLV_MEM_SIZE	0x80000000

#define CONFIG_NET_MULTI	1
#define CONFIG_PCI_PNP		/* do pci plug-and-play */

#define CONFIG_PCI_SCAN_SHOW
#define CONFIG_SYS_PCI_SUBSYS_VENDORID	0x1957	/* Freescale */

#define CONFIG_EEPRO100
#define CONFIG_CMD_NET

#endif  /* CONFIG_PCI */

/*
 * QE UEC ethernet configuration
 */
#define CONFIG_UEC_ETH

#ifdef CONFIG_UEC_ETH

#define CONFIG_MII		1

#ifndef CONFIG_NET_MULTI
#define CONFIG_NET_MULTI
#endif

#define CONFIG_ETHPRIME		"FSL UEC0"
#define CONFIG_PHY_MODE_NEED_CHANGE

/* defines for STK85xxNG with TQM8360 and TQF-ETH2x1000 as FlexiFace 1c */
#define CONFIG_UEC_ETH1 /* GETH1 */
#ifdef CONFIG_UEC_ETH1
#define CONFIG_HAS_ETH0
#define CONFIG_SYS_UEC1_UCC_NUM		0   /* UCC1 */
#define CONFIG_SYS_UEC1_RX_CLK		QE_CLK_NONE
#define CONFIG_SYS_UEC1_TX_CLK		QE_CLK9
#define CONFIG_SYS_UEC1_ETH_TYPE	GIGA_ETH
#define CONFIG_SYS_UEC1_PHY_ADDR	2
#define CONFIG_SYS_UEC1_INTERFACE_MODE	ENET_1000_RGMII
#endif

#define CONFIG_UEC_ETH2 /* GETH2 */
#ifdef CONFIG_UEC_ETH2
#define CONFIG_HAS_ETH1
#define CONFIG_SYS_UEC2_UCC_NUM		1   /* UCC2 */
#define CONFIG_SYS_UEC2_RX_CLK		QE_CLK_NONE
#define CONFIG_SYS_UEC2_TX_CLK		QE_CLK4
#define CONFIG_SYS_UEC2_ETH_TYPE	GIGA_ETH
#define CONFIG_SYS_UEC2_PHY_ADDR	1
#define CONFIG_SYS_UEC2_INTERFACE_MODE	ENET_1000_RGMII
#endif

#if defined(CONFIG_UEC_ETH1) | defined(CONFIG_UEC_ETH2)
#define CONFIG_SYS_QE_ENET10_ERRATA	/* fix MPC8360 erratum QE_ENET10 */
#endif

#undef CONFIG_QUAD_ETH /* use UCC3 - UCC6 as Fast Ethernet */
#define CONFIG_QUAD_ETH /* use UCC3 - UCC6 as Fast Ethernet */

#ifdef CONFIG_QUAD_ETH
/* defines for STK85xxNG with TQM8360 and TQF-ETH4x100 as FlexiFace 1a */
#define CONFIG_UEC_ETH3

#ifdef CONFIG_UEC_ETH3
#define CONFIG_HAS_ETH2
#define CONFIG_SYS_UEC3_UCC_NUM		2   /* UCC3 */
#define CONFIG_SYS_UEC3_RX_CLK		QE_CLK11
#define CONFIG_SYS_UEC3_TX_CLK		QE_CLK12
#define CONFIG_SYS_UEC3_ETH_TYPE	FAST_ETH
#define CONFIG_SYS_UEC3_PHY_ADDR	9
#define CONFIG_SYS_UEC3_INTERFACE_MODE	ENET_100_MII
#endif

#define CONFIG_UEC_ETH4

#ifdef CONFIG_UEC_ETH4
#define CONFIG_HAS_ETH3
#define CONFIG_SYS_UEC4_UCC_NUM		3   /* UCC4 */
#define CONFIG_SYS_UEC4_RX_CLK		QE_CLK7
#define CONFIG_SYS_UEC4_TX_CLK		QE_CLK8
#define CONFIG_SYS_UEC4_ETH_TYPE	FAST_ETH
#define CONFIG_SYS_UEC4_PHY_ADDR	10
#define CONFIG_SYS_UEC4_INTERFACE_MODE	ENET_100_MII
#endif

#define CONFIG_UEC_ETH5

#ifdef CONFIG_UEC_ETH5
#define CONFIG_HAS_ETH4
#define CONFIG_SYS_UEC5_UCC_NUM		4   /* UCC5 */
#define CONFIG_SYS_UEC5_RX_CLK		QE_CLK13
#define CONFIG_SYS_UEC5_TX_CLK		QE_CLK14
#define CONFIG_SYS_UEC5_ETH_TYPE	FAST_ETH
#define CONFIG_SYS_UEC5_PHY_ADDR	11
#define CONFIG_SYS_UEC5_INTERFACE_MODE	ENET_100_MII
#endif

#define CONFIG_UEC_ETH6

#ifdef CONFIG_UEC_ETH6
#define CONFIG_HAS_ETH5
#define CONFIG_SYS_UEC6_UCC_NUM		5   /* UCC6 */
#define CONFIG_SYS_UEC6_RX_CLK		QE_CLK5
#define CONFIG_SYS_UEC6_TX_CLK		QE_CLK6
#define CONFIG_SYS_UEC6_ETH_TYPE	FAST_ETH
#define CONFIG_SYS_UEC6_PHY_ADDR	12
#define CONFIG_SYS_UEC6_INTERFACE_MODE	ENET_100_MII
#endif

#endif /* CONFIG_QUAD_ETH */

#endif /* CONFIG_UEC_ETH */

/*-----------------------------------------------------------------------
 * NAND-FLASH stuff
 *-----------------------------------------------------------------------
 */
#ifdef CONFIG_NAND

#undef CONFIG_SYS_NAND_LEGACY

#define CONFIG_NAND_FSL_UPM		1

#define	CONFIG_MTD_NAND_ECC_JFFS2	1	/* use JFFS2 ECC	*/

/* address distance between chip selects */
#define	CONFIG_SYS_NAND_CS_DIST		0x200

#define CONFIG_SYS_NAND_SIZE		0x8000

#define CONFIG_SYS_NAND0_BASE		(CONFIG_SYS_IMMR + 0x00800000)
#define CONFIG_SYS_NAND1_BASE		(CONFIG_SYS_NAND0_BASE + CONFIG_SYS_NAND_CS_DIST)
#define CONFIG_SYS_NAND2_BASE		(CONFIG_SYS_NAND1_BASE + CONFIG_SYS_NAND_CS_DIST)
#define CONFIG_SYS_NAND3_BASE		(CONFIG_SYS_NAND2_BASE + CONFIG_SYS_NAND_CS_DIST)

#define	CONFIG_SYS_NAND_SELECT_DEVICE	1
#define CONFIG_SYS_MAX_NAND_DEVICE	2
#define NAND_MAX_CHIPS			1


#if (CONFIG_SYS_MAX_NAND_DEVICE == 1)
#define CONFIG_SYS_NAND_BASE_LIST	{ CONFIG_SYS_NAND0_BASE }
#elif (CONFIG_SYS_MAX_NAND_DEVICE == 2)
#define	CONFIG_SYS_NAND_QUIET_TEST	1
#define CONFIG_SYS_NAND_BASE_LIST	{ CONFIG_SYS_NAND0_BASE, \
					CONFIG_SYS_NAND1_BASE }
#elif (CONFIG_SYS_MAX_NAND_DEVICE == 4)
#define	CONFIG_SYS_NAND_QUIET_TEST	1
#define CONFIG_SYS_NAND_BASE_LIST	{ CONFIG_SYS_NAND0_BASE, \
					CONFIG_SYS_NAND1_BASE, \
					CONFIG_SYS_NAND2_BASE, \
					CONFIG_SYS_NAND3_BASE }
#endif

#undef CONFIG_HAVE_NAND_RB	/* define to use R/B# polling */

#ifdef CONFIG_HAVE_NAND_RB
/*
 * example configuration for NAND R/B# polling.
 * On STK85XX connect X21.6 with X20.15 to connect the
 * NAND R/B# output with PF1.
 */
#define CONFIG_NAND_RB_PORT	5	/* 0 = PORTF */
#define CONFIG_NAND_RB_PIN	1	/* PF1 */
#endif /* CONFIG_HAVE_NAND_RB */

/* 8 bit port size, use UPMB */
#define CONFIG_SYS_NAND_BR	((CONFIG_SYS_NAND0_BASE & BR_BA) \
				| BR_PS_8 \
				| BR_MS_UPMB \
				| BR_V)

/* 32kB address range, burst inhibit */
#define CONFIG_SYS_NAND_OR	(OR_UPM_AM | OR_UPM_BI)

#define NAND_BIG_DELAY_US       25 /* max tR for Samsung/Micron devices   */
#endif /* CONFIG_NAND */

/*
 * Environment
 */
#define CONFIG_ENV_OVERWRITE

#ifndef CONFIG_SYS_RAMBOOT
#define CONFIG_ENV_IS_IN_FLASH		1
#define CONFIG_ENV_ADDR			(CONFIG_SYS_MONITOR_BASE \
					+ CONFIG_SYS_MONITOR_LEN)
#define CONFIG_ENV_SECT_SIZE		0x40000	/* 256K(one sector) for env */
#define CONFIG_ENV_SIZE			CONFIG_ENV_SECT_SIZE
#define CONFIG_ENV_OFFSET_REDUND	(CONFIG_ENV_OFFSET \
					+ CONFIG_ENV_SECT_SIZE)
#define CONFIG_ENV_SIZE_REDUND		(CONFIG_ENV_SIZE)
#else
#define CONFIG_SYS_NO_FLASH		1	/* Flash is not usable now */
#define CONFIG_ENV_IS_NOWHERE		1	/* Store ENV in memory only */
#define CONFIG_ENV_ADDR			(CONFIG_SYS_MONITOR_BASE - 0x1000)
#define CONFIG_ENV_SIZE			0x2000
#endif

#define CONFIG_ENV_BUFFER_PRINT		1

#define CONFIG_LOADS_ECHO		1 /* echo on for serial download */
#define CONFIG_SYS_LOADS_BAUD_CHANGE	1 /* allow baudrate change */

/*
 * BOOTP options
 */
#define CONFIG_BOOTP_BOOTFILESIZE
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME


/*
 * Command line configuration.
 */
#include <config_cmd_default.h>

#define CONFIG_CMD_PING
#define CONFIG_CMD_ASKENV
#define CONFIG_CMD_DHCP

#define CONFIG_CMD_I2C
#define CONFIG_CMD_DATE
#define CONFIG_CMD_EEPROM
#define CONFIG_CMD_DTT

#ifdef CONFIG_NAND
/*
 * Use NAND-FLash as JFFS2 device
 */
#define CONFIG_CMD_NAND
#define CONFIG_CMD_JFFS2

#define	CONFIG_JFFS2_NAND	1

/* mtdparts command line support */
#ifdef CONFIG_CMD_MTDPARTS
#define CONFIG_MTD_DEVICE	/* needed for mtdparts commands */
#define CONFIG_FLASH_CFI_MTD

/* default mtd partition table */
#define MTDIDS_DEFAULT		"nor0=80000000.flash,nand0=e0800000.nand"
#define MTDPARTS_DEFAULT	"mtdparts=80000000.flash:512k(u-boot),"\
				"256k(env1),"\
				"256k(env2),"\
				"256k(fdt),"\
				"2M(kernel),"\
				"4M(root),"\
				"-(user);"\
				"e0800000.nand:2M(kernel),"\
				"-(fs);"
#else
#define CONFIG_JFFS2_DEV 	"nand0"	/* NAND device jffs2 lives on	*/
#define CONFIG_JFFS2_PART_OFFSET 0	/* start of jffs2 partition	*/
#define CONFIG_JFFS2_PART_SIZE	0x200000 /* size of jffs2 partition	*/
#endif /* CONFIG_JFFS2_CMDLINE */
#endif /* CONFIG_NAND */

#define CONFIG_CMD_MII

#if defined(CONFIG_PCI)
    #define CONFIG_CMD_PCI
#endif

#if defined(CONFIG_SYS_RAMBOOT)
    #undef CONFIG_CMD_SAVEENV
    #undef CONFIG_CMD_LOADS
#endif

#undef CONFIG_WATCHDOG		/* watchdog disabled */

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP			/* undef to save memory */
#define CONFIG_SYS_LOAD_ADDR	0x2000000	/* default load address */
#define CONFIG_SYS_PROMPT	"=> "		/* Monitor Command Prompt */

#if defined(CONFIG_CMD_KGDB)
#define CONFIG_SYS_CBSIZE	1024 /* Console I/O Buffer Size */
#else
#define CONFIG_SYS_CBSIZE	256 /* Console I/O Buffer Size */
#endif

#define CONFIG_SYS_PBSIZE	(CONFIG_SYS_CBSIZE \
				+sizeof(CONFIG_SYS_PROMPT)+16)
#define CONFIG_SYS_MAXARGS	16
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE
#define CONFIG_SYS_HZ		1000

/*
 * For booting Linux, the board info and command line data
 * have to be in the first 8 MB of memory, since this is
 * the maximum mapped by the Linux kernel during initialization.
 */
#define CONFIG_SYS_BOOTMAPSZ	(8 << 20) /* Initial Memory map for Linux */

/*
 * Core HID Setup
 */
#define CONFIG_SYS_HID0_INIT	0x000000000
#define CONFIG_SYS_HID0_FINAL	HID0_ENABLE_MACHINE_CHECK
#define CONFIG_SYS_HID2	HID2_HBE

/*
 * Cache Config
 */
#define CONFIG_SYS_DCACHE_SIZE		32768
#define CONFIG_SYS_CACHELINE_SIZE	32
#if defined(CONFIG_CMD_KGDB)
#define CONFIG_SYS_CACHELINE_SHIFT	5 /*log base 2 of the above value */
#endif

/*
 * MMU Setup
 */
#define CONFIG_HIGH_BATS	1 /* High BATs supported */

/* IMMRBAR: cache-inhibit and guarded */
#define CONFIG_SYS_IBAT0L	(CONFIG_SYS_IMMR \
				| BATL_PP_10 \
				| BATL_CACHEINHIBIT \
				| BATL_GUARDEDSTORAGE)
#define CONFIG_SYS_IBAT0U	(CONFIG_SYS_IMMR \
				| BATU_BL_256M \
				| BATU_VS \
				| BATU_VP)
#define CONFIG_SYS_DBAT0L	CONFIG_SYS_IBAT0L
#define CONFIG_SYS_DBAT0U	CONFIG_SYS_IBAT0U

/* FLASH: icache cacheable, but dcache-inhibit and guarded */
#define CONFIG_SYS_IBAT1L	(CONFIG_SYS_FLASH_BASE \
				| BATL_PP_10 \
				| BATL_MEMCOHERENCE)
#define CONFIG_SYS_IBAT1U	(CONFIG_SYS_FLASH_BASE \
				| BATU_BL_256M \
				| BATU_VS \
				| BATU_VP)
#define CONFIG_SYS_DBAT1L	(CONFIG_SYS_FLASH_BASE \
				| BATL_PP_10 \
				| BATL_CACHEINHIBIT \
				| BATL_GUARDEDSTORAGE)
#define CONFIG_SYS_DBAT1U	CONFIG_SYS_IBAT1U

/* Stack in dcache: cacheable */
#define CONFIG_SYS_IBAT2L	(CONFIG_SYS_INIT_RAM_ADDR \
				| BATL_PP_10 )
#define CONFIG_SYS_IBAT2U	(CONFIG_SYS_INIT_RAM_ADDR \
				| BATU_BL_256M \
				| BATU_VS \
				| BATU_VP)
#define CONFIG_SYS_DBAT2L	CONFIG_SYS_IBAT2L
#define CONFIG_SYS_DBAT2U	CONFIG_SYS_IBAT2U

/* DDR: 512M, cache cacheable */
#define CONFIG_SYS_IBAT3L	(CONFIG_SYS_SDRAM_BASE \
				| BATL_PP_10 \
				| BATL_MEMCOHERENCE)
#define CONFIG_SYS_IBAT3U	(CONFIG_SYS_SDRAM_BASE \
				| BATU_BL_256M \
				| BATU_VS \
				| BATU_VP)
#define CONFIG_SYS_DBAT3L	CONFIG_SYS_IBAT3L
#define CONFIG_SYS_DBAT3U	CONFIG_SYS_IBAT3U

#define CONFIG_SYS_IBAT4L	(CONFIG_SYS_SDRAM_BASE + 0x10000000 \
				| BATL_PP_10 \
				| BATL_MEMCOHERENCE)
#define CONFIG_SYS_IBAT4U	(CONFIG_SYS_SDRAM_BASE + 0x10000000 \
				| BATU_BL_256M \
				| BATU_VS \
				| BATU_VP)
#define CONFIG_SYS_DBAT4L	CONFIG_SYS_IBAT4L
#define CONFIG_SYS_DBAT4U	CONFIG_SYS_IBAT4U

/* PCI */
#ifdef CONFIG_PCI
/* PCI MEM space: cacheable */
#define CONFIG_SYS_IBAT5L	(CONFIG_SYS_PCI1_MEM_PHYS \
				| BATL_PP_10 \
				| BATL_MEMCOHERENCE)
#define CONFIG_SYS_IBAT5U	(CONFIG_SYS_PCI1_MEM_PHYS \
				| BATU_BL_256M \
				| BATU_VS \
				| BATU_VP)
#define CONFIG_SYS_DBAT5L	CONFIG_SYS_IBAT5L
#define CONFIG_SYS_DBAT5U	CONFIG_SYS_IBAT5U

#define CONFIG_SYS_IBAT6L	(CONFIG_SYS_PCI1_IO_PHYS \
				| BATL_PP_10 \
				| BATL_CACHEINHIBIT \
				| BATL_GUARDEDSTORAGE)
#define CONFIG_SYS_IBAT6U	(CONFIG_SYS_PCI1_IO_PHYS \
				| BATU_BL_16M \
				| BATU_VS \
				| BATU_VP)
#define CONFIG_SYS_DBAT6L	CONFIG_SYS_IBAT6L
#define CONFIG_SYS_DBAT6U	CONFIG_SYS_IBAT6U

#else /* !CONFIG_PCI */

#define CONFIG_SYS_IBAT5L	(0)
#define CONFIG_SYS_IBAT5U	(0)
#define CONFIG_SYS_DBAT5L	CONFIG_SYS_IBAT5L
#define CONFIG_SYS_DBAT5U	CONFIG_SYS_IBAT5U

#define CONFIG_SYS_IBAT6L	(0)
#define CONFIG_SYS_IBAT6U	(0)
#define CONFIG_SYS_DBAT6L	CONFIG_SYS_IBAT6L
#define CONFIG_SYS_DBAT6U	CONFIG_SYS_IBAT6U

#endif /* CONFIG_PCI */

#define CONFIG_SYS_IBAT7L	(0)
#define CONFIG_SYS_IBAT7U	(0)
#define CONFIG_SYS_DBAT7L	CONFIG_SYS_IBAT7L
#define CONFIG_SYS_DBAT7U	CONFIG_SYS_IBAT7U

/*
 * Internal Definitions
 *
 * Boot Flags
 */
#define BOOTFLAG_COLD		0x01 /* Normal Power-On: Boot from FLASH */
#define BOOTFLAG_WARM		0x02 /* Software reboot */

#if defined(CONFIG_CMD_KGDB)
#define CONFIG_KGDB_BAUDRATE	230400  /* speed of kgdb serial port */
#define CONFIG_KGDB_SER_INDEX	2   /* which serial port to use */
#endif

/*
 * Environment Configuration
 */

#define CONFIG_ENV_OVERWRITE

#define CONFIG_BAUDRATE		115200

#define CONFIG_LOADADDR		800000

#define CONFIG_BOOTDELAY	6  /* -1 disables auto-boot */
#undef  CONFIG_BOOTARGS		/* the boot command will set bootargs */

#define CONFIG_PREBOOT  "echo;" \
	"echo Type \\\"run flash_nfs\\\" to mount root filesystem over NFS;" \
	"echo"

#ifdef CONFIG_TQM8360
#define CONFIG_EXTRA_ENV_SETTINGS \
	"netdev=eth0\0" \
	"consoledev=ttyS0\0" \
	"hostname=tqm8360\0" \
	"nfsargs=setenv bootargs root=/dev/nfs rw " \
		"nfsroot=${serverip}:${rootpath}\0" \
	"ramargs=setenv bootargs root=/dev/ram rw\0" \
	"addip=setenv bootargs ${bootargs} " \
		"ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}" \
		":${hostname}:${netdev}:off panic=1\0" \
	"addtty=setenv bootargs ${bootargs} console=ttyS0,${baudrate}\0" \
	"addmtd=set bootargs ${bootargs} ${mtdparts}" \
	"flash_nfs=run nfsargs addip addtty;" \
		"bootm ${kernel_addr} - ${fdt_addr}\0" \
	"flash_self=run ramargs addip addtty;" \
		"bootm ${kernel_addr} ${ramdisk_addr} ${fdt_addr}\0" \
	"net_nfs=tftp ${kernel_addr_r} ${bootfile};" \
		"tftp ${fdt_addr_r} ${fdt_file};" \
		"run nfsargs addip addtty;" \
		"bootm ${kernel_addr_r} - ${fdt_addr_r}\0" \
	"rootpath=/opt/eldk/ppc_6xx\0" \
	"bootfile=uImage_tqm8360\0" \
	"ramdiskfile=uRamdisk\0" \
	"uboot_addr=80000000\0" \
	"fdt_addr=80100000\0" \
	"kernel_addr=80140000\0" \
	"ramdisk_addr=80340000\0" \
	"fdt_addr_r=C00000\0" \
	"kernel_addr_r=1000000\0" \
	"ramdiskaddr_r=2000000\0" \
	"fdt_file=tqm8360.dtb\0" \
	"uboot=u-boot_tqm8360.bin\0" \
	"load=tftp 100000 ${uboot}\0" \
	"update=protect off ${uboot_addr} +${filesize};" \
		"erase ${uboot_addr} +${filesize};" \
		"cp.b 100000 ${uboot_addr} ${filesize};" \
		"setenv filesize\0" \
	"upd=run load update\0" \
	"mtdparts=MTDPARTS_DEFAULT\0" \
	""
#endif

#define CONFIG_BOOTCOMMAND	"run flash_self"

#define CONFIG_NFSBOOTCOMMAND \
	"setenv bootargs root=/dev/nfs rw " \
	"nfsroot=${serverip}:${rootpath} " \
	"ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}" \
	":${hostname}:${netdev}:off " \
	"console=$consoledev,$baudrate $othbootargs;" \
	"tftp ${kernel_addr_r} ${bootfile};" \
	"tftp ${fdt_addr_r} ${fdt_file};" \
	"bootm ${kernel_addr_r} - ${fdt_addr_r}"

#define CONFIG_RAMBOOTCOMMAND \
	"setenv bootargs root=/dev/ram rw " \
	"console=$consoledev,$baudrate $othbootargs;" \
	"tftp ${ramdiskaddr_r} ${ramdiskfile};" \
	"tftp ${kernel_addr_r} ${bootfile};" \
	"tftp ${fdt_addr_r} ${fdt_file};" \
	"bootm ${kernel_addr_r} ${ramdiskaddr_r} ${fdt_addr_r}" \

#endif  /* __CONFIG_H */
