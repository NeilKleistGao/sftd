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
notationsList = ['[', ']', '(', ')', '>', '<', '~', '-', '@', '#', '+', '*', '/', '\\',
                 '=', '!', '|', '&', '$', ':', '"', '.', '?', '!', '%']

ignoredList :: [Char]
ignoredList = ['\n', '\r', '\t']
