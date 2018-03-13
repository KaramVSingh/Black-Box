GATES = 16:
0. DFLIPFLOP
1. DFLIPFLOP
2. OUTPUT
3. OUTPUT
4. DFLIPFLOP
5. DFLIPFLOP
6. OUTPUT
7. OUTPUT
8. DFLIPFLOP
9. DFLIPFLOP
10. OUTPUT
11. OUTPUT
12. DFLIPFLOP
13. DFLIPFLOP
14. OUTPUT
15. OUTPUT
~
CONNECTIONS:
0,1 -> 0,0
0,0 -> 1,1
1,1 -> 1,0
0,1 -> 2,0
1,1 -> 3,0
4,1 -> 4,0
1,0 -> 4,1
4,0 -> 5,1
5,1 -> 5,0
4,1 -> 6,0
5,1 -> 7,0
8,1 -> 8,0
5,0 -> 8,1
8,0 -> 9,1
9,1 -> 9,0
8,1 -> 10,0
9,1 -> 11,0
12,1 -> 12,0
9,0 -> 12,1
12,0 -> 13,1
13,1 -> 13,0
12,1 -> 14,0
13,1 -> 15,0
~
INPUTS = 1:
0. clk~ -> 0,1~
~
OUTPUTS = 9:
0. o0~ <- 0,1
1. o1~ <- 1,1
2. o2~ <- 4,1
3. o3~ <- 5,1
4. o4~ <- 8,1
5. o5~ <- 9,1
6. o6~ <- 12,1
7. common~ <- 13,0
8. o7~ <- 13,1
~