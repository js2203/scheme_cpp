# Scheme interpreter written in C++20 in Clion
This interpreter was developed for the lecture "Design und Implementation fortgeschrittener Programmiersprachen" at HdM Stuttgart in SS2021.
The lecture was given by Claus Gittinger.

### Syntax
| syntax                   |       symbol       | example                              |                                                    result |
|--------------------------|:------------------:|--------------------------------------|----------------------------------------------------------:|
| define                   |         `define`   | `(define a "value")`                 |                                                      ` `  |
| if                       |         `if`       | `(if #t (display "true") (display "false"))`   |                                          `true` |
| set                      |         `set!`     | `(set! a "new value")`               |                                                      ` `  |
| begin                    |         `begin`    | `(begin (define a 2) (+ 3 a))`       |                                                       `5` |
| lambda                   |         `lambda`   | `((lambda (x y) (+ x y)) 1 2)`       |                                                       `3` |
| quote                    |         `quote`    | `(quote 42)`                            |                                                   `42` |



### Functions

| function                 |       symbol       | type    | example                              |                                                    result |
|--------------------------|:------------------:|---------|--------------------------------------|----------------------------------------------------------:|
| addition                 |         `+`        | builtin | `(+ 1 2 5)`                          |                                                       `8` |
| subtraction              |         `-`        | builtin | `(- 10 5)`                           |                                                       `5` |
| multiplication           |         `*`        | builtin | `(* 2 2)`                            |                                                       `4` |
| division                 |         `/`        | builtin | `(/ 5 2)`<br/>`(/ 6 2)`              |                                             `2.5`<br/>`3` |
| power of n               | `pow`              | udf     | `(pow 10 3)`                         | `1000`                                                    |
| greater than             |         `>`        | udf     | `(> 5 2)`                            |                                                      `#t` |
| lesser than              |         `<`        | builtin | `(< 5 2)`                            |                                                      `#f` |
| greater than or equal    |        `>=`        | udf     | `(>= 5 5)`                           |                                                      `#t` |
| lesser than or equal     |        `<=`        | udf     | `(<= 5 5)`                           |                                                      `#t` |
| equal numbers            |         `=`        | builtin | `(= 1 11)`                           |                                                      `#f` |
| equal strings            |   `equal-string?`  | builtin | `(equal-string? "a" "a")`            |                                                      `#t` |
| equal objects            | `eq?`              | builtin | `(eq? 1 1)`                          | `#f`                                                      |
| equal?                   |      `equal?`      | udf     | `(equal? 1.0 0)`                     |                                                      `#t` |
| list                     |       `list`       | builtin | `(list 1 2 3)`                       |                                                 `(1 2 3)` |
| cons                     |       `cons`       | builtin | `(cons 1 '(2 3))`                    |                                                 `(1 2 3)` |
| car                      |        `car`       | builtin | `(car '(1 2 3))`                     |                                                       `1` |
| cdr                      |        `cdr`       | builtin | `(cdr '(1 2 3))`                     |                                                   `(2 3)` |
| function arglist         | `function-arglist` | builtin | `(function-arglist plus1)`           | `(x)`                                                     |
| function body            | `function-body`    | builtin | `(function-body plus1)`              | `(+ x 1)`                                                 |
| is string                |      `string?`     | builtin | `(string? "asd")`                    |                                                      `#t` |
| is number                |      `number?`     | builtin | `(number? 0)`                        |                                                      `#t` |
| is cons                  |       `cons?`      | builtin | `(cons? '(1 2 3))`                   |                                                      `#t` |
| is function              |     `function?`    | builtin | `(function? +)`                      |                                                      `#t` |
| is user defined function |  `user-function?`  | builtin | `(user-function? +)`                 |                                                      `#f` |
| is real bool value       |       `bool?`      | builtin | `(bool? 1)`<br/>`(bool? #f)`         |                                             `#f`<br/>`#t` |
| display                  |      `display`     | builtin | `(display 1)`                        |                                              displays `1` |
| factorial                |     `factorial`    | udf     | `(factorial 4)`                      |                                                      `10` |