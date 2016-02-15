# IES
*Image Encryption System*
## Description
A basic small utility to hide and retrieve any text into a 24 Bitmap image.
## Build
Use the given makefile:
```shell
> make
```
## Run
Commands available:
- -e to encrypt
```shell
> ./ies -e some_image.bmp
> Type your phrase: <Input_text>
```
- -d to decrypt
```shell
> ./ies -d some_image.bmp
> The phrase was: <Input_text>
```

## Encoding method
The 2 lower bits of the RGB pixels are used to store the input data split in chunks of 2 bits.
```c
...
// Keep only the higher 6 bits of the color byte, add the 2 bits of the text (which have been shifted to the right accordingly then ANDED to keep only the 2 lowest bits). For 1 byte of text we need 4 bytes of color.
buffer = (pixelData[i] & 0xFC) | ((inputText[k] >> (0x6 - (j * 2))) & 0x3);
...
```
## Decoding method
Just do it in reverse order.
```c
...
// Take a color pixel, keep the lowest 2 bits (0x3), shift to the left by an offset: 0x6 minus 2 times the separate counter j.
buffer |= (pixelData[i] & 0x3) << (0x6 - (j * 0x2));
...
```

## Known bugs
- If the user inputs more text than the image can hold, the overflow is ignored.
- Only works with 24 bitmap images.
- Not optimized to write in the noisy parts of the image.
