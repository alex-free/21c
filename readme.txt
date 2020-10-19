21C by Alex Free
1.0 Release - September 28th 2020
1.0.1 Release - September 30th 2020

==About==
A portable and open source command line game of Blackjack/21 written in Ansi C/C89.

==Release - Info==
- 21c_1.0.1_src - Source
- 21c_1.0.1_src_1.44mb_floppy - Source floppy image
- 21c_1.0.1_win32 - Windows 95-Windows 10 (64 & 32-bit)
- 21c_1.0.1_dos16 - DOS/some Windows versions (DOS 3.30+)
- 21c_1.0.1_dos16_1.44mb_floppy - DOS/some Windows versions (DOS 3.30+) floppy image
- 21c_1.0.1_classic_mac_os - 68k/PPC universal SIOW app for Mac OS 8+
- 21c_1.0_mac_os_x - PPC, PPC64, PPC750, PPC7400, PPC7450, PPC970, i386, x86_64 universal executable 
for Mac OS X 10.3.9-Mac OS 11 (ARM requires Rosetta and is untested but should work).

==Features==
- 'Infinite' deck to draw from
- Infinite splitting
- Dealer hits until 16
- Double Down on an intial sum of 9, 10, or 11
- Insurance bet if the Dealer's first card is an Ace
- Early surrender (late surrender turned out to not be fun)
- Bet and save your balance and pick up where you left off later
- BSD 3-Clause License, Ansi/C89 100% compilant
- Saving:
When you save, a file named 'save.txt' will be generated in the current directory. If 'save.txt' 
is in the current directory when executing '21c' you will be prompted to load it if so desired.

==License==
3-Clause BSD License, see 'license.txt'

==Changelog==
Version 1.0.1 
- fixes dealer hit bug
- fixes save bug
- improves data input methods and bugs
- adds DOS support (not just MSDOS!)

==Building==
Tested To Compile With:
- Macintosh Programmer's Workshop 3.0 GM (MrC - PowerPC, SC - 68k) (Classic Mac OS 8-9.2.2)
- Xcode 3.0 (Apple GCC 4.0) (Mac OS X 10.3.9+)
- DevCPP 4.99.5 (MingW GCC 3.4.2) (Windows 95 OSR 2.5+)
- TurboC++ 3.0 (TCC 3.0) (DOS 3.30+)

This should essentially compile on any OS with a C89 compilant compiler.

Each folder contains the same source file '21c.c' in a different EOL format: 
- eol_cr contains the Classic Mac OS EOL format.
- eol_crlf contains the Windows/DOS EOL format.
- eol_lf contains the Unix EOL format.

Universal Mac OS X 10.3.9+ compile one liner:
gcc -mmacosx-version-min=10.3 -isysroot /Developer/SDKs/MacOSX10.4u.sdk -arch ppc -arch ppc64 -arch ppc750 -arch ppc7400 -arch ppc7450 -arch ppc970 -arch i386 -arch x86_64 -o 21c 21c.c




