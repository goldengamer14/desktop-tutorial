console.log("Welcome to the CV Screener");

// for (let i = 0; i < 7; i++) {
//     fetch("https://randomuser.me/api/").then(r => r.json()).then(d => data.push(d.results));
// }

data = [
    {
        name: "Zayn Saifi",
        gender: "male",
        language: "Python",
        experience: 5,
        image: "https://randomuser.me/api/portraits/men/6.jpg"
    },

    {
        name: "Mahira Khan",
        gender: "female",
        language: "Javascript",
        experience: 0,
        image: "https://randomuser.me/api/portraits/women/85.jpg"
    },

    {
        name: "Raju Sotewala",
        gender: "male",
        language: "Ruby",
        experience: 2,
        image: "https://randomuser.me/api/portraits/men/35.jpg"
    },

    {
        name: "Kamin Rajput",
        gender: "female",
        language: "C++",
        experience: 10,
        image: "https://randomuser.me/api/portraits/women/45.jpg"
    }
];

console.log(data);

// Profile Iterator
function profileIterator(profiles) {
    this.index = -1;

    return {
        next: () => {
            this.index++;
            return (this.index < profiles.length && this.index >= 0 ? { value: profiles[this.index], done: false } : { value: undefined, done: true })
        },
        previous: () => {
            this.index -= 1;
            return (this.index >= 0 && this.index < profiles.length ? { value: profiles[this.index], done: false } : { value: undefined, done: true })
        },
        restart: () => this.index = -1,
        end: () => this.index = profiles.length
    };
}

let candidates = profileIterator(data);
nextCV("next");

// Button listener
document.getElementById("next").onclick = () => nextCV("next");
document.getElementById("previous").onclick = () => nextCV("previous");

function nextCV(direction) {
    let candidate = candidates[direction]();

    if (candidate.done) {
        console.log("could'nt be done");

        direction == "next" ? candidates.restart() : candidates.end();

        candidate = candidates[direction]();
        console.log(direction);
    }

    let img = document.getElementById("img");
    let profile = document.getElementById("profile");

    console.log(candidate);

    img.innerHTML = `<img src="${candidate.value.image}" style="width: 10vw; height: 10vw">`;

    profile.innerHTML = `<span id="name" class="data">name: ${candidate.value.name}</span> <span id="gender" class="data">gender: ${candidate.value.gender}</span> <br>`
        + `<span id="exp" class="data">experience: ${candidate.value.experience}</span> <span id="lang" class="data">language: ${candidate.value.language}</span>`;
}