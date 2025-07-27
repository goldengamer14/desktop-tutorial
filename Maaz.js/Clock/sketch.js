let hour = document.getElementById("Hour");
let minute = document.getElementById("Minute");
let second = document.getElementById("Second");

setInterval(() => {
    let time = new Date();

    let htime = time.getHours();
    let mtime = time.getMinutes();
    let stime = time.getSeconds();

    let hrotation = htime / 12 * 360;
    let mrotation = mtime / 60 * 360;
    let srotation = stime / 60 * 360;

    hour.style.transform = `rotate(${hrotation + mtime * 360 / 12 / 60}deg)`;
    // 60 * mtime = 360 / 12
    minute.style.transform = `rotate(${mrotation + stime * 360 / 60 / 60}deg)`;
    // 60 * stime = 360 / 60
    second.style.transform = `rotate(${srotation}deg)`;
}, 1000);