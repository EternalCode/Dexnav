#include <pokeagb/pokeagb.h>
#include "../pokemon_base_stats.h"

u8 get_ability(u16 species, u8 ability_index) {
    u8 ability = 0;
    //struct PokemonBaseStatNew* bst = (struct PokemonBaseStatNew*)&;
    switch (ability_index) {
        case 0:
            {
            // first
            ability = gBaseStats[species].ability1;
            break;
            }
        case 1:
            {
            // second
            ability = gBaseStats[species].ability2;
            break;
            }
        default:
            {
            // hidden
            ability = gBaseStats[species].hidden_ability;
            break;
            }
    };
    return ability;
}
