#include <kernel/drivers/fb_terminal.h>
#include <kernel/bootdata.h>
#include <kernel/fonts/fonts.h> // includes some basic fonts
#include <string.h> // includes some basic fonts



int fb_term_height = 0; // how many chars tall
int fb_term_width = 0; // how many chars wide
static size_t fb_term_size = 0; // full absolute size not how many chars it holds
int fb_term_bpp = 0; // full absolute size not how many chars it holds
static glyph_size =0;

struct fb_cursor_pos {
    int x;
    int y;
};


int FB_TERM_COLOR = 0xFFFFFFFF;

int* fb = FB_ADDR;
struct fb_cursor_pos cur;

font_data_t* font = &f_vga_basic;

void fb_term_init(boot_data_t* data){
    fb_term_height = data->fb_height;
    fb_term_width = data->fb_width;
    fb_term_bpp = data->fb_bpp;
    fb_term_size = data->fb_width * data->fb_height * (fb_term_bpp/4);
    glyph_size = font->height;
    cur.x = 0;
    cur.y = 0;

    memset(fb,0, 1024 * 768 * 4);
}


void buffer_putchar(char c){
    if(c == '\n'){
        cur.x = 0;
        cur.y += font->height;

        if(cur.y >= fb_term_height){
            buffer_scroll_down();
            cur.y -= font->height;
        }
        return;
    }

    unsigned int shift = 0;
    
    for(int i = 0; i < font->height; i++){
        shift = 1 << font->width;
        for(int j = 0; j < font->width; j++){
            if((font->data[(c * glyph_size) + i] & shift) > 0 ){
                fb[(((cur.y + i) * fb_term_width) + j + cur.x)] = FB_TERM_COLOR;
            }
            shift = shift >> 1;
        }
    }
    cur.x += font->width;

    if(cur.x >= fb_term_width){
        cur.x = 0;
        cur.y += font->height;

        if(cur.y >= fb_term_height){
            buffer_scroll_down();
            cur.y -= font->height;
        }
    } 

}

void buffer_write(const char* data, size_t size){
    for(int i = 0; i < size; i++){
        buffer_putchar(data[i]);
    }
}


void buffer_write_string(const char* data){
    for(int i = 0; i < 5000; i++){
        if(data[i] == '\0'){break;}
        buffer_putchar(data[i]);
    }
}

void buffer_scroll_down(){
    int h = font->height * fb_term_width;
    if(fb_term_height/font->height != 48){
        return;
    }
    memcpy(fb, (fb + h), h * 4 * ((fb_term_height/font->height) - 1));
    memset(fb + h * ((fb_term_height/font->height) - 1) , 0, h * 4);
    cur.x = 0;
}


// maby implement if needed
// i want to be into usermode soon
// so it will be in the new terminal
void buffer_scroll_up(){
    return;
}
