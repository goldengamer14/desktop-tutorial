function promises() {
    this.promise = new Promise(function (done, notDone) {
        let error = true;

        if (error != true) {
            done("Now Give me my chocolate.");
        } else {
            notDone("Sorry! Something went wrong. Take your chocolate back.");
        }
    });

    return this.promise;
}

promises().then((fix) => window.console.log("Promise Completed? " + fix)).catch((err) =>
    window.console.log("Promise not done? " + err));

// I'vs used Promise in Student_Management/function.js/enrollStudent/The last if statement
// And Student_Management/sketch.js/document.getElementById("enroller").onclick/enrollStudent