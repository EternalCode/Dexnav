#include <pokeagb/pokeagb.h>
#include "dexnav_hud.h"
#include "../../../generated/images/pokenav/dexnav_stars.h"
#include "../../../generated/images/pokenav/empty.h"


const struct Frame (**nullframe)[] = (const struct Frame (**)[])0x8231CF0;
const struct RotscaleFrame (**nullrsf)[] = (const struct RotscaleFrame (**)[])0x8231CFC;
// 32x64 oam with highest priority
const struct OamData font_oam = {    .y = ICONY,
                                    .affine_mode = 0,
                                    .obj_mode = 0,
                                    .mosaic = 0,
                                    .bpp = 0,
                                    .shape = 1,
                                    .x = ICONX,
                                    .matrix_num = 0,
                                    .size = 3,
                                    .tile_num = 0,
                                    .priority = 0,
                                    .palette_num = 0,
                                    .affine_param = 0
};

// 8x8 oam with highest priority
const struct OamData held_oam = {   .y = ICONY,
                                    .affine_mode = 0,
                                    .obj_mode = 0,
                                    .mosaic = 0,
                                    .bpp = 0,
                                    .shape = 0,
                                    .x = ICONX,
                                    .matrix_num = 0,
                                    .size = 0,
                                    .tile_num = 0,
                                    .priority = 0,
                                    .palette_num = 0,
                                    .affine_param = 0
};

struct SpritePalette heldpal = {(const u8*)&pal_heldicons, 0x8472};


// 32x32 object with priority 1, one less than held item which overlaps it
const struct OamData picon_oam = {  .y = ICONY,
                                    .affine_mode = 0,
                                    .obj_mode = 0,
                                    .mosaic = 0,
                                    .bpp = 0,
                                    .shape = 0,
                                    .x = ICONX,
                                    .matrix_num = 0,
                                    .size = 2,
                                    .tile_num = 0,
                                    .priority = 1,
                                    .palette_num = 0,
                                    .affine_param = 0
};
const struct Template bulbtemp = {0x3139, 0x3139, &picon_oam, (const struct Frame (**)[])0x8231CF0, 0x0,
                            (const struct RotscaleFrame (**)[])0x8231CFC, (ObjectCallback)0x800760D};
