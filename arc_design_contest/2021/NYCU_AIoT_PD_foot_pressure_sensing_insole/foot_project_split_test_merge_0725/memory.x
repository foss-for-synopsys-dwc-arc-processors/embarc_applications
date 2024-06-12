/* SYSTEM memory regions indicate where external memory might be located.
   The TCF has no specific knowledge of whether SYSTEM regions contain 
   external memory or not.
   CCMWRAP memory regions indicate unusable portions of the address space
   due to CCM memory wrapping into upper addresses beyond its size
*/

MEMORY {
    ICCM0   : ORIGIN = 0x00000000, LENGTH = 0x00010000
    ICCM1   : ORIGIN = 0x10000000, LENGTH = 0x00050000
    SYSTEM0 : ORIGIN = 0x20020800, LENGTH = 0x000E9AF0
    DCCM    : ORIGIN = 0x80000000, LENGTH = 0x00040000
    XCCM    : ORIGIN = 0x90000000, LENGTH = 0x00008000
    YCCM    : ORIGIN = 0xa0000000, LENGTH = 0x00008000
}

REGION_ALIAS("startup", ICCM1)
REGION_ALIAS("text", ICCM1)
REGION_ALIAS("readonly", SYSTEM0)
REGION_ALIAS("data", SYSTEM0)
REGION_ALIAS("sdata", SYSTEM0)


ENTRY(_arc_reset)

SECTIONS {

    .ivt DEFINED (ivtbase_addr) ? ivtbase_addr : 0x10000004 :
    {
        *(.init_vector)
        *(.init_bootstrap)
    } > startup

    .vector : 
    {
        . = ALIGN(1024);
        *(.vector)
    } > startup

    /* Read-only sections, merged into text segment: */
    .hash          : { *(.hash)		}
    .dynsym        : { *(.dynsym)		}
    .dynstr        : { *(.dynstr)		}
    .gnu.version   : { *(.gnu.version)	}
    .gnu.version_d   : { *(.gnu.version_d)	}
    .gnu.version_r   : { *(.gnu.version_r)	}
    .rel.init       : { *(.rel.init) }
    .rela.init      : { *(.rela.init) }
    .rel.text       : { *(.rel.text .rel.text.* .rel.gnu.linkonce.t.*) }
    .rela.text      : { *(.rela.text .rela.text.* .rela.gnu.linkonce.t.*) }
    .rel.fini       : { *(.rel.fini) }
    .rela.fini      : { *(.rela.fini) }
    .rel.rodata     : { *(.rel.rodata .rel.rodata.* .rel.gnu.linkonce.r.*) }
    .rela.rodata    : { *(.rela.rodata .rela.rodata.* .rela.gnu.linkonce.r.*) }
    .rel.data       : { *(.rel.data .rel.data.* .rel.gnu.linkonce.d.*) }
    .rela.data      : { *(.rela.data .rela.data.* .rela.gnu.linkonce.d.*) }
    .rel.tdata	  : { *(.rel.tdata .rel.tdata.* .rel.gnu.linkonce.td.*) }
    .rela.tdata	  : { *(.rela.tdata .rela.tdata.* .rela.gnu.linkonce.td.*) }
    .rel.tbss	  : { *(.rel.tbss .rel.tbss.* .rel.gnu.linkonce.tb.*) }
    .rela.tbss	  : { *(.rela.tbss .rela.tbss.* .rela.gnu.linkonce.tb.*) }
    .rel.ctors      : { *(.rel.ctors) }
    .rela.ctors     : { *(.rela.ctors) }
    .rel.dtors      : { *(.rel.dtors) }
    .rela.dtors     : { *(.rela.dtors) }
    .rel.got        : { *(.rel.got) }
    .rela.got       : { *(.rela.got) }
    .rel.sdata      : { *(.rel.sdata .rel.sdata.* .rel.gnu.linkonce.s.*) }
    .rela.sdata     : { *(.rela.sdata .rela.sdata.* .rela.gnu.linkonce.s.*) }
    .rel.sbss       : { *(.rel.sbss .rel.sbss.* .rel.gnu.linkonce.sb.*) }
    .rela.sbss      : { *(.rela.sbss .rela.sbss.* .rela.gnu.linkonce.sb.*) }
    .rel.sdata2     : { *(.rel.sdata2 .rel.sdata2.* .rel.gnu.linkonce.s2.*) }
    .rela.sdata2    : { *(.rela.sdata2 .rela.sdata2.* .rela.gnu.linkonce.s2.*) }
    .rel.sbss2      : { *(.rel.sbss2 .rel.sbss2.* .rel.gnu.linkonce.sb2.*) }
    .rela.sbss2     : { *(.rela.sbss2 .rela.sbss2.* .rela.gnu.linkonce.sb2.*) }
    .rel.bss        : { *(.rel.bss .rel.bss.* .rel.gnu.linkonce.b.*) }
    .rela.bss       : { *(.rela.bss .rela.bss.* .rela.gnu.linkonce.b.*) }
    
    .text           :
    {
        . = ALIGN(4);
        PROVIDE (_f_text = .);
        *(TYPE bss)
        KEEP (*(SORT_NONE(.init)))
        /* Start here after reset.  */
        . = ALIGN(4);
        KEEP (*crt0.o(.text.__startup))
        /* Remaining code.  */
        . = ALIGN(4);
        *(.text .stub .text.* .gnu.linkonce.t.*)
        /* .gnu.warning sections are handled specially by elf.em.  */
        *(.gnu.warning)
        . = ALIGN(4);
        *(.mli_lib.*)

        KEEP (*(SORT_NONE(.fini)))
        PROVIDE (__etext = .);
        PROVIDE (_etext = .);
        PROVIDE (etext = .);
        PROVIDE (_e_text = .);
    }  > text 

    .jcr   :
    {
        KEEP (*(.jcr))
    } > text

    .eh_frame   :
    {
        KEEP (*(.eh_frame))
    } > text

    .gcc_except_table   :
    {
        *(.gcc_except_table) *(.gcc_except_table.*)
    } > text
    
    .plt   :
    {
        *(.plt)
    } > text

    .jlitab   :
    {
        __JLI_TABLE__ = .;
        jlitab*.o:(.jlitab*) *(.jlitab*)

    } > text
    
    .read_only_data   :
    {
        PROVIDE (_f_rodata = .);
        *(.rodata) *(.rodata.*) *(.gnu.linkonce.r.*)
        *(.rodata1)
        PROVIDE (_e_rodata = .);
    } > readonly

        

    .data	  :
    {
        /* Start of the data section image in ROM.  */
        PROVIDE (__data_image = .);
        PROVIDE (__data_start = .) ;
        /* --gc-sections will delete empty .data. This leads to wrong start
        addresses for subsequent sections because -Tdata= from the command
        line will have no effect, see PR13697.  Thus, keep .data  */
        KEEP (*(.data))
        *(.data.* .gnu.linkonce.d.*)
        SORT(CONSTRUCTORS)
    	/* gcc uses crtbegin.o to find the start of
        the constructors, so we make sure it is
        first.  Because this is a wildcard, it
        doesn't matter if the user does not
        actually link against crtbegin.o; the
        linker won't look for a file to match a
        wildcard.  The wildcard also means that it
        doesn't matter which directory crtbegin.o
        is in.  */
        KEEP (*crtbegin*.o(.ctors))
        /* We don't want to include the .ctor section from
        from the crtend.o file until after the sorted ctors.
        The .ctor section from the crtend file contains the
        end of ctors marker and it must be last */
        KEEP (*(EXCLUDE_FILE (*crtend*.o ) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*(.ctors))
        KEEP (*crtbegin*.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend*.o ) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*(.dtors))
    }  > data

    .got            : 
    { 
        *(.got.plt) *(.got) 
    }  > data


    /* We want the small data sections together, so single-instruction offsets
        can access them all, and initialized data all before uninitialized, so
        we can shorten the on-disk segment size.  */
    .sdata          :
    {
        __SDATA_BEGIN__ = . + 0x100;
        _f_sdata = .;
        *(.sdata .sdata.* .gnu.linkonce.s.*)
        _edata  =  .;
        PROVIDE (edata = .);
        
        *(.sdata2 .sdata2.* .gnu.linkonce.s2.*) 
        /* Global data not cleared after reset.  */
        *(.noinit*)
        . = ALIGN(32 / 8);
    }  > sdata

    .sbss_all           :
    {
        PROVIDE (__sbss_start = .);
        PROVIDE (___sbss_start = .);
        *(.dynsbss)
        *(.sbss .sbss.* .gnu.linkonce.sb.*)
        *(.scommon)
        PROVIDE (__sbss_end = .);
        PROVIDE (___sbss_end = .);
        *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*)   
    }  > sdata
    
    .tensor           :
    {
        *(.tensor_arena*) 
    }  > sdata
    
    .bss_all           :
    {
        PROVIDE (_f_bss = .) ;
        *(.dynbss)
        *(.bss .bss.* .gnu.linkonce.b.*)
        *(COMMON)
        /* Align here to ensure that the .bss section occupies space up to
            _end.  Align after .bss to ensure correct alignment even if the
            .bss section disappears because there are no input sections.  */
        . = ALIGN(32 / 8);
        _end = .;
        PROVIDE (end = .);

        PROVIDE (_e_bss = .) ;
    }  > sdata
    

    .Zdata :
    {
        . = ALIGN(16);
        *(.Zdata*)
        PROVIDE (__start_heap = .) ;
        PROVIDE (_f_heap = .) ;
    }  > DCCM

    .Xdata :
    {
        *(.Xdata*)
    }  > XCCM
    
    .Ydata :
    {
        *(.Ydata*)
    }  > YCCM
    
    PROVIDE (__stack_top = (ORIGIN (DCCM) + LENGTH (DCCM) - 1) & -4);
    PROVIDE (__end_heap = ORIGIN (DCCM) + LENGTH (DCCM) - 1); 
    
    PROVIDE (_e_stack = __stack_top);
    PROVIDE (_e_heap = __end_heap); 
    PROVIDE (_load_addr_text = _f_text);
    PROVIDE (_load_addr_rodata = _f_rodata);

    /* Stabs debugging sections.  */
    .stab          0 : { *(.stab) }
    .stabstr       0 : { *(.stabstr) }
    .stab.excl     0 : { *(.stab.excl) }
    .stab.exclstr  0 : { *(.stab.exclstr) }
    .stab.index    0 : { *(.stab.index) }
    .stab.indexstr 0 : { *(.stab.indexstr) }
    .comment       0 : { *(.comment) }
    .note.gnu.build-id : { *(.note.gnu.build-id) }
    /* DWARF debug sections.
        Symbols in the DWARF debugging sections are relative to the beginning
        of the section so we begin them at 0.  */
    /* DWARF 1 */
    .debug          0 : { *(.debug) }
    .line           0 : { *(.line) }
    /* GNU DWARF 1 extensions */
    .debug_srcinfo  0 : { *(.debug_srcinfo) }
    .debug_sfnames  0 : { *(.debug_sfnames) }
    /* DWARF 1.1 and DWARF 2 */
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    /* DWARF 2 */
    .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line .debug_line.* .debug_line_end) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
    .debug_macinfo  0 : { *(.debug_macinfo) }
    /* SGI/MIPS DWARF 2 extensions */
    .debug_weaknames 0 : { *(.debug_weaknames) }
    .debug_funcnames 0 : { *(.debug_funcnames) }
    .debug_typenames 0 : { *(.debug_typenames) }
    .debug_varnames  0 : { *(.debug_varnames) }
    /* DWARF 3 */
    .debug_pubtypes 0 : { *(.debug_pubtypes) }
    .debug_ranges   0 : { *(.debug_ranges) }
    /* DWARF Extension.  */
    .debug_macro    0 : { *(.debug_macro) }
    .debug_addr     0 : { *(.debug_addr) }
    /* ARC Extension Sections */
    .arcextmap	  0 : { *(.arcextmap.*) }    
}


