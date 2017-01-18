#lang racket

(require htdp/matrix)
(require readline)
(require (planet neil/charterm:3:0))

(define-struct gameMatrix ([hasMine #:mutable] [statusVisit #:mutable] [minesAround #:mutable] [isFlagged #:mutable #:auto]) #:auto-value 0)

(define-struct coordCell (x y))
(define stackCells (list))

(define (stackPush stack coord)
  (append stack (list coord)))

(define (stackPop stack)
  (when (not (empty? stack))
    (define aux (rest(reverse stack)))
    (set! aux (reverse aux))
    aux))

(define (stackTop stack)
  (when (not (empty? stack))
    (define aux (reverse stack))
    (define top (first aux))
    top))

(define (clearScreen)
  (with-charterm (void (charterm-clear-screen))))

(define (getSize)
  (display "Please input the board's size:\n")
  (define aux (read-line (current-input-port)))
  (define boardSize (string->number aux))
  (cond
    [(or (not (number? boardSize)) (not (positive? boardSize))) (and ((display "Invalid entry.\n") (exit)))]
  )
  boardSize)

(define boardSize (getSize))

(define (getMatrixCells)
  (define auxList (list))
  (define cell (make-gameMatrix 0 0 0))
  (for ([i (* boardSize boardSize)])
    (set! auxList (append auxList (list cell)))
  )
  auxList)

(define cellList (getMatrixCells))

(define gameBoard (make-matrix boardSize boardSize cellList))

(define (getNumMines)
  (display "Please input the number of mines:\n")
  (define aux (read-line (current-input-port)))
  (define nMines (string->number aux))
  (cond
    [(or (not (number? nMines)) (not (positive? nMines))) (and ((display "Invalid entry.\n") (exit)))]
  )
  nMines)

(define numMines (getNumMines))
(define numflags numMines)

(define (setMines board nMines)
  (define i 0)
  (define j 0)
  (when (> nMines 0)
    (set! i (random boardSize))
    (set! j (random boardSize))
    (let ([aux (matrix-ref board i j)])
      (cond 
        [(= (gameMatrix-hasMine aux) 0)
        (set! board (matrix-set board i j (make-gameMatrix 1 0 0)))
        (set! board (setMines board (sub1 nMines)))]
        [else (set! board(setMines board nMines))])))
  board)

;Testing purposes
(define (debugPrintGameBoard board)
  (for ([i boardSize]) (for ([j boardSize])
    (let ([cell (matrix-ref board i j)])
      (if (= (gameMatrix-hasMine cell) 1) (display "( x )") (display "( - )"))))
    (display "\n")))
;Testing purposes 

(define (printGameBoardEnd board)
  (for ([i boardSize]) (for ([j boardSize])
    (let ([cell (matrix-ref board i j)])
      (cond
        [(= (gameMatrix-statusVisit cell) 0) (display "( - )")]
        [(= (gameMatrix-hasMine cell) 1) (display "( x )")]
        [else 
          (define minesInCell (gameMatrix-minesAround cell))
          (display "( ") (display minesInCell) (display " )")])))
    (display "\n")))

(define (printGameBoard board)
  (for ([i boardSize]) (for ([j boardSize])
    (let ([cell (matrix-ref board i j)])
      (cond
        [(= (gameMatrix-statusVisit cell) 0) (display "( - )")]
        [else 
          (define minesInCell (gameMatrix-minesAround cell))
          (display "( ") (display minesInCell) (display " )")])))
    (display "\n")))

(define (checkCell board boardSize x y)
  (define n 0)
  (for ([i (in-range (- x 1) (+ x 2))]) (for ([j (in-range (- y 1) (+ y 2))])
    (cond 
      [(and (and (< i boardSize) (< j boardSize)) (and (> i -1) (> j -1)))
        (let ([aux (matrix-ref board i j)])
          (when (= (gameMatrix-hasMine aux ) 1) (set! n (add1 n))))]
      [else (void)])))
  n)

(define toWin (* boardSize boardSize))

(define (runStart)
  (displayln "========== Minesweeper ==========")
  (displayln "Use ! x y to walk")
  (displayln "Use @ x y to flag")
  (displayln "Use exit to end the game")
  (displayln "Press any key to continue.")
  (define key (read-line (current-input-port)))
  (clearScreen))

(define (recursiveVisit stack board boardSize)
  (when (not (empty? stack))
    (define x (coordCell-x (stackTop stack)))
    (define y (coordCell-y (stackTop stack)))
    (set! stack (stackPop stack))
    (define cell (matrix-ref board x y))
    (define minesAround (checkCell board boardSize x y))
    (set! board (matrix-set board x y (make-gameMatrix 0 1 minesAround)))
    (set! toWin (sub1 toWin))
    (if (= minesAround 0)
      (for ([i (in-range (- x 1) (+ x 2))]) (for ([j (in-range (- y 1) (+ y 2))])
        (when (and (and (> i -1) (> j -1)) (and (< i boardSize) (< j boardSize)))
          (when (= (gameMatrix-statusVisit (matrix-ref board i j)) 0)
            (set! stack (stackPush stackCells (make-coordCell i j)))
            (set! board (recursiveVisit stack board boardSize))))))
    (set! board (recursiveVisit stack board boardSize)))
    )
  board
)

(define (walk board boardSize x y)
  (let ([cell (matrix-ref board x y)])
    (define minesAround (checkCell board boardSize x y))
    (set! board (matrix-set board x y (make-gameMatrix (gameMatrix-hasMine cell) 1 minesAround)))
    (set! toWin (sub1 toWin))
    (cond 
      [(= (gameMatrix-hasMine cell) 1)
        (displayln "BOOM! You lose.")
        (printGameBoardEnd gameBoard)
        (exit)]
      [(= (gameMatrix-minesAround cell) 0)
        (when (= (gameMatrix-statusVisit cell) 0)
          (set! stackCells (stackPush stackCells (make-coordCell x y)))
          (set! board (recursiveVisit stackCells board boardSize)))]
    ))
  board
)

(define (runGame)
  (define move (void))
  (printGameBoard gameBoard)
  (displayln "Make your move: ")
  (set! move (string-split (read-line (current-input-port))))
  (clearScreen)
  (case (list-ref move 0)
    [("!") 
      (set! gameBoard (walk gameBoard boardSize (string->number (list-ref move 1)) (string->number (list-ref move 2))))
      (when (= toWin 0) 
        (printGameBoardEnd gameBoard)
        (displayln "You win !!")
        (exit))
      (runGame)]

    [("@") (display "Flag!")]

    [("exit") (exit)]
  )
)

(set! gameBoard (setMines gameBoard numMines))
(runStart)
(runGame)
(displayln "All OK.")

