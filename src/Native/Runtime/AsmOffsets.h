// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

//
// This file is used by AsmOffsets.cpp to validate that our
// assembly-code offsets always match their C++ counterparts.

// You must #define PLAT_ASM_OFFSET and PLAT_ASM_SIZEOF before you #include this file

#ifdef BIT64
#define ASM_OFFSET(offset32, offset64, cls, member) PLAT_ASM_OFFSET(offset64, cls, member)
#define ASM_SIZEOF(sizeof32, sizeof64, cls        ) PLAT_ASM_SIZEOF(sizeof64, cls)
#define ASM_CONST(const32, const64, expr)           PLAT_ASM_CONST(const64, expr)
#else
#define ASM_OFFSET(offset32, offset64, cls, member) PLAT_ASM_OFFSET(offset32, cls, member)
#define ASM_SIZEOF(sizeof32, sizeof64, cls        ) PLAT_ASM_SIZEOF(sizeof32, cls)
#define ASM_CONST(const32, const64, expr)           PLAT_ASM_CONST(const32, expr)
#endif

// NOTE: the values MUST be in hex notation WITHOUT the 0x prefix

//        32-bit,64-bit, constant symbol
ASM_CONST( 14c08, 14c08, RH_LARGE_OBJECT_SIZE)
ASM_CONST(   400,   800, CLUMP_SIZE)
ASM_CONST(     a,     b, LOG2_CLUMP_SIZE)

//        32-bit,64-bit, class, member
ASM_OFFSET(    0,     0, Object, m_pEEType)

ASM_OFFSET(    4,     8, Array, m_Length)

ASM_OFFSET(    0,     0, EEType, m_usComponentSize)
ASM_OFFSET(    2,     2, EEType, m_usFlags)
ASM_OFFSET(    4,     4, EEType, m_uBaseSize)
#if defined(CORERT)
// If this ever changes, you must update src\ILCompiler.Compiler\src\Compiler\DependencyAnalysis\EETypeNode.cs GetVTableOffset()
// to reflect the updated VTable offset
ASM_OFFSET(   18,    20, EEType, m_VTable)
#else
ASM_OFFSET(   14,    18, EEType, m_VTable)
#endif

ASM_OFFSET(    0,     0, Thread, m_rgbAllocContextBuffer)
ASM_OFFSET(   28,    38, Thread, m_ThreadStateFlags)
ASM_OFFSET(   2c,    40, Thread, m_pTransitionFrame)
ASM_OFFSET(   30,    48, Thread, m_pHackPInvokeTunnel)
ASM_OFFSET(   40,    68, Thread, m_ppvHijackedReturnAddressLocation)
ASM_OFFSET(   44,    70, Thread, m_pvHijackedReturnAddress)
ASM_OFFSET(   48,    78, Thread, m_pExInfoStackHead)

ASM_SIZEOF(   14,    20, EHEnum)

ASM_OFFSET(    0,     0, alloc_context, alloc_ptr)
ASM_OFFSET(    4,     8, alloc_context, alloc_limit)

ASM_OFFSET(    4,     8, RuntimeInstance, m_pThreadStore)

#ifdef FEATURE_CACHED_INTERFACE_DISPATCH
ASM_OFFSET(    4,     8, InterfaceDispatchCell, m_pCache)
#ifndef BIT64
ASM_OFFSET(    8,     0, InterfaceDispatchCache, m_pCell)
#endif
ASM_OFFSET(   10,    20, InterfaceDispatchCache, m_rgEntries)
ASM_SIZEOF(    8,    10, InterfaceDispatchCacheEntry)
#endif

ASM_OFFSET(    4,     8, StaticClassConstructionContext, m_initialized)

#ifdef FEATURE_DYNAMIC_CODE
ASM_OFFSET(    0,     0, CallDescrData, pSrc)
ASM_OFFSET(    4,     8, CallDescrData, numStackSlots)
ASM_OFFSET(    8,     C, CallDescrData, fpReturnSize)
ASM_OFFSET(    C,    10, CallDescrData, pArgumentRegisters)
ASM_OFFSET(   10,    18, CallDescrData, pFloatArgumentRegisters)
ASM_OFFSET(   14,    20, CallDescrData, pTarget)
ASM_OFFSET(   18,    28, CallDescrData, pReturnBuffer)
#endif

// Undefine macros that are only used in this header for convenience.
#undef ASM_OFFSET
#undef ASM_SIZEOF
#undef ASM_CONST

// Define platform specific offsets
#include "AsmOffsetsCpu.h"

//#define USE_COMPILE_TIME_CONSTANT_FINDER // Uncomment this line to use the constant finder
#if defined(__cplusplus) && defined(USE_COMPILE_TIME_CONSTANT_FINDER)
// This class causes the compiler to emit an error with the constant we're interested in
// in the error message. This is useful if a size or offset changes. To use, comment out
// the compile-time assert that is firing, enable the constant finder, add the appropriate
// constant to find to BogusFunction(), and build.
//
// Here's a sample compiler error:
// In file included from corert/src/Native/Runtime/AsmOffsetsVerify.cpp:38:
// corert/src/Native/Runtime/Full/../AsmOffsets.h:117:61: error: calling a private constructor of class
//      'AsmOffsets::FindCompileTimeConstant<25>'
//    FindCompileTimeConstant<offsetof(ExInfo, m_passNumber)> bogus_variable;
//                                                            ^
// corert/src/Native/Runtime/Full/../AsmOffsets.h:111:5: note: declared private here
//    FindCompileTimeConstant();
//    ^
template<size_t N>
class FindCompileTimeConstant
{
private:
    FindCompileTimeConstant();
};

void BogusFunction()
{
    // Sample usage to generate the error
    FindCompileTimeConstant<offsetof(ExInfo, m_passNumber)> bogus_variable;
}
#endif // defined(__cplusplus) && defined(USE_COMPILE_TIME_CONSTANT_FINDER)
