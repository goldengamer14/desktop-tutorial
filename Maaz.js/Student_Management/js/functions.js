function enrollStudent(name, roll, subject, myArr) {

    return new Promise((resolve, reject) => {
        var chars = "abcdefghijklmnopqrstuvwxyz";
        chars = chars.split("");

        var nums = "1234567890";
        nums = nums.split("");

        var nameAlert = "";
        var rollAlert = "";
        var subjectAlert = "";

        var validName = false;
        var validRollNo = false;
        var validSubject = false;

        for (var c of chars) {

            if (String(name).includes(c) && String(name).length > 2) {
                validName = true;
                nameAlert = "";
            }

            if (subject.includes(c) && subject.length > 2) {
                validSubject = true;
                subjectAlert = "";
            }

        }

        if (validSubject !== true) {
            subjectAlert = `Please Mention the Subject of the Student\n`;
        }

        if (validName !== true) {
            nameAlert = `Please Mention the Name of the Student\n`;
        }

        for (var n of nums) {
            if (roll.includes(n)) {
                validRollNo = true;
                rollAlert = "";
            }
        }

        if (validRollNo !== true) {
            rollAlert = `Please Mention the Roll No. of the Student\n`;
        }

        setTimeout(function () {
            if (validName === true && validSubject === true && validRollNo === true) {

                let obj = { name: name, roll: roll, subject: subject };
                myArr.push(JSON.stringify(obj));
                window.localStorage.setItem("Students", JSON.stringify(studentsArray));

                resolve();

                // trying to Save the Object it using XMLHttpRequest.

                // let xhr = new XMLHttpRequest();
                // xhr.open("POST", "http://127.0.0.1:5500/Student_Management/students.txt", true);
                // xhr.getResponseHeader("Content-type", "application/json");

                // xhr.onload = function () {
                //     window.console.log(this.response);
                // }
                // const param =
                // {
                //     "name": name,
                //     "roll": roll,
                //     "subject": subject
                // }
                //     ;
                // xhr.send(param);

            } else {
                reject(() => {window.alert(nameAlert + rollAlert + subjectAlert); window.console.error(nameAlert + rollAlert + subjectAlert);});
            }
        }, 500);
    });
}

function showStudents() {

    var enrollShow = document.getElementById("enrollShow");
    enrollShow.innerHTML = `<thead>
                            <tr>
                                <th>Name</th>
                                <th>Roll No.</th>
                                <th>Subject</th>
                            </tr>
                            </thead>
                            <br>
                            <tbody id="enrollShowBody">
                            
                            </tbody>`;

    for (var obj of studentsArray) {

        let e = JSON.parse(obj);

        var inside = `
                <td>${e.name}</td>
                <td>${e.roll}</td>
                <td>${e.subject}</td>
            <br>
            `;
        var enrollShowBody = document.getElementById("enrollShowBody");
        enrollShowBody.innerHTML += inside;

    }

}