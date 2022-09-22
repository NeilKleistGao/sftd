module Lexer.Tokens(
  Token(..), keywordsList, notationsList, ignoredList
) where

data Token = IntLit Int -- e.g. 42
           | FloatLit Float -- e.g. 3.14
           | StringLit String -- e.g. "to be or not to be"
           | Variable String -- e.g. foo
           | Notation Char -- e.g. *
           | Keyword String -- e.g. if
           | Indent -- e.g. space
           | Empty -- e.g. comments
           | Error String

keywordsList :: [String]
keywordsList = ["if", "else", "default", "where", "in","true", "false"]

notationsList :: [Char]
notationsList = ['[', ']', '(', ')', '>', '<', '{', '}', '~', '-', '@', '#', '+', '*',
                 '/', '\\', '=', '!', '|', '&', '$', ':', '"', '.', '?', '!', '%', ',']

ignoredList :: [Char]
ignoredList = ['\n', '\r', '\t']

instance Show Token where
  show (IntLit i) = "(IntLit " ++ show i ++ ")"
  show (FloatLit f) = "(FloatLit " ++ show f ++ ")"
  show (StringLit s) = "(StringLit " ++ show s ++ ")"
  show (Variable name) = "(Variable " ++ show name ++ ")"
  show (Notation n) = "(Notation " ++ show n ++ ")"
  show (Keyword name) = "(Keyword " ++ show name ++ ")"
  show (Indent) = "(Indent)"
  show (Empty) = ""
  show (Error err) = "(Error " ++ show err ++ ")"
