# Intermediate Language Document
## Requirements
- Show dialogues on the screen
- Show avatars, names of characters
- Play sound effects during dialogues
- Add text effects
- Show several dialogues at the same time
- Play animations of characters
- Move characters to another place
- Move the camera
- Add post process effects
- Store or load variables in memory
- I18N support

## Binary Structure
### Dialogue Binary File
suffix: `.dialogue` -> `.dd` (Dialogue Data)
- Magic number: 2 Bytes
- Default language code: 4 Bytes
- Intern Symbol Table (Symbol Name, Address)
- Extern Symbol Table (Symbol Name, Filename)
- The number of parts: 4 Bytes
- Parts: an int(4 Bytes) for the number of commands and the commands blocks
- The number of events: 4 Bytes
- Events: an int(4 Bytes) for the number of commands and the commands blocks


## Commands
### Format
`OperatorID` + [`OperationParameters`]
`OperatorID`: 4 Byte

### Start Dialogues
Start a dialogue.
OperatorID: 1
Parameters: null

### Set Speakers
Set the speakers' data, including avatars, sounds and names.
OperatorID: 2, 3, 4
Parameters:
- Avatar String's Index (for Operator 2)
- Sounds String's Index (for Operator 3)
- Name String's Index (for Operator 4)

### Talk
Set the dialogue content.
OperatorID: 5, 6
Parameters:
- String's ID
- Time (for Operator 6)

### Jump
Call or jump to another part of dialogues.
OperatorID: 7(for call), 8(for jump)
Parameters:
- Target part's index

### Effect
Add or Remove an effect.
OperatorID: 9, 10
Parameters:
- Effect ID(for Operator 9)

### Options
Provide options for players.
OperatorID: 11
Parameters:
- String's ID
- Next Address

### Get Data From Games
Get variables/characters from the game
OperatorID: 12(variables), 13(characters)
Parameters:
- Data's name

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

### End
End a dialogue.
OperatorID: 27
Parameters: null