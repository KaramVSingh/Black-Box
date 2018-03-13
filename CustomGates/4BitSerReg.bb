GATES = 9:
0. DFLIPFLOP
1. DFLIPFLOP
2. OUTPUT
3. OUTPUT
4. DFLIPFLOP
5. DFLIPFLOP
6. OUTPUT
7. OUTPUT
8. OUTPUT
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
~
INPUTS = 2:
0. in~ -> 0,0~
1. clk~ -> 0,1 -> 1,1 -> 4,1 -> 5,1~
~
OUTPUTS = 4:
0. o0~ <- 0,0
1. o1~ <- 1,0
2. o2~ <- 4,0
3. o3~ <- 5,0
~