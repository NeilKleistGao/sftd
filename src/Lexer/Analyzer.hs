module Lexer.Analyzer(
  analyzeTokens
) where

import Lexer.Tokens

isDigit :: Char -> Bool
isDigit c = c `elem` "0123456789"

isAlphabet :: Char -> Bool
isAlphabet c = c `elem` "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

type TokenCC = (Token, String)

takeRest :: String -> Int -> String
takeRest str newStart = snd(splitAt newStart str)

parseNumber :: String -> TokenCC
parseNumber str = do
  let numStr = takeWhile (\c -> isDigit(c) || c == '.') str
  let dots = length ([c | c <- numStr, c == '.'])
  if (dots > 1) then (Error("invalid number literal"), takeRest str (length numStr))
  else if (dots == 1) then (FloatLit (read numStr :: Float), takeRest str (length numStr))
  else (IntLit (read numStr :: Int), takeRest str (length numStr))

findStringEnd :: String -> Int -> Int
findStringEnd s i
  | (s !! i == '"' && not (s !! (i - 1) == '\\')) = i
  | (i + 1 == length s || s !! i == '\n') = -1
  | True = findStringEnd s (i + 1)

parseString :: String -> TokenCC
parseString str = do
  let end = findStringEnd str 0
  if (end == -1) then (Error "expect end of string", takeRest str (end + 1))
  else (StringLit (take end str), takeRest str (end + 1))

parseComment :: String -> TokenCC
parseComment str = (Empty, dropWhile (/= '\n') str)

parseVariableAndKeyword :: String -> TokenCC
parseVariableAndKeyword str = do
  let name = takeWhile (\c -> isDigit c || isAlphabet c || c == '_') str
  if (name `elem` keywordsList) then (Keyword name, takeRest str ((length name) + 1))
  else (Variable name, takeRest str ((length name) + 1))

getNextToken :: String -> TokenCC
getNextToken str
  | isDigit (str !! 0) = parseNumber(str)
  | (str !! 0) == '"' = parseString(takeRest str 1)
  | (str !! 0) == ' ' = (Indent, takeRest str 1)
  | (str !! 0) `elem` notationsList = (Notation(str !! 0), takeRest str 1)
  | ((take 2 str) == "//") = parseComment(str)
  | (str !! 0) `elem` ignoredList = (Empty, takeRest str 1)
  | True = parseVariableAndKeyword(str)

getTokens :: String -> [Token] -> [Token]
getTokens "" tokens = tokens
getTokens str tokens = do
  let cc = getNextToken str
  case (fst cc) of Empty -> getTokens (snd cc) tokens
                   _     -> getTokens (snd cc) tokens ++ [(fst cc)]

analyzeTokens :: String -> [Token]
analyzeTokens str = getTokens str []
  