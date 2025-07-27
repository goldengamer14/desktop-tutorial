
function findTheWords(theWord, containers) {


    // var input = inp;
    var word = theWord;

    // btn["onclick"] = () => {

    // words.push(input);
    var docBody = containers;
    // [document.getElementById("searchContainer"), document.getElementsByTagName("div")];//document.body.children];

    for (let k = 0; k < docBody.length; k++) {

        var container = (Array.isArray(docBody[k]) ? docBody[k] : docBody);
        // docBody[k] = Array.from(docBody[k]);
        for (var e of container) {

            // var replacementBody = ["lorem_color: rgba(255,0,0,0.8); background-color: lightgreen; border: 2px solid red; font-weight: bolder; margin: 2px 2px 2px 2px", "ipsum_color: lightgreen; background-color: rgba(255,0,0,0.8); border: 2px solid red; font-weight: bolder; margin: 2px 2px 2px 2px"];
            var replacementBody = [word,
                "color: hsl(0, 75%, 50%); background-color: #afa; border: 2px solid red; font-weight: bolder; margin: 0px"];

            // for (let j = 0; j < replacementBody.length; j++) {

            // replacementBody[j] = String(replacementBody[j]).split("_");

            if (String(e.innerHTML).toLowerCase().includes(replacementBody/*[j]*/[0])) {


                var index = [];

                var f = String(e.innerHTML).split(" ");

                var textHolder = "";


                for (let i = 0; i < f.length; i++) {

                    if (String(f[i]).toLowerCase().includes(replacementBody/*[j]*/[0])) {

                        var validate = (dir, symbol) => {

                            var sym = [symbol];
                            sym[1] = (sym[0] == ">" ? "<" : ">");

                            var index = (dir == "front" ? 1 : dir == "back" ? -1 : null);
                            var str = String(f[i]);
                            console.log(dir, "Str", str);

                            while (!str.includes(sym[0])) {
                                index += (dir == "front" ? 1 : dir == "back" ? -1 : null);
                                str += f[i + index];
                                console.log("%c" + dir, "color:#00e", "Str", str, "f[i]", f[i]);
                            }


                            var valid = (str.includes(sym[1]) ? false : true);

                            return valid;

                        }

                        var back = validate("back", "<");
                        var front = validate("front", ">");

                        console.log("%c" + back, "color:#f00");
                        console.log("%c" + front, "color:#f55");

                        if (back & front) {
                            index[0] = container.indexOf(e);
                            index[1] = String(f[i]).toLowerCase().indexOf(replacementBody/*[j]*/[0]);
                            index[2] = String(f[i]).substring(index[1], index[1] + replacementBody/*[j]*/[0].length);

                            textHolder += String(f[i]).replace(index[2], `<span style="${replacementBody/*[j]*/[1]}">${index[2]}</span> `);
                            console.log("%c" + textHolder, "color:#0f0");

                        } else { continue; }

                    }
                    else { textHolder += String(f[i]) + " " }

                }

                e.innerHTML = textHolder;

            }

            // }
        }

    }
    // }


}