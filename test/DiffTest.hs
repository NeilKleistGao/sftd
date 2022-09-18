module Main where

import RW.Reader
import RW.Writer

testCases :: [String]
testCases = ["empty"]

getFullInputPath :: String -> String
getFullInputPath caseName = "test/ds/" ++ caseName ++ ".ds"

getFullOutputPath :: String -> String
getFullOutputPath caseName = "test/dd/" ++ caseName ++ ".dd"

doTest :: [String] -> IO () -> ()
doTest [] _ = ()
doTest (x: xs) _ = do
  let script = readDialogueScript(getFullInputPath(x))
  doTest xs (writeDialogueData (getFullOutputPath(x)) "")

main :: IO ()
main = do
  let _ = doTest testCases (putStrLn("Tests Start."))
  putStrLn("Tests End.")
