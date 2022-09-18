module RW.Writer(
  writeDialogueData
) where

import System.IO

writeDialogueData :: String -> String -> IO ()
writeDialogueData filename content = do
  handle <- openFile filename WriteMode
  isOk <- hIsOpen(handle)
  if isOk
  then do
    hPutStr handle ("sftd" ++ content)
    hClose(handle)
  else putStrLn("can't write file: " ++ filename)
