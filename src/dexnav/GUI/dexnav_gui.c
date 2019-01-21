#include <pokeagb/pokeagb.h>
#include "dexnav_gui.h"
#include "../HUD/dexnav_hud.h"
#include "../../pokemon_base_stats.h"
#include "../../../generated/images/dexnav_gui.h"
#include "../../../generated/images/pokenav/selection_cursor.h"


#define ICON_PAL_TAG 0xDAC0
#define ICON_GFX_TAG 0xD75A
#define SELECTION_CURSOR_TAG 0x200

static struct DexnavHudData** DNavState = (struct DexnavHudData**)(NAV_SAVERAM + SPECIES_MAX);
static u8* SearchLevels = (u8*)NAV_SAVERAM;
extern void vblank_cb_spq(void);
extern void c2_dexnav_gui(void);
extern void setup(void);
extern u16 rand_range(u16 min, u16 max);

/* Entry point into dexnav + dexnav gui */
u8 exec_dexnav() {
    fade_screen(0xFFFFFFFF, 0, 0, 16, 0x0000);
    void dexnav_gui_handler(void);
    set_callback1(dexnav_gui_handler);
    super.multi_purpose_state_tracker = 0;
    return true;
}

// gfx clean
void dexnav_gui_setup() {
    setup();
    rboxes_free();
    bgid_mod_x_offset(0, 0, 0);
    bgid_mod_y_offset(0, 0, 0);
    bgid_mod_x_offset(1, 0, 0);
    bgid_mod_y_offset(1, 0, 0);
    gpu_tile_bg_drop_all_sets(0);
    bg_vram_setup(0, (struct BgConfig *)&bg_config_dexnav_gui, 4);
    u32 set = 0;
    CpuFastSet((void*)&set, (void*)ADDR_VRAM, CPUModeFS(0x10000, CPUFSSET));
    gpu_sync_bg_hide(1);
    gpu_sync_bg_hide(0);
    set_callback2(c2_dexnav_gui);
    vblank_handler_set(vblank_cb_spq);
}

void dexnav_load_pokemon_icons() {
    /* allocate all pallettes */
    for (u32 i = 0; i < 3; i++) {
        struct SpritePalette pal = {&pokeicon_pals[i], ICON_PAL_TAG + i};
        gpu_pal_obj_alloc_tag_and_apply(&pal);
    }

    for (u8 i = 0; i < 12; i++) {
        u16 species = (*DNavState)->grass_species[i];
        u32 pid = 0xFFFFFFFF;
        //if (species > 0) {
            void *icon_gfx = load_party_icon_tiles_with_form(species, pid, false);
            u16 gfx_tag = ICON_GFX_TAG + i;
            u16 pal_tag = ICON_PAL_TAG + pokeicon_pal_indices[species];
            struct SpriteTiles icon_tiles = {icon_gfx, 4 * 8 * 32, gfx_tag};
            gpu_tile_obj_alloc_tag_and_upload(&icon_tiles);
            struct Template icon_template = {
                                            .tiles_tag = gfx_tag,
                                            .pal_tag = pal_tag,
                                            .oam = &icon_oam,
                                            .animation = (const struct Frame (**)[])0x8231CF0,
                                            .graphics = &icon_tiles,
                                            .rotscale = (const struct RotscaleFrame (**)[])0x8231CFC,
                                            .callback = oac_nullsub,
                                            };
                                                                // x and y pos of sprite
            template_instanciate_forward_search(&icon_template, 20 + (24 * (i % 6)), 92 + (i > 5 ? 28 : 0), 0);
        //}
    }
    for (u8 i = 0; i < 5; i++) {
        u16 species = (*DNavState)->water_species[i];
        u32 pid = 0xFFFFFFFF;
        //if (species > 0) {
            void *icon_gfx = load_party_icon_tiles_with_form(species, pid, false);
            u16 gfx_tag = ICON_GFX_TAG + i + 12;
            u16 pal_tag = ICON_PAL_TAG + pokeicon_pal_indices[species];
            struct SpriteTiles icon_tiles = {icon_gfx, 4 * 8 * 32, gfx_tag};
            gpu_tile_obj_alloc_tag_and_upload(&icon_tiles);
            struct Template icon_template = {
                                            .tiles_tag = gfx_tag,
                                            .pal_tag = pal_tag,
                                            .oam = &icon_oam,
                                            .animation = (const struct Frame (**)[])0x8231CF0,
                                            .graphics = &icon_tiles,
                                            .rotscale = (const struct RotscaleFrame (**)[])0x8231CFC,
                                            .callback = oac_nullsub,
                                            };
                                                                // x and y pos of sprite
            template_instanciate_forward_search(&icon_template, 30 + 24 * i, 48, 0);
        //}
    }
}



void update_cursor_position() {
    // update cursor position
    if ((*DNavState)->selected_arr) {
        // water
        objects[(*DNavState)->cursor_id].pos1.x = cursor_positions2[(*DNavState)->selected_index];
        objects[(*DNavState)->cursor_id].pos1.y = cursor_positions2[(*DNavState)->selected_index + 1];
    } else {
        // grass
        objects[(*DNavState)->cursor_id].pos1.x = cursor_positions1[(*DNavState)->selected_index];
        objects[(*DNavState)->cursor_id].pos1.y = cursor_positions1[(*DNavState)->selected_index + 1];
    }
}
void spawn_pointer_arrow() {
    struct SpriteTiles cursor_gfx = {(void*)selection_cursorTiles, 32 * 32, SELECTION_CURSOR_TAG};
    struct SpritePalette cursor_pal = {(void*)selection_cursorPal, SELECTION_CURSOR_TAG};
    struct Template cursor_temp = {SELECTION_CURSOR_TAG, SELECTION_CURSOR_TAG, &cursor_oam,
                                    (const struct Frame (**)[])0x8231CF0, &cursor_gfx,
                                    (const struct RotscaleFrame (**)[])0x8231CFC, (ObjectCallback)oac_nullsub};

    // uncompressed
    gpu_tile_obj_alloc_tag_and_upload(&cursor_gfx);
    gpu_pal_obj_alloc_tag_and_apply(&cursor_pal);
    (*DNavState)->cursor_id = template_instanciate_forward_search(&cursor_temp, 30, 48, 0);
    update_cursor_position();
}



const pchar no_info[] = _("--------");
const pchar resting_text[] = _("Choose a Pokémon");
const pchar invalid_pick_text[] = _("That Pokémon can’t be searched!");
const pchar select_locked_text[] = _("Pokémon searchable via Select!");
void dexnav_load_pnames(u8 status) {
    // clean boxes
    for (u32 i = 0; i < 5; ++i) {
        rboxid_clear_pixels(i, 0);
    }

    // rbox commit species name
    u16 species = (*DNavState)->selected_arr ? (*DNavState)->water_species[(*DNavState)->selected_index >> 1] : (*DNavState)->grass_species[(*DNavState)->selected_index>>1];
    if (species) {
        rboxid_print(0, 0, 0, 4, &dnav_text_black, 0, pokemon_names[species]);
    } else {
        rboxid_print(0, 0, 0, 4, &dnav_text_black, 0, &no_info[0]);
    }

    // rbox commit search level
    fmt_int_10(string_buffer, SearchLevels[species], 0, 4);
    rboxid_print(1, 0, 0, 4, &dnav_text_black, 0, &string_buffer[0]);

    // rbox commit level bonus
    u8 search_level_bonus = 0;
    if ((SearchLevels[species] >> 2) > 20) {
        search_level_bonus = 20;
    } else {
        search_level_bonus = (SearchLevels[species] >> 2);
    }
    fmt_int_10(string_buffer, search_level_bonus, 0, 4);
    rboxid_print(2, 0, 0, 4, &dnav_text_black, 0, &string_buffer[0]);

    // rbox commit hidden ability name
    if (gBaseStats[species].hidden_ability) {
        rboxid_print(3, 0, 0, 4, &dnav_text_black, 0, pokemon_ability_names[gBaseStats[species].hidden_ability]);
    } else {
        rboxid_print(3, 0, 0, 4, &dnav_text_black, 0, &no_info[0]);
    }

    // rbox commit status message bar
    switch(status) {
        case 0:
            rboxid_print(4, 1, 0, 8, &dnav_text_white, 0, &invalid_pick_text[0]);
            break;
        case 1:
            rboxid_print(4, 1, 0, 8, &dnav_text_white, 0, &resting_text[0]);
            break;
        case 2:
            rboxid_print(4, 1, 0, 8, &dnav_text_white, 0, &select_locked_text[0]);
    }
    // display committed gfx
    for (u8 i = 0; i < 5; i++) {
        rboxid_update(i, 3);
        rboxid_tilemap_update(i);
    }
}

void c1_dexnavOW() {
    extern void exec_dexnav_hud(void);
    exec_dexnav_hud();
}

void dexnav_gui_exit_search() {
    switch (super.multi_purpose_state_tracker) {
        case 0:
            fade_screen(~0, 0, 0x0, 0x10, 0);
            audio_play(SOUND_PC_OPEN);
            super.multi_purpose_state_tracker++;
            break;
        case 1:
            if (!pal_fade_control.active) {
                free((*DNavState)->backbuffer);
                super.multi_purpose_state_tracker++;
            }
            break;
        case 2:
            m4aMPlayVolumeControl(&mplay_BGM, 0xFFFF, 256);
            set_callback1(c1_dexnavOW);
            set_callback2(c2_return_overworld_music_scripts_cont);
            break;
        }
}

void dexnav_gui_exit_nosearch() {
    switch (super.multi_purpose_state_tracker) {
        case 0:
            fade_screen(~0, 0, 0x0, 0x10, 0);
            super.multi_purpose_state_tracker++;
            break;
        case 1:
            if (!pal_fade_control.active) {
                free((*DNavState)->backbuffer);
                free((void*)*DNavState);
                super.multi_purpose_state_tracker++;
            }
            break;
        case 2:
            m4aMPlayVolumeControl(&mplay_BGM, 0xFFFF, 256);
            set_callback1(c1_overworld);
            set_callback2(c2_overworld_switch_start_menu);
            break;
        }
}


extern void dexnav_populate_encounter_list();

void dexnav_gui_handler() {
    switch(super.multi_purpose_state_tracker) {
        case 0:
            if (!pal_fade_control.active) {
                dexnav_gui_setup();
                set_callback1(dexnav_gui_handler);
                // allocate dexnav struct
                *DNavState = (struct DexnavHudData*)malloc_and_clear(sizeof(struct DexnavHudData));
                super.multi_purpose_state_tracker++;
            }
            break;
        case 1:
        {
                // load BG assets
                void *dexnav_gbackbuffer = malloc(0x800);
                (*DNavState)->backbuffer = dexnav_gbackbuffer;
                gpu_pal_apply((void *)(&dnav_text_pal), 15 * 16, 32);
                gpu_pal_apply_compressed((void *)dexnav_guiPal, 0, 32);
                LZ77UnCompWram((void *)dexnav_guiMap, (void *)dexnav_gbackbuffer);
                lz77UnCompVram((void *)dexnav_guiTiles, (void *)0x06000000);
                bgid_set_tilemap(1, dexnav_gbackbuffer);
                bgid_mark_for_sync(1);
                bgid_mark_for_sync(0);
                super.multi_purpose_state_tracker++;
        }
            break;
        case 2:
            rbox_init_from_templates(dexnav_boxes);
            dexnav_populate_encounter_list();
            dexnav_load_pnames(1);
            super.multi_purpose_state_tracker++;
            break;
        case 3:
            fade_screen(0xFFFFFFFF, 0, 16, 0, 0x0000);
            gpu_sync_bg_show(0);
            gpu_sync_bg_show(1);
            dexnav_load_pokemon_icons();
            spawn_pointer_arrow();
            (*DNavState)->selected_arr = 0;
            (*DNavState)->selected_index = 0;
            super.multi_purpose_state_tracker++;
            break;
        case 4:
            if (!pal_fade_control.active) {
                switch (super.buttons_new_remapped & (KEY_A | KEY_B | KEY_DOWN | KEY_UP | KEY_LEFT | KEY_RIGHT | KEY_SELECT)) {
                    case KEY_A:
                        {
                        // check selection is valid. Play sound if invalid
                        u16 species = (*DNavState)->selected_arr ? (*DNavState)->water_species[(*DNavState)->selected_index >> 1] : (*DNavState)->grass_species[(*DNavState)->selected_index>>1];
                        // if species is MISSINGNO then error
                        if (species) {
                            // species was valid, save and enter OW HUD mode
                            var_8000 = species;
                            var_8001 = (*DNavState)->selected_arr;
                            set_callback1(dexnav_gui_exit_search);
                            super.multi_purpose_state_tracker = 0;
                            return;
                        } else {
                            // beep and update
                            dexnav_load_pnames(0);
                            audio_play(SOUND_RSE_BERRY_MIX_CLICK);
                        }
                        break;
                        }
                    case KEY_B:
                        // exit to start menu
                        set_callback1(dexnav_gui_exit_nosearch);
                        super.multi_purpose_state_tracker = 0;
                        return;
                    case KEY_DOWN:
                    case KEY_UP:
                        (*DNavState)->selected_arr = !(*DNavState)->selected_arr;
                        (*DNavState)->selected_index = 0;
                        dexnav_load_pnames(1);
                        break;
                    case KEY_LEFT:
                        // wrap cursor around
                        if ((*DNavState)->selected_arr) {
                            (*DNavState)->selected_index = (*DNavState)->selected_index == 0 ? 4 * 2 : (*DNavState)->selected_index - 2;
                        } else {
                            (*DNavState)->selected_index = (*DNavState)->selected_index == 0 ? 11 * 2 : (*DNavState)->selected_index - 2;
                        }
                        dexnav_load_pnames(1);
                        break;
                    case KEY_RIGHT:
                        if ((*DNavState)->selected_arr) {
                            // don't go past index 4 in water
                            (*DNavState)->selected_index = (*DNavState)->selected_index == 4 * 2 ? 0 : (*DNavState)->selected_index + 2;
                        } else {
                            // don't go past index 11 in grass
                            (*DNavState)->selected_index = (*DNavState)->selected_index == 11 * 2 ? 0 : (*DNavState)->selected_index + 2;
                        }
                        dexnav_load_pnames(1);
                        break;
                    case KEY_SELECT:
                    {
                        // check selection is valid. Play sound if invalid
                        u16 species = (*DNavState)->selected_arr ? (*DNavState)->water_species[(*DNavState)->selected_index >> 1] : (*DNavState)->grass_species[(*DNavState)->selected_index>>1];
                        // if species is MISSINGNO then error
                        if (species) {
                            // species was valid
                            dexnav_load_pnames(2);
                            audio_play(SOUND_PC_OPEN);
                            // create value to store in a var
                            u16 var_store = ((*DNavState)->selected_arr << 15) | species;
                            var_set(DEXNAV_VAR, var_store);
                        } else {
                            // beep and update
                            dexnav_load_pnames(0);
                            audio_play(SOUND_RSE_BERRY_MIX_CLICK);
                        }
                        break;
                    }
                    default:
                        return;
                };
                update_cursor_position();
                audio_play(SOUND_GENERIC_CLINK);
            }
            break;
        default:
            break;
    };
}
