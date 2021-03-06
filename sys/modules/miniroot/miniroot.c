/*	$NetBSD: miniroot.c,v 1.3 2008/11/16 15:47:35 ad Exp $	*/

/*-
 * Copyright (c) 2008 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: miniroot.c,v 1.3 2008/11/16 15:47:35 ad Exp $");

#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/module.h>

#include <dev/md.h>

MODULE(MODULE_CLASS_MISC, miniroot, NULL);

static int
miniroot_modcmd(modcmd_t cmd, void *arg)
{
	void *addr;
	int error;
	size_t size;

	switch (cmd) {
	case MODULE_CMD_INIT:
		error = module_find_section("miniroot", &addr, &size);
		if (error == 0) {
			if (size == 0) {
				error = EINVAL;
			} else {
				md_root_setconf(addr, size);
			}
		}
		return error;

	case MODULE_CMD_FINI:
		return EOPNOTSUPP;

	default:
		return ENOTTY;
	}
}
