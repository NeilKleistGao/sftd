module Main where

import Test.QuickCheck (quickCheck)
import RW.Reader
import RW.Writer
import Compiler

testCases :: [String]
testCases = ["empty", "demo"]

getFullInputPath :: String -> String
getFullInputPath caseName = "test/ds/" ++ caseName ++ ".ds"

getFullOutputPath :: String -> String
getFullOutputPath caseName = "test/dd/" ++ caseName ++ ".dd"

doTest :: [String] -> IO ()
doTest [] = putStrLn("Tests End.")
doTest (x: xs) = do
  script <- readDialogueScript(getFullInputPath(x))
  writeDialogueData (getFullOutputPath(x)) (compile script)
  doTest xs

main :: IO ()
main = do
  putStrLn("Tests Start.")
  doTest testCases
