# sftd
[![Tests CI](https://github.com/NeilKleistGao/sftd/actions/workflows/tests.yml/badge.svg?branch=main)](https://github.com/NeilKleistGao/sftd/actions/workflows/tests.yml)

<del>Sympathy for the Devil</del>Script for the Director is a script language allowing
you to create stories in 2D RPG games.

Game Engine Support:
- [ ] Unity3D
- [ ] Godot
- [ ] ...

## A Simple Demo
```
#{"en-GB", "zh-CN"}

dialogue GetCookie {
    [Alice]: {
        "Give me some cookies please."
        "I'm not hungry now."
    }
    
    (NewItem, "cookie")
}

dialogue GetCake {
    [Alice]: {
        "Give me some cake please."
        "It looks great." in 2
    }
    
    (NewItem, "cake")
}

interact dialogue Talk1 {
    [Bob(happy)]: {
        sound "hint"
        "Hello!"
        "My name is Bob!"
        "Nice to meet you!"
    }
    [Alice]: {
        "I'm Alice."
        "Nice to meet you too!"
    }
    
    $first = false
}

interact dialogue Talk2 when (not $first) {
    [Bob]: {
        "It is {{$time}}. Do you want some food?"
        select {
            "cookie": use GetCookie
            "cake": use GetCake
        }
    }
    
    [Bob]: "OK."
    [Alice]: "Thnak you!"
    anime @alice "thank"
}

auto dialogue Idle {
    move @bob (2, 0) in 4
    delay 3
    move @bob (-2, 0) in 4
}
```