#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned char *bits;
    size_t size;
} Bitmap;

Bitmap *bitmap_init(size_t n);
void bitmap_destroy(Bitmap *bitmap);
int bitmap_allocate_udx(Bitmap *bitmap);
void bitmap_free(Bitmap *bitmap, int bit_id);

static int find_first_zero_bit(const unsigned char *bits, size_t size);
static void set_bit(unsigned char *bits, int bit_id);
static void clear_bit(unsigned char *bits, int bit_id);

int main() {
    size_t bitmap_size = 32; // Example size
    Bitmap *bitmap = bitmap_init(bitmap_size);

    if (!bitmap) {
        fprintf(stderr, "Failed to initialize bitmap.\n");
        return 1;
    }

    // Test allocation and deallocation
    printf("Allocating and freeing bits...\n");
    for (int i = 0; i < bitmap_size; ++i) {
        int bit_id = bitmap_allocate_udx(bitmap);
        if (bit_id == -1) {
            printf("No free bit available.\n");
            break;
        } else {
            printf("Allocated bit %d\n", bit_id);
        }

        bitmap_free(bitmap, bit_id);
        printf("Freed bit %d\n", bit_id);
    }

    bitmap_destroy(bitmap);
    return 0;
}

Bitmap *bitmap_init(size_t n) {
    Bitmap *bitmap = malloc(sizeof(Bitmap));
    if (!bitmap) return NULL;

    size_t byte_size = (n + 7) / 8;
    bitmap->bits = calloc(byte_size, sizeof(unsigned char));
    if (!bitmap->bits) {
        free(bitmap);
        return NULL;
    }

    bitmap->size = n;
    return bitmap;
}

void bitmap_destroy(Bitmap *bitmap) {
    if (bitmap) {
        free(bitmap->bits);
        free(bitmap);
    }
}

int bitmap_allocate_udx(Bitmap *bitmap) {
    int bit_id = find_first_zero_bit(bitmap->bits, bitmap->size);
    if (bit_id >= 0) {
        set_bit(bitmap->bits, bit_id);
    }
    return bit_id;
}

void bitmap_free(Bitmap *bitmap, int bit_id) {
    if (bit_id >= 0 && bit_id < bitmap->size) {
        clear_bit(bitmap->bits, bit_id);
    }
}

static int find_first_zero_bit(const unsigned char *bits, size_t size) {
    size_t i;
    for (i = 0; i < (size + 7) / 8; ++i) {
        if (bits[i] != 0xFF) {
            break;
        }
    }

    if (i == (size + 7) / 8) {
        return -1;
    }

    for (int j = 0; j < 8; ++j) {
        int bit_id = i * 8 + j;
        if (bit_id < size && !(bits[i] & (1 << j))) {
            return bit_id;
        }
    }

    return -1;
}

static void set_bit(unsigned char *bits, int bit_id) {
    bits[bit_id / 8] |= (1 << (bit_id % 8));
}

static void clear_bit(unsigned char *bits, int bit_id) {
    bits[bit_id / 8] &= ~(1 << (bit_id % 8));
}
