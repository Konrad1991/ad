install.packages("ast2ast", repos = NULL, type = 'source')
.rs.restartR()


f <- function(a, b) {
  c <- a + (b-a)^2
}

fcpp <- ast2ast::translate(f)
fcpp(1, 1)



