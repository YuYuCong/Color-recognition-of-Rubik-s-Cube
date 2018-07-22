BeginPackage["TwoPhase`",{"Combinatorica`"}]

Print["On the first run, a few tables are generated. This may take several minutes,"]

CircleDot::usage = "CircleDot[a,b] gives the product of two cubes a and b.
Examples:
CircleDot[U1,R1]
In a notebook, the CircleDot operator  \[CircleDot] can be entered as Esc c. Esc and
the above product can be written as U1\[CircleDot]R1 .";

solveCube::usage =
"solveCube[c,max]
gives solvers for the cube c with decreasing length.
The function returns when a solution with not more than max moves is found. Use Alt+. to abort then computation earlier.
The cube data is entered as a list which describes the permutations, twists and flips of the corners and edges:  

{{{Corner Permutation},{Corner Twists}},{{Edge Permutation},{Edge Flips}}}
Example: {{{3,1,7,6,2,8,5,4},{2,2,0,2,2,1,0,0}},{{5,11,12,4,3,9,2,7,1,6,10,8},{1,0,0,0,1,0,0,0,1,1,0,0}}}

solveCube[c] returns with the first solution found.";

randomCube::usage = "randomCube generates a random cube in the format
{{{Corner Permutation},{Corner Twists}},{{Edge Permutation},{Edge Flips}}}";

showCube::usage = "showCube[c] gives a 3D-representation of a cube entered in the format
{{{Corner Permutation},{Corner Twists}},{{Edge Permutation},{Edge Flips}}}";

editor::usage = "editor allows to define the colors of the facelets interactively."

editorToCube::usage = "editorToCube builds a cube in the format
{{{Corner Permutation},{Corner Twists}},{{Edge Permutation},{Edge Flips}}} from the editor data.
No error checks for missing or wrong facelets are performed." 

import::usage = "import[c] imports the cubes of a textfile with generators of cubes.
Generators in the Cube Explorer Format with for example U instead of U1 and U' instead of U3 are supported." 

(***************************************************************************************************************)
(* Face Turn and cubes are represented by permutations with attached orientations.                         *)         
(***************************************************************************************************************)

(* corner permutations of basic face turns *)
cU =
    {{4,1,2,3,5,6,7,8},{0,0,0,0,0,0,0,0}}; 
cR =
    {{5,2,3,1,8,6,7,4},{2,0,0,1,1,0,0,2}};
cF =
    {{2,6,3,4,1,5,7,8},{1,2,0,0,2,1,0,0}};
cD =
    {{1,2,3,4,6,7,8,5},{0,0,0,0,0,0,0,0}};
cL =
    {{1,3,7,4,5,2,6,8},{0,1,2,0,0,2,1,0}};
cB =
    {{1,2,4,8,5,6,3,7},{0,0,1,2,0,0,2,1}};
    
(* edge permutations of basic face turns *)
eU =
    {{4,1,2,3,5,6,7,8,9,10,11,12},{0,0,0,0,0,0,0,0,0,0,0,0}};
eR =
    {{9,2,3,4,12,6,7,8,5,10,11,1},{0,0,0,0,0,0,0,0,0,0,0,0}};
eF =
    {{1,10,3,4,5,9,7,8,2,6,11,12},{0,1,0,0,0,1,0,0,1,1,0,0}};
eD =
    {{1,2,3,4,6,7,8,5,9,10,11,12},{0,0,0,0,0,0,0,0,0,0,0,0}};
eL =
    {{1,2,11,4,5,6,10,8,9,3,7,12},{0,0,0,0,0,0,0,0,0,0,0,0}};
eB =
    {{1,2,3,12,5,6,7,11,9,10,4,8},{0,0,0,1,0,0,0,1,0,0,1,1}} ;   
    
(* Identity permutation *)
cId =
    {Range[8],ConstantArray[0,8]};
eId =
    {Range[12],ConstantArray[0,12]};
    
(* Define all cube move permutations *)
U1 = {cU,eU};
R1 = {cR,eR};
F1 = {cF,eF};
D1 = {cD,eD};
L1 = {cL,eL};
B1 = {cB,eB};
Id = {cId,eId};  
U2 = CircleDot[U1,U1];
U3 = CircleDot[U1,U1,U1];
R2 = CircleDot[R1,R1];
R3 = CircleDot[R1,R1,R1];
F2 = CircleDot[F1,F1];
F3 = CircleDot[F1,F1,F1];
D2 = CircleDot[D1,D1];
D3 = CircleDot[D1,D1,D1];
L2 = CircleDot[L1,L1];
L3 = CircleDot[L1,L1,L1];
B2 = CircleDot[B1,B1];
B3 = CircleDot[B1,B1,B1];


Begin["`Private`"]

(* Define the facelet positions of untwisted corners in their home positions                                   *)
(* Each facelet is described by the triple {x,y,z}, where  1<=x<=6 is face U, R, D, F, L, B,                   *)
(* 1<=y<=3 and 1<=z<=3 define the relative coordinate (y,z) of the facelet on a face.                          *)
(* (x,1,1) is upper left facelet on face x .                                                                   *) 
cf = {{{1, 3, 3}, {2, 1, 1}, {3, 1, 3}}, {{1, 3, 1}, {3, 1, 1}, {5, 1,3}},
      {{1, 1, 1}, {5, 1, 1}, {6, 1, 3}}, {{1, 1, 3}, {6, 1, 1}, {2, 1, 3}},
      {{4, 1, 3}, {3, 3, 3}, {2, 3, 1}}, {{4, 1, 1}, {5, 3, 3}, {3, 3, 1}},
      {{4, 3, 1}, {6, 3, 3}, {5, 3, 1}}, {{4, 3, 3}, {2, 3, 3}, {6, 3, 1}}};
ef = {{{1, 2, 3}, {2, 1, 2}}, {{1, 3, 2}, {3, 1, 2}}, {{1, 2, 1}, {5, 1, 2}},
      {{1, 1, 2}, {6, 1, 2}}, {{4, 2, 3}, {2, 3, 2}}, {{4, 1, 2}, {3, 3, 2}},
      {{4, 2, 1}, {5, 3, 2}}, {{4, 3, 2}, {6, 3, 2}}, {{3, 2, 3}, {2, 2, 1}},
      {{3, 2, 1}, {5, 2, 3}}, {{6, 2, 3}, {5, 2, 1}}, {{6, 2, 1}, {2, 2, 3}}};
      


(***************************************************************************************************************)
(* Multiplication of two or several corner or edge permutations                                                *)
(***************************************************************************************************************)

cornMult[x_, y_ ] :=
    Module[ {z,oa,ob},
        { Table[x[[1,y[[1,z]]]],{z,8}],
          Table[oa = x[[2,y[[1,z]]]];
                ob = y[[2,z]];
                If[ oa<3,
                    If[ ob<3,
                        Mod[oa+ob,3],  (* only this case needed in the implementation used here*)
                        Mod[oa+ob,3]+3 (* this case and other cases only needed when reflections are involved *)
                    ],
                    If[ ob<3,          (* just for completeness *)
                        Mod[oa-ob,3]+3,
                        Mod[oa-ob,3]
                    ]
                ]
                ,{z,8}]      
        }
    ]
  
edgeMult[x_, y_ ] :=
    Module[ {z,oa,ob},
        { Table[x[[1,y[[1,z]]]],{z,12}],
          Table[oa = x[[2,y[[1,z]]]];
                ob = y[[2,z]];
                Mod[oa+ob,2]
                ,{z,12}]      
        }
    ]

multSequence[x__]:= Module[{},Fold[cornMult,cId,{x}]]/;(Count[{x}[[1,2]],_Integer]==8)
multSequence[x__]:= Module[{},Fold[edgeMult,eId,{x}]]/;(Count[{x}[[1,2]],_Integer]==12)

CircleDot[x__] :=
    Module[ {c,e},
        c = Fold[cornMult,cId,{x}[[All,1]]];
        e = Fold[edgeMult,eId,{x}[[All,2]]];
        {c,e}
    ]

	
	
(***************************************************************************************************************)
(* Generate tables with the pure permutations of the 18 possible moves, orientations are ignored               *) 
(* Needed for the transition from phase1 to phase2                                                             *)
(***************************************************************************************************************) 
 
cMovePerms = Module[ {f},
                f[move_] :=
                    NestList[multSequence[move, #] &, move, 2];
                Flatten  [f /@ {cU, cR, cF, cD, cL, cB}, 2][[1 ;; 35 ;; 2]]
            ];
eMovePerms = Module[ {f},
                f[move_] :=
                    NestList[multSequence[move, #] &, move, 2];
                Flatten  [f /@ {eU, eR, eF, eD, eL, eB}, 2][[1 ;; 35 ;; 2]]
            ];

(***************************************************************************************************************)
(* Functions and their inverses for the phase1 and phase2 coordinates                                          *)
(***************************************************************************************************************)

(* phase1 coordinates: *)

(* The position of the FR, FL, BL and BR edges *)
slice[p_] := 494-(RankKSubset[Sort[Take[InversePermutation[p],-4]],Range[12]]);

invSlice[n_] :=
    Module[ {s,r,i,x},
        s =
            UnrankKSubset[494-n,4,Range[12]];
        r =
            {0,0,0,0,0,0,0,0,0,0,0,0};
        Do[r[[s[[i]]]] = 8+i ,{i,4}];
        x = 1;
        Do[If[ r[[i]]==0,
               r[[i]] = x++
           ],{i,12}];
        r
    ]

(* The flip of the 12 edges *)
flip[o_]:= FromDigits[Mod[Most[o],2],2]
invFlip[n_]:=Module[{d},d=IntegerDigits[n+2048,2];Rest[Append[d,Mod[Total[d]+1,2]]]]

(* The twist of thee 8 corners *)
twist[o_]:= FromDigits[Mod[Most[o],3],3]
invTwist[n_]:=Module[{d},d=IntegerDigits[n+2187,3];Rest[Append[d,Mod[4-Total[d],3]]]]

(* phase2 coordinates: *)

(* The permutation of the 8 corners *)
 corner[p_] :=
    RankPermutation[p];
    
 invCorner[n_] :=
     UnrankPermutation[n,8]
 
 (* The permutation of the edges UR, UF, UL, UB, DR, DF, DL, DB in the U and D face. *)
 edge8[p_] :=
    Module[ {t},
        t = Take[p,8];
        If[ PermutationQ[t],
            RankPermutation[t],
            -1
        ]
    ]
     
 invEdge8[n_] :=
     Join[UnrankPermutation[n,8],{9,10,11,12}]

 (* The permutation of the edges FR, FL, BL and BR in the slice between the U and D face. *)
 edge4[p_] :=
    Module[ {t},
        t = Take[p,-4]-8;
        If[ PermutationQ[t],
            RankPermutation[t],
            -1
        ]
    ]

 invEdge4[n_] :=
     Join[Range[8],UnrankPermutation[n,4]+8]

(***************************************************************************************************************)
(* The movetables store how the coordinates are mapped by the 18 moves. In the phase2 tables, this mapping is  *)
(* undefined for the moves R, R', F, F', L, L', B and B' which are no phase2 moves and the entries are -1 here *)
(***************************************************************************************************************)

moveTable[name_,cornerQ_,permQ_,coord_,invCoord_,size_] :=
    Module[ {f,d,cubies,j,t},
        SetDirectory[NotebookDirectory[]];
        If[ FileExistsQ[name],
            Print["Loading ",name," Table"];
            t = Get[name],
            Print["Building ",name," Table"];
            cubies = If[ cornerQ,
                         8,
                         12
                     ];
            d = If[ permQ,
                    Array[0 &, cubies],
                    Array[# &, cubies]
                ];
            f[move_,i_] :=
                If[ permQ,
                    coord /@ Flatten[Rest[NestList[multSequence[#, move] &, {invCoord[i],d}, 3]], 
                    1][[1 ;; 5 ;; 2]],
                    coord /@ Flatten[Rest[NestList[multSequence[#, move] &, {d, invCoord[i]}, 3]], 
                    1][[2 ;; 6 ;; 2]]
                ];
            t = Table[If[ Mod[j,1000]==0,
                          Print[j,"/",size]
                      ];
                      If[ cornerQ,
                          Join[f[cU,j],f[cR,j],f[cF,j],f[cD,j],f[cL,j],f[cB,j]],
                          Join[f[eU,j],f[eR,j],f[eF,j],f[eD,j],f[eL,j],f[eB,j]]
                      ],{j,0,size-1}];
            Put[t,name];
            ResetDirectory[];
            t
        ]
    ]

flipMove = moveTable["flipMove",False,False,flip,invFlip,2048];
twistMove = moveTable["twistMove",True,False,twist,invTwist,2187];
sliceMove = moveTable["sliceMove",False,True,slice,invSlice,495];

cornerMove = moveTable["cornerMove",True,True,corner,invCorner,40320];
edge4Move = moveTable["edge4Move",False,True,edge4,invEdge4,24];
edge8Move = moveTable["edge8Move",False,True,edge8,invEdge8,40320];

(***************************************************************************************************************)     
(* The phase1 pruning tables store the minimum distances to reach a phase2 position for all possible           *)
(* (slice,flip) and  (slice,twist) coordinate pairs. The phase2 pruning tables store the minimum distances to  *)
(* reach the solved cube position for all possible (edge4,edge8) and (edge4,corner) coordinate pair.           *)
(***************************************************************************************************************)

prunTable[name_,move1_,size1_,move2_,size2_] :=
    Module[ {i,m,depth,count,p},
        SetDirectory[NotebookDirectory[]];
        If[ FileExistsQ[name],
            Print["Loading ",name," Table"];
            p = Get[name],
            p = ConstantArray[-1,size1*size2];
            p[[0+1]] = 0;
            count = 1;
            depth = 0;
            Print["Building ",name," Table"];
            Print[depth," ", count];
            While [count<size1*size2,
            Do[
                If[ p[[i+1]]==depth,
                    m = move1[[Quotient[i,size2]+1]]*size2+move2[[Mod[i,size2]+1]];(*list of 18 values, invalid values <0*)
                    (If[#>=0 && p[[#+1]]==-1,
                         count++;
                         p[[#+1]] = depth+1
                     ]) & /@ m;
                ]
              ,{i,0,size1*size2-1}];
            depth++;
            Print[depth," ", count];
            ];
            Put[p,name]
        ];
        ResetDirectory[];
        p
    ]


(* phase1 *)
sliceflipPrun = prunTable["sliceflipPrun",sliceMove,495,flipMove,2048];
slicetwistPrun = prunTable["slicetwistPrun",sliceMove,495,twistMove,2187];
(* phase2 *)
edge4edge8Prun = prunTable["edge4edge8Prun",edge4Move,24,edge8Move,40320];  
edge4cornerPrun = prunTable["edge4cornerPrun",edge4Move,24,cornerMove,40320];


(***************************************************************************************************************)
(* Do the phase1 and phase2 searches recursively.                                                              *)
(***************************************************************************************************************)
 
 solveCube[c_,min_] :=
    Module[ {i},
        Print[c];
        If[ !PermutationQ[c[[1,1]]] ||!PermutationQ[c[[2,1]]] || Signature[c[[1,1]]]*Signature[c[[2,1]]]!=1
        	|| Mod[Total[c[[1,2]]],3]!=0 ||  Mod[Total[c[[2,2]]],2]!=0,
            Print["Cube not solvable!"],
            If[ c[[1]]==cId && c[[2]]==eId,
                Print["(0f)"],
                solver1 = ConstantArray[0,21];
                startPerm = {c[[1,1]],c[[2,1]]};
                maxLength = min;
                currLength = 100;
                phase1ExitQ = False;
                Do [phase1search[flip[c[[2,2]]],twist[c[[1,2]]],slice[c[[2,1]]],i],{i,20}]
            (* never more than 20 moves *)
            ]
        ]
    ]
 
 solveCube[c_]:= solveCube[c,30]
 	
 phase1search[flip_,twist_,slice_,togo_] :=
     Module[ {i,fl,tw,sl,sliceflipdist,slicetwistdist,m},
         If[ !phase1ExitQ,
             If[ togo==0,
                 phase2start[],
                 Do[If[ validPairQ[i,solver1[[togo+1]]],
                        fl = flipMove[[flip+1,i]];
                        tw = twistMove[[twist+1,i]];
                        sl = sliceMove[[slice+1,i]];
                        sliceflipdist = sliceflipPrun[[2048*sl+fl+1]];
                        slicetwistdist = slicetwistPrun[[2187*sl+tw+1]];
                        m = Max[sliceflipdist,slicetwistdist];
                        If[ m<togo && (togo>5|| m!=0 || togo==1),
                            solver1[[togo]] = i;
                            phase1search[fl,tw,sl,togo-1]
                        ]
                    ],{i,18}]
             ]
         ]
     ]; 

phase2start[] :=
    Module[ {cperm,eperm,s,c,e4,e8,i,l},
        s = Reverse[Select[solver1, # > 0 &]];
        cperm = Fold[Table[#1[[#2[[i]]]], {i, 8}] &, startPerm[[1]], 
        cMovePerms[[s]]];
        eperm = Fold[Table[#1[[#2[[i]]]], {i, 12}] &,startPerm[[2]], 
        eMovePerms[[s]]];
        c = corner[cperm];
        e4 = edge4[eperm];
        e8 = edge8[eperm];
        solver2 = ConstantArray[0,19];
        If[ c==0&&e4==0&&e8==0,
            printSolver;
            phase1ExitQ = True,(* optimal solution found *)
            l = Length[s]; (* phase1 length *)
            phase2ExitQ = False;
            Do [ If [i<currLength-l ,phase2search[c,e4,e8,i]],{i,15}](* 15 moves in phase 2 is enough *)
        ]
    ]
    
phase2search[corner_,edge4_,edge8_,togo_] :=
    Module[ {i,c,e4,e8,edge4cornerdist,edge4edge8dist,m},
        If[ !phase2ExitQ,
            If[ togo==0,
                printSolver;
                currLength = Length[Select[solver1, # > 0 &]]+Length[Select[solver2, # > 0 &]];
                If [currLength<=maxLength,phase1ExitQ =True];
                
                solver2 = ConstantArray[0,19];
                phase2ExitQ = True,
                Do[If[ Count[{4,6,7,9,13,15,16},i]==0 &&validPairQ[i,solver2[[togo+1]]]&&(solver2[[togo+1]]!=0 || validPairQ[i,solver1[[1]]]),
                       c = cornerMove[[corner+1,i]];
                       e4 = edge4Move[[edge4+1,i]];
                       e8 = edge8Move[[edge8+1,i]];
                       edge4cornerdist = edge4cornerPrun[[40320*e4+c+1]];
                       edge4edge8dist = edge4edge8Prun[[40320*e4+e8+1]];
                       m = Max[edge4cornerdist,edge4edge8dist];
                       If[ m<togo,
                           solver2[[togo]] = i;
                           phase2search[c,e4,e8,togo-1]
                       ]
                   ],{i,17}]
            ]
        ]
    ]
 
 (**************************************************************************************************************)
 (* Some helper functions to generate random cubes, print the solver string and to edit or display cubes.      *) 
 (**************************************************************************************************************)   
  
 (* validPairQ returns False for invalid adjacent moves like R R2. *)
  validPairQ[a_,b_] :=
   Module[ {qa,qb},
        If[ b==0,
            True,
            qa = Quotient[a-1,3];(* 1<=a<=18 *)
            qb = Quotient[b-1,3];
            If [qa==qb || qa==qb-3,False,True]
        ]
    ]
 

randomCube :=
    Module[ {a,i,cp,ep,co,eo},
        cp = RandomSample[Range[8], 8];
        ep = RandomSample[Range[12], 12];
        If[ Signature[cp]*Signature[ep]==-1,
            cp = RotateLeft[cp]
        ];
        a = Table[RandomInteger[{0, 2}], {i, 7}];
        co = Append[a, Mod[-Total[a], 3]];
        a = Table[RandomInteger[{0, 1}], {i, 11}];
        eo = Append[a, Mod[-Total[a], 2]];
        {{cp,co},{ep,eo}}
    ] 
    
 printSolver :=
    Module[ {a,b,i},
        a = {"U1 ","U2 ","U3 ","R1 ","R2 ","R3 ","F1 ","F2 ","F3 ","D1 ","D2 ","D3 ","L1 ","L2 ","L3 ","B1 ","B2 ","B3 "};
        b = ""; 
        Do[If[solver1[[21-i]]!=0,b=b<> a[[solver1[[21-i]]]]],{i,20}]; (* phase1 never longer than 20 moves *)
        b=b<>". ";
        Do[If[solver2[[19-i]]!=0,b=b<> a[[solver2[[19-i]]]]],{i,18}]; (* phase2 never longer than 18 moves *)
        b=b<>"("<>ToString[Length[Select[solver1, # > 0 &]]+Length[Select[solver2, # > 0 &]]]<>"f)";
        Print[b];
    ]
    
 (* Display 3D image  of a cube in the format *)
 showCube[cube_] :=
    Module[ {cc,col,i,j,uf,df,ff,bf,lf,rf,n,setCorner,setEdge},
              
        cc = {Yellow, Green, Red, White, Blue, Orange}; (*center colors of the faces default scheme *)
        Do[If[Count[c[[i,2,2]],_]==3,cc[[i]]=c[[i,2,2]]] ,{i,6}]; (* use editor scheme if possible *)
        col = ConstantArray[Black, {6, 3, 3}]; (* facelet colors, col[[i]] is sublist of colors of face i *)
        Do [col[[i, 2, 2]] = cc[[i]], {i, 6}]; (* color the centers *)
        
        (* Generate polygones for Graphics3D, paramater c_ defines the facelet colors of a face *)
        uf[c_] :=
            Table[{c[[i + 1, j + 1]], Polygon[{{i, j, 3}, {i + 1, j, 3}, {i + 1, j + 1, 3}, {i, j + 1, 3}}]}, {i, 0, 2}, {j, 0, 2}];
        df[c_] :=
            Table[{c[[i + 1, j + 1]], Polygon[{{2 - i, j, 0}, {3 - i, j, 0}, {3 - i, j + 1, 0}, {2 - i, j + 1, 0}}]}, {i, 0, 2}, {j, 0, 2}];
        ff[c_] :=
            Table[{c[[i + 1, j + 1]], Polygon[{{3, j, 2 - i}, {3, j + 1, 2 - i}, {3, j + 1, 3 - i}, {3, j, 3 - i}}]}, {i, 0, 2}, {j, 0, 2}];
        bf[c_] :=
            Table[{c[[i + 1, j + 1]], Polygon[{{0, 2 - j, 2 - i}, {0, 3 - j, 2 - i}, {0, 3 - j, 3 - i}, {0, 2 - j, 3 - i}}]}, {i, 0, 2}, {j, 0, 2}];
        lf[c_] :=
            Table[{c[[i + 1, j + 1]], Polygon[{{j, 0, 2 - i}, {j + 1, 0, 2 - i}, {j + 1, 0, 2 - i + 1}, {j, 0, 2 - i + 1}}]}, {i, 0, 2}, {j, 0, 2}];
        rf[c_] :=
            Table[{c[[i + 1, j + 1]], Polygon[{{2 - j, 3, 2 - i}, {3 - j, 3, 2 - i}, {3 - j, 3, 2 - i + 1}, {2 - j, 3, 3 - i }}]}, {i, 0, 2}, {j, 0, 2}];
        setCorner[to_, from_, o_] :=
            Do[col[[Sequence @@ cf[[to, n]]]] = cc[[cf[[from, 1 + Mod[n + 2 - o, 3], 1]]]];, {n, 3}];
        setEdge[to_, from_, o_] :=
            Do[col[[Sequence @@ ef[[to, n]]]] = cc[[ef[[from, 1 + Mod[n + 1 + o, 2], 1]]]];, {n, 2}];
            
       Do [setCorner[i, cube[[1, 1, i]], cube[[1, 2, i]]], {i, 8}];
       Do [setEdge[i, cube[[2, 1, i]], cube[[2, 2, i]]], {i, 12}];     
            
        Graphics3D[{uf[col[[1]]], rf[col[[2]]], ff[col[[3]]], df[col[[4]]], lf[col[[5]]], bf[col[[6]]]},
            Lighting -> "Neutral" ,ViewVertical -> {0, 0, 1}, ViewPoint -> {10, 10, 10}, Boxed -> False, ImageSize -> 150,
         BaseStyle -> {EdgeForm[{GrayLevel[0.5], Thickness[0.02]}]}, SphericalRegion -> True]
    ]


coordToFacelet[co_]:= 9*(co[[1]]-1)+3*(co[[2]]-1)+co[[3]]

(* Compute the color representation of the cube *)   
cubeToCols[cube_] :=
    Module[ {f,i,j},
        f = ConstantArray[0,54];
        Do[f[[9*i+5]] = i+1,{i,0,5}];
        Do[f[[coordToFacelet@cf[[i,j]]]] = cf[[cube[[1,1,i]], 1+ Mod[j+2-cube[[1,2,i]],3],1]],{i,8},{j,3}];
        Do[f[[coordToFacelet@ef[[i,j]]]] = ef[[cube[[2,1,i]], 1+ Mod[j+1+cube[[2,2,i]],2],1]],{i,12},{j,2}];
        f
    ]
 
 (* Compute a cube from the color representation *)
 colsToCube[f_] :=
    Module[ {i,j,o,c},
        c = {{ConstantArray[-1,8],ConstantArray[0,8]},{ConstantArray[-1,12],ConstantArray[0,12]} };
        Do [If[ RotateRight[cf[[j, All, 1]], o]==f[[coordToFacelet /@ cf[[i]]]],
                c[[1,1,i]] = j;
                c[[1,2,i]] = o
            ],{i,8},{j,8},{o,0,2}];
        Do [If[ RotateRight[ef[[j, All, 1]], o]==f[[coordToFacelet /@ ef[[i]]]],
                c[[2,1,i]] = j;
                c[[2,2,i]] = o
            ],{i,12},{j,12},{o,0,1}];
       c
    ]   


    
c = ConstantArray[Gray, {6, 3, 3}]; (* the facelet colors in the editor *) 
c[[1,2,2]]=Yellow;c[[2,2,2]]=Green;c[[3,2,2]]=Red;c[[4,2,2]]=White;c[[5,2,2]]=Blue;c[[6,2,2]]=Orange; 
curCol = Gray; (* the current color of the input mask *)
faceOffset = {{3, 6}, {6, 3}, {3, 3}, {3, 0}, {0, 3}, {9, 3}};

face[n_] :=
    Table[{c[[n, i + 1, j + 1]], Rectangle[{i, j} + faceOffset[[n]], {i + 1, j + 1} + faceOffset[[n]]]}, {i, 0, 2}, {j, 0, 2}]

setcolors :=
    Graphics[{EdgeForm[Thin],face[1], face[2], face[3], face[4], face[5], face[6],Text["U", {4.5, 7.5}],Text["R", {7.5, 4.5}],
    	                             Text["F", {4.5, 4.5}],Text["D", {4.5, 1.5}],Text["L", {1.5, 4.5}],Text["B", {10.5, 4.5}],
    	                             Yellow,Rectangle[{7,1},{8,2}],Blue,Rectangle[{8,1},{9,2}],Red,Rectangle[{9,1},{10,2}],
    	                             White,Rectangle[{7,0},{8,1}],Green,Rectangle[{8,0},{9,1}],Orange,Rectangle[{9,0},{10,1}],curCol,Rectangle[{11,0},{12,1}]}
    	                                    ]
(* Editor for the facelet colors*)
editor :=
    Module[ {pt},
        ClickPane[Dynamic[setcolors],(pt = Floor[#];
                                    Switch[pt,{7,1},curCol = Yellow,{8,1},curCol = Blue,{9,1},curCol = Red,{7,0},curCol = White,{8,0},curCol = Green,{9,0},curCol = Orange];
                                    If[ pt[[2]]>2&&pt[[2]]<6,
                                        If[ pt[[1]]<3,
                                            c[[5, pt[[1]] + 1, pt[[2]] -2]] = curCol,
                                            If[ pt[[1]]<6,
                                                c[[3, pt[[1]]-2, pt[[2]] -2]] = curCol,
                                                If[ pt[[1]]<9,
                                                    c[[2, pt[[1]]-5, pt[[2]] -2]] = curCol,
                                                    If[ pt[[1]]<12,
                                                        c[[6, pt[[1]]-8, pt[[2]] -2]] = curCol
                                                    ]
                                                ]
                                            ]
                                        ],
                                        If[ pt[[1]]>2&&pt[[1]]<6,
                                            If[ pt[[2]]<3,
                                                c[[4,pt[[1]] -2, pt[[2]]+1]] = curCol,
                                                If[ pt[[2]]>5,
                                                    c[[1,pt[[1]] -2, pt[[2]]-5]] = curCol
                                                ]
                                            ]
                                        ]
                                    ];
                                    )&]
    ]
    
(* Generate a cube in the format {{{Corner Permutation},{Corner Twists}},{{Edge Permutation},{Edge Flips}}} from the editor data*)
editorToCube :=
    Module[ {i,j,k,f},
        f = ConstantArray[0,54];
        Do[f[[9*(k-1)+3*(2-(j-1))+i]] = Switch[c[[k,i,j]],Gray,0,c[[1,2,2]],1,c[[2,2,2]],2,c[[3,2,2]],3,c[[4,2,2]],4,c[[5,2,2]],5,c[[6,2,2]],6],{k,6},{i,3},{j,3}];
        colsToCube[f]
    ]

import[filename_] :=
    Module[ {x},
    	x=Import[filename];
        x=StringReplace[x, RegularExpression[" *//.*"] -> ""];(* delete comments *)
        x=StringReplace[x, RegularExpression[" +(\\(\\d+.*\\))* *"] -> " "];(* delete maneuver length infromation *)
        x=StringReplace[x, r : Characters["URFDLB"] ~~ " " -> r ~~ "1 "];(* U->U1 etc.*)
        x=StringReplace[x, r : Characters["URFDLB"] ~~ "' " -> r ~~ "3 "]; (* U'->U3 etc.*)
        x=StringSplit[x, "\n"];
        x=StringReplace[x, (* insert CircleDot operator between basic moves *)
          a : DigitCharacter ~~ " " ~~ b : LetterCharacter -> 
           a ~~ "\[CircleDot]" ~~ b];
        ToExpression[x]
    ]

Print["Done!"]

End[]

EndPackage[]

