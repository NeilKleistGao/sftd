module Main where

import Test.QuickCheck (quickCheck)
import RW.Reader
import RW.Writer

testCases :: [String]
testCases = ["empty"]

getFullInputPath :: String -> String
getFullInputPath caseName = "test/ds/" ++ caseName ++ ".ds"

getFullOutputPath :: String -> String
getFullOutputPath caseName = "test/dd/" ++ caseName ++ ".dd"

doTest :: [String] -> IO ()
doTest [] = putStrLn("Tests End.")
doTest (x: xs) = do
  let script = readDialogueScript(getFullInputPath(x))
  writeDialogueData (getFullOutputPath(x)) ""
  doTest xs

main :: IO ()
main = do
  putStrLn("Tests Start.")
  doTest testCases
