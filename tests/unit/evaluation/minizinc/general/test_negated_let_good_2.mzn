% RUNS ON mzn20_fd
% RUNS ON mzn20_fd_linear
array[1..1] of var 0..2: x;
constraint not (let { var int: y = x[x[1]] } in y > 0);

solve :: int_search(x, input_order, indomain_min, complete)
      satisfy;

output [show(x)];
