# Documentation on task 5
-------------------------

## Decrypting the secret codes

We decrypted the secret codes by trial and error. We did this by entering a
character into the encrypter program and compare it with the encrypted
sentence.

This resulted in the following decrypted sentences:

 - the code is a lie.
 - not a great deal if you're a hacker.
 - the world isn't run by weapons anymore, or energy, or money.

## Analyzing the encrypter

After the decryption of the secret codes, we analyzed the encrypter in order
to figure out it's algorithm. Entering the character 'a' showed 'c' as
encrypted text. But entering 'aa' didn't show 'cc', but 'cp' instead. We
came to the conclusion that the caesar shift depends on the position of the
character in the sentence.

Using a very long sequence of a's we found repition after 130 characters. This
led us to conclude that the shift of each character depends on it's position and
can be looked up in a predfined array of offsets.

## Building the decrypter

The program is quite straightforward. `getline` is used to receive user
input. This input is then decrypted using the `decrypt` function. The outcome
is printed to stdout.

The `decrypt` function first converts the character to lower case. This will
only affect upper case characters. Then it is checked whether the character is
in the range `'a'..'z'`. If it is not, then the character itself is returned.
Otherwise it is decrypted by looking up the shift value from the `shift` array.

## Try it out

Run the decrypter using the following command:

    $ gcc decrypt.c && ./a.out
