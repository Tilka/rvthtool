/***************************************************************************
 * RVT-H Tool (libwiicrypto)                                               *
 * gcn_structs.h: Nintendo GameCube data structures.                       *
 *                                                                         *
 * Copyright (c) 2016-2024 by David Korth.                                 *
 * SPDX-License-Identifier: GPL-2.0-or-later                               *
 ***************************************************************************/

#pragma once

#include <stdint.h>
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * GameCube/Wii disc image header.
 * This matches the disc image format exactly.
 *
 * NOTE: Strings are NOT null-terminated!
 */
#define GCN_MAGIC 0xC2339F3D
#define WII_MAGIC 0x5D1C9EA3
#pragma pack(1)
typedef struct PACKED _GCN_DiscHeader {
	union {
		char id6[6];	// [0x000] Game code. (ID6)
		struct {
			char id4[4];		// [0x000] Game code. (ID4)
			char company[2];	// [0x004] Company code.
		};
	};

	uint8_t disc_number;		// [0x006] Disc number.
	uint8_t revision;		// [0x007] Revision.
	uint8_t audio_streaming;	// [0x008] Audio streaming flag.
	uint8_t stream_buffer_size;	// [0x009] Streaming buffer size.

	uint8_t reserved1[14];		// [0x00A]

	uint32_t magic_wii;		// [0x018] Wii magic. (0x5D1C9EA3)
	uint32_t magic_gcn;		// [0x01C] GameCube magic. (0xC2339F3D)

	char game_title[64];		// [0x020] Game title.

	// Wii: Disc encryption status.
	// Normally 0 on retail and RVT-R (indicating the disc is encrypted).
	uint8_t hash_verify;		// [0x060] If non-zero, disable hash verification.
	uint8_t disc_noCrypt;		// [0x061] If non-zero, disable disc encryption.
	uint8_t reserved2[6];		// [0x062] Reserved.
} GCN_DiscHeader;
ASSERT_STRUCT(GCN_DiscHeader, 0x68);
#pragma pack()

/**
 * GameCube region codes.
 * Used in bi2.bin (GameCube) and RVL_RegionSetting.
 */
typedef enum {
	GCN_REGION_JPN = 0,	// Japan / Taiwan
	GCN_REGION_USA = 1,	// USA
	GCN_REGION_PAL = 2,	// Europe / Australia
	GCN_REGION_ALL = 3,	// Region-Free

	// The following region codes are Wii-specific,
	// but we'll allow them for GameCube.
	GCN_REGION_KOR = 4,	// South Korea
	GCN_REGION_CHN = 5,	// China
	GCN_REGION_TWN = 6,	// Taiwan
} GCN_Region_Code;

/**
 * DVD Boot Block.
 * References:
 * - http://wiibrew.org/wiki/Wii_Disc#Decrypted
 * - http://hitmen.c02.at/files/yagcd/yagcd/chap13.html
 * - http://www.gc-forever.com/wiki/index.php?title=Apploader
 *
 * All fields are big-endian.
 */
#define GCN_Boot_Block_ADDRESS 0x420
typedef struct _GCN_Boot_Block {
	uint32_t bootFilePosition;	// NOTE: 34-bit RSH2 on Wii.
	uint32_t FSTPosition;		// NOTE: 34-bit RSH2 on Wii.
	uint32_t FSTLength;		// FST size. (NOTE: 34-bit RSH2 on Wii.)
	uint32_t FSTMaxLength;		// Size of biggest additional FST. (NOTE: 34-bit RSH2 on Wii.)

	uint32_t FSTAddress;	// FST address in RAM.
	uint32_t userPosition;	// Data area start. (Might be wrong; use FST.)
	uint32_t userLength;	// Data area length. (Might be wrong; use FST.)
	uint32_t reserved;
} GCN_Boot_Block;
ASSERT_STRUCT(GCN_Boot_Block, 32);

/**
 * DVD Boot Info. (bi2.bin)
 * Reference: http://www.gc-forever.com/wiki/index.php?title=Apploader
 *
 * All fields are big-endian.
 */
#define GCN_Boot_Info_ADDRESS 0x440
typedef struct _GCN_Boot_Info {
	uint32_t debugMonSize;		// Debug monitor size. [FIXME: Listed as signed?]
	uint32_t simMemSize;		// Simulated memory size. (bytes) [FIXME: Listed as signed?]
	uint32_t argOffset;		// Command line arguments.
	uint32_t debugFlag;		// Debug flag. (set to 3 if using CodeWarrior on GDEV)
	uint32_t TRKLocation;		// Target resident kernel location.
	uint32_t TRKSize;		// Size of TRK. [FIXME: Listed as signed?]
	uint32_t region_code;		// Region code. (See GCN_Region_Code.)
	uint32_t reserved1[3];
	uint32_t dolLimit;		// Maximum total size of DOL text/data sections. (0 == unlimited)
	uint32_t reserved2;
} GCN_Boot_Info;
ASSERT_STRUCT(GCN_Boot_Info, 48);

/**
 * DOL header.
 * Reference: http://wiibrew.org/wiki/DOL
 *
 * All fields are big-endian.
 */
typedef struct _DOL_Header {
	uint32_t textData[7];	// File offsets to Text sections.
	uint32_t dataData[11];	// File offsets to Data sections.
	uint32_t text[7];	// Load addresses for Text sections.
	uint32_t data[11];	// Load addresses for Data sections.
	uint32_t textLen[7];	// Section sizes for Text sections.
	uint32_t dataLen[11];	// Section sizes for Data sections.
	uint32_t bss;		// BSS address.
	uint32_t bssLen;	// BSS size.
	uint32_t entry;		// Entry point.
	uint8_t padding[28];	// Padding.
} DOL_Header;
ASSERT_STRUCT(DOL_Header, 256);

/**
 * AppLoader errors.
 *
 * Reference: https://www.gc-forever.com/wiki/index.php?title=Apploader
 */
typedef enum {
	// Unknown.
	APLERR_UNKNOWN			= 0,
	// No errors.
	APLERR_OK			= 1,

	// FSTLength > FSTMaxLength
	APLERR_FSTLENGTH,

	// Debug Monitor Size is not a multiple of 32.
	APLERR_DEBUGMONSIZE_UNALIGNED,

	// Simulated Memory Size is not a multiple of 32.
	APLERR_SIMMEMSIZE_UNALIGNED,

	// (PhysMemSize - SimMemSize) must be > DebugMonSize
	APLERR_PHYSMEMSIZE_MINUS_SIMMEMSIZE_NOT_GT_DEBUGMONSIZE,

	// Simulated Memory Size must be <= Physical Memory Size
	APLERR_SIMMEMSIZE_NOT_LE_PHYSMEMSIZE,

	// Illegal FST address. (must be < 0x81700000)
	APLERR_ILLEGAL_FST_ADDRESS,

	// DOL exceeds size limit.
	APLERR_DOL_EXCEEDS_SIZE_LIMIT,

	// DOL exceeds retail address limit.
	APLERR_DOL_ADDR_LIMIT_RETAIL_EXCEEDED,

	// DOL exceeds debug address limit.
	APLERR_DOL_ADDR_LIMIT_DEBUG_EXCEEDED,

	// Text segment is too big.
	APLERR_DOL_TEXTSEG2BIG,

	// Data segment is too big.
	APLERR_DOL_DATASEG2BIG,
} AppLoader_Error_e;
#pragma pack()

#ifdef __cplusplus
}
#endif
