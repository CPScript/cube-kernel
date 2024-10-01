#include <dos.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define VIDEO_MEMORY 0xA0000
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define CUBE_SIZE 5
#define CUBE_COLOR 0xF0 // White color for the cube
#define ENCRYPTED_BOOTLOADER "bootloader.encrypted"
#define ENCRYPTED_KERNEL "kernel.encrypted"
#define DECRYPTED_BOOTLOADER "bootloader.bin"
#define DECRYPTED_KERNEL "kernel.bin"
#define KEY_SIZE 32 // Adjust based on your encryption key size

unsigned char *video = (unsigned char *)VIDEO_MEMORY;

// Placeholder for decryption key
unsigned char decryption_key[KEY_SIZE] = { /* Your decryption key here */ };

void draw_pixel(int x, int y, unsigned char color) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        video[y * SCREEN_WIDTH + x] = color;
    }
}

void clear_screen() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            draw_pixel(x, y, 0x1F); // Baby blue color
        }
    }
}

void draw_terrain() {
    srand(time(NULL)); // Seed for random colors
    for (int y = SCREEN_HEIGHT / 2; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            unsigned char color = rand() % 256; // Random color
            draw_pixel(x, y, color);
        }
    }
}

void draw_cube(int centerX, int centerY) {
    for (int y = -CUBE_SIZE; y <= CUBE_SIZE; y++) {
        for (int x = -CUBE_SIZE; x <= CUBE_SIZE; x++) {
            draw_pixel(centerX + x, centerY + y, CUBE_COLOR);
        }
    }
}

void handle_input(int *posX, int *posY) {
    if (kbhit()) {
        switch (getch()) {
            case 'w': (*posY)--; break; // Move up
            case 's': (*posY)++; break; // Move down
            case 'a': (*posX)--; break; // Move left
            case 'd': (*posX)++; break; // Move right
            case 'r': terminal_interface(); break; // Enter terminal interface
            default: break;
        }
    }
}

void print_string(const char *str, int x, int y) {
    while (*str) {
        draw_pixel(x++, y, *str++); // Simple string printing at (x, y)
    }
}

void get_input(char *command) {
    int i = 0;
    while (1) {
        char ch = getch();
        if (ch == '\r') { // Enter key
            command[i] = '\0';
            break;
        }
        if (ch == 8) { // Backspace
            if (i > 0) {
                i--;
                print_string(" ", 10 + i, 10); // Clear character
                print_string(command, 10, 10); // Reprint command
            }
        } else {
            command[i++] = ch;
            draw_pixel(10 + i - 1, 10, ch); // Print character
        }
    }
}

void terminal_interface() {
    char command[100];
    print_string("Enter command: ", 10, 10);
    
    while (1) {
        get_input(command);
        if (strcmp(command, "reboot") == 0) {
            decrypt_original_files(); // Call decryption function
            break; // Exit terminal interface
        }
    }
}

void decrypt_original_files() {
    // Placeholder for file reading and writing logic
    FILE *enc_file, *dec_file;
    unsigned char buffer[512]; // Buffer for reading chunks of data
    size_t bytesRead;

    // Open the encrypted bootloader
    enc_file = fopen(ENCRYPTED_BOOTLOADER, "rb");
    dec_file = fopen(DECRYPTED_BOOTLOADER, "wb");

    if (enc_file == NULL || dec_file == NULL) {
        print_string("Error opening files!", 10, 12);
        return;
    }

    // Simple decryption loop (for demonstration)
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), enc_file)) > 0) {
        // Decrypt the buffer (implement your decryption logic here)
        for (size_t i = 0; i < bytesRead; i++) {
            buffer[i] ^= decryption_key[i % KEY_SIZE]; // XOR decryption for demonstration
        }
        fwrite(buffer, 1, bytesRead, dec_file);
    }

    fclose(enc_file);
    fclose(dec_file);

    // Repeat for the kernel
    enc_file = fopen(ENCRYPTED_KERNEL, "rb");
    dec_file = fopen(DECRYPTED_KERNEL, "wb");

    if (enc_file == NULL || dec_file == NULL) {
        print_string("Error opening files!", 10, 12);
        return;
    }

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), enc_file)) > 0) {
        for (size_t i = 0; i < bytesRead; i++) {
            buffer[i] ^= decryption_key[i % KEY_SIZE]; // XOR decryption for demonstration
        }
        fwrite(buffer, 1, bytesRead, dec_file);
    }

    fclose(enc_file);
    fclose(dec_file);

    print_string("Files decrypted!", 10, 14);
}

void main() {
    int posX = SCREEN_WIDTH / 2, posY = SCREEN_HEIGHT / 2;

    clear_screen(); // Clear the screen to baby blue
    draw_terrain(); // Draw the terrain at the start

    while (1) {
        clear_screen(); // Clear screen each frame
        draw_terrain(); // Draw terrain
        draw_cube(posX, posY); // Draw the cube

        handle_input(&posX, &posY); // Update position based on input

        // Add a delay for frame rate control (simple busy-wait)
        for (volatile int i = 0; i < 10000; i++);
    }
}
