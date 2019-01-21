#include <pokeagb/pokeagb.h>

/* Overworld textbox functions */
void task_delete_completed_textbox(u8 t_id) {
    if (!box_status_and_type) {
        textbox_close();
        task_del(t_id);
        script_env_disable();
    }
}

void msg_normal(pchar *str){
   textbox_fdecode_auto_and_task_add(str);
   box_status_and_type = 1;
   task_add((TaskCallback)task_delete_completed_textbox, 0x1);
   script_env_enable();
   return;
}

/* GUI Rboxes */
#define rgb5(r, g, b) (u16)((r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10))

const u16 dnav_text_pal[] = {rgb5(255, 0, 255),   rgb5(248, 248, 248), rgb5(112, 112, 112), rgb5(96, 96, 96),
                                rgb5(208, 208, 208), rgb5(76, 154, 38),   rgb5(102, 194, 66),  rgb5(168, 75, 76),
                                rgb5(224, 114, 75),  rgb5(180, 124, 41),  rgb5(241, 188, 60),  rgb5(255, 0, 255),
                                rgb5(255, 0, 255),   rgb5(255, 0, 255),   rgb5(255, 133, 200),   rgb5(64, 200, 248)};

struct TextColor dnav_text_black = {0, 3, 4};
struct TextColor dnav_text_white = {0, 1, 2};
struct TextColor dnav_text_red = {0, 7, 8};
struct TextColor dnav_text_green = {0, 5, 6};

struct TextboxTemplate dexnav_boxes[] = {
    {
        /*Species */
        .bg_id = 0,
        .x = 21,
        .y = 6,
        .width = 9,
        .height = 2,
        .pal_id = 15,
        .charbase = 1,
    },
    {
        /*Search level */
        .bg_id = 0,
        .x = 21,
        .y = 9,
        .width = 9,
        .height = 2,
        .pal_id = 15,
        .charbase = 31,
    },
    {
        /*Level bonus */
        .bg_id = 0,
        .x = 21,
        .y = 12,
        .width = 3,
        .height = 3,
        .pal_id = 15,
        .charbase = 61,
    },
    {
        /*Hidden Ability */
        .bg_id = 0,
        .x = 21,
        .y = 15,
        .width = 12,
        .height = 3,
        .pal_id = 15,
        .charbase = 76,
    },
    {
        /* Reply text */
        .bg_id = 0,
        .x = 1,
        .y = 17,
        .width = 22,
        .height = 3,
        .pal_id = 15,
        .charbase = 136,
    },
    {
        .bg_id = 0xFF, /* marks the end of the tb array */
    },

};
