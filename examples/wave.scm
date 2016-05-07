(cload "../ku.so" "init_ku")

(set-size 512 128)

(define (vline x_pos y_start y_size) 
  (if (= y_size 0) 
    ()
    (begin 
      (set-color x_pos y_start 8)
      (vline x_pos (+ y_start 1) (- y_size 1)))))

(define (amp-bar x amp)
    (vline x 
         (floor (* (abs (- 1 amp)) (/ height 2)))
         (floor (* (abs amp) height))))

(define (centerline size)
  (if (= 0 size) 
    ()
    (begin
      (set-color size (- (/ height 2) 1) 8)
      (centerline (- size 1)))))

(define (do-plot x amp np)
  (if (= x np) 
    (begin 
      (amp-bar x amp) 
      (write-png "out.png"))
    (begin 
      (amp-bar x amp))))



;(vline 0 0 (/ height 2))

(centerline width)
(define (run)
    (do-plot (floor (ps-tget "in" 0)) (abs (ps-tget "in" 1)) 512))

