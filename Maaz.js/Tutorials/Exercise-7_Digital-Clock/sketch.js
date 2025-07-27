console.log("Hey there, this's a clock!");

const time = document.getElementById("time");
const meridian = document.getElementById("meridian");

function resetTimer() {
    const d = new Date();

    let [hours, minutes, seconds] = [d.getHours(), d.getMinutes(), d.getSeconds()];


    if (hours > 12) {
        hours -= 12;
        meridian.innerHTML = "P.M.";
    } else {
        meridian.innerHTML = "A.M.";
    }

    hours = (hours == 12 ? 0 : hours);

    hours = (hours > 9 ? hours : `0${hours}`);
    minutes = (minutes > 9 ? minutes : `0${minutes}`);
    seconds = (seconds > 9 ? seconds : `0${seconds}`);

    time.innerHTML = `${hours} : ${minutes} : ${seconds}`;
}
resetTimer();

setInterval(resetTimer, 1000);