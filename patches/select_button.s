// Select button hook
    .org 0x0810AD98
        ldr r1, =(check_dexnav_select|1)
        bx r1
    .pool
