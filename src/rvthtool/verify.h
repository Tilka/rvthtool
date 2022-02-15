/***************************************************************************
 * RVT-H Tool                                                              *
 * verify.h: Verify a bank in an RVT-H disk image.                         *
 *                                                                         *
 * Copyright (c) 2018-2022 by David Korth.                                 *
 * SPDX-License-Identifier: GPL-2.0-or-later                               *
 ***************************************************************************/

#ifndef __RVTHTOOL_RVTHTOOL_VERIFY_H__
#define __RVTHTOOL_RVTHTOOL_VERIFY_H__

#include "tcharx.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 'verify' command.
 * @param rvth_filename	RVT-H device or disk image filename.
 * @param s_bank	Bank number (as a string). (If NULL, assumes bank 1.)
 * @return 0 on success; non-zero on error.
 */
int verify(const TCHAR *rvth_filename, const TCHAR *s_bank);

#ifdef __cplusplus
}
#endif

#endif /* __RVTHTOOL_RVTHTOOL_VERIFY_H__ */
