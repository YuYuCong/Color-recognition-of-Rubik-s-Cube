Author: David Barr <davidbarr@iname.com>
Location: Laurel, MD, USA

This program solves the cubes in several steps:

- bottom edges
- three bottom corners
- three middle edges
- last bottom corner
- last bottom edge
- orient last layer edges

On the last layer we don't do the corners or worry about edge
permutation.  We just generate lots of solutions until one of them
fixes everything that's left.

Compile the program as follows:

gcc -O2 -o contest contest.c

Run the program as follows:

./contest UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR

NOTES:

The expression "R(m)=R(--m)" is probably undefined.  I've tested it
with GCC, and it works how I expect it to (left hand side evaluated
before right hand side).  I could easily get rid of this ambiguity by
adding two bytes to the program size.

TO DO:

Save space in the algorithm table by listing similar algs once.  Other
algs that are similar would have a single turn to reach the first,
then the piece would be solved twice. (I know that isn't clear, but I
know what I mean :)).

Combine turns in the final solution to get rid of things like U1U1.
