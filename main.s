        .gba
        .thumb
        .open "roms/BPRE0.gba","build/multi.gba", 0x08000000

        .include "patches/overworld_cave_effect.s"
        .include "patches/start_menu.s"
        .include "patches/select_button.s"
        .include "patches/hidden_abilities.s"
        .include "patches/pokemon_bst.s"

        .org 0x08800000
        .importobj "build/linked.o"
        .close
