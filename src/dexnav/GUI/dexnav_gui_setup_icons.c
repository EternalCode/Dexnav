#include <pokeagb/pokeagb.h>
#include "../HUD/dexnav_hud.h"

static struct DexnavHudData** DNavState = (struct DexnavHudData**)(NAV_SAVERAM + SPECIES_MAX);

/* Functions responsible for populating icons species array with seen and encounterable Pokemon */
bool species_in_array(u16 species, u8 index_count) {
    // disallow species not seen
    if (!dex_flag_pokedex_index(species, DEX_FLAG_CHECK_SEEN))
        return true;
    for (u8 i = 0; i < index_count; i++) {
        if (index_count == 12) {
            if ((*DNavState)->grass_species[i] == species) {
                return true;
            }
        } else {
            if ((*DNavState)->water_species[i] == species) {
                return true;
            }
        }
    }
    return false;
}

void dexnav_populate_encounter_list() {
    // nop struct data
    memset(&((*DNavState)->grass_species[0]), 0, 34);
    // populate unique wild grass encounters
    u8 insertion_index = 0;
    u8 index = get_wild_data_index_for_map();

    if (wild_pokemon_data[index].grass_encounter) {
        for (u8 i = 0; i < 12; i++) {
            struct wild_pokemon w_pkmn = wild_pokemon_data[index].grass_encounter->wild_encounters->wild_grass[i];
            if (!species_in_array(w_pkmn.species, 12)) {
                (*DNavState)->grass_species[insertion_index] = w_pkmn.species;
                insertion_index++;
            }
        }
    }
    // populate unique wild water encounters
    insertion_index = 0;
    // exit if no water encounters
    if (wild_pokemon_data[index].water_encounter == NULL)
        return;
    for (u8 i = 0; i < 5; i++) {
        struct wild_pokemon w_pkmn = wild_pokemon_data[index].water_encounter->wild_encounters->wild_water[i];
        if (!species_in_array(w_pkmn.species, 5)) {
            (*DNavState)->water_species[insertion_index] = w_pkmn.species;
            insertion_index++;
        }
    }
}
