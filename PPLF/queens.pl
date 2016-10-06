listSize([_ | L], S) :-
  listSize(L, S0),
  S is S0 + 1.

listSize([], 0).

queens(L, S):-
  listSize(L, X),
  X > 3,
  permutation(L, S),
  safeQueen(S).

queens(L, _):-
  listSize(L, X),
  X =< 3,
  writeln('Number of queens must be higher than 3.'),
  !.

permutation([], []).

permutation([Elem | Rest], SolList):-
  permutation(Rest, Rest1),
  remove(Elem, SolList, Rest1).

remove(Elem, [Elem | List], List). %True if List = [Elem | List] (SolList - Elem = Rest1)

remove(Elem, [X | List], [X | List1]):- 
  remove(Elem, List, List1).

safeQueen([]). % Empty board is safe

safeQueen([Queen | OtherQueens]):-
  safeQueen(OtherQueens), % Other queens also must be safe
  noAttack(Queen, OtherQueens, 1). % 1 is the distance (each block of the board)

noAttack(_, [], _). % True if the new queen isn't attacking other queens.

noAttack(Q, [OtherQ | QList], Dist):-
  OtherQ - Q =\= Dist,
  Q - OtherQ =\= Dist,
  NextDist is Dist + 1,
  noAttack(Q, QList, NextDist).

printRest(10):-
  !.

printRest(X):-
  write('x '),
  X1 is X + 1,
  printRest(X1).

printBoard([], _, _).

printBoard(S, 11, Y):-
  nl,
  X = 1,
  Y1 is Y + 1,
  printBoard(S, X, Y1),
  nl,
  !.

printBoard(L, X, Y):-
  [S | S1] = L,
  S == X,
  write('Q '),
  printRest(X),
  nl,
  X1 is 1,
  Y1 is Y + 1,
  printBoard(S1, X1, Y1),
  !.

printBoard(L, X, Y):-
  [S | _] = L,
  S =\= X,
  write('x '),
  X1 is X + 1,
  printBoard(L, X1, Y),
  !.

queens10(A, B, C, D, E, F, G, H, I, J):-
( A < 0 ; A > 10 ; B < 0 ; B > 10;
  C < 0 ; C > 10 ; C < 0 ; C > 10;
  D < 0 ; D > 10 ; E < 0 ; E > 10;
  F < 0 ; F > 10 ; G < 0 ; G > 10;
  H < 0 ; H > 10 ; I < 0 ; I > 10;
  J < 0 ; J > 10),
  writeln('Values must be higher than 0 and lesser than 11.'),
  !.
  
queens10(A, B, C, D, E, F, G, H, I, J):-
  L = [A, B, C, D, E, F, G, H, I, J],
  not(safeQueen(L)),
  printBoard(L, 1, 1),
  nl,
  writeln('There is a conflict.'),
  !.

queens10(A, B, C, D, E, F, G, H, I, J):-
  L = [A, B, C, D, E, F, G, H, I, J],
  safeQueen(L),
  printBoard(L, 1, 1),
  nl,
  writeln('No conflicts'),
  !.
