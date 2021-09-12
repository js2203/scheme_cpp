;(display "hier init.scm\n")

; get the void object (this is the ret-value of display)
(define #void (display))
(define nil ())
(define (not b) (if b #f #t))

(define (< n1 n2) (> n2 n1))
(define (<= n1 n2) (not (> n1 n2)))
(define (>= n1 n2) (not (< n1 n2)))
(define (!= n1 n2) (not (= n1 n2)))

(define first car)
(define (second l) (car (cdr l)))
(define (third l)  (car (cdr (cdr l))))
(define (fourth l) (car (cdr (cdr (cdr l)))))

(define (fac n)
    ; a deep recursive version of factorial
    (if (< n 1)
	1
    ;
	(* n (fac (- n 1)))))

(define (fac_t n)
    ; a tail-recursive version of factorial
    (define (helper accu n)
	(if (= n 1)
	    accu
	;
	    (helper (* n accu) (- n 1))))

    (helper 1 n))

(define (count-down n)
    ; for measurements
    (if (= n 0)
	0
    ;
	(count-down (- n 1))))

(define (length-helper countSoFar restList)
    (if (eq? restList ())
	countSoFar
    ; else
	(length-helper (+ countSoFar 1) (cdr restList))))
(define (length list)
    (length-helper 0 list))

;;
;; memq? -> true if an element is in a list;
;; compares using eq? (i.e. for identity)
;;
(define (memq? element list)
    (if (eq? list ())
	#f
    ; else
	(if (eq? element (car list))
	    #t
	; else
	    (memq? element (cdr list)))))

;;
;; index-in-list -> index of the element within a list;
;; nil if not.
;; compares using eq? (i.e. for identity)
;;
(define (index-in-list-helper element list idxIn )
    (if (eq? list ())
	nil
    ; else
	(if (eq? element (car list))
	    idxIn
	; else
	    (index-in-list-helper element (cdr list) (+ idxIn 1) ))))

(define (index-in-list? element list)
    (index-in-list-helper element list 0)
)

;;
;; a recursive equal compare
;;
(define (equal-pair? a b)
    (if (equal? (car a) (car b))
	(equal? (cdr a) (cdr b))
    ; else
	#f
    )
)

(define (vector-equal-from-to? startIndex stopIndex a b)
    (if (= startIndex stopIndex)
	#t
    ; else
	(if (equal? (vector-ref startIndex a)
		    (vector-ref startIndex b))
	    (vector-equal-from-to? (+ 1 startIndex) stopIndex a b)
	; else
	    #else
	)
    )
)

(define (vector=? a b)
    (if (eq? (vector-length a) (vector-length b))
	(vector-equal-from-to? 0 (vector-length a) a b)
    ; else
	#f
    )
)

(define (equal? a b)
    (if (eq? a b)
	#t
    (if (eq? a nil)
	(eq? b nil)
    (if (eq? a #t)
	b
    (if (eq? b #t)
	#f
    (if (cons? a)
	(if (cons? b)
	    (equal-pair? a b)
	; else
	    #false
	)
    (if (number? a)
	(if (number? b)
	    (= a b)
	; else
	    #false
	)
    (if (string? a)
	(if (string? b)
	    (string=? a b)
	; else
	    #false
	)
    (if (vector? a)
	(if (vector? b)
	    (vector=? a b)
	; else
	    #false
	)
    ; else
	#f
    ))))))))
)


(define (foreach list func)
    ; evaluate func for each element in a list
    (if (eq? list ())
	#void
    ; else
	(begin
	    (func (car list))
	    (foreach (cdr list) func))))

(define (range start stop)
    ; generate a list start..stop - similar to python's range
    (define (helper start stop listTail)
	(if (> start stop)
	    listTail
	;
	    (helper start (- stop 1) (cons stop listTail))))
    (helper start stop ()))

;(load "compiler.scm")

;(display "end of init.scm\n")
