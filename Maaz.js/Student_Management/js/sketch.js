var studentsArray = [];
if (window.localStorage.getItem("Students")) {
    studentsArray = JSON.parse(window.localStorage.getItem("Students"));
}

var nameTaker = document.getElementsByClassName("nameTaker")[0];
var rollNoTaker = document.getElementsByClassName("rollNoTaker")[0];
var subjectTaker = document.getElementsByClassName("subjectTaker")[0];

var name = "";
var roll = "";
var subject = "";

nameTaker.oninput = function () { name = nameTaker.value; }
rollNoTaker.oninput = function () { roll = rollNoTaker.value; }
subjectTaker.oninput = function () { subject = subjectTaker.value; }

document.getElementById("enroller").onclick = function () {
    enrollStudent(name, roll, subject, studentsArray).then(() => { showStudents(); window.console.log("Successfully Enrolled the student") }).catch(
        (err) => { err(); }
    );
    name = ""; roll = ""; subject = "";
    nameTaker.value = ""; rollNoTaker.value = ""; subjectTaker.value = "";
}

document.getElementById("enrollShower").onclick = function () {
    showStudents();
}

document.getElementById("enrollHider").onclick = function () {
    document.getElementById("enrollShow").innerHTML = "";
}