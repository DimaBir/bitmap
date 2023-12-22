# Bitmap Data Structure

This is a data structure that efficiently represents a bitmap. Also API implementation for bitmap.

## Features

- **Bitmap Initialization**: Initialize a bitmap to manage a specified number of bits.
- **Bit Allocation**: Allocate the smallest free bit in the bitmap.
- **Bit Freeing**: Free a specific bit in the bitmap.
- **Bit Setting**: Set a specific bit to 1.
- **Bit Clearing**: Clear a specific bit to 0.
- **Finding First Zero Bit**: Find the first zero bit in the bitmap.
- **Efficient Bit Checking**: Checks bytes using bitmasking for efficient bit manipulation.

## Efficiency

The program enhances efficiency by applying a mask to check bits byte by byte, rather than bit by bit.

### Example of Efficient Bit Checking

When finding the first zero bit, the program scans each byte and applies a bitmask to check all bits within that byte simultaneously:

```c
if (bits[i] != 0xFF) {
    // This byte contains a zero bit; further checking is needed
    for (int j = 0; j < 8; ++j) {
        int bit_id = i * 8 + j;
        if (bit_id < size && !(bits[i] & (1 << j))) {
            return bit_id; // Found the first zero bit
        }
    }
}
```
NOTE: If needed, the `0xFF` mask can be enlarged to cover bigger parts of the bitmap.

## Usage

1. **Initialize Bitmap**: Call `bitmap_init(size_t n)` with the desired number of bits.
2. **Allocate Bit**: Use `bitmap_allocate_idx(Bitmap *bitmap)` to allocate a free bit.
3. **Free Bit**: Use `bitmap_free(Bitmap *bitmap, int bit_id)` to free a specific bit.
4. **Set/Clear Bit**: Use `set_bit` and `clear_bit` functions to manually set or clear a bit.
5. **Destroy Bitmap**: Release the memory by calling `bitmap_destroy(Bitmap *bitmap)`.

## Example

```c
size_t bitmap_size = 33; // Example size for the bitmap
Bitmap *bitmap = bitmap_init(bitmap_size);

// Allocate and free bits as required
int bit_id = bitmap_allocate_idx(bitmap);
bitmap_free(bitmap, bit_id);

// Destroy the bitmap after use
bitmap_destroy(bitmap);
```

## Compilation

Compile the program using a C compiler like `gcc`:
```c
gcc program.c -o bitmap
```
