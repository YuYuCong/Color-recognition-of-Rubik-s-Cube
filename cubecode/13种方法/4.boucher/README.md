Name:    Antony BOUCHER
Email:   antony.boucher@free.fr
City:    Gennevilliers
Country: France

Source code: solver.c
Language: standard C
Instructions: none


Details:

This version uses 4 successive IDA* searches:
- try and solve the whole cube with up to 5 face turns
- solve the cross in the upper layer, using simple face rotations
- solve the 8 remaining edges, using moves that do not change the upper cross
- solve the 8 corners, using 6 different algorithms that permute 3 corners
  without moving edges
