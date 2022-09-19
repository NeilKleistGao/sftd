# sftd
[![Tests CI](https://github.com/NeilKleistGao/sftd/actions/workflows/tests.yml/badge.svg?branch=main)](https://github.com/NeilKleistGao/sftd/actions/workflows/tests.yml)

<del>Sympathy for the Devil</del>Script for the Director is a script language allowing
you to create stories in 2D RPG games.

## Demo
```js
// this is a comment
// show a dialogue like this:
--- ANewPart // indicate the starting of a part
[CharacterName]: "hello"
[AnotherCharacter]:
  "I can say many things at the same time"
  "just like this! just use indents"
  (*gladly) "you can indicate an emotion for the sentences below(include this)"
  (*default) "or reset it like this"
  "you can also inditate the speed i say(in 1.5 seconds)" in 1.5
[]:
  "when you don't need a speaking character, just leave it blank"
  "create selections like this:"
  ? "play music" > MusicPart
  ? "play sound effect" > EffectPart
  ? "play animation" > AnimationPart
  ? "modify backpack" > BackpackPart
  ? "modify property" > PropertyPart
  ? "if/else" > IfElsePart
  ? "wait" > WaitPart
  ? "do nothing"
  "that how it works!"
--- // indicate the end of a part

--- MusicPart
[]:
  "update bgm like this:"
  ~ BGMVariableInDatabase
  "or stop it:"
  ~ !
  "or pause it:"
  ~ ||
  "or resume it:"
  ~ |>
---

--- EffectPart
[]:
  "play effect like this:"
  - EffectVariableInDatabase
  "it will be played once"
---

--- AnimationPart
[]:
  "you need play animations on a given game object by using \"@\":"
  @GameObjectName.\jump/
  "you need to use an animation state machine. \"jump\" is a name of some state"
---

--- BackpackPart
[]:
  "you can get the number of some object in some game object's backpack:"
  @GameObjectName.#SomeObjectName
  "this is an integer value. you can modify it:"
  @GameObjectName.#SomeObjectName += 1
  "do some complex calculation:"
  @Player.#Obj1 = a + b - c * d where
  a = @Player.#Obj2,
  b = @Player.#Obj3,
  c = @Player.#Obj4,
  d = @Player.#Obj5
---

--- PropertyPart
[]:
  "property is similar to backpack object:"
  @GameObjectName.SomeProperty = false
  "\"$\" is specified for currency, and it can only be int:"
  @GameObjectName.$ -= 10
---

--- IfElsePart
[]:
  "access a variable like this:"
  &VariableName

if (&VariableName > 10) {
  // do something
  // cannot speak here. you need to jump to another part
  > AnotherPart
}
else if (&VariableName < 0) {
  // do something
}
else {
 // do something
}
---

--- WaitPart
[]:
  "you can do nothing but wait for seconds:"
  ... in 1
---
```
