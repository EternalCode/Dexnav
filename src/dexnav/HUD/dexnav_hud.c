#include <pokeagb/pokeagb.h>
#include "dexnav_hud.h"
#include "../../pokemon_base_stats.h"

static struct DexnavHudData** DNavState = (struct DexnavHudData**)(NAV_SAVERAM + SPECIES_MAX);
static u8* SearchLevels = (u8*)NAV_SAVERAM;


void init_dexnav_hud(u16 species, u8 environment) {
    *DNavState = (struct DexnavHudData*)malloc_and_clear(sizeof(struct DexnavHudData));
    // assign non-objects to struct
    (*DNavState)->species = species;
    (*DNavState)->environment = environment;
    u8 search_level = SearchLevels[species];
    (*DNavState)->search_level = search_level;
    (*DNavState)->pokemon_level = dexnav_generate_pokemonlvl(species, search_level, environment);


    if ((*DNavState)->pokemon_level < 1) {
        free((void*)*DNavState);
        pchar empty_string[] = _("This Pokemon cannot be found here.\p");
        msg_normal(empty_string);
        return;
    }

    // draw shaking grass
    extern u8 shaking_grass(u8, u8, u8, bool);
    if (!shaking_grass(environment, 12, 12, 0)) {
        free((void*)*DNavState);
        pchar empty_string[] = _("It couldn’t be found nearby.\nTry looking in a different area!\p");
        msg_normal(empty_string);
        return;
    }

    // Populate DNavState objects
    dexnav_generate_move(species, search_level, (*DNavState)->pokemon_level, &(*DNavState)->move_id[0]);
    (*DNavState)->held_item = dexnav_generate_helditem(species, search_level);
    (*DNavState)->ability = dexnav_generate_hiddenability(species, search_level);
    (*DNavState)->potential = dexnav_generate_potential(search_level);
    dexnav_proximity_update();

    // draw icons
    dexnav_draw_icons();

    // hide icons based on proximity and search level
    dexnav_icons_vision_update((*DNavState)->proximity, search_level);

    // enable Hblank interrupt
    interrupts_enable(2);
    hblank_handler_set((SuperCallback)dexhud_hblank);

    // task update HUD
    u8 t_id = task_add((TaskCallback)dexnav_hud_manage, 0x1);
    tasks[t_id].priv[0] = objects[walkrun_state.oamid].pos1.x;

}

/* This is called via a c1 from the GUI, while waiting to return to the OW */
void exec_dexnav_hud() {
    if (!pal_fade_control.active && !script_env_2_is_enabled() && super.callback2 == c2_overworld) {
        set_callback1(c1_overworld);
        init_dexnav_hud(var_8000, var_8001);
    }
}
