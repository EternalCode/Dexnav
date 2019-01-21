#ifndef PKMN_BASE_STATS_H_
#define PKMN_BASE_STATS_H_

#define PERCENT_FEMALE(percent) MIN(254, ((percent * 255) / 100))
#define MON_MALE       0x00
#define MON_FEMALE     0xFE
#define MON_GENDERLESS 0xFF


struct PokemonBaseStatNew
{
 /* 0x00 */ u8 baseHP;
 /* 0x01 */ u8 baseAttack;
 /* 0x02 */ u8 baseDefense;
 /* 0x03 */ u8 baseSpeed;
 /* 0x04 */ u8 baseSpAttack;
 /* 0x05 */ u8 baseSpDefense;
 /* 0x06 */ u8 type1;
 /* 0x07 */ u8 type2;
 /* 0x08 */ u8 catchRate;
 /* 0x09 */ u8 expYield;
 /* 0x0A */ u16 evYield_HP:2;
 /* 0x0A */ u16 evYield_Attack:2;
 /* 0x0A */ u16 evYield_Defense:2;
 /* 0x0A */ u16 evYield_Speed:2;
 /* 0x0B */ u16 evYield_SpAttack:2;
 /* 0x0B */ u16 evYield_SpDefense:2;
 /* 0x0C */ u16 item1;
 /* 0x0E */ u16 item2;
 /* 0x10 */ u8 genderRatio;
 /* 0x11 */ u8 eggCycles;
 /* 0x12 */ u8 friendship;
 /* 0x13 */ u8 growthRate;
 /* 0x14 */ u8 eggGroup1;
 /* 0x15 */ u8 eggGroup2;
 /* 0x16 */ u8 ability1;
 /* 0x17 */ u8 ability2;
 /* 0x18 */ u8 safariZoneFleeRate;
 /* 0x19 */ u8 bodyColor : 7;
            u8 noFlip : 1;
            u8 hidden_ability;
            u8 padding;
};


#define OLD_UNOWN_BASE_STATS                \
    {                                       \
        .baseHP = 50,                       \
        .baseAttack = 150,                  \
        .baseDefense = 50,                  \
        .baseSpAttack = 150,                \
        .baseSpDefense = 50,                \
        .baseSpeed = 150,                   \
        .type1 = TYPE_NORMAL,               \
        .type2 = TYPE_NORMAL,               \
        .catchRate = 3,                     \
        .expYield = 1,                      \
        .evYield_HP = 2,                    \
        .evYield_Attack = 2,                \
        .evYield_Defense = 2,               \
        .evYield_Speed = 2,                 \
        .evYield_SpAttack = 2,              \
        .evYield_SpDefense = 2,             \
        .item1 = ITEM_NONE,                 \
        .item2 = ITEM_NONE,                 \
        .genderRatio = MON_GENDERLESS,      \
        .eggCycles = 120,                   \
        .friendship = 0,                    \
        .growthRate = EXP_MEDIUM_FAST,   \
        .eggGroup1 = EGG_GROUP_UNDISCOVERED,\
        .eggGroup2 = EGG_GROUP_UNDISCOVERED,\
        .ability1 = ABILITY_NONE,           \
        .ability2 = ABILITY_NONE,           \
        .safariZoneFleeRate = 0,            \
        .bodyColor = POKEMON_COLOR_BLACK,      \
        .noFlip = false,                    \
        .hidden_ability = ABILITY_NONE,     \
        .padding = 0                       \
    }

extern const struct PokemonBaseStatNew gBaseStats[POKEAGB_POKEMON_SLOTS];


#endif /* PKMN_BASE_STATS_H_ */
