/* $NetBSD: exynos_platform.c,v 1.4 2017/06/11 01:15:11 jmcneill Exp $ */

/*-
 * Copyright (c) 2017 Jared D. McNeill <jmcneill@invisible.ca>
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "opt_exynos.h"
#include "opt_multiprocessor.h"
#include "opt_fdt_arm.h"

#include "ukbd.h"

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: exynos_platform.c,v 1.4 2017/06/11 01:15:11 jmcneill Exp $");

#include <sys/param.h>
#include <sys/bus.h>
#include <sys/cpu.h>
#include <sys/device.h>
#include <sys/termios.h>

#include <dev/fdt/fdtvar.h>

#include <uvm/uvm_extern.h>

#include <machine/bootconfig.h>
#include <arm/cpufunc.h>

#include <arm/samsung/exynos_reg.h>
#include <arm/samsung/exynos_var.h>

#include <evbarm/exynos/platform.h>

#include <arm/cortex/gtmr_var.h>

#include <arm/fdt/arm_fdtvar.h>

#define	DEVMAP_ALIGN(a)	((a) & ~L1_S_OFFSET)
#define	DEVMAP_SIZE(s)	roundup2((s), L1_S_SIZE)
#define	DEVMAP_ENTRY(va, pa, sz)			\
	{						\
		.pd_va = DEVMAP_ALIGN(va),		\
		.pd_pa = DEVMAP_ALIGN(pa),		\
		.pd_size = DEVMAP_SIZE(sz),		\
		.pd_prot = VM_PROT_READ|VM_PROT_WRITE,	\
		.pd_cache = PTE_NOCACHE			\
	}
#define	DEVMAP_ENTRY_END	{ 0 }

static const struct pmap_devmap *
exynos_platform_devmap(void)
{
	static const struct pmap_devmap devmap[] = {
		DEVMAP_ENTRY(EXYNOS_CORE_VBASE,
			     EXYNOS_CORE_PBASE,
			     EXYNOS5_CORE_SIZE),
		DEVMAP_ENTRY(EXYNOS5_AUDIOCORE_VBASE,
			     EXYNOS5_AUDIOCORE_PBASE,
			     EXYNOS5_AUDIOCORE_SIZE),
		DEVMAP_ENTRY_END
	};	

	return devmap;
}

#define EXYNOS_IOPHYSTOVIRT(a) \
    ((vaddr_t)(((a) - EXYNOS_CORE_PBASE) + EXYNOS_CORE_VBASE))

static void
exynos_platform_bootstrap(void)
{
	paddr_t uart_address = armreg_tpidruro_read();	/* XXX */
	exynos_bootstrap(EXYNOS_CORE_VBASE, EXYNOS_IOPHYSTOVIRT(uart_address));
}

static void
exynos_platform_init_attach_args(struct fdt_attach_args *faa)
{
	extern struct bus_space armv7_generic_bs_tag;
	extern struct bus_space armv7_generic_a4x_bs_tag;
	extern struct arm32_bus_dma_tag armv7_generic_dma_tag;

	faa->faa_bst = &armv7_generic_bs_tag;
	faa->faa_a4x_bst = &armv7_generic_a4x_bs_tag;
	faa->faa_dmat = &armv7_generic_dma_tag;
}

static void
exynos_platform_early_putchar(char c)
{
#if defined(VERBOSE_INIT_ARM)
	extern void exynos_putchar(int);	/* XXX from exynos_start.S */

	exynos_putchar(c);
#endif
}

static void
exynos_platform_device_register(device_t self, void *aux)
{
	exynos_device_register(self, aux);
}

static void
exynos_platform_reset(void)
{
	printf("%s: not implemented\n", __func__);
}

static void
exynos_platform_delay(u_int us)
{
	extern void mct_delay(u_int);

	mct_delay(us);
}

static u_int
exynos_platform_uart_freq(void)
{
	return EXYNOS_UART_FREQ;
}

static const struct arm_platform exynos5_platform = {
	.devmap = exynos_platform_devmap,
	.bootstrap = exynos_platform_bootstrap,
	.init_attach_args = exynos_platform_init_attach_args,
	.early_putchar = exynos_platform_early_putchar,
	.device_register = exynos_platform_device_register,
	.reset = exynos_platform_reset,
	.delay = exynos_platform_delay,
	.uart_freq = exynos_platform_uart_freq,
};

ARM_PLATFORM(exynos5, "samsung,exynos5", &exynos5_platform);
