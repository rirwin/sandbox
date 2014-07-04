;; to run: $ clojure simple_functions.clj

(def start-script (fn [] "Staring Clojure script"))
(println (start-script))

(defn add [a b]
      (+ a b))

(defn abs [x]
 (if (< x 0)
  (- x)
  x))


(defn factorial [n]
  (reduce * (range 1 (inc n))))

(println "factorial" (factorial 4))


;(println (+ 4 (add 1 2)))

(defn fib-seq [a] 
 (reduce + (range 1 (inc a))))

;fib-seq (- a 1)))
;   (+ a (fib-seq (- a 1)))))

(println "Fibonacci" (fib-seq 4))


;(println (abs -1))
;(println (abs (+ 4 3)))
