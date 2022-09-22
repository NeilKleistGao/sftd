module RW.Reader(
  readDialogueScript
) where

import System.IO

readContent :: Handle -> IO String
readContent handle = do
  isEof <- hIsEOF(handle)
  if not isEof
  then do
    current <- hGetLine(handle)
    others <- readContent(handle)
    return $ current ++ "\n" ++ others
  else return []

readDialogueScript :: FilePath -> IO String
readDialogueScript filename = do
  handle <- openFile filename ReadMode
  isOk <- hIsOpen(handle)
  if isOk
  then do
    content <- readContent(handle)
    hClose(handle)
    return content
  else return []
