#include <stddef.h> //size_t NULL
#include <stdint.h> //intx_t uintx_t

// 0 - 15 values for each color.
enum vga_color{
	VGA_BLACK = 0,
	VGA_BLUE = 1,
	VGA_GREEN = 2,
	VGA_CYAN = 3,
	VGA_RED = 4,
	VGA_MAGNETA = 5,
	VGA_BROWN = 6,
	VGA_LIGHT_GREY = 7,
	VGA_DARK_GREY = 8,
	VGA_LIGHT_BLUE = 9,
	VGA_LIGHT_GREEN = 10,
	VGA_LIGHT_CYAN = 11,
	VGA_LIGHT_RED = 12,
	VGA_LIGHT_MAGNETA = 13,
	VGA_LIGHT_BROWN = 14,
	VGA_WHITE = 15,
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
    terminal_color = VGA_WHITE | VGA_BLACK << 4;
    terminal_buffer = (uint16_t *) 0xB8000; // The VGA Buffer is ar B8000
    for (size_t i = 0; i < VGA_HEIGHT; i++) {
        for (size_t j = 0; j < VGA_WIDTH; j++) {
            const size_t index = i * VGA_WIDTH + j;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void scroll_term()
{
    size_t index = 0;
    size_t index_to = (VGA_HEIGHT - 1) * VGA_WIDTH;
    // copy the contents from the next line to the line above
    // TODO : break if no content to be copied ( optimize this)
    while(index < index_to){
        terminal_buffer[index] = terminal_buffer[index + VGA_WIDTH];
        index++;
    }
    // clear the last line
    index_to += VGA_WIDTH;
    for(; index < index_to; index++)
        terminal_buffer[index] = vga_entry(' ', terminal_color);
    terminal_row = 0;
    terminal_column = VGA_HEIGHT;
}

void terminal_putat(const char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(const char c)
{
	if (c == '\n') {
		++terminal_row;
		terminal_column = 0;
		return;
	}
    terminal_putat(c, terminal_color, terminal_column, terminal_row);
    if(++terminal_column == VGA_WIDTH){
        terminal_column = 0;
        if ( ++terminal_row == VGA_HEIGHT){
			terminal_row = 0;
			scroll_term();
		}
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
    scroll_term();
    /*
    for (size_t i = 0; i < VGA_HEIGHT; i++) {
    terminal_writestring("Welcome to qwnOS \n");        
    }
    terminal_writestring("asdf");
    */
}
