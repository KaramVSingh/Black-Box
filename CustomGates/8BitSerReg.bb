GATES = 19:
0. DFLIPFLOP
1. DFLIPFLOP
2. OUTPUT
3. OUTPUT
4. DFLIPFLOP
5. DFLIPFLOP
6. OUTPUT
7. OUTPUT
8. OUTPUT
9. DFLIPFLOP
10. DFLIPFLOP
11. OUTPUT
12. OUTPUT
13. DFLIPFLOP
14. DFLIPFLOP
15. OUTPUT
16. OUTPUT
17. OUTPUT
18. OUTPUT
~
CONNECTIONS:
0,0 -> 1,0
0,0 -> 2,0
1,0 -> 3,0
1,0 -> 4,0
4,0 -> 5,0
4,0 -> 6,0
5,0 -> 7,0
1,0 -> 8,0
5,0 -> 9,0
9,0 -> 10,0
9,0 -> 11,0
10,0 -> 12,0
10,0 -> 13,0
13,0 -> 14,0
13,0 -> 15,0
14,0 -> 16,0
10,0 -> 17,0
5,0 -> 18,0
~
INPUTS = 2:
0. in~ -> 0,0~
1. clk~ -> 0,1 -> 1,1 -> 4,1 -> 5,1 -> 9,1 -> 10,1 -> 13,1 -> 14,1~
~
OUTPUTS = 8:
0. o0~ <- 0,0
1. o1~ <- 1,0
2. o2~ <- 4,0
3. o3~ <- 5,0
4. o4~ <- 9,0
5. o5~ <- 10,0
6. o6~ <- 13,0
7. o7~ <- 14,0
~