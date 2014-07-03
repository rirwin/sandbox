(def f(parserfn [_ (val-test :a)
           _ (val-test :a)
           i next-item]
           {:foo i }))

(f [:a :a 1])

(f [:a :a 1 :b :d :e "aaa"])

(f [:a :b 1 :b :d :e "aaa"]) // fails

(def f2 (parserfn [_ (val-test :a)
           _ (val-test :b)
           i next-item]
           i2 next-item]
           [i2 i ]))

(f2 [:a :b 1])

(def f3 (parserfn [p1 f p2 f2] {:result p1 :result2 p2}))

(f3 [:a :a 1 :a :b 2 1 :c])

(f3 [:a :a 1 :a :b 2]) //fails

(def f4 (match-one f f2))

(f4 [:a :a 1])
(f4 [:a :b 1]) // fails
(f4 [:a :b 1 2])

(def f5 (none-or-more f4))