# arduino
Various Arduino sketches and projects

# Pineapple Pen Halloween Extravaganza 2019
TODO. Completed and worked well for Halloween 2019. Kids really liked it.

# Christmas Light Blowout 2019
Uses Elegoo 8 relay module to run 8 sets of lights. The fun of this light show will be general pseudo randomness, rather than a highly choreographed, synchronized show. It will go dark for about 30 seconds, followed by about a 30 second "show" of cacophony, then repeat.

Like:

[30s show][30s dark][30s show][30s dark][30s show][30s dark]...

Each "show" is defined by a function. Each 30s dark pause is managed by the outermost loop (top-level, or parent, loop).

For 2019, the lights will be:

-  (3) flood LED flood lights, two with red gel (filter), one with green
-  (5) strands of various common LED and incandescent Christmas lights

Each relay will handle a light (or strand of lights).

## I/O Assignments

Digital I/O pins will be assigned as follows:

1.  D5: Flood (red)
2.  D6: Flood (green)
3.  D7: Flood (red)
4.  D8: strand
5.  D9: strand
6.  D10: strand
7.  D11: strand
8.  D12: strand

## Show Functions

Outer loop picks a "show" function to execute, selected via a PSRN and a lookup table or switch.

Key light show dimensions are:

Per I/O pin:
-  (onDuration) Pulse duration of ON state per light or strand (per I/O pin) (i.e., how long the light is illuminated for)
-  (offDuration) Duration between OFF state between each ON state (i.e., how much pause between illuminations)

Brainstorm of show functions:

PRN: pseudo-random number (as generated with Arduino std lib random() and randomSeed()).

**totalChaosShow**: No relationship across (among) any of the pins and and no consistency within each single I/O (strand or flood). For example, D9 strand might have a progression like the following (no apparent duration pattern):
[520ms ON][300ms OFF][873ms ON][249ms OFF][744ms ON][973ms OFF]...

Each and every ON and OFF value is PRN.

My only concern about this is the time consumed during the generation of the next psueo-random number (PRN), as this will occur for each of the 8 I/O pins. An alternative is to precompute lists, per pin, of ON & OFF durations during the 30 second pause between shows. If the end of the list is reached during the show, then just start again from 0th item.

**semiChaosShow**: No relationship across (among) any of the pins (like totalChaosShow), but with consistency within each single I/O (strand or flood). For example, D5 flood might have a progression as follows:
[221ms ON][349ms OFF][221ms ON][349ms OFF][221ms ON][349ms OFF]...

The ON and OFF durations are PRN, but are consistent throughout the given show.

More...


