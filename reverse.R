walk <- function(expr) {
  if(!is.call(expr)) {
    return(expr)
  }
  cat("\n\n\n")
  lapply(expr, walk)
}

expr <- quote(x^2 + sin(x^2))
walk(expr)
