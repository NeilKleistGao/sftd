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
character "kusanagi motoko" {
    default {
        avatar: "motoko.png"
        font: default
        sound: null
    }
    
    state happy {
        // avatar: "motoko_happy.png"
        // font: default
        // sound: null
    }
    state angry {
        avatar: "motoko_angry_pic.png"
        // font: default
        // sound: null
    }
}
```

### Create Effects
```
effect HighlightEffect on Text {
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
        @Motoko moveto (1, 2) in 2s
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
        @Motoko moveto (0, 3) in 2s
    }
    else {
        @Motoko moveto (2, 3) in 2s
    }
}

event Greet {
    insert ask
}
```