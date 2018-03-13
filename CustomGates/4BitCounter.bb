GATES = 8:
0. DFLIPFLOP
1. DFLIPFLOP
2. OUTPUT
3. OUTPUT
4. DFLIPFLOP
5. DFLIPFLOP
6. OUTPUT
7. OUTPUT
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
~
INPUTS = 1:
0. clk~ -> 0,1~
~
OUTPUTS = 5:
0. o0~ <- 0,1
1. o1~ <- 1,1
2. o2~ <- 4,1
3. common~ <- 5,0
4. o3~ <- 5,1
~