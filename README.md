# sftd
[![Tests CI](https://github.com/NeilKleistGao/sftd/actions/workflows/tests.yml/badge.svg?branch=main)](https://github.com/NeilKleistGao/sftd/actions/workflows/tests.yml)

<del>Sympathy for the Devil</del>Script for the Director is a script language allowing
you to create stories in 2D RPG games.

Game Engine Support:
- [ ] Unity3D
- [ ] Godot
- [ ] ...

## Some Demos
### Create Character Information
```
Character "kusanagi motoko" {
    default {
        avatar: "motoko.png"
        font: default
        sound: null
    }
    
    State happy {
        // avatar: "motoko_happy.png"
        // font: default
        // sound: null
    }
    State angry {
        avatar: "motoko_angry_pic.png"
        // font: default
        // sound: null
    }
}
```

### Create Effects
```
Effect HighlightEffect on Text {
    color: "#FF0000"
    bold: true
}
```

### Dialogue
```
#en-GB

part intro {
    [tachikoma]: "tachikoma desu!"
    [kusanagi motoko]: "Hello! tachikoma number{$id}"
}

part ask {
    [kusanagi motoko]: {
        "Hello."
        @Motoko move_to (1, 2) in 2s
        "What's your <use HighlightEffect>name<end> ?"
    }
    
    select {
        "tachikoma": {
            goto intro
        }
        "go away": {}
    }
    
    [kusanagi motoko(confused)]: {
        "Alright."
    }
    
    if ($left) {
        @Motoko move_to (0, 3) in 2s
    }
    else {
        @Motoko move_to (2, 3) in 2s
    }
}

event Greet {
    insert ask
}
```