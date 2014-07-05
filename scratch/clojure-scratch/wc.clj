(use '[clojure.string :only (join split)])
(def sentence "san francisco is foggy a lot; it is a fun place to live")
(println
 (join "\n"
  (split sentence #"\s")))

(println 
 (join "\n"
  (map #(str "'" % "' is a string in the sentence") 
   (split sentence #"\s"))))

(defn map-count [ma ke] 
  (assoc ma ke (inc (get ma ke 0))))

(println 
 (reduce map-count {} 
  (split sentence #"\s")))

(println 
 (reduce #(assoc %1 %2 (inc (get %1 %2 0))) {}
  (split sentence #"\s")))



   