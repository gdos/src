#	$NetBSD: Makefile,v 1.1 2017/01/11 12:02:29 joerg Exp $

LIB=	LLVMGlobalISel

.include <bsd.init.mk>

.PATH: ${LLVM_SRCDIR}/lib/CodeGen/GlobalISel

SRCS+=	CallLowering.cpp \
	GlobalISel.cpp \
	InstructionSelect.cpp \
	InstructionSelector.cpp \
	IRTranslator.cpp \
	MachineIRBuilder.cpp \
	Legalizer.cpp \
	LegalizerHelper.cpp \
	LegalizerInfo.cpp \
	RegBankSelect.cpp \
	RegisterBank.cpp \
	RegisterBankInfo.cpp \
	Utils.cpp

.if defined(HOSTLIB)
.include <bsd.hostlib.mk>
.else
.include <bsd.lib.mk>
.endif
