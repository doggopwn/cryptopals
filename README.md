# cryptopals
The solutions to cryptographic cracking challenges located at cryptopals.com, coded using pure C and zsh scripts. 

For any inquiries or questions, please submit an issue here.

Stay memory safe.

# Credits
In [challenge 03 (set 1)](03-crack-sxor), [a letter frequency of English text](http://www.fitaly.com/board/domper3/posts/136.html) was used as no other good source was available. It includes all the ASCII printable characters, allowing better analysis.

In [challenge 07 (set 1)](07-aes128-ecb), [tiny-AES128-C](https://github.com/bitdust/tiny-AES128-C/) was used to avoid coding AES encryption/decryption. It is minimalistic and works with unsigned char arrays out of the box. The relevant files are [aes.h](07-aes128-ecb/aes.h) and [aes.c](07-aes128-ecb/aes.c).
