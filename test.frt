: IMMEDIATE save_lw @ cfa 1 - dup c@ 1 or swap c! ;

: rot >r swap r> swap ;
: -rot swap >r swap  r> ;

: over >r dup r> swap ;
: 2dup over over ;

: if ' branchifz , here 0 , ; IMMEDIATE

: else ' branch , here 0 , swap here swap ! ; IMMEDIATE

: then here swap ! ; IMMEDIATE

: repeat here ; IMMEDIATE

: until ' branchifz , , ; IMMEDIATE

: dub_over >r dup r> swap >r dup r> swap ;

: for
        ' swap ,
        ' >r ,
        ' >r ,
here    ' r> ,
        ' r> ,
        ' dub_over ,
        ' >r ,
        ' >r ,
        ' < ,
        ' branchifz ,
here      0 ,
        swap ; IMMEDIATE

: endfor
        ' r> ,
        ' lit , 1 ,
        ' + ,
        ' >r ,
        ' branch ,
         , here swap !
        ' r> ,
        ' drop ,
        ' r> ,
        ' drop ,
;  IMMEDIATE

: fill_stack
    1
    for
      dup
    endfor
;

: factorial
  dup -1 > if
    dup 0 > if
      1 swap
        repeat
          dup rot *
          swap 1 -
          dup 0 =
        until
      drop
    else drop 1 then
  else drop 0 then
;

: is_even
  2 % not ;

: is_prime
  dup 0 < if else
  dup 2 < if drop 0 else
  dup 2 = if drop 1 else
  dup 3 = if drop 1 else
  dup is_even if drop 0
  else
    1 >r
    3 swap
    repeat
        2dup
        swap 2 + swap
        >r >r
        swap % 0 =
        if r> r> drop drop r> 0 >r
        else
          r> r> 2dup
          swap dup * <
          dup >r if drop drop then r>
        then
    until
    r>
  then
  then
  then
  then
  then
;

-1 factorial . printnl
0 factorial . printnl
2 factorial . printnl
4 factorial . printnl
printnl

-1 is_prime . printnl
0 is_prime . printnl
1 is_prime . printnl
2 is_prime . printnl
4 is_prime . printnl
13 is_prime . printnl
10007 is_prime . printnl
printnl

123 4 fill_stack .S
