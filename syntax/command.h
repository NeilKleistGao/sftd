/**
* @copybrief
* MIT License
* Copyright (c) 2022 NeilKleistGao
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
 */

/// @file command.h

#ifndef SFTD_COMMAND_H
#define SFTD_COMMAND_H

#include <vector>

enum class CommandType {
  EMPTY = 0,
  START_DIALOGUE = 1,
  SET_SPEAKER,
  SET_SPEAKER_WITH_STATE,
  TALK,
  TALK_IN_TIME,
  CALL,
  JUMP,
  OPTION,
  GET_DATA,
  SET_DATA,
  PLAY_ANIMATION,
  PLAY_SOUND,
  ADD,
  SUB,
  MUL,
  DIV,
  MOD,
  EQUAL,
  AND,
  OR,
  NOT,
  NEG,
  CONDITIONAL_JUMP,
  MOVE,
  DELAY,
  PUBLISH,
  END_DIALOGUE
};

struct ILCommand {
  CommandType type{};
  std::vector<int> parameters;

  ILCommand() = default;
  explicit ILCommand(CommandType p_type) : type(p_type) {}
};

#endif // SFTD_COMMAND_H