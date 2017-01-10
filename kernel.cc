#include <stddef.h> //size_t NULL
#include <stdint.h> //intx_t uintx_t

// 0 - 15 values for each color.
enum vga_color{
    VGA_BLACK = 0, VGA_BLUE, VGA_GREEN, VGA_CYAN, VGA_RED, VGA_MAGNETA, VGA_BROWN, VGA_LIGHT_GREY,
    VGA_DARK_GREY, VGA_LIGHT_BLUE, VGA_LIGHT_GREEN, VGA_LIGHT_CYAN, VGA_LIGHT_RED, VGA_LIGHT_MAGNETA,
    VGA_LIGHT_BROWN, VGA_WHITE,
};

static inline uint8_t vga_entry_color(enum vga_color a, enum vga_color b)
{
    return a|b << 4 ;
}

static inline uint16_t vga_entry(unsigned char c, uint8_t color)
{
    return (uint16_t)c | (uint16_t)color << 8; 
}

size_t strlen(const char *str)
{
    size_t len = 0;
    while(str[len])
        len++;
    return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer;

void terminal_init()
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = (uint8_t)VGA_BLACK << 4;
    terminal_buffer = (uint16_t *) 0xB8000; // The VGA Buffer is ar B8000
    for (size_t i = 0; i < VGA_HEIGHT; i++) {
        for (size_t j = 0; i < VGA_WIDTH; j++) {
            const size_t index = i * VGA_WIDTH + j;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_putat(const char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(const char c)
{
    terminal_putat(c, terminal_color, terminal_column, terminal_row);
    if( ++ terminal_column = VGA_WIDTH){
        terminal_column = 0;
        if ( ++terminal_row = VGA_HEIGHT)
            terminal_row = 0;
    }
}

void terminal_write(const char *str, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(str[i]);
    }
}

void terminal_writestring(const char *str)
{
    terminal_write(str, strlen(str));
}

extern "C"
void kernel_main(void)
{
    terminal_init();
    terminal_writestring("Hello");
}
