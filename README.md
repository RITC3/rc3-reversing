RC3 Reversing
=============
Hello! Here are the solutions:<br>

Easy
----
$ strings easy<br>
You win! The flag is RC3-ICAN-1337<br>
better luck next time<br>

Flag **RC3-ICAN-1337**<br>

Medium
------
Disassembling shows this section:<br>
$ objdump -S
<pre>
...
 8048ccc:       c6 85 d8 f3 ff ff 48    movb   $0x48,-0xc28(%ebp)
 8048cd3:       c6 85 d9 f3 ff ff 41    movb   $0x41,-0xc27(%ebp)
 8048cda:       c6 85 da f3 ff ff 43    movb   $0x43,-0xc26(%ebp)
 8048ce1:       c6 85 db f3 ff ff 4b    movb   $0x4b,-0xc25(%ebp)
 8048ce8:       c6 85 dc f3 ff ff 20    movb   $0x20,-0xc24(%ebp)
 8048cef:       c6 85 dd f3 ff ff 54    movb   $0x54,-0xc23(%ebp)
 8048cf6:       c6 85 de f3 ff ff 48    movb   $0x48,-0xc22(%ebp)
 8048cfd:       c6 85 df f3 ff ff 45    movb   $0x45,-0xc21(%ebp)
 8048d04:       c6 85 e0 f3 ff ff 20    movb   $0x20,-0xc20(%ebp)
 8048d0b:       c6 85 e1 f3 ff ff 47    movb   $0x47,-0xc1f(%ebp)
 8048d12:       c6 85 e2 f3 ff ff 49    movb   $0x49,-0xc1e(%ebp)
 8048d19:       c6 85 e3 f3 ff ff 42    movb   $0x42,-0xc1d(%ebp)
 8048d20:       c6 85 e4 f3 ff ff 53    movb   $0x53,-0xc1c(%ebp)
 8048d27:       c6 85 e5 f3 ff ff 4f    movb   $0x4f,-0xc1b(%ebp)
 8048d2e:       c6 85 e6 f3 ff ff 4e    movb   $0x4e,-0xc1a(%ebp)
 8048d35:       c6 85 e7 f3 ff ff 0b    movb   $0xb,-0xc19(%ebp)
...
</pre>

These are characters:<br>
0x48, 0x41, 0x43, 0x4b, 0x20, 0x54, 0x48, 0x45, 0x20, 0x47, 0x49, 0x42, 0x53, 0x4f, 0x4e<br>
When converted to ASCII...<br>
<pre>
>>> import sys
>>> key = [ 0x48, 0x41, 0x43, 0x4b, 0x20, 0x54, 0x48, 0x45, 0x20, 0x47, 0x49, 0x42, 0x53, 0x4f, 0x4e ]
>>> for i in key:
...   sys.stdout.write(chr(i))
...
HACK THE GIBSON
</pre>
$ nc reverseme.havefuninside.me 54321<br>
The executables can be downloaded from this server. Visit in web browser<br>
Enter the super secret key: HACK THE GIBSON<br>

You win! The flag is RC3-WUMB-0055<br>
You can also win this challenge by hosting locally and checking out the following comparison in gdb:<br>
<pre>
8048dc4:       8d 85 d8 f3 ff ff       lea    -0xc28(%ebp),%eax
8048dca:       89 44 24 04             mov    %eax,0x4(%esp)
8048dce:       8d 85 ec f7 ff ff       lea    -0x814(%ebp),%eax
8048dd4:       89 04 24                mov    %eax,(%esp)
8048dd7:       e8 64 f9 ff ff          call   8048740 <strstr@plt>
$ gdb -q ./medium
(gdb) break *0x8048dd7
(gdb) run
...nc 127.0.0.1 54321 and enter a password...
Breakpoint 1 at 0x8048dd7
(gdb) x/s $ebp-0xc28
0xf7e3a770:      "HACK THE GIBSON"
</pre>
strstr is a string comparison function. It is located right before the fopen jump.<br>
The instruction:<br>
8048dc4:       8d 85 d8 f3 ff ff       lea    -0xc28(%ebp),%eax<br>
Shows something being loaded from the ebp register minus 0xc28<br>
We can look there and find the key!<br>

**Flag: RC3-WUMB-0055**<br>

HARD
----
There were two flags in this one.<br>
The key for the first flag is understanding how random number generation works on computers.<br>
In the disassembly we can see that srand is seeded off of 1 every time.<br>
<pre>
$ objdump -S hard
...
 8048c20:       c7 04 24 01 00 00 00    movl   $0x1,(%esp)
 8048c27:       e8 e4 fb ff ff          call   8048810 <srand@plt>
 8048c2c:       e8 4f fc ff ff          call   8048880 <rand@plt>
 8048c31:       89 c3                   mov    %eax,%ebx
 8048c33:       e8 48 fc ff ff          call   8048880 <rand@plt>
 8048c38:       01 c3                   add    %eax,%ebx
 8048c3a:       e8 41 fc ff ff          call   8048880 <rand@plt>
 8048c3f:       29 c3                   sub    %eax,%ebx
...
</pre>
Additionally, we can see that rand is used 3 times.<br>
The first srand is moved to ebx.<br>
The second is added to ebx.<br>
And then the third is subtracted from ebx.<br>
So: srand(1); num = rand() + rand() - rand() is the number it is looking for.<br>
The srand.c program was a hint. Change it to output the solution.<br>
<pre>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    srand(atoi(argv[1]));
    printf("%d\n", rand() + rand() - rand());
    return 0;
}
</pre>
<pre>
$ gcc -o srand srand.c
$ ./srand 1
969527492
</pre>
So to solve:<br>
<pre>
$ ./srand 1 | nc reverseme.havefuninside.me 54322
It may be useful to know that this program is running on Ubuntu 14.04 x32.
The executables can be downloaded from this server. Visit in web browser
I'm thinking of a number... Take a guess:
You win! The flag is RC3-REAL-HAXX
</pre>

Flag 1: **RC3-REAL-HAXX**
