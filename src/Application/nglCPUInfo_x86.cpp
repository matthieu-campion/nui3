/*
  Copyright (c) 1999 Advanced Micro Devices, Inc.

  LIMITATION OF LIABILITY:  THE MATERIALS ARE PROVIDED *AS IS* WITHOUT ANY
  EXPRESS OR IMPLIED WARRANTY OF ANY KIND INCLUDING WARRANTIES OF MERCHANTABILITY,
  NONINFRINGEMENT OF THIRD-PARTY INTELLECTUAL PROPERTY, OR FITNESS FOR ANY
  PARTICULAR PURPOSE.  IN NO EVENT SHALL AMD OR ITS SUPPLIERS BE LIABLE FOR ANY
  DAMAGES WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS,
  BUSINESS INTERRUPTION, LOSS OF INFORMATION) ARISING OUT OF THE USE OF OR
  INABILITY TO USE THE MATERIALS, EVEN IF AMD HAS BEEN ADVISED OF THE POSSIBILITY
  OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE EXCLUSION OR LIMITATION
  OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES, THE ABOVE LIMITATION MAY
  NOT APPLY TO YOU.

  AMD does not assume any responsibility for any errors which may appear in the
  Materials nor any responsibility to support or update the Materials.  AMD retains
  the right to make changes to its test specifications at any time, without notice.

  NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
  further information, software, technical information, know-how, or show-how
  available to you.

  So that all may benefit from your experience, please report  any  problems
  or  suggestions about this software to 3dsdk.support@amd.com

  AMD Developer Technologies, M/S 585
  Advanced Micro Devices, Inc.
  5900 E. Ben White Blvd.
  Austin, TX 78741
  3dsdk.support@amd.com
*/

#include "nui.h"

#ifdef _NGL_X86_

#ifndef __APPLE__

// Detected CPU Vendors - returned by GetCPUCaps (CPU_VENDOR_STRING);
typedef enum CPU_VENDORS
{
    VENDOR_UNKNOWN,
    VENDOR_AMD,
    VENDOR_INTEL,
    VENDOR_CYRIX,
    VENDOR_CENTAUR
} CPU_VENDORS;

// Detected CPU models - returned by GetCPUCaps (CPU_TYPE);
typedef enum CPU_TYPES
{
    UNKNOWN,
    AMD_Am486,
    AMD_K5,
    AMD_K6,
    AMD_K6_MMX,
    AMD_K6_2,
    AMD_K6_3,
    AMD_K7,
    AMD_ATHLON,

    INTEL_486DX,
    INTEL_486SX,
    INTEL_486DX2,
    INTEL_486SL,
    INTEL_486SX2,
    INTEL_486DX2E,
    INTEL_486DX4,
    INTEL_Pentium,
    INTEL_Pentium_MMX,
    INTEL_Pentium_Pro,
    INTEL_Pentium_II,
    INTEL_Celeron,
    INTEL_Pentium_III,
} CPU_TYPES;


// Detected CPU capabilities - used as input to the GetCPUCaps() function
typedef enum CPUCAPS
{
    // Synthesized values
    CPU_VENDOR,     // Manufacturer (returns enum CPU_VENDORS)
    CPU_TYPE,       // CPU type (return enum CPU_TYPES)
    CPU_VENDOR_STRING,  // CPU vendor name string (return const char *)
    CPU_NAME_STRING,// CPU Processor string (extended functions 0x80000002 - 0x80000004, return const char *)

    // Processor Features - returned as boolean values
    HAS_CPUID,      // Supports CPUID instruction
    HAS_FPU,        // FPU present
    HAS_VME,        // Virtual Mode Extensions
    HAS_DEBUG,      // Debug extensions
    HAS_PSE,        // Page Size Extensions
    HAS_TSC,        // Time Stamp Counter
    HAS_MSR,        // Model Specific Registers
    HAS_PAE,        // Page Address Extensions
    HAS_MCE,        // Machine Check Extensions
    HAS_CMPXCHG8,   // CMPXCHG8 instruction
    HAS_APIC,       // APIC
    HAS_SYSENTER,   // SYSENTER/SYSEXIT instruction
    HAS_MTRR,       // Memory Type Range Registers
    HAS_GPE,        // Global Paging Extensions
    HAS_MCA,        // Machine Check Architecture
    HAS_CMOV,       // CMOV instruction
    HAS_PAT,        // Page Attribue Table
    HAS_PSE36,      // PSE36 (Page Size Extensions)

    HAS_MMX_EXT,    // MMX Extensions
    HAS_MMX,        // MMX support
    HAS_FXSAVE,     // FXSAVE/FXRSTOR instruction

    HAS_3DNOW_EXT,  // Extended 3DNow! support
    HAS_3DNOW,      // 3DNow! support

    HAS_SSE_MMX,    // SSE MMX support (same as HAS_MMXEXT)
    HAS_SSE,        // SSE
    HAS_SSE2,       // SSE2
    HAS_SSE_FP,     // SSE FP support

    // Cache parameters (not all values apply to all cpus)
    CPU_L1_DTLB_ASSOC,      // L1 Data Cache TLB Associativity
    CPU_L1_DTLB_ENTRIES,    // L1 Data Cache TLB Entries
    CPU_L1_ITLB_ASSOC,      // L1 Instruction Cache TLB Associativity (0xff = full associativity)
    CPU_L1_ITLB_ENTRIES,    // L1 Instruction Cache TLB Entries

    CPU_L1_EDTLB_ASSOC,     // Extended (2/4 Mbyte) L1 Data Cache TLB Associativity (0xff = full associativity)
    CPU_L1_EDTLB_ENTRIES,   // Extended (2/4 Mbyte) L1 Data Cache TLB Entries
    CPU_L1_EITLB_ASSOC,     // Extended (2/4 Mbyte) L1 Instruction Cache TLB Associativity
    CPU_L1_EITLB_ENTRIES,   // Extended (2/4 Mbyte) L1 Instruction Cache TLB Entries

    CPU_L1_DCACHE_SIZE,     // L1 Data Cache Size (kbytes)
    CPU_L1_DCACHE_ASSOC,    // L1 Data Cache Associativity (0xff = full associativity)
    CPU_L1_DCACHE_LINES,    // L1 Data Cache Lines
    CPU_L1_DCACHE_LSIZE,    // L1 Data Cache Line Size (bytes)

    CPU_L1_ICACHE_SIZE,     // L1 Instruction Cache Size (kbytes)
    CPU_L1_ICACHE_ASSOC,    // L1 Instruction Cache Associativity (0xff = full associativity)
    CPU_L1_ICACHE_LINES,    // L1 Instruction Cache Lines
    CPU_L1_ICACHE_LSIZE,    // L1 Instruction Cache Line Size (bytes)

    CPU_L2_CACHE_SIZE,      // L2 Unified Cache Size (Kbytes)
    CPU_L2_CACHE_ASSOC,     // L2 Unified Cache Associativity (0xf = full associativity)
    CPU_L2_CACHE_LINES,     // L2 Unified Cache Lines (lines per tag)
    CPU_L2_CACHE_LSIZE,     // L2 Unified Cache Line Size (bytes)

    CPU_L2_DTLB_ASSOC,      // L2 Data Cache TLB Associativity
    CPU_L2_DTLB_ENTRIES,    // L2 Data Cache TLB Entries
    CPU_L2_UTLB_ASSOC,      // L2 Instruction or Unified Cache TLB Associativity (0xf = full associativity)
    CPU_L2_UTLB_ENTRIES,    // L2 Instruction or Unified Cache TLB Entries

    CPU_L2_EDTLB_ASSOC,     // Extended (2/4 Mbyte) L2 Data Cache TLB Associativity (0xf = full associativity)
    CPU_L2_EDTLB_ENTRIES,   // Extended (2/4 Mbyte) L2 Data Cache TLB Entries
    CPU_L2_EUTLB_ASSOC,     // Extended (2/4 Mbyte) L2 Instruction or Unified Cache TLB Associativity
    CPU_L2_EUTLB_ENTRIES,   // Extended (2/4 Mbyte) L2 Instruction or Unified Cache TLB Entries

    CPU_COUNT // Return the number of CPUs present on the host
} CPUCAPS;

/******************************************************************************
 Routine:   GetCPUCaps
 Input:     Which capability to query (see enum CPUCAPS for an exhaustive list)
 Returns:   Depends on input, either a boolean or an enumeration value.
            CPU_TYPE - enum CPU_TYPES
            CPU_MFG  - enum CPU_MFGS
 Comment:   This function returns information about the capabilies of the
            CPU on which it is called.  The input enumeration covers both
            processor feature bits (the HAS_* values) and "synthesized"
            information.
            
            THE HAS_* QUERIES SHOULD ALWAYS BE USED IN PREFERENCE TO DIRECTLY 
            CHECKING THE CPU TYPE WHEN LOOKING FOR FEATURES.  For instance,
            it is *always* better to check for HAS_3DNOW directly, rather
            than rely on checking for a K6_2, K6_3, or K7.  Likewise,
            HAS_MMX should always be used in preference to other methods
            of checking for MMX instructions.

            The features bits are checked against either the base feature
            bits (CPUID function 1, edx) or the extended feature bits
            (CPUID function 0x80000001, edx), as appropriate.  The return
            value is 1 for feature present or 0 for feature not present,

            The synthesized information is created by interpreting the CPUID
            results in some way.

            The CPUCAPS currently implemented are not exhaustive by any means,
            but they cover the basic features and 3DNow! extensions, which
            are the majority of the used bits.  In future revisions, more
            feature bits and synthesized values will be added.

            Note that this routine caches the feature bits when first called,
            so checking multiple features is relatively efficient after the
            first invocation.  However, tt is not recommended practice to
            use GetCPUCaps() inside time-critical code.

******************************************************************************/
static uint32 GetCPUCaps (CPUCAPS cap)
{
    uint32 res = 0;

    static uint32 features       = 0;
    static uint32 ext_features   = 0;
    static uint32 processor      = 0;
    static int   init           = 0;

    // Detect CPUID presence once, since all other requests depend on it
    if (init == 0)
    {
    	
#ifdef _LINUX_
    	//TODO : use /proc/cpuinfo : 
    	//some command like"cat /proc/cpuinfo |grep vendor_id|tail -1|cut -d\  -f2|tr -d \"
#else
        __asm {
            pushfd                  // save EFLAGS to stack.
            pop     eax             // store EFLAGS in EAX.
            mov     edx, eax        // save in EBX for testing later.
            xor     eax, 0x200000   // switch bit 21.
            push    eax             // copy "changed" value to stack.
            popfd                   // save "changed" EAX to EFLAGS.
            pushfd
            pop     eax
            xor     eax, edx        // See if bit changeable.
            jnz     foundit   // if so, mark 
            mov     eax,-1
            jmp     around

            ALIGN   4
        foundit:
            // Load up the features and (where appropriate) extended features flags
            mov     eax,1               // Check for processor features
            CPUID
            mov     [features],edx      // Store features bits
            mov     eax,0x80000000      // Check for support of extended functions.
            CPUID
            cmp     eax,0x80000001      // Make sure function 0x80000001 supported.
            jb      short around
            mov     eax,0x80000001      // Select function 0x80000001
            CPUID
            mov     [processor],eax     // Store processor family/model/step
            mov     [ext_features],edx  // Store extende features bits
            mov     eax,1               // Set "Has CPUID" flag to true

        around:
            mov     [init],eax

        }
#endif
    }
    if (init == -1)
    {
        // No CPUID, so no CPUID functions are supported
        return 0;
    }

    // Otherwise, perform the requested tests
    switch (cap)
    {
    // Synthesized Capabilities
    case HAS_CPUID:
        // Always true if this code gets executed
        res = 1;
        break;

    case CPU_VENDOR:
#ifdef _LINUX_
		//TODO : use /proc/cpuinfo : 
		//some command like "cat /proc/cpuinfo |grep vendor_id|tail -1|cut -d\  -f2|tr -d \"	
#else
        __asm {
            // Query manufacturer string
            mov     eax,0           // function 0 = manufacturer string
            CPUID

            // These tests could probably just check the 'ebx' part of the string,
            // but the entire string is checked for completeness.  Plus, this function
            // should not be used in time-critical code, because the CPUID instruction
            // serializes the processor. (That is, it flushes out the instruction pipeline.)

            // Test for 'AuthenticAMD'
            cmp     ebx,'htuA'
            jne     short not_amd
            cmp     edx,'itne'
            jne     short not_amd
            cmp     ecx,'DMAc'
            jne     short not_amd
            mov     eax,VENDOR_AMD
            jmp     short next_test
 
            // Test for 'GenuineIntel'
         not_amd:
            cmp     ebx,'uneG'
            jne     short not_intel
            cmp     edx,'Ieni'
            jne     short not_intel
            cmp     ecx,'letn'
            jne     short not_intel
            mov     eax,VENDOR_INTEL
            jmp     short next_test
 
            // Test for 'CyrixInstead'
         not_intel:
            cmp     ebx,'iryC'
            jne     short not_cyrix
            cmp     edx,'snIx'
            jne     short not_cyrix
            cmp     ecx,'deat'
            jne     short not_cyrix
            mov     eax,VENDOR_CYRIX
            jmp     short next_test
 
            // Test for 'CentaurHauls'
         not_cyrix:
            cmp     ebx,'tneC'
            jne     short not_centaur
            cmp     edx,'Hrua'
            jne     short not_centaur
            cmp     ecx,'slua'
            jne     short not_centaur
            mov     eax,VENDOR_CENTAUR
            jmp     short next_test
 
         not_centaur:
            mov     eax,VENDOR_UNKNOWN
 
         next_test:
            mov     [res],eax       // store result of previous tests
        }
#endif
        break;

    case CPU_TYPE:                                                 
        // Return a member of the CPUTYPES enumeration
        // Note: do NOT use this for determining presence of chip features, such
        // as MMX and 3DNow!  Instead, use GetCPUCaps (HAS_MMX) and GetCPUCaps (HAS_3DNOW),
        // which will accurately detect the presence of these features on all chips which
        // support them.
        switch (GetCPUCaps (CPU_VENDOR))
        {
        case VENDOR_AMD:
            switch ((processor >> 8) & 0xf) // extract family code
            {
            case 4: // Am486/AM5x86
                res = AMD_Am486;
                break;

            case 5: // K6
                switch ((processor >> 4) & 0xf) // extract model code
                {
                case 0: res = AMD_K5;       break;
                case 1: res = AMD_K5;       break;
                case 2: res = AMD_K5;       break;
                case 3: res = AMD_K5;       break;
                case 4: res = AMD_K6_MMX;   break;
                case 5: res = AMD_K6_MMX;   break;
                case 6: res = AMD_K6_MMX;   break;
                case 7: res = AMD_K6_MMX;   break;
                case 8: res = AMD_K6_2;     break;
                case 9: res = AMD_K6_3;     break;
                }
                break;

            case 6: // K7 Athlon
                res = AMD_K7;
                break;
            }
            break;

        case VENDOR_INTEL:
            switch ((processor >> 8) & 0xf) // extract family code
            {
            case 4:
                switch ((processor >> 4) & 0xf) // extract model code
                {
                case 0: res = INTEL_486DX;  break;
                case 1: res = INTEL_486DX;  break;
                case 2: res = INTEL_486SX;  break;
                case 3: res = INTEL_486DX2; break;
                case 4: res = INTEL_486SL;  break;
                case 5: res = INTEL_486SX2; break;
                case 7: res = INTEL_486DX2E;break;
                case 8: res = INTEL_486DX4; break;
                }
                break;

            case 5:
                switch ((processor >> 4) & 0xf) // extract model code
                {
                case 1: res = INTEL_Pentium;    break;
                case 2: res = INTEL_Pentium;    break;
                case 3: res = INTEL_Pentium;    break;
                case 4: res = INTEL_Pentium_MMX;break;
                }
                break;

            case 6:
                switch ((processor >> 4) & 0xf) // extract model code
                {
                case 1: res = INTEL_Pentium_Pro;break;
                case 3: res = INTEL_Pentium_II; break;
                case 5: res = INTEL_Pentium_II; break;  // actual differentiation depends on cache settings
                case 6: res = INTEL_Celeron;    break;
                case 7: res = INTEL_Pentium_III;break;  // actual differentiation depends on cache settings
                }
                break;
            }
            break;

        case VENDOR_CYRIX:
            res = UNKNOWN;
            break;

        case VENDOR_CENTAUR:
            res = UNKNOWN;
            break;
        }
        break;

    // Feature Bit Test Capabilities
    case HAS_FPU:       res = (features >> 0) & 1;      break;  // bit 0 = FPU
    case HAS_VME:       res = (features >> 1) & 1;      break;  // bit 1 = VME
    case HAS_DEBUG:     res = (features >> 2) & 1;      break;  // bit 2 = Debugger extensions
    case HAS_PSE:       res = (features >> 3) & 1;      break;  // bit 3 = Page Size Extensions
    case HAS_TSC:       res = (features >> 4) & 1;      break;  // bit 4 = Time Stamp Counter
    case HAS_MSR:       res = (features >> 5) & 1;      break;  // bit 5 = Model Specific Registers
    case HAS_MCE:       res = (features >> 6) & 1;      break;  // bit 6 = Machine Check Extensions
    case HAS_CMPXCHG8:  res = (features >> 7) & 1;      break;  // bit 7 = CMPXCHG8 instruction
    case HAS_MMX:       res = (features >> 23) & 1;     break;  // bit 23 = MMX
    case HAS_SSE:       res = (features >> 25) & 1;     break;  // bit 25 = SSE
    case HAS_SSE2:      res = (features >> 26) & 1;     break;  // bit 26 = SSE2
    case HAS_3DNOW:     res = (ext_features >> 31) & 1; break;  // bit 31 (ext) = 3DNow!
    case CPU_COUNT:     res = (features>>16) & 7; break; // bit 16-23 = CPU count
    }

    return res;
}

// End of code from AMD SDK


#ifdef _WIN32_
void nglCPUInfo::FillCPUInfo()
{
  if (mCount)
    return;
  mMMX = GetCPUCaps(HAS_MMX)!=0;
  mSSE = GetCPUCaps(HAS_SSE)!=0;
  mSSE2 = GetCPUCaps(HAS_SSE2)!=0;
  m3DNow = GetCPUCaps(HAS_3DNOW)!=0;
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  mCount = sysinfo.dwNumberOfProcessors ;
}
#endif

#endif // __APPLE__
#endif // _NGL_X86_
