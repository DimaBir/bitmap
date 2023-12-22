#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned char *bits; // Pointer to the array of bits
    size_t size;         // Total number of bits in the bitmap
} Bitmap;

// Helper functions to manipulate bits
static void set_bit(unsigned char *bits, int bit_id) {
    bits[bit_id / 8] |= (1 << (bit_id % 8));
}

static void clear_bit(unsigned char *bits, int bit_id) {
    bits[bit_id / 8] &= ~(1 << (bit_id % 8));
}

// Finds the first zero bit in the bitmap
static int find_first_zero_bit(const unsigned char *bits, size_t size) {
    size_t i;
    for (i = 0; i < (size + 7) / 8; ++i) {
        if (bits[i] != 0xFF) {
            break;
        }
    }

    if (i == (size + 7) / 8) {
        return -1; // No zero bit found
    }

    for (int j = 0; j < 8; ++j) {
        int bit_id = i * 8 + j;
        if (bit_id < size && !(bits[i] & (1 << j))) {
            return bit_id;
        }
    }

    return -1;
}

// Initializes a bitmap to manage 'n' bits
Bitmap *bitmap_init(size_t n) {
    Bitmap *bitmap = malloc(sizeof(Bitmap));
    if (!bitmap) return NULL; // Ensure memory allocation was successful

    size_t byte_size = (n + 7) / 8; // Calculate number of bytes needed
    bitmap->bits = calloc(byte_size, sizeof(unsigned char));
    if (!bitmap->bits) {
        free(bitmap);
        return NULL;
    }

    bitmap->size = n;
    return bitmap;
}

// Destroys a bitmap and frees its memory
void bitmap_destroy(Bitmap *bitmap) {
    if (bitmap) {
        free(bitmap->bits);
        free(bitmap);
    }
}

// Allocates the smallest free bit in the bitmap
int bitmap_allocate_idx(Bitmap *bitmap) {
    int bit_id = find_first_zero_bit(bitmap->bits, bitmap->size);
    if (bit_id >= 0) {
        set_bit(bitmap->bits, bit_id);
    }
    return bit_id;
}

// Frees a bit in the bitmap
void bitmap_free(Bitmap *bitmap, int bit_id) {
    if (bit_id >= 0 && bit_id < bitmap->size) {
        clear_bit(bitmap->bits, bit_id);
    }
}

int main() {
    size_t bitmap_size = 33; // Example size for the bitmap
    Bitmap *bitmap = bitmap_init(bitmap_size);

    if (!bitmap) {
        fprintf(stderr, "Failed to initialize bitmap.\n");
        return 1;
    }

    // Test allocation and deallocation of bits
    printf("Allocating and freeing bits...\n");
    for (int i = 0; i < bitmap_size; ++i) {
        int bit_id = bitmap_allocate_idx(bitmap);
        if (bit_id == -1) {
            printf("No free bit available.\n");
            break;
        } else {
            printf("Allocated bit %d\n", bit_id);
        }

        // Every 10th bit free 5th bit to tests if minimum updates to 5th bit
        if(i > 0 && i % 10 == 0){
            bitmap_free(bitmap, 5);
            printf("Freed bit %d\n", 5);
        }
    }

    bitmap_destroy(bitmap);
    return 0;
}
