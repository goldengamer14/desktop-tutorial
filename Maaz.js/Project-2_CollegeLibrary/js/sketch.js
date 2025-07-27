//This array will store all books
var BookArray = [];
if (window.localStorage.getItem("Books")) {
    BookArray = JSON.parse(window.localStorage.getItem("Books"));

    for (let i = 0; i < BookArray.length; i++) {
        document.getElementById("tablebody").innerHTML += `
        <tr>
            <td>${document.getElementById("tablebody").childElementCount}</td>
            <td class="BookNames">${BookArray[i].name}</td>
            <td>${BookArray[i].owner}</td>
            <td>${BookArray[i].type}</td>
        </tr>
        `;
    }
}

//Making the headings something so they change color on mouseover
Array.from(document.getElementsByClassName("topperChild")).forEach(function (e) {
    e.onmouseover = function () {
        e.style.color = "yellow";
    }
    e.onmouseleave = function () {
        e.style.color = "white";
    }
})

//Making only one radio checkbox of declaring type to be selected
Array.from(document.getElementsByClassName("typeCheck")).forEach(function (e, i, arr) {
    e.onclick = function () {
        if (e.checked == true) {
            for (let j = 0; j < arr.length; j++) {
                if (j != i) {
                    arr[j].checked = false;
                } else { continue; }
            }
        }
    }
})

//Constructor to find if a parameter to newBook is Not Given
class checkBook {

    constructor(name, notThis, returnWhat) {

        this.name = name;
        this.returnWhat = returnWhat;

        var letters = "abcdefghijklmnopqrstuvwxyz";
        letters = letters.split("");
        this.is = false;
        for (let i = 0; i < letters.length; i++) {
            if (this.name.includes(letters[i])) {
                this.is = true;
            }
        }

        if (this.name == notThis) {
            this.is = false;
        }

    }

    checkThis() {

        if (this.is == false) {
            return this.returnWhat;
        }

    }

}

//Adding new books in the list
class newBook {
    constructor(name, owner, type) {
        this.name = name;
        this.owner = owner;
        this.type = type;
        this.numberOfAlerts = {
            name: false,
            owner: false,
            type: false
        };
        BookArray.push(this);

        var that = [];
        that[0] = new checkBook(this.name, "Book", "Not Given");
        that[1] = that[0].checkThis();
        if (that[1] == that[0].returnWhat) {
            this.name = that[1];
            this.numberOfAlerts.name = true;
            // window.alert("Please give a name to this book");
        }

        that[0] = new checkBook(this.owner, "Owner's name", "Not Mentioned");
        that[1] = that[0].checkThis();
        if (that[1] == that[0].returnWhat) {
            this.owner = that[1];
            this.numberOfAlerts.owner = true;
            // window.alert("Please mention the name of the Owner of this book");
        }

    }

    display() {

        var nameAlert = "";
        var ownerAlert = "";
        var typeAlert = "";

        if (this.numberOfAlerts.name == true) {
            nameAlert = `Name Not Given: Please give a name to this book.

`;
        }
        if (this.numberOfAlerts.owner == true) {
            ownerAlert = `Owner Not Mentioned: Please mention the name of the Owner of this book.

`;
        }
        if (this.numberOfAlerts.type == true) {
            typeAlert = `Type Not Given: Please mention the type of this book.`;
        }

        if (this.numberOfAlerts.name == true || this.numberOfAlerts.owner == true
            || this.numberOfAlerts.type == true) {
            window.alert(nameAlert + ownerAlert + typeAlert);
        }

        //Displaying the Object on the Library table
        document.getElementById("tablebody").innerHTML += `
        <tr>
            <td>${document.getElementById("tablebody").childElementCount}</td>
            <td class="BookNames">${this.name}</td>
            <td>${this.owner}</td>
            <td>${this.type}</td>
        </tr>
        `;
    }
}

//This function will listen the button click
let btn = document.getElementById("BookAdder").onclick = function () {
    var Owner = document.getElementById("OwnerTaker").value;
    var Book = document.getElementById("BookTaker").value;
    var Type = undefined;

    var rightChecks = false;
    Array.from(document.getElementsByClassName("typeCheck")).forEach(function (e) {
        if (e.checked === true) {
            if (e.getAttribute("id") == "Custom") {


                let CustomLabel = document.getElementById("CustomLabel")
                e.value = String(CustomLabel.value).trim();

                if (String(e.value).length > 0) {
                    e.value = String(e.value)[0].toUpperCase() + String(e.value).substring(1, String(e.value).length);
                }
                if (e.value == "") {
                    e.value = "unknown";
                    alert("Please type something in the Custom Input");
                }

                CustomLabel.value = "";

            }
            Type = e.getAttribute("value");
            rightChecks = true;
        }
    })

    if (rightChecks == false) {
        Type = "unknown";
        // alert("Please mention the type of this book");
    }

    var typeCheck = document.getElementsByClassName("typeCheck")
    for (let i = 0; i < typeCheck.length; i++) {
        typeCheck[i].checked = false;
    }

    let g = new newBook(Book, Owner, Type);

    if (rightChecks == false) {
        g.numberOfAlerts.type = true;
    }

    g.display();

    document.getElementById("OwnerTaker").value = "";
    document.getElementById("BookTaker").value = "";

    window.localStorage.setItem("Books", JSON.stringify(BookArray));
}

document.getElementById("BookRemover").onclick = function () {
    var rem = document.getElementById("RemoveBookInput");
    var name = Array.from(document.getElementsByClassName("BookNames"));

    for (let i = 0; i < name.length; i++) {

        if (String(name[i].innerText).includes(rem.value)) {

            let a = window.confirm(`Are you sure you want to remove the Book ${name[i].innerText} From The Library?`);

            if (a === true) {

                BookArray.splice(i, 1);

                window.localStorage.setItem("Books", JSON.stringify(BookArray));

                rem.value = "";

                name[i].parentNode.parentNode.removeChild(name[i].parentNode);

                i = name.length + 1;

            }
            else {
                window.alert("You cancelled to remove this Book now. You can remove it anytime you want");
                i = name.length + 1;
            }

        }

    }
}

document.getElementById("ClearBooks").onclick = function () {
    window.localStorage.clear();
    BookArray.length = 0;

    for (let i = 1; i < document.getElementById("tablebody").childElementCount; i++) {
        let rem = document.getElementById("tablebody");
        rem.innerHTML = `
        <th>sr.</th>
        <th>Name</th>
        <th>Owner</th>
        <th>Type</th>
        `;
    }
}


for (let c of Array.from(document.getElementsByTagName("input"))) {

    if (c.getAttribute("type") == "text") {

        c.value = c.getAttribute("placer");

        c.onfocus = function (e) {
            if (e.target.value == e.target.getAttribute("placer")) {
                e.target.value = "";
            }
        }

        c.onblur = function (e) {
            if (e.target.value == "") {
                e.target.value = e.target.getAttribute("placer");
            }
        }

    }

}