module Main where

import System.IO
import System.Environment(getArgs)

import RW.Reader
import RW.Writer

main :: IO ()
main = do
  args <- getArgs
  if length args  /= 2
  then fail "Usage: sftd inputFilename outputFilename"
  else do
    content <- readDialogueScript(args !! 0)
    writeDialogueData (args !! 1) ""
    putStrLn("Compiled.")
