(cload "./ku.so" "init_ku")

(define straightline (lambda (clr pos w) 
    (if (= w 0) 
        () 
        (begin 
            (set-color (- w 1) pos clr)
            (straightline clr pos (- w 1))))))

(define colorbars (lambda (num) 
    (if (= num 0) 
        ()
        (begin (straightline (random 16) (- num 1) width) 
                (colorbars (- num 1))))))


(ps-eval 0 "'x' '0' gen_vals")

(set-size 512 256)

(define writeframe (lambda () 
(begin 
    (colorbars height))
    (write-png (string-append (number->string (floor (ps-tget "x" 0)))  ".png"))
    (ps-tset "x" 0 (+ (ps-tget "x" 0) 1))
    (ps-tset "in" 1 (ps-tget "in" 0))
))

(writeframe)

(define tick (lambda () (begin (write "tick")  (newline))))

(define run (lambda () (begin)))
