module Lexer.Tokens(
  Token, keywordsList, notationsList
) where

data Token = IntLit Int -- e.g. 42
           | FloatLit Float -- e.g. 3.14
           | StringLit String -- e.g. "to be or not to be"
           | BoolLit Int -- e.g. false
           | Variable String -- e.g. foo
           | Notation Char -- e.g. *
           | Keyword String -- e.g. if
           | Blank -- e.g. \n

keywordsList :: [String]
keywordsList = ["if", "else", "default", "where", "in"]

notationsList :: [Char]
notationsList = ['[', ']', '(', ')', '>', '<', '~', '-', '@', '#', '+', '*', '/', '\\',
                 '=', '!', '|', '&', '$', ':', '"', '.', '?', '!', '%']
