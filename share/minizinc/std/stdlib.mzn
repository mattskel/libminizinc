%-----------------------------------------------------------------------------%
% MiniZinc standard library.
%-----------------------------------------------------------------------------%
% This file contains declarations of all functions, predicates and annotations
% available in the base MiniZinc language.

/***
 @groupdef MAIN The MiniZinc library
*/

/***
 @groupdef annotations Annotations
 
 These annotations control evaluation and solving behaviour.
*/


/***
 @groupdef annotations.general General annotations
*/

/** @group annotations.general Declare function as total, i.e. it does not put
  any constraints on its arguments. */
annotation promise_total;

/** @group annotations.general Declare the annotated variable as being functionally defined.
  This annotation is introduced into FlatZinc code by the compiler. */
annotation is_defined_var;
/** @group annotations.general Declare a variable as being introduced by the compiler. */
annotation var_is_introduced;
/** @group annotations.general Declare variable: \a c as being functionally defined
  by the annotated constraint. This annotation is introduced into FlatZinc code by the compiler. */
annotation defines_var(var $t: c);
/** @group annotations.general Declare that the annotated array should be printed by
  the solver with the given index sets \a a. This annotation is introduced into FlatZinc code by
  the compiler. */
annotation output_array(array[$u] of set of int:a);
/** @group annotations.general Declare that the annotated variable should be printed by
  the solver. This annotation is introduced into FlatZinc code by
  the compiler. */
annotation output_var;
/** @group annotations.general Declare that the annotated expression is used to map
  an expression back from FlatZinc to MiniZinc.  */
annotation is_reverse_map;
/** @group annotations.general Document the function or variable declaration item with
  the string \a s. */
annotation doc_comment(string: s);

/***
 @groupdef annotations.prop Propagation strength annotations
*/

/** @group annotations.prop Annotate a constraint to use domain propagation */
annotation domain;
/** @group annotations.prop Annotate a constraint to use bounds propagation */
annotation bounds;


/***
 @groupdef annotations.search Search annotations
*/

/** @group annotations.search Sequentially perform the searches specified in array \a s */
annotation seq_search(array[int] of ann: s);

/** @group annotations.search Specify search on variables \a x, with variable selection
  strategy \a select, value choice strategy \a choice, and exploration strategy
  \a explore.
*/
annotation int_search(
    array[int] of var int: x,
    ann: select,
    ann: choice,
    ann: explore,
);

/** @group annotations.search Specify search on variables \a x, with variable selection
  strategy \a select, value choice strategy \a choice, and exploration strategy
  \a explore.
*/
annotation bool_search(
    array[int] of var bool: x,
    ann: select,
    ann: choice,
    ann: explore
);

/** @group annotations.search Specify search on variables \a x, 
  with precision \a prec, variable selection
  strategy \a select, value choice strategy \a choice, and exploration strategy
  \a explore.
*/
annotation float_search(
    array[int] of var float: x,
    float: prec,
    ann: select,
    ann: choice,
    ann: explore
);

/** @group annotations.search Specify search on variables \a x, with variable selection
  strategy \a select, value choice strategy \a choice, and exploration strategy
  \a explore.
*/
annotation set_search(
    array[int] of var set of int: x,
    ann: select,
    ann: choice,
    ann: explore
);

/***
 @groupdef annotations.search.varsel Variable selection annotations
*/

/** @group annotations.search.varsel Search variables in the given order */
annotation input_order;
/** @group annotations.search.varsel Choose the variable with the smallest domain */
annotation first_fail;
/** @group annotations.search.varsel Choose the variable with the largest domain */
annotation anti_first_fail;
/** @group annotations.search.varsel Choose the variable with the smallest value in its domain */
annotation smallest;
/** @group annotations.search.varsel Choose the variable with the largest value in its domain */
annotation largest;
/** @group annotations.search.varsel Choose the variable with the largest number of attached constraints */
annotation occurrence;
/** @group annotations.search.varsel Choose the variable with the smallest domain,
  breaking ties using the number of attached constraints */
annotation most_constrained;
/** @group annotations.search.varsel Choose the variable with largest difference
  between the two smallest values in its domain  */
annotation max_regret;
/** @group annotations.search.varsel Choose the variable with largest domain, divided
  by the number of attached constraints weighted by how often they have caused failure */
annotation dom_w_deg;
/** @group annotations.search.varsel Choose the variable with the highest impact so
  far during the search  */
annotation impact;

/***
 @groupdef annotations.search.choice Value choice annotations
*/

/** @group annotations.search.choice Assign values in ascending order */
annotation indomain;
/** @group annotations.search.choice Assign the smallest value in the domain */
annotation indomain_min;
/** @group annotations.search.choice Assign the largest value in the domain */
annotation indomain_max;
/** @group annotations.search.choice Assign the value in the domain closest to
  the mean of its current bounds */
annotation indomain_middle;
/** @group annotations.search.choice Assign the middle value in the domain */
annotation indomain_median;
/** @group annotations.search.choice Assign a random value from the domain */
annotation indomain_random;
/** @group annotations.search.choice Bisect the domain, excluding the upper half first */
annotation indomain_split;
/** @group annotations.search.choice Bisect the domain, randomly selecting which half to exclude first */
annotation indomain_split_random;
/** @group annotations.search.choice Bisect the domain, excluding the lower half first */
annotation indomain_reverse_split;
/** @group annotations.search.choice 
  If the domain consists of several contiguous intervals, reduce the
  domain to the first interval. Otherwise bisect the domain.
 */
annotation indomain_interval;
/** @group annotations.search.choice Exclude the smallest value from the domain */
annotation outdomain_min;
/** @group annotations.search.choice Exclude the largest value from the domain */
annotation outdomain_max;
/** @group annotations.search.choice Exclude the middle value from the domain */
annotation outdomain_median;
/** @group annotations.search.choice Exclude a random value from the domain */
annotation outdomain_random;

/***
 @groupdef annotations.search.explore Exploration strategy annotations
*/

/** @group annotations.search.explore Perform a complete search */
annotation complete;


/***
  @groupdef optiontypes Option type support
  
  These functions and predicates implement the standard library for working
  with option types. Note that option type support is still incomplete.
*/

/** @group optiontypes Return value of \a x if \a x is not absent. Aborts 
  when evaluated on absent value. */
function $T: deopt(opt $T: x);

/** @group optiontypes Return value \a x unchanged (since \a x is guaranteed
  to be non-optional). */
function var $T: deopt(var $T: x) = x;

/** @group optiontypes Test if \a x is not absent (always returns true) */
test occurs(var $T: x) = true;
/** @group optiontypes Test if \a x is not absent */
test occurs(opt $T: x);
/** @group optiontypes Test if \a x is absent (always returns false) */
test absent(var $T: x) = false;
/** @group optiontypes Test if \a x is absent */
test absent(opt $T: x) = not occurs(x);

/***
  @groupdef optiontypes.bool Option type support for Booleans
*/

/** @group optiontypes.bool True iff \a x is not absent */
function var bool : occurs(var opt bool: x) ::promise_total =
  let {
    var bool : b = occurs_internal(x);
    var bool : dx = deopt_internal(x);
    constraint (x = reverse_map(b,dx)) :: is_reverse_map;
  } in b;
  
/** @group optiontypes.bool Return value of \a x (assumes that \a x is not absent) */
function var bool : deopt(var opt bool : x) ::promise_total =
  let {
    var bool : b = occurs_internal(x);
    var bool : dx = deopt_internal(x);
    constraint (x = reverse_map(b,dx)) :: is_reverse_map;
  } in dx;

/** @group optiontypes.bool True iff \a x is absent */
predicate absent(var opt bool: x) = not occurs(x);

function var bool: occurs_internal(var opt bool: x) ::promise_total =
  let { var bool : b; } in b;
function var bool : deopt_internal(var opt bool : x) ::promise_total =
  let { var bool: y } in y;

function var opt bool: reverse_map(var bool: occ, var bool: d);
function opt bool: reverse_map(bool: occ, bool: d) ::promise_total =
  if occ then d else <> endif;


/** @group optiontypes.bool True iff both \a b0 and \a b1 are absent or
  both are present and have the same value. */
predicate bool_eq(var opt bool: b0, var opt bool: b1) =
     (absent(b0) /\ absent(b1))
  \/ (occurs(b0) /\ occurs(b1) /\ deopt(b0)=deopt(b1));

/** @group optiontypes.bool True iff \a b0 occurs and is equal to \a b1 */
predicate bool_eq(var opt bool: b0, var bool: b1) =
  occurs(b0) /\ deopt(b0)=b1;

  /** @group optiontypes.bool True iff \a b1 occurs and is equal to \a b0 */
predicate bool_eq(var bool: b0, var opt bool: b1) =
  occurs(b1) /\ deopt(b1)=b0;

/** @group optiontypes.bool True iff for any \p i, \a x[i] is absent or true */
predicate forall (array[int] of var opt bool: x) =
  forall ([absent(x[i]) \/ deopt(x[i]) | i in index_set(x)]);

/** @group optiontypes.bool True iff for at least one \p i, \a x[i] occurs and is true */
predicate exists (array[int] of var opt bool: x) =
  exists ([occurs(x[i]) /\ deopt(x[i]) | i in index_set(x)]);

/** @group optiontypes.bool True iff \a x is absent or false */
function var bool: 'not'(var opt bool: x) = absent(x) \/ not deopt(x);

/** @group optiontypes.bool Return absent if \a idx is absent, otherwise return \a x[\a idx] */
function var opt bool: element(var opt int: idx, array[int] of var bool: x) =
  if absent(idx) then <> else element(deopt(idx),x) endif;

/** @group optiontypes.bool Return absent if \a idx1 or \a idx2 is absent, otherwise return \a x[\a idx1, \a idx2] */
function var opt bool: element(var opt int: idx1, var opt int: idx2, array[int,int] of var bool: x) =
  if absent(idx1) \/ absent(idx2) then <> else element(deopt(idx1),deopt(idx2),x) endif;

/** @group optiontypes.bool Return \a x[\a idx] */
function var opt bool: element(var int: idx, array[int] of var opt bool: x) =
  let {
    var opt bool: r;
    constraint occurs(r) = element(idx,array1d(index_set(x),[occurs(x[i]) | i in index_set(x)]));
    constraint deopt(r) = element(idx,array1d(index_set(x),[deopt(x[i]) | i in index_set(x)]));
  } in r;

/** @group optiontypes.bool Return \a x[\a idx1, \a idx2] */
function var opt bool: element(var int: idx1, var int: idx2, array[int,int] of var opt bool: x) =
  let {
    var opt bool: r;
    constraint occurs(r) = element(idx1,idx2,
      array2d(index_set_1of2(x),index_set_2of2(x),[occurs(x[i,j]) | i in index_set_1of2(x), j in index_set_2of2(x)]));
    constraint deopt(r) = element(idx1,idx2,
      array2d(index_set_1of2(x),index_set_2of2(x),[deopt(x[i,j]) | i in index_set_1of2(x), j in index_set_2of2(x)]));
  } in r;

/** @group optiontypes.bool Return absent if \a idx is absent, otherwise return \a x[\a idx] */
function var opt bool: element(var opt int: idx, array[int] of var opt bool: x) =
  if absent(idx) then <> else element(deopt(idx),x) endif;

/** @group optiontypes.bool Return absent if \a idx1 or \a idx2 is absent, otherwise return \a x[\a idx1, \a idx2] */
function var opt bool: element(var opt int: idx1, var opt int: idx2, array[int,int] of var opt bool: x) =
  if absent(idx1) \/ absent(idx2) then <> else element(deopt(idx1),deopt(idx2),x) endif;



/***
  @groupdef optiontypes.int Option type support for integers
*/

/** @group optiontypes.int True iff \a x is not absent */
function var bool : occurs(var opt int : x) ::promise_total =
  let {
    var bool : b = occurs_internal(x);
    var int  : dx = deopt_internal(x);
    constraint (x = reverse_map(b,dx)) :: is_reverse_map;
  } in b;

/** @group optiontypes.bool Return value of \a x (assumes that \a x is not absent) */
function var int : deopt(var opt int : x) ::promise_total =
  let {
    var bool : b = occurs_internal(x);
    var int  : dx = deopt_internal(x);
    constraint (x = reverse_map(b,dx)) :: is_reverse_map;
  } in dx;

/** @group optiontypes.bool True iff \a x is absent */
function var bool: absent(var opt int: x) ::promise_total = not occurs(x);

function var bool: occurs_internal(var opt int: x) ::promise_total =
  let { var bool : b; } in b;
function var int : deopt_internal(var opt int : x) ::promise_total =
  let { var lb(x)..ub(x): y } in y;

function var opt int: reverse_map(var bool: occ, var int: d);
function opt int: reverse_map(bool: occ, int: d) ::promise_total =
  if occ then d else <> endif;

/** @group optiontypes.int Weak comparison: true iff either \a x or \a y is absent, or both
  occur and the value of \a x is greater than the value of \a y. */
function var bool: '>'(var opt int: x, var opt int: y) = absent(x) \/ absent(y) \/ deopt(x) > deopt(y);
/** @group optiontypes.int Weak comparison: true iff either \a x or \a y is absent, or both
  occur and the value of \a x is greater than or equal to the value of \a y. */
function var bool: '>='(var opt int: x, var opt int: y) = absent(x) \/ absent(y) \/ deopt(x) >= deopt(y);
/** @group optiontypes.int Weak comparison: true iff either \a x or \a y is absent, or both
  occur and the value of \a x is less than the value of \a y. */
function var bool: '<'(var opt int: x, var opt int: y) = absent(x) \/ absent(y) \/ deopt(x) < deopt(y);
/** @group optiontypes.int Weak comparison: true iff either \a x or \a y is absent, or both
  occur and the value of \a x is less than or equal to the value of \a y. */
function var bool: '<='(var opt int: x, var opt int: y) = absent(x) \/ absent(y) \/ deopt(x) <= deopt(y);

/** @group optiontypes.int Return minimum of elements in \a x that are not absent, or
absent if all elements in \a x are absent. */
function var opt int: min(array[int] of var opt int: x) ::promise_total =
    let {
        var opt lb_array(x)..ub_array(x): m;
        int: xmax = ub_array(x);
        constraint occurs(m) <-> exists (i in index_set(x)) (occurs(x[i]));
        constraint occurs(m) ->
            deopt(m) = min([if occurs(xi) then deopt(xi) else xmax endif | xi in x]);
    } in m;

/** @group optiontypes.int Return maximum of elements in \a x that are not absent, or
absent if all elements in \a x are absent. */
function var opt int: max(array[int] of var opt int: x) ::promise_total =
    let {
        var opt lb_array(x)..ub_array(x): m;
        int: xmin = lb_array(x);
        constraint occurs(m) <-> exists (i in index_set(x)) (occurs(x[i]));
        constraint occurs(m) ->
            deopt(m) = max([if occurs(xi) then deopt(xi) else xmin endif | xi in x]);
    } in m;

/** @group optiontypes.int Weak addition. Return sum of \a x and \a y if both
are present, otherwise return absent. */
function var opt int: '~+'(var opt int: x, var opt int: y) ::promise_total =
    let { 
    int: l = if lb(x)=-infinity \/ lb(y)=-infinity then -infinity else lb(x)+lb(y) endif;
    int: u = if ub(x)=infinity \/ ub(y)=infinity then infinity else ub(x)+ub(y) endif;
    var opt l..u: result; 
    constraint absent(x) \/ absent(y) -> result = <>;
    constraint absent(x) \/ absent(y) \/ result = deopt(x)+deopt(y);
    } in result;

/** @group optiontypes.int Weak subtraction. Return difference of \a x and \a y if both
are present, otherwise return absent. */
function var opt int: '~-'(var opt int: x, var opt int: y) ::promise_total =
    let { 
    int: l = if lb(x)=-infinity \/ ub(y)=infinity then -infinity else lb(x)-ub(y) endif;
    int: u = if ub(x)=infinity \/ lb(y)=-infinity then infinity else ub(x)-lb(y) endif;
    var opt l..u: result; 
    constraint absent(x) \/ absent(y) -> result = <>;
    constraint absent(x) \/ absent(y) \/ result = deopt(x)-deopt(y);
    } in result;

/** @group optiontypes.int Weak multiplication. Return product of \a x and \a y if both
are present, otherwise return absent. */
function var opt int: '~*'(var opt int: x, var opt int: y) ::promise_total =
    if absent(x) \/ absent(y) then <>
    else deopt(x)*deopt(y) endif;

/** @group optiontypes.int Weak equality. True if either \a x or \a y are absent, or
present and equal.*/
function var bool: '~='(var opt int: x, var opt int: y) ::promise_total =
    absent(x) \/ absent(y) \/ deopt(x)=deopt(y);

/** @group optiontypes.int Return optional 0/1 integer that is absent iff \a x
  is absent, and 1 iff \a x occurs and is true. */
function var opt int: bool2int(var opt bool: x) ::promise_total =
  let {
    var opt 0..1: xi;
    constraint absent(xi)=absent(x);
    constraint deopt(xi)=bool2int(deopt(x));
  } in xi;

/** @group optiontypes.int True iff both \a x and \a y are absent or
  both are present and have the same value. */
predicate int_eq(var opt int: x, var opt int: y) =
     (absent(x) /\ absent(y))
  \/ (occurs(x) /\ occurs(y) /\ deopt(x)=deopt(y));

/** @group optiontypes.int True iff only one of \a x and \a y is absent or
  both are present and have different values. */
predicate int_ne(var opt int : x, var opt int : y) =
     (absent(x) != absent(y))
  \/ (occurs(x) /\ occurs(y) /\ deopt(x)!=deopt(y));

/** @group optiontypes.int Return sum of non-absent elements of \a x. */
function var int: sum(array[int] of var opt int: x) =
  sum (i in index_set(x)) (let { var int: dx = deopt(x[i]) } in if occurs(x[i]) then dx else 0 endif);

/** @group optiontypes.int Return absent if \a idx is absent, otherwise return \a x[\a idx] */
function var opt int: element(var opt int: idx, array[int] of var int: x) =
  if absent(idx) then <> else element(deopt(idx),x) endif;

/** @group optiontypes.int Return absent if \a idx1 or \a idx2 is absent, otherwise return \a x[\a idx1, \a idx2] */
function var opt int: element(var opt int: idx1, var opt int: idx2, array[int,int] of var int: x) =
  if absent(idx1) \/ absent(idx2) then <> else element(deopt(idx1),deopt(idx2),x) endif;

/** @group optiontypes.int Return \a x[\a idx] */
function var opt int: element(var int: idx, array[int] of var opt int: x) =
  let {
    var opt int: r;
    constraint occurs(r) = element(idx,array1d(index_set(x),[occurs(x[i]) | i in index_set(x)]));
    constraint deopt(r) = element(idx,array1d(index_set(x),[deopt(x[i]) | i in index_set(x)]));
  } in r;

/** @group optiontypes.int Return \a x[\a idx1, \a idx2] */
function var opt int: element(var int: idx1, var int: idx2, array[int,int] of var opt int: x) =
  let {
    var opt int: r;
    constraint occurs(r) = element(idx1,idx2,
      array2d(index_set_1of2(x),index_set_2of2(x),[occurs(x[i,j]) | i in index_set_1of2(x), j in index_set_2of2(x)]));
    constraint deopt(r) = element(idx1,idx2,
      array2d(index_set_1of2(x),index_set_2of2(x),[deopt(x[i,j]) | i in index_set_1of2(x), j in index_set_2of2(x)]));
  } in r;

/** @group optiontypes.int Return absent if \a idx is absent, otherwise return \a x[\a idx] */
function var opt int: element(var opt int: idx, array[int] of var opt int: x) =
  if absent(idx) then <> else element(deopt(idx),x) endif;

/** @group optiontypes.int Return absent if \a idx1 or \a idx2 is absent, otherwise return \a x[\a idx1, \a idx2] */
function var opt int: element(var opt int: idx1, var opt int: idx2, array[int,int] of var opt int: x) =
  if absent(idx1) \/ absent(idx2) then <> else element(deopt(idx1),deopt(idx2),x) endif;

/***
 @groupdef options Compiler options
*/

/*** @group options Whether to only generate domains that are contiguous ranges */
opt bool: mzn_opt_only_range_domains;

/*** @group options Check whether to only generate domains that are contiguous ranges */

test mzn_check_only_range_domains() =
  if absent(mzn_opt_only_range_domains) then false
  else deopt(mzn_opt_only_range_domains) endif;

include "builtins.mzn";
