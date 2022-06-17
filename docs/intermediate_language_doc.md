# Intermediate Language Document
## Requirements
- Show dialogues on the screen
- Show avatars, names of characters
- Play sound effects during dialogues
- Add text effects
- <del>Show several dialogues at the same time</del>
- Play animations of characters
- Move characters to another place
- <del>Move the camera</del>
- <del>Add post process effects</del>
- Store or load variables in memory
- I18N support

## Binary Structure
suffix: `.dialogue` -> `.dd` (Dialogue Data)
- Magic number: 2 Bytes
- Enable I18N: 2 Bytes
- Symbol table Size: 4 Bytes
- Symbol table
- The number of strings: 4 Bytes
- Strings list
- The number of auto dialogues: 4 Bytes
  Dialogues: an int(4 Bytes) for the number of commands and the commands blocks
- The number of trigger dialogues: 4 Bytes
  Dialogues: an int(4 Bytes) for the number of commands and the commands blocks
- The number of interact dialogues: 4 Bytes
  Dialogues: an int(4 Bytes) for the number of commands and the commands blocks
- The number of other dialogues: 4 Bytes
- Dialogues: an int(4 Bytes) for the number of commands and the commands blocks


## Commands
### Format
`OperatorID` + [`OperationParameters`]
`OperatorID`: 4 Bytes

### Start Dialogues
Start a dialogue.

OperatorID: 1

Parameters: null

### Set Speakers
Set the speakers' data.

OperatorID: 2, 3

Parameters:
- Speaker's name
- Speaker's state (for Operator 3)

### Talk
Set the dialogue content.

OperatorID: 4, 5

Parameters:
- String's ID
- Time in float (for Operator 5)

### Jump
Call or jump to another part of dialogues.

OperatorID: 6(for call), 7(for jump)

Parameters:
- Target address

### Options
Provide options for players.

OperatorID: 8

Parameters:
- String's ID
- Next Address

### Play Animation
Play animation.

OperatorID: 9

Parameters:
- Target's name
- Type(0 -> in memory, 1 -> string)
- Animation's name

### Play Sound
Play sound effect.

OperatorID: 10

Parameters:
- Type(0 -> in memory, 1 -> string)
- Sound's name

### Move
Move a character to another place.

OperatorID: 11

Parameters:
- Character ID
- Position X
- Position Y
- Time(for Operator 26)

### Conditions
Jump by the given condition.

OperatorID: 12

Parameters:
- Condition value type
- Condition value
- False address

### Calculations
Calculate.

OperatorID: 13(+), 14(-), 15(*), 16(/), 17(%), 18(=), 29(and), 20(or), 21(not), 22(-neg), 23(<), 24(>), 25(<=), 26(>=)

Parameters:
- Target variable
- First Expression type(0 -> in memory, 1 -> string, 2 -> int, 3 -> float, 4 -> bool)
- First parameter
- Second Expression type(0 -> in memory, 1 -> string, 2 -> int, 3 -> float, 4 -> bool)
- Second parameter(except 19, 22, 23)

### Delay
Wait for seconds.

OperatorID: 27

Parameters:
- Time(in seconds)


### Publish
Publish an event.

OperatorID: 28

Parameters:
- Event name
- Parameter count
- parameter types(0 -> in memory, 1 -> string, 2 -> int, 3 -> float, 4 -> bool)
- parameters

### Set data
Set variable in memory

OperatorID: 29

Parameters:
- Target Address
- Type
- Value

### End
End a dialogue.

OperatorID: 255

Parameters: null