#include <pokeagb/pokeagb.h>
#include "../../../generated/images/pokenav/cave_smoke.h"


const struct SpriteTiles cave_gfx[4] = {
                                {.data = (const u8*)&cave_smokeTiles[128 * 0],
                                .size = 0x80,
                                .tag = 0xFFFF
                                },
                                {.data = (const u8*)&cave_smokeTiles[128 * 1],
                                .size = 0x80,
                                .tag = 0xFFFF
                                },
                                {.data = (const u8*)&cave_smokeTiles[128 * 2],
                                .size = 0x80,
                                .tag = 0xFFFF
                                },
                                {.data = (const u8*)&cave_smokeTiles[128 * 3],
                                .size = 0x80,
                                .tag = 0xFFFF
                                },
                                };
//struct SpriteTiles* ptr_cave_gfx =
struct Template objt_cave = { .tiles_tag = 0xFFFF,
                                    .pal_tag = 0x1005,
                                    .oam = (struct OamData*)0x83A36F0,
                                    .animation = (const struct Frame (**)[])0x83A5B70,
                                    .graphics = &cave_gfx[0],
                                    .rotscale = (const struct RotscaleFrame (**)[])0x8231CFC,
                                    .callback = (ObjectCallback)0x80DCD1D
};

void oei_1A_cave() {
    struct SpritePalette cave_smoke = {(const u8*)&cave_smokePal, 0x1005};
    gpu_pal_obj_alloc_tag_and_apply(&cave_smoke);
    gpu_pal_apply((u8 *)&cave_smokePal, 29 * 16, 32);
    log_coords_relative_camera(&oe_state.effect_pos.x, &oe_state.effect_pos.y, 8, 8);
    u8 obj_id = template_instanciate_reverse_search(&objt_cave, oe_state.effect_pos.x, oe_state.effect_pos.y, 0xFF);
    if (obj_id != 64) {
        objects[obj_id].bitfield2 |= 2;
        objects[obj_id].priv[0] = 22;
    }

}


struct oie_state_2 {
    const struct SpritePalette* p;
    SuperCallback s;
};
const struct oie_state_2 cave_state = { .p = (const struct SpritePalette*)0x083A5348,
                                        .s = (SuperCallback)oei_1A_cave,
                                        };
extern const struct oie_state_2 cave_state;
