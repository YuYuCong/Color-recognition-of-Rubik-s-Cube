----------------------------------------------------------------------
Author Info
----------------------------------------------------------------------

Name:		Stefan Pochmann
Email:		pochmann@gmx.de
City:		Darmstadt
Country:	Germany


----------------------------------------------------------------------
Description
----------------------------------------------------------------------

This is my variant of the Thistlethwaite algorithm. Look here if you
don't know it already:

http://www.geocities.com/jaapsch/puzzles/cube3.htm
http://www.geocities.com/jaapsch/puzzles/compcube.htm

I implemented the four searches (one for each phase) as bidirectional
BFS. Since not the whole cube has to be considered in every phase,
I compute "id's" for the states. In the first phase, the id for a state
only contains the orientations of the edges. The id's in the other
phases are done similar and according to Thistlethwaite's algorithm.
There's only one difference to the original algorithm, namely in
phase 3 my subgoal is more restrictive. I could not find out a nice
way to determine the "tetrad twists", instead I split up the tetrads
in half to get the four groups UFR+UBL, DFL+DBR, URB+ULF and DRF+DLB.
Getting the edges in their correct slices, the corners in their correct
pair groups and ensuring overall even parity results in a cube solvable
by phase 4. It also doesn't reach too far into phase 4 and the original
phase 3 was very small anyway.

I could've made this code a bit smaller or faster or generate shorter
solutions but I just hope it will be fast enough and correct so that
it will be a valid entry for Tom's contest. There's not much time left
on the contest and I don't even know how to use a profiler for C++ so
this is where I'll leave it. Some ideas I would've liked to carry out:

- Moves can be computed better (U1+U2+U3 should be done by three U moves,
  not by 1+2+3 like I do).
- Using Python instead of C++ might save a lot of code, but I didn't
  manage to learn this language early enough.
- The real Thistlethwaite algorithm could be used, but I don't know
  how complex it would be. Probably not much, I guess...

----------------------------------------------------------------------
Etc
----------------------------------------------------------------------

Thanks to (obviously) Morwen Thistlethwaite for the basic algorithm.

Thanks to Jaap Scherphuis for his wonderful website containing among
lots of other useful stuff descriptions of Thistlethwaite's algorithm.
And thanks for helping me with some questions I had about it :-).

Thanks to the other contestants for making me write this program in
addition to my first approach.

Thanks again to Tomas Rokicki for running this contest. It has been
great fun, I overcame my fear of BFS and learned a few new things
during the whole process.

Thanks to Johnny, my invisible friend, named after Johnny Depp if
I recall correctly for reasons I do not recall anymore, because in
my first year at university I came up with BFS myself (I didn't know
any interesting algorithms at that time) to guide a robot I called
Johnny through mazes (that was part of our final project).

Thank you for reading if you made it this far.
