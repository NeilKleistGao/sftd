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
- Default language code: 4 Bytes
- Symbol table Size: 4Bytes
- Symbol table
- The number of strings(not in dialogues): 4Bytes
- Strings list
- The number of auto dialogues: 4 Bytes
- Auto dialogues start addresses
- The number of trigger dialogues: 4 Bytes
- Trigger dialogues start addresses
- The number of interact dialogues: 4 Bytes
- Interact dialogues start addresses
- The number of other dialogues: 4 Bytes
- Other dialogues start addresses
- Dialogues: an int(4 Bytes) for the number of commands and the commands blocks


## Commands
### Format
`OperatorID` + [`OperationParameters`]
`OperatorID`: 4 Byte

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
- Time (for Operator 5)

### Jump
Call or jump to another part of dialogues.
OperatorID: 6(for call), 7(for jump)
Parameters:
- Target part's index

### Options
Provide options for players.
OperatorID: 8
Parameters:
- String's ID
- Next Address

### Get Data From Games
Get variables/characters from the game
OperatorID: 9(variables), 10(characters)
Parameters:
- Data's name

### Set Data From Games
Get variables/characters from the game
OperatorID: 11
Parameters:
- Data's name
- Data's value

### Play Animation
Play animation.
OperatorID: 12
Parameters:
- Target's name
- Animation's name

### Play Sound
Play sound effect.
OperatorID: 13
Parameters:
- Sound's name

### Calculations
Calculate.
OperatorID: 14(+), 15(-), 16(*), 17(/), 18(%), 19(=), 20(and), 21(or), 22(not), 23(-neg)
Parameters:
- Target variable
- First parameter
- Second parameter(except 19, 22, 23)

### Conditions
Jump by the given condition.
OperatorID: 24
Parameters:
- Condition's value
- True address
- False address

### Move
Move a character to another place.
OperatorID: 25, 26
Parameters:
- Character ID
- Position X
- Position Y
- Time(for Operator 26)

### Delay
Wait for seconds.
OperatorID: 27
Parameters:
- Time(in seconds)

### End
End a dialogue.
OperatorID: 28
Parameters: null