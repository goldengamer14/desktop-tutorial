console.log("This sketch.js file is included");

let alarm = new Audio("https://interactive-examples.mdn.mozilla.net/media/cc0-audio/t-rex-roar.mp3");

function play() {
    alarm.play();
}

let Year = document.getElementById("Year");
let Month = document.getElementById("Month");
let date = document.getElementById("Date");

let Hours = document.getElementById("Hours");
let Minutes = document.getElementById("Minutes");
let Seconds = document.getElementById("Seconds");

let alertDiv = document.getElementById("Alert");

document.getElementById("setButton").onclick = () => {
    let presentDate = new Date();

    Year.value == "" ? Year.value = presentDate.getFullYear() : null;
    Month.value == "" ? Month.value = presentDate.getMonth() : null;
    date.value == "" ? date.value = presentDate.getDate() : null;

    Hours.value == "" ? Hours.value = presentDate.getHours() : null;
    Minutes.value == "" ? Minutes.value = presentDate.getMinutes() : null;
    Seconds.value == "" ? Seconds.value = presentDate.getSeconds() : null;

    let theDate = `${Number(Month.value) + 1} ${date.value} ${Year.value} ` +
        `${Hours.value}:${Minutes.value}:${Seconds.value}`;

    let setDate = new Date(theDate);

    console.log("Date", setDate);
    console.log(theDate);

    if (setDate == "Invalid Date") Alert.innerHTML = "Please give a Valid Date";

    let interval = Math.ceil((setDate - presentDate) / 1000);
    console.log("Alarm will play after", interval, "seconds");

    setTimeout(play, interval * 1000);
}

let dateShower = document.getElementById("DateShower");
let timeShower = document.getElementById("TimeShower");

setInterval(() => {
    let date = new Date();
    dateShower.innerHTML = `${date.getDate()}:${date.getMonth()}:${date.getFullYear()}`;
    timeShower.innerHTML = `${date.getHours()}:${date.getMinutes()}:${date.getSeconds()}`;
}, 1000);

// let interval;
// let timeTable = [];

// let setHours;
// let setMinutes;
// let setSeconds;
// let hours;
// let minutes;
// let seconds;

// document.getElementById("setButton").onclick = () => {
//     interval ? clearInterval(interval) : null;

//     setHours = (Hours.value != "" ? Hours.value : new Date().getHours());
//     setMinutes = (Minutes.value != "" ? Minutes.value : new Date.getMinutes());
//     setSeconds = (Seconds.value != "" ? Seconds.value : 0);

//     timeTable.push({
//         Hours: setHours, Minutes: setMinutes, Seconds: setSeconds
//     });

//     interval = setInterval(() => {
//         hours = new Date().getHours();
//         minutes = new Date().getMinutes();
//         seconds = new Date().getSeconds();

//         // hours == setHours && minutes == setMinutes
//         //     && seconds == setSeconds ? play() : null;
//     }, 1000);

//     let hoursArr = [];
//     let minutesArr = [];
//     let secondsArr = [];

//     let fun = (arr, time) => {
//         let newArr = [];
//         let returnArr = [];

//         // arr.forEach(e => {
//         //     newArr.push(Number.parseFloat(e[time]));
//         // });

//         newArr = arr.map(e => Number.parseFloat(e[time]));

//         for (let i = 0; i < newArr.length + 1; i++) {
//             console.log("too old", newArr, time);
//             let value = Math.min(...newArr);
//             console.log(Math.min(...newArr));
//             // let index = newArr.indexOf(value);

//             console.log("old", newArr, value, time);
//             newArr.shift();
//             returnArr.push(value);
//             // ar.splice(index, 1);

//             console.log("new", newArr, time, i, newArr.length);
//             console.log("ret", returnArr, time);
//         };

//         return returnArr;
//     }

//     let finalArr = [];
//     let arr = timeTable.map(e => e);
//     hoursArr = fun(timeTable, "Hours");

//     console.log("Arr", arr, ...timeTable);
//     for (let i = 0; i < arr.length + 1; i++) {
//         if (i < arr.length) {
//             if (arr[i].Hours == hoursArr[0]) {
//                 hoursArr.shift();
//                 finalArr.push(arr[i]);
//                 arr.splice(i, 1);
//                 i = -1;
//                 continue;
//             }
//         }
//     }

//     console.log("Final", finalArr);

//     // minutesArr = fun(timeTable, "Minutes");
//     // secondsArr = fun(timeTable, "Seconds");

//     timeTable = finalArr;
//     console.log(hoursArr, minutesArr, secondsArr);

//     // I've to watch the solution from codewithharry youtube video
//     // for this exercise
// }