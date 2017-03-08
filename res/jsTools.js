
function func() {
    console.log("this is a js function!")
    composeMsg()
}

//generate a msg
function composeMsg(){
    var freq;
    freq = Math.floor(Math.random()*10+1);
    console.log(freq)
}

//generate "518" "490" "486" randomly
function randomType(){
    var freq;
    switch (Math.floor(Math.random()*3+1)){
    case 1: freq = "518";break;
        case 2: freq = "490";break;
            case 3: freq = "486";break;
    }
    return freq
}
