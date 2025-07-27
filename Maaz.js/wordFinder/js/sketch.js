
function ele(element) {
    var e = document.querySelectorAll(String(element));
    return e;
}

// ele("b"),ele("i"), ele("span"), ele("a"), ele("strong"),

var input = document.getElementById("findWord");
var btn = document.getElementById("makeFindWord");
// var words = ["lorem_color: rgba(255,0,0,0.8); background-color: lightgreen; border: 2px solid red; font-weight: bolder; margin: 2px 2px 2px 2px", "ipsum_color: lightgreen; background-color: rgba(255,0,0,0.8); border: 2px solid red; font-weight: bolder; margin: 2px 2px 2px 2px"];
var words = [];

btn.onclick = () => {

    words.push(input.value);
    input.value = "";
    var docBody = [document.getElementById("searchContainer")];//, Array.from(ele("div")), document.body.children];

    for (let k = 0; k < docBody.length; k++) {

        var container = (Array.isArray(docBody[k]) ? docBody[k] : docBody);
        for (var e of container/*[k]*/) {
            console.log(e);

            // var replacementBody = ["lorem_color: rgba(255,0,0,0.8); background-color: lightgreen; border: 2px solid red; font-weight: bolder; margin: 2px 2px 2px 2px", "ipsum_color: lightgreen; background-color: rgba(255,0,0,0.8); border: 2px solid red; font-weight: bolder; margin: 2px 2px 2px 2px"];
            var replacementBody = words.map(e => [e,
                "color: hsl(0, 75%, 50%); background-color: #afa; border: 2px solid red; font-weight: bolder; margin: 0px 0px 0px 0px"]
            );

            for (let j = 0; j < replacementBody.length; j++) {

                // replacementBody[j] = String(replacementBody[j]).split("_");

                if (String(e.innerHTML).toLowerCase().includes(replacementBody[j][0])) {

                    var index = [];
                    var check = [];

                    var f = String(e.innerHTML).split(" ");
                    let textHolder = "";

                    for (let i = 0; i < f.length; i++) {

                        if (String(f[i]).toLowerCase().includes(replacementBody[j][0])) {

                            index[0] = container/*[k]*/.indexOf(e);
                            index[1] = String(f[i]).toLowerCase().indexOf(replacementBody[j][0]);
                            index[2] = String(f[i]).substring(index[1], index[1] + replacementBody[j][0].length);

                            check[0] = e.innerHTML.indexOf(index[2]);
                            check[1] = replacementBody[j][1];

                            console.log(check);
                            console.log(e.innerHTML.substring(check[0] - 5 - check[1].length, check[0]));
                            console.log(e.innerHTML.indexOf(index[2]));

                            if (!e.innerHTML.substring(check[0] - check[1].length - 5, check[0]).includes(check[1])) {
                                textHolder += String(f[i]).replace(index[2], `<span style="${replacementBody[j][1]}">${index[2]}</span> `);
                            }

                        }
                        else { textHolder += String(f[i]) + " " }

                    }

                    e.innerHTML = textHolder;

                }

            }
        }

    }
}