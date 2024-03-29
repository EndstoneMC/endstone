.text

#if defined(__MINGW32__)
#define call_get_val_in_dll _call_get_val_in_dll
#define jump_get_val_in_dll _jump_get_val_in_dll
#define __get_val_in_dll    _get_val_in_dll
    .def get_val_in_dll;
.scl 2;
.type 32;
.endef
#elif defined(__APPLE__)
#define call_get_val_in_dll _call_get_val_in_dll
#define jump_get_val_in_dll _jump_get_val_in_dll
#define __get_val_in_dll    _get_val_in_dll
#else
    .type __get_val_in_dll,
    @function __get_val_in_dll : pushl % ebx call __x86.get_pc_thunk.bx addl $_GLOBAL_OFFSET_TABLE_, % ebx subl $8,
    % esp call get_val_in_dll @PLT addl $8,
    % esp popl %
        ebx ret

            // padding in order not to use 1-byte relative addressing jump in jump_get_val_in_dll
            .byte 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 .byte 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 .byte 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 .byte 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 .byte 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 .byte 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90, 0x90, 0x90 .byte 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90, 0x90, 0x90 .byte 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
    0x90
#endif

    .p2align 4,
    ,
    15 .globl call_get_val_in_dll
#if defined(__MINGW32__)
        .def call_get_val_in_dll;
.scl 2;
.type 32;
.endef
#elif !defined(__APPLE__)
        .type call_get_val_in_dll,
    @function
#endif
    call_get_val_in_dll : call __get_val_in_dll ret

                              .p2align 4,
    ,
    15 .globl jump_get_val_in_dll
#if defined(__MINGW32__)
        .def jump_get_val_in_dll;
.scl 2;
.type 32;
.endef
#elif !defined(__APPLE__)
        .type jump_get_val_in_dll,
    @function
#endif
    jump_get_val_in_dll : jmp __get_val_in_dll

#if defined(__MINGW32__)
                              .section.drectve.ascii " -export:\"jump_get_val_in_dll\"".ascii
                          " -export:\"call_get_val_in_dll\""
#elif !defined(__APPLE__)
                                  .section.text.__x86.get_pc_thunk.bx,
    "axG", @progbits, __x86.get_pc_thunk.bx,
    comdat.globl __x86.get_pc_thunk.bx.hidden __x86.get_pc_thunk.bx.type __x86.get_pc_thunk.bx,
    @function __x86.get_pc_thunk.bx : movl(% esp), % ebx ret
#endif
