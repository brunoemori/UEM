#lang racket

(require htdp/matrix)
(require readline)

(define-struct gameMatrix ([hasMine #:mutable] [statusVisit #:mutable]))

(define (getSize)
  (display "Please input the board's size:\n")
  (define aux (read-line (current-input-port)))
  (define boardSize (string->number aux))
  (cond
    [(or (not (number? boardSize)) (not (positive? boardSize))) (and ((display "Invalid entry.\n") (exit)))]
  )
  boardSize
)

(define boardSize (getSize))

(define (getMatrixCells)
  (define auxList (list))
  (define cell (make-gameMatrix 0 0))
  (for ([i (* boardSize boardSize)])
    (set! auxList (append auxList (list cell)))
  )
  auxList
)

(define cellList (getMatrixCells))

(define gameBoard (make-matrix boardSize boardSize cellList))

(define (getNumMines)
  (display "Please input the number of mines:\n")
  (define aux (read-line (current-input-port)))
  (define nMines (string->number aux))
  (cond
    [(or (not (number? nMines)) (not (positive? nMines))) (and ((display "Invalid entry.\n") (exit)))]
  )
  nMines
)

(define numMines (getNumMines))

(define (setMines board nMines)
  (define i 0)
  (define j 0)
  (when (> nMines 0)
    (set! i (random boardSize))
    (set! i (random boardSize))
    (let ([aux (matrix-ref board i j)])
      (when (= (gameMatrix-hasMine aux) 0)
        (matrix-set board i j (make-gameMatrix 1 0))
        (setMines board (sub1 nMines))
      )
      (setMines board nMines)
    )
  )
)

(define (printGameBoard board)
  (for ([i boardSize])
    (for ([j boardSize])
      (display "( - )")
    )
    (display "\n")
  )
)

(setMines gameBoard numMines)
(displayln "All OK.")
