#lang racket

(require math/matrix)
(require readline)

(struct gameMatrix ([hasMine #:mutable] [statusVisit #:mutable]))

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
(define gameBoard (make-matrix boardSize boardSize (gameMatrix 0 0)))

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
  (define minesList (list))
  (for ([i nMines])
    (append minesList (list 1))     
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

(printGameBoard gameBoard)
