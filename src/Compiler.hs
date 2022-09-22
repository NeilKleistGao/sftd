module Compiler(
  compile
) where

import Lexer.Analyzer

compile :: String -> String
compile src = do
  let tokens = analyzeTokens src
  foldl (\res tk -> show (tk) ++ "\n" ++ res) ("") (tokens) -- TODO
