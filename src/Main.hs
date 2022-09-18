module Main where

import RW.Reader

main :: IO ()
main = do
  filename <- getLine
  content <- readDialogueScript(filename)
  putStrLn("Compiled.")
