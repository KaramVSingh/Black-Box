GATES = 12:
0. DFLIPFLOP
1. DFLIPFLOP
2. AND
3. AND
4. NOT
5. OR
6. AND
7. AND
8. NOT
9. OR
10. OUTPUT
11. OUTPUT
~
CONNECTIONS:
5,0 -> 0,0
9,0 -> 1,0
4,0 -> 2,1
2,0 -> 5,0
3,0 -> 5,1
8,0 -> 6,1
0,0 -> 6,0
6,0 -> 9,0
7,0 -> 9,1
0,0 -> 10,0
1,0 -> 11,0
~
INPUTS = 5:
0. clk~ -> 0,1 -> 1,1~
1. regIn~ -> 2,0~
2. load0~ -> 3,0~
3. sel~ -> 3,1 -> 4,0 -> 7,1 -> 8,0~
4. load1~ -> 7,0~
~
OUTPUTS = 2:
0. o0~ <- 0,0
1. o1~ <- 1,0
~