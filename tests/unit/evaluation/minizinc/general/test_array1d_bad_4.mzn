% RUNS ON mzn20_fd
% RUNS ON mzn20_fd_linear
% RUNS without failure ON mzn_cd_fd
array [1..3] of var int: a = [1, 2, 3];
array [11..13] of var int: b = a;

solve satisfy;
output [show([a,b])];