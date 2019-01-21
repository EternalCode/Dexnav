#include <pokeagb/pokeagb.h>
#include "dexnav_hud.h"
#include "../../../generated/images/pokenav/dexnav_stars.h"
#include "../../../generated/images/pokenav/empty.h"

void outlined_font_draw(u8 obj_id, u8 tile_num, u16 size);
extern u16 rand_range(u16 min, u16 max);
extern const struct OamData font_oam;
extern const struct OamData held_oam;
extern struct SpritePalette heldpal;
extern const struct OamData picon_oam;
extern const struct Template bulbtemp;
extern const struct Frame (**nullframe)[];
extern const struct RotscaleFrame (**nullrsf)[];
static struct DexnavHudData** DNavState = (struct DexnavHudData**)(NAV_SAVERAM + SPECIES_MAX);

void dexnav_draw_ability(enum PokemonAbility ability, u8* objid) {
    // create empty object of size 32x64 to draw font on
    struct SpriteTiles fontsprite_ability = {(const u8*)(&emptyTiles), 0x800, 0x1EE7};
    struct Template font_temp_ability = {0x1EE7, 0x8472, &font_oam, nullframe, 0x0,
                                nullrsf, (ObjectCallback)0x800760D};
    gpu_tile_obj_alloc_tag_and_upload(&fontsprite_ability);
    gpu_pal_obj_alloc_tag_and_apply(&heldpal);
    u8 obj_id = template_instanciate_forward_search(&font_temp_ability, ICONX + 80, ICONY + 0x12, 0x0);
    *objid = obj_id;
    objects[obj_id].final_oam.affine_mode = 2;

    // ability name beside move name
    u8 len = (*DNavState)->move_name_length;
    objects[obj_id].pos1.x += ((8 * (len/2)) + (4 * (len % 2)));

    // Copy ability string from table using state id
    memcpy((void *)(string_buffer), (void *)&(pokemon_ability_names[ability]), POKEAGB_ABILITY_NAME_LENGTH);

    // format string so it's even length or if it's odd ends in two spaces
    string_buffer[POKEAGB_ABILITY_NAME_LENGTH + 1] = 0xFF;
    len = pstrlen(string_buffer);
    if (!(len % 2)) {
        string_buffer[len] = 0x0;
        string_buffer[len + 1] = 0x0;
        string_buffer[len + 2] = 0xFF;
    }

    // write name to object
    outlined_font_draw(obj_id, 0, 32 * 8);
    objects[obj_id].final_oam.obj_mode = 1;
    return;
}



void dexnav_draw_move(u16 move, u8 search_level, u8* objid) {
    // create empty object of size 32x64 to draw font on
    struct SpriteTiles fontsprite_move = {(const u8*)(&emptyTiles), 0x800, 0x4736};
    struct Template font_temp_move = {0x4736, 0x8472, &font_oam, nullframe, 0x0,
                            nullrsf, (ObjectCallback)0x800760D};
    gpu_tile_obj_alloc_tag_and_upload(&fontsprite_move);
    gpu_pal_obj_alloc_tag_and_apply(&heldpal);
    u8 obj_id = template_instanciate_forward_search(&font_temp_move, ICONX + 80, ICONY + 0x12, 0x0);
    *objid = obj_id;
    objects[obj_id].final_oam.affine_mode = 2;

    // Copy move string from table using state id, add '/' character to the end of it
    memcpy((void *)string_buffer, (void *)pokemon_move_names[move], POKEAGB_MOVE_NAME_LENGTH);
    string_buffer[POKEAGB_MOVE_NAME_LENGTH + 1] = 0xFF;
    u8 len = pstrlen(string_buffer);

    if (search_level > 2) {
        string_buffer[len] = 0xBA;
        len += 1;
    }

    // record length of move with slash for ability name to be placed beside it
    (*DNavState)->move_name_length = len;

    // adjust string to be even chars, if odd end in two spaces
    if (!(len % 2)) {
        string_buffer[len] = 0x0;
        string_buffer[len + 1] = 0x0;
        string_buffer[len + 2] = 0xFF;
    } else {
        string_buffer[len] = 0xFF;
    }

    // write name to object
    outlined_font_draw(obj_id, 0, 32 * 8);
    objects[obj_id].final_oam.obj_mode = 1;
}


void dexnav_draw_potential(u8 potential, u8* objid) {
    // 19 tiles per row, stars are on the 4th row. 1 tile is 32 bytes. Hence 19 * 4 *32
    struct SpriteTiles staricon_lit = {(const u8*)(&(dexnav_starsTiles[19 * 4 * 32])), 64, 0x61};
    struct SpriteTiles staricon_off = {(const u8*)(&(dexnav_starsTiles[((19 * 4) + 1) *32])), 64, 0x2613};

    // TODO: put these function pointers and other data ptrs in PokeAGB.
    struct Template star_lit_temp = {0x61, 0x8472, &held_oam, nullframe, 0x0,
                                        nullrsf, (ObjectCallback)0x800760D};
    struct Template star_off_temp = {0x2613, 0x8472, &held_oam, nullframe, 0x0,
                                        nullrsf, (ObjectCallback)0x800760D};

    // allocate both the lit and unlit star to VRAM
    gpu_tile_obj_alloc_tag_and_upload(&staricon_lit);
    gpu_tile_obj_alloc_tag_and_upload(&staricon_off);
    gpu_pal_obj_alloc_tag_and_apply(&heldpal);

    // create star objects and space them according to potential 0 - 3
    u8 obj_id;
    u8 i;
    for (i = 0; i < 3; i++) {
        if (potential > i) {
            obj_id = template_instanciate_forward_search(&star_lit_temp, ICONX + 23 + (i * 8), ICONY + 0x5, 0x0);
        } else {
            obj_id = template_instanciate_forward_search(&star_off_temp, ICONX + 23 + (i * 8), ICONY + 0x5, 0x0);
        }
        objid[i] = obj_id;
        objects[obj_id].final_oam.affine_mode = 2;
        objects[obj_id].final_oam.obj_mode = 1;
    }


}


void dexnav_draw_sight(u8 sight_lvl, u8* objid) {

    // create empty object of size 64x32 to draw icons on
    struct SpriteTiles sight_tiles = {(const u8*)(&emptyTiles), 0x800, 0x5424};
    struct Template font_temp_sight = {0x5424, 0x8472, &font_oam, nullframe, 0x0,
                                            nullrsf, (ObjectCallback)0x800760D};
    gpu_tile_obj_alloc_tag_and_upload(&sight_tiles);
    gpu_pal_obj_alloc_tag_and_apply(&heldpal);
    u8 obj_id = template_instanciate_forward_search(&font_temp_sight, ICONX + 192, ICONY + 0x12, 0x0);
    *objid = obj_id;
    objects[obj_id].final_oam.affine_mode = 2;
    extern void dexnav_sight_update(u8);
    dexnav_sight_update(sight_lvl);
    objects[obj_id].final_oam.obj_mode = 1;
}


void dexnav_sight_update(u8 sight) {
    u8 obj_id = (*DNavState)->obj_id_sight;
    // draw sight eye on first tile, takes up two tiles
    u8 tileid = objects[obj_id].final_oam.tile_num;
    u8* towrite = (u8*)((tileid * 32) + (SPRITE_RAM));
    memcpy((void*)towrite, (void*) &(dexnav_starsTiles[((19 * 4) + (7 - (2 * sight))) * 32]), 64);

    // draw the B button tile
    memcpy((void*)(towrite + 128), (void*) &(dexnav_starsTiles[((19 * 4) + 2) *32]), 64);

    // draw info text on the 5th tile
    pchar back[] = _(" Back  ");
    pstrcpy(string_buffer, back);
    outlined_font_draw(obj_id, 5, 32 * 8);
}


void dexnav_draw_helditem(u8* objid) {
    // create object for held item icon
    struct SpriteTiles heldicon = {(const u8*)(&gfx_heldicons), 64, 0x8472};
    struct Template heldtemp = {0x8472, 0x8472, &held_oam, nullframe, 0x0,
                                    nullrsf, (ObjectCallback)0x800760D};
    gpu_tile_obj_alloc_tag_and_upload(&heldicon);
    gpu_pal_obj_alloc_tag_and_apply(&heldpal);
    u8 obj_id = template_instanciate_forward_search(&heldtemp, ICONX + 0x8, ICONY + 0xC, 0x0);
    *objid = obj_id;
    objects[obj_id].final_oam.affine_mode = 2;
    objects[obj_id].final_oam.obj_mode = 1;
}


void dexnav_draw_speciesicon(u16 species, u8* objid) {

    // check which palette the species icon uses
    u8 icon_pal = pokeicon_pal_indices[species];
    struct SpritePalette bulbpal = {(u8*)&(pokeicon_pals[icon_pal]), 0x3139};
    gpu_pal_obj_alloc_tag_and_apply(&bulbpal);

    u32 pid = rand_range(0, 0xFFFF) | rand_range(0, 0xFFFF) << 16;
    void *icon_gfx = load_party_icon_tiles_with_form(species, pid, false);
    struct SpriteTiles bulbicon = {icon_gfx, 4 * 8 * 32, 0x3139};
    gpu_tile_obj_alloc_tag_and_upload(&bulbicon);

    // create object
    u8 obj_id = template_instanciate_forward_search(&bulbtemp, ICONX, ICONY, 0x0);
    *objid = obj_id;
    //dprintf("address is: %x\n", objects[obj_id].image);
    dprintf("address is: %x\n", &picon_oam);
    objects[obj_id].final_oam.affine_mode = 2;
    objects[obj_id].final_oam.obj_mode = 1;
}


void dexnav_draw_icons() {
    u8 search_level = (*DNavState)->search_level;
    dexnav_draw_sight((*DNavState)->proximity, &(*DNavState)->obj_id_sight);
    dexnav_draw_speciesicon((*DNavState)->species, &(*DNavState)->obj_id_species);
    dexnav_draw_move((*DNavState)->move_id[0], search_level, &(*DNavState)->obj_id_move);
    dexnav_draw_ability((*DNavState)->ability, &(*DNavState)->obj_id_ability);
    dexnav_draw_helditem(&(*DNavState)->obj_id_item);
    dexnav_draw_potential((*DNavState)->potential, &(*DNavState)->obj_id_potential[0]);
}


void outlined_font_draw(u8 obj_id, u8 tile_num, u16 size) {
    u8 tile = objects[obj_id].final_oam.tile_num + tile_num;
    u8* towrite = (u8*)((tile * TILE_SIZE) + (SPRITE_RAM));
    //u8* dst = (u8*)malloc(size + TILE_SIZE);
    u8 *dst = (u8*)0x202402C;
    pchar *string_buff_ptr = string_buffer;
    pchar element = *string_buff_ptr;
    u8 counter = 1;
    u16 index = 320;
    u16 prev_index;
    while (element != 0xFF) {
        prev_index = index;
        if ((element <= 0xEE) && (element >= 0xD5)) {
            // lower case letters
            index = (((element - 0xD5) * TILE_SIZE) + 1600);
        } else if ((element <= 0xD4) && (element >= 0xBB)) {
            // upper case letters
            index = (((element - 0xBB) * TILE_SIZE) + 768);
        } else if ((element <= 0xAA) && (element >= 0xA1)) {
            // numbers
            index = (element - 0xA1) * TILE_SIZE;
        } else {
            // misc pchars
            u8 symbol_id = 0;
            switch (element) {
                case 0xF0: // colon
                case 0x0: // space bar
                {
                    symbol_id = 1;
                    break;
                }
                case 0x36: // semi colon used indication of str end
                {
                    symbol_id = 2;
                    break;
                }
                case 0xAC: // question mark
                {
                    symbol_id = 3;
                    break;
                }
                case 0xAE: // dash
                {
                    symbol_id = 4;
                    break;
                }
                case 0xAD: // period
                {
                    symbol_id = 5;
                    break;
                }
                case 0xBA: // slash
                {
                    symbol_id = 6;
                    break;
                }
                case 0xB1: // open double quote
                {
                    symbol_id = 7;
                    break;
                }
                case 0xB2: // close double quote
                {
                    symbol_id = 8;
                    break;
                }
                case 0xB3: // open single quote
                {
                    symbol_id = 9;
                    break;
                }
                case 0xB4: // close single quote
                {
                    symbol_id = 10;
                    break;
                }
                case 0xB0: // elipsis ...
                {
                    symbol_id = 11;
                    break;
                }
                case 0xB8: // comma
                {
                    symbol_id = 12;
                    break;
                }
                case 0xB5: // male
                {
                    symbol_id = 13;
                    //dst =
                    break;
                }
                case 0xB6: // f
                {
                    symbol_id = 14;
                    break;
                }
                case 0xFF: // empty
                {
                    symbol_id = 1;
                    break;
                }
            };
            index = (symbol_id + 9) * TILE_SIZE;
        }

        /* TODO: Use macros here */

        if ((counter == 0) || (*(string_buff_ptr + 1) == 0xFF))  {
            // first or last pcharacters don't need pixel merging
            memcpy((void*)dst, (void*)(&dexnav_starsTiles[index]), TILE_SIZE);
        } else if ((element == 0x0)){
            memcpy((void*)dst, (void*)(&dexnav_starsTiles[index]), TILE_SIZE);
            u8 *prev_letter = (u8*)(&dexnav_starsTiles[prev_index]);
            *(dst + 0) = *(prev_letter + 2);
            *(dst + 4) = *(prev_letter + 6);
            *(dst + 8) = *(prev_letter + 10);
            *(dst + 12) = *(prev_letter + 14);
            *(dst + 16) = *(prev_letter + 18);
            *(dst + 20) = *(prev_letter + 22);
            *(dst + 24) = *(prev_letter + 26);
            *(dst + 28) = *(prev_letter + 30);
        } else if ((*(string_buff_ptr + 1) != 0xFF)) {

            // pcharacter in middle, if blank space fill blank with previous pcharacter's last pixel row IFF previous pchar's last pixel row non-empty
            memcpy((void*)dst, (void*)(&dexnav_starsTiles[index]), TILE_SIZE);
            u8 *prev_letter = (u8*)(&dexnav_starsTiles[prev_index]);
            *(dst) |= (((*(prev_letter + 0) & 0xF) == 0) ? (*(dst + 0) & 0xF) : (*(prev_letter + 0) & 0xF));
            *(dst + 4) |= (((*(prev_letter + 4) & 0xF) == 0) ? (*(dst + 4) & 0xF) : (*(prev_letter + 4) & 0xF));
            *(dst + 8) |= (((*(prev_letter + 8) & 0xF) == 0) ? (*(dst + 8) & 0xF) : (*(prev_letter + 8) & 0xF));
            *(dst + 12) |= (((*(prev_letter + 12) & 0xF) == 0) ? (*(dst + 12) & 0xF) : (*(prev_letter + 12) & 0xF));
            *(dst + 16) |= (((*(prev_letter + 16) & 0xF) == 0) ? (*(dst + 16) & 0xF) : (*(prev_letter + 16) & 0xF));
            *(dst + 20) |= (((*(prev_letter + 20) & 0xF) == 0) ? (*(dst + 20) & 0xF) : (*(prev_letter + 20) & 0xF));
            *(dst + 24) |= (((*(prev_letter + 24) & 0xF) == 0) ? (*(dst + 24) & 0xF) : (*(prev_letter + 24) & 0xF));
            *(dst + 28) |= (((*(prev_letter + 28) & 0xF) == 0) ? (*(dst + 28) & 0xF) : (*(prev_letter + 28) & 0xF));
        }

        if ((counter == 2) && (*(string_buff_ptr + 1) != 0xFF)) {
            // every two pchars, we need to merge
            // 8x8px made of 4x8px from previous pchar and 4x8px of this pchar
            *(dst - 30) = (((*(dst - 30) & 0x0F) == 0) ? (*(dst) & 0xF) :(*(dst - 30) & 0x0F)) | (*(dst) & 0xF0);
            *(dst - 26) = (((*(dst - 26) & 0x0F) == 0) ? (*(dst + 4) & 0xF): (*(dst - 26) & 0x0F))  | (*(dst + 4) & 0xF0);
            *(dst - 22) = (((*(dst - 22) & 0x0F) == 0) ? (*(dst + 8) & 0xF): (*(dst - 22) & 0x0F)) | (*(dst + 8) & 0xF0);
            *(dst - 18) = (((*(dst - 18) & 0x0F) == 0) ? (*(dst + 12) & 0xF): (*(dst - 18) & 0x0F)) | (*(dst + 12) & 0xF0);
            *(dst - 14) = (((*(dst - 14) & 0x0F) == 0) ? (*(dst + 16) & 0xF): (*(dst - 14) & 0x0F)) | (*(dst + 16) & 0xF0);
            *(dst - 10) = (((*(dst - 10) & 0x0F) == 0) ? (*(dst + 20) & 0xF): (*(dst - 10) & 0x0F)) | (*(dst + 20) & 0xF0);
            *(dst - 6) = (((*(dst - 6) & 0x0F) == 0) ? (*(dst + 24) & 0xF): (*(dst - 6) & 0x0F)) | (*(dst + 24) & 0xF0);
            *(dst - 2) = (((*(dst - 2) & 0x0F) == 0) ? (*(dst + 28) & 0xF): (*(dst - 2) & 0x0F)) | (*(dst + 28) & 0xF0);

            // last two pixels unconditional
            *(dst - 29) |= *(dst + 1);
            *(dst - 25) |= *(dst + 5);
            *(dst - 21) |= *(dst + 9);
            *(dst - 17) |= *(dst + 13);
            *(dst - 13) |= *(dst + 17);
            *(dst - 9) |= *(dst + 21);
            *(dst - 5) |= *(dst + 25);
            *(dst - 1) |= *(dst + 29);

            dst -= TILE_SIZE;
            counter = 0;
        }
        counter++;
        dst += TILE_SIZE; // next tile
        string_buff_ptr++;
        element = *string_buff_ptr;
    }
    memcpy((void*)towrite, (void*)(0x202402C), size);
    memset((void*)(0x202402C), 0x0, size + TILE_SIZE);
}
