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
### Character Binary File
- Magic number: 2 Bytes
- Character Name
- Default Properties Block(Avatar, Sound, Font)
- The number of states: 4 Bytes
- States list(Optional Overload/Default conversion rules)

### Dialogue Binary File
- Magic number: 2 Bytes
- Default language code: 4 Bytes
- The number op dependent files: 4 Bytes
- Dependent files list
- The number of events: 4 Bytes
- Events: an int(4 Bytes) for the number of commands and the commands blocks

### Effect Binary File
- Magic number: 2 Bytes
- The number of effects: 4 Bytes
- Effects list

## Commands
TODO: