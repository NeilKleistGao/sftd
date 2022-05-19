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
Character Motoko("kusanagi motoko") {
    default {
        avatar = "motoko.png"
        font = default
        sound = null
    }
    
    State happy {}
    State angry {
        avatar = "motoko_angry_pic.png"
    }
}
```

### Create Effects
```
Effect HighlightEffect : Text {
    color = "#FF0000"
    bold = true
}
```

### Dialogue
```
#en-GB
import chara.Motoko
import chara.Tachikoma
import effect.HighlightEffect

func add() {
    << "You are the " << $num << noflush
    if ($num > 1) {
        << "tachikomas!"
    } 
    else {
        << "tachikoma!"
    }
    
    $num = $num + 1
    return "next."
}

proc count() {
    [Motoko]
    << "Hello."
    MoveTo(@Motoko, (1, 2))
    while ($num < 10) {
        << "What's your " << HighlightEffect("name") << "?"
        select {
            case "tachikoma": {
                << add()
            }
            case "tachikoma": {
                << add()
            }
            case "tachikoma": {
                << add()
            }
        }
    }
    
    group {
        @Tachikoma1 << "Bye!"
        Anim(@Tachikoma1, "bye")
        @Tachikoma2 << "Bye!"
        Anim(@Tachikoma2, "bye")
        @Tachikoma3 << "Bye!"
        Anim(@Tachikoma3, "bye")
    }
}

Event Greet {
    goto count
}
```