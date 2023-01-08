#include "map.hpp"
#include "path.hpp"
#include "alinit.hpp"
#include "images.hpp"
#include "draw.hpp"
#include "util.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

#define SCROLL_ZONE 32
#define CAM_SPEED 4

int g_cx = 0, g_cy = 0;
int g_camX = 0, g_camY = 0;

unsigned char g_map1[] = {
    15,15,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,0,0,1,0,0,0,0,0,1,0,1,0,1,
    1,0,0,0,4,0,0,0,0,0,1,0,1,0,1,
    1,0,0,0,4,0,0,0,0,0,0,0,0,0,1,
    1,1,4,2,1,0,0,1,2,1,1,1,2,1,1,
    1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
    1,1,4,2,1,0,0,1,0,0,0,0,0,0,1,
    1,0,0,0,1,0,0,1,0,0,0,0,0,0,1,
    1,0,0,0,1,0,0,4,0,0,0,0,0,0,1,
    1,0,0,0,1,0,0,4,0,0,0,0,0,0,1,
    1,1,2,1,1,0,0,1,0,0,0,0,0,0,1,
    1,0,0,1,0,0,0,4,0,0,0,0,0,0,1,
    1,0,0,1,0,0,0,4,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

void draw() {
    int w = al_get_display_width(g_disp);
    int h = al_get_display_height(g_disp);

    al_clear_to_color(al_map_rgb(0, 0, 0));

    drawMap(-g_camX, -g_camY);

    drawBitmap(b_ui, 16, 0, 16, 16, (g_cx*16)-g_camX, (g_cy*16)-g_camY);

    char buf[200];
    sprintf(buf, "%d %d", -g_camX, -g_camY);
    drawText(buf, 0, 0);
    drawText("\"Hmm, how about this font?\"", 0, 8);

    ALLEGRO_MOUSE_STATE ms;
    al_get_mouse_state(&ms);
    drawBitmap(b_ui, 0, 0, 8, 8, ms.x/g_scale, ms.y/g_scale);

    al_flip_display();
}

void update() {
    int w = al_get_display_width(g_disp);
    int h = al_get_display_height(g_disp);

    ALLEGRO_MOUSE_STATE ms;
    al_get_mouse_state(&ms);
    int cx = ((ms.x/g_scale)+g_camX)/16;
    int cy = ((ms.y/g_scale)+g_camY)/16;
    if(inMapBounds(cx, cy)) { g_cx = cx; g_cy = cy; }

    if(ms.x < SCROLL_ZONE) g_camX -= CAM_SPEED;
    else if(ms.x >= w-SCROLL_ZONE) g_camX += CAM_SPEED;
    if(ms.y < SCROLL_ZONE) g_camY -= CAM_SPEED;
    else if(ms.y >= h-SCROLL_ZONE) g_camY += CAM_SPEED;
    clamp(g_camX, -(w/g_scale)/2, g_mapw*16-(w/g_scale)/2);
    clamp(g_camY, -(h/g_scale)/2, g_maph*16-(h/g_scale)/2);
}

int main() {
    initAl();
    loadImages();
    loadMapArr(g_map1);

    bool quit = false;
    while(!quit) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(g_queue, &ev);
        switch(ev.type) {
        case ALLEGRO_EVENT_TIMER:
            update();
            break;
        case ALLEGRO_EVENT_DISPLAY_CLOSE: quit = true; break;
        case ALLEGRO_EVENT_KEY_DOWN: break;
        case ALLEGRO_EVENT_KEY_UP: break;
        case ALLEGRO_EVENT_DISPLAY_RESIZE:
            al_acknowledge_resize(g_disp);
            break;
        }
        if(al_is_event_queue_empty(g_queue))
            draw();
    }

    freeImages();
    endAl();
    return 0;
}