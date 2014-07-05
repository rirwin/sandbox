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

(defn summation [n] 
 (reduce + (range 1 (inc n))))

; test to see if calling function properly
(defn mysort [ar]
 (sort ar))

(def a[3,5,1,3,2,6,1])
(println (mysort a))


(println (map #(+ % %) [0 1 2 3 4 5 6]))

(println "4 factorial" (factorial 4))

(println "summation (x=1,x=4)" (summation 4))



(loop [x 10]
 (when (> x 1)
  (println x)
   (recur (- x 2))))


