#lang racket

(require htdp/matrix)
(require readline)
(require (planet neil/charterm:3:0))
(require racket/date)

(define-struct gameMatrix ([hasMine #:mutable] [statusVisit #:mutable] [minesAround #:mutable] [isFlagged #:mutable]))

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

(define (getPlayerName)
  (display "Please input your name:")
  (define aux (read-line (current-input-port)))
  aux)

(define playerName (getPlayerName))
(define boardSize (getSize))

(define (getMatrixCells)
  (define auxList (list))
  (define cell (make-gameMatrix 0 0 0 0))
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
(define numFlags numMines)

(define (setMines board nMines)
  (define i 0)
  (define j 0)
  (when (> nMines 0)
    (set! i (random boardSize))
    (set! j (random boardSize))
    (let ([aux (matrix-ref board i j)])
      (cond 
        [(= (gameMatrix-hasMine aux) 0)
        (set! board (matrix-set board i j (make-gameMatrix 1 0 0 0)))
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
        [(= (gameMatrix-isFlagged cell) 1)
          (if (= (gameMatrix-hasMine cell) 0)
            (display "( W )")
            (display "( F )"))]
        [(= (gameMatrix-hasMine cell) 1) (display "( x )")]
        [(= (gameMatrix-statusVisit cell) 0) (display "( - )")]
        [else 
          (define minesInCell (gameMatrix-minesAround cell))
          (display "( ") (display minesInCell) (display " )")])))
    (display "\n")))

(define (printGameBoard board)
  (for ([i boardSize]) (for ([j boardSize])
    (let ([cell (matrix-ref board i j)])
      (cond
        [(= (gameMatrix-isFlagged cell) 1) (display "( F )")]
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
    (when (= (gameMatrix-isFlagged (matrix-ref board x y)) 1)
      (set! numFlags (add1 numFlags)))
    (when (= (gameMatrix-statusVisit (matrix-ref board x y)) 0)
      (set! toWin (sub1 toWin))
      (set! board (matrix-set board x y (make-gameMatrix 0 1 minesAround 0))))
    (if (= minesAround 0)
      (for ([i (in-range (- x 1) (+ x 2))]) (for ([j (in-range (- y 1) (+ y 2))])
        (when (and (and (> i -1) (> j -1)) (and (< i boardSize) (< j boardSize)))
          (when (= (gameMatrix-statusVisit (matrix-ref board i j)) 0)
            (set! stack (stackPush stackCells (make-coordCell i j)))
            (set! board (recursiveVisit stack board boardSize))))))
    (set! board (recursiveVisit stack board boardSize))))
  board
)

(define (walk board boardSize x y)
  (let ([cell (matrix-ref board x y)])
    (define minesAround (checkCell board boardSize x y))
    (set! board (matrix-set board x y (make-gameMatrix (gameMatrix-hasMine cell) 1 minesAround 0)))
    (set! toWin (sub1 toWin))
    (cond 
      [(= (gameMatrix-hasMine cell) 1)
        (clearScreen)
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

(define (flag board x y)
  (define currentMine (gameMatrix-hasMine (matrix-ref board x y)))
  (define currentVisit (gameMatrix-statusVisit (matrix-ref board x y)))
  (define currentMinesAround (gameMatrix-minesAround (matrix-ref board x y)))
  (set! board (matrix-set board x y (make-gameMatrix currentMine currentVisit currentMinesAround 1)))
  board)

(define (deflag board x y)
  (define currentMine (gameMatrix-hasMine (matrix-ref board x y)))
  (define currentVisit (gameMatrix-statusVisit (matrix-ref board x y)))
  (define currentMinesAround (gameMatrix-minesAround (matrix-ref board x y)))
  (set! board (matrix-set board x y (make-gameMatrix currentMine currentVisit currentMinesAround 0)))
  board)


(define (runGame seconds minutes)
  (define move (void))
  (printGameBoard gameBoard)
  (display "Make your move: ")
  (display "(")
  (display numFlags)
  (displayln " flag (s) available.)")
  (set! move (string-split (read-line (current-input-port))))
  (clearScreen)
  (case (list-ref move 0)
    [("!") 
      (set! gameBoard (walk gameBoard boardSize (string->number (list-ref move 1)) (string->number (list-ref move 2))))
      (when (= toWin 0) 
        (printGameBoardEnd gameBoard)
        (displayln "You win !!")
        (define endMinutes (- (date-minute (current-date)) minutes))
        (define endSeconds (- (date-second (current-date)) seconds))
        (when (negative? endMinutes) 
          (set! endMinutes (+ endMinutes 60)))
        (when (negative? endSeconds) 
          (set! endSeconds (+ endSeconds 60)))
          (fprintf (current-output-port) "Player: ~s\n" playerName)
          (fprintf (current-output-port) "Time: ~s minute (s) and ~s second (s)\n" endMinutes endSeconds)
        (exit))
      (runGame seconds minutes)]

    [("@")
      (define auxX (string->number (list-ref move 1)))
      (define auxY (string->number (list-ref move 2)))

      (cond 
        [(= (gameMatrix-isFlagged (matrix-ref gameBoard auxX auxY)) 1)
          (set! gameBoard (deflag gameBoard auxX auxY))
          (set! numFlags (add1 numFlags))
          (set! toWin (add1 toWin))
          (runGame seconds minutes)]

        [(= (gameMatrix-statusVisit (matrix-ref gameBoard auxX auxY)) 0)
          (if (> numFlags 0) 
            ((set! toWin (sub1 toWin)) 
            (set! gameBoard (flag gameBoard (string->number (list-ref move 1)) (string->number (list-ref move 2))))
            (set! numFlags (sub1 numFlags))
            (when (= toWin 0) 
              (printGameBoardEnd gameBoard)
              (displayln "You win !!")
              (define endMinutes (- (date-minute (current-date)) minutes))
              (define endSeconds (- (date-second (current-date)) seconds))
              (when (negative? endMinutes) 
                (set! endMinutes (+ endMinutes 60)))
              (when (negative? endSeconds) 
                (set! endSeconds (+ endSeconds 60)))
              (fprintf (current-output-port) "Player: ~s\n" playerName)
              (fprintf (current-output-port) "Time: ~v minute (s) and ~v second (s)\n" endMinutes endSeconds)
              (exit))
            (runGame seconds minutes)) 
          ;else
            ((displayln "You don't have enough flags!\n")
            (runGame seconds minutes)))]
        
        [else (runGame seconds minutes)])]

    [("exit") (exit)]))

(clearScreen)
(set! gameBoard (setMines gameBoard numMines))
(define startTimeMinutes (date-minute (current-date)))
(define startTimeSeconds (date-second (current-date)))
(runStart)
(runGame startTimeSeconds startTimeMinutes)
