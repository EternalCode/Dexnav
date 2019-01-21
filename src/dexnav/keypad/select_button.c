#include <pokeagb/pokeagb.h>
#include "../HUD/dexnav_hud.h"

extern void init_dexnav_hud(u16 species, u8 environment);


u8 select_hook_restore()
{
    script_env_init_script((void*) 0x81A77A0); // script that lets you know you can register item via select
    return 0;
}

u8 check_registered_select() {
    u16 var_val = var_load(DEXNAV_VAR);
    if (var_val) {
        if (task_is_running(dexnav_hud_manage)) {
            // don't redo the search...
            return 0;
        }
        init_dexnav_hud(var_val & 0x7FFF, var_val >> 15);
        return 0;
    }
    return 1;
}
