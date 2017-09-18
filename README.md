# cs50

******
Solutions to Problem Sets for Harvard CS50 on EdX - 2017
By Dennis Pitman
******

Problem Set 1
______________
1) Hello
 Implement a program that prints out a simple greeting to the user, per the below.

  $ ./hello
  hello, world

2) Water
Implement a program that reports a user’s water usage, converting minutes spent in the shower to bottles of drinking water.

$ ./water
Minutes: 1
Bottles: 12

$ ./water
Minutes: 10
Bottles: 120

3) Mario
Implement a program that prints out a double half-pyramid of a specified height, per the below.

$ ./mario
Height: 4
   #  #
  ##  ##
 ###  ###
####  ####

4) Credit
Implement a program that determines whether a provided credit card number is valid according to Luhn’s algorithm.

$ ./credit
Number: 378282246310005
AMEX


Problem Set 2
______________
1) Initials
Implement a program that, given a person’s name, prints a person’s initials, per the below.

$ ./initials
Regulus Arcturus Black
RAB

2) Caesar
Implement a program that encrypts messages using Caesar’s cipher, per the below.

$ ./caesar 13
plaintext:  HELLO
ciphertext: URYYB

3) Vigenere
Implement a program that encrypts messages using Vigenère’s cipher, per the below.

$ ./vigenere ABC
plaintext:  HELLO
ciphertext: HFNLP


Problem Set 3
______________
1) Find

Implement a program that finds a number among numbers, per the below.

$ ./generate 1000 | ./find 42
Didn't find needle in haystack.


2) Fifteen

Implement the Game of Fifteen, per the below.

$ ./fifteen 3
WELCOME TO GAME OF FIFTEEN

8  7  6

5  4  3

2  1  _

Tile to move:


Problem Set 4
______________
1) Whodunit

Answer some questions and then implement a program that reveals a hidden message in a BMP, per the below.

$ ./whodunit clue.bmp verdict.bmp


2) Resize

Implement a program that resizes BMPs, per the below.

$ ./resize 4 small.bmp large.bmp


3) Recover

Implement a program that recovers JPEGs from a forensic image, per the below.

$ ./recover card.raw


Problem Set 5
______________
1) Speller

Implement a program that spell-checks a file, per the below.

$ ./speller texts/austinpowers.txt
MISSPELLED WORDS

[...]
Bigglesworth
[...]
Virtucon
[...]
friggin'
[...]
trippy
[...]

WORDS MISSPELLED:
WORDS IN DICTIONARY:
WORDS IN TEXT:
TIME IN load:
TIME IN check:
TIME IN size:
TIME IN unload:
TIME IN TOTAL:

