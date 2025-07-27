document.addEventListener("DOMContentLoaded", function () {


    //eve stands for Everything
    var eve = document.all;

    class NewClass {


        constructor(className, what, measurement = false) {

            this.className = className;
            this.what = what;
            this.measurement = measurement;

            if (String(this.what).includes(" ")) {
                this.what = this.what.split(" ");
            }

        }

        normal() {

            for (let e of eve) {

                var tester = [`${String(this.className).replaceAll("-", "")}`];
                tester[0] += "_";

                if (e.getAttribute("class")) {

                    let k = String(e.getAttribute("class"));

                    k = k.split(" ");
                    for (let i = 0; i < k.length; i++) {

                        if (k[i].startsWith(`${this.className}`)) {

                            var abcd = String(k[i]);
                            abcd = String(abcd);

                            var l = k[i].substring(k[i].indexOf("-") + 1, k[i].length);
                            l = l.replaceAll("_", " ");
                            l = l.replaceAll("hash-", "#");
                            l = l.replaceAll("bracket-", "(");
                            l = l.replaceAll("-bracket", ")");
                            l = l.replaceAll("comma-", ",");

                            let content = "";
                            loo: for (let i = 0; i < this.what.length; i++) {

                                var what = "";
                                if (Array.isArray(this.what)) {

                                    if (this.measurement === true) {
                                        var way;
                                        way = measure(l);
                                        l += String(way);
                                    }

                                    what += this.what[i] + " ";
                                    content += `${what}: ${l}; `;

                                } else {

                                    if (this.measurement === true) {
                                        var way;
                                        way = measure(l);
                                        l += way;
                                    }

                                    what = this.what;
                                    content += `${what}: ${l}; `;
                                    break loo;

                                }

                            }


                            var inc = `.${abcd} { ${content} } `;


                            if (document.getElementsByTagName("style")[0]) {

                                tester[1] = Array.from(document.getElementsByClassName(abcd));
                                if (tester[1].indexOf(e) < 1) {
                                    document.getElementsByTagName("style")[0].innerHTML += inc;
                                }

                            }
                            else {

                                let st = document.createElement("style");
                                st.innerHTML += inc;
                                document.body.appendChild(st);

                            }

                            function measure(l) {
                                var alph = "abcdefghijklmnopqrstuvwxyz";
                                alph = alph.split("");

                                var isThere = false;;
                                for (var a of alph) {
                                    if (String(l).endsWith(a)) {
                                        isThere = true;
                                    }
                                }

                                if (isThere === true) { return ""; }
                                else { return "px"; }

                            }

                        }

                        else if (k[i].includes(`${tester[0]}`)) {

                            var abcd = String(k[i]);
                            abcd = String(abcd);

                            let hello = k[i].split("_");

                            var l = k[i].substring(k[i].indexOf("-") + 1, k[i].length);
                            l = l.replaceAll("_", " ");


                            let m = 1;
                            do {
                                hello[m] = hello[m].substring(0, hello[m].lastIndexOf("-"));
                                var what = this.what + "-" + hello[m];
                                m++;

                                if (this.measurement === true) {
                                    var way;
                                    way = measure(l);
                                    l += String(way);
                                }

                            }
                            while (m < hello.length)

                            let content = "";
                            content = `${what}: ${l};`;


                            loo: for (let i = 0; i < this.what.length; i++) {

                                what = "";
                                if (Array.isArray(this.what)) {

                                    if (this.measurement === true) {
                                        var way;
                                        way = measure(l);
                                        l += String(way);
                                    }

                                    what += this.what[i] + " ";

                                } else {
                                    what = this.what;
                                    break loo;
                                }
                                content += `${what}: ${l}; `;

                            }

                            var inc = `.${abcd} { ${content} } `;

                            if (document.getElementsByTagName("style")[0]) {

                                tester[1] = Array.from(document.getElementsByClassName(abcd));
                                if (tester[1].indexOf(e) < 1) {
                                    document.getElementsByTagName("style")[0].innerHTML += inc;
                                }

                            }
                            else {

                                let st = document.createElement("style");
                                st.setAttribute("type", "text/css");
                                st.innerHTML += inc;
                                document.body.appendChild(st);

                            }

                            function measure(l) {
                                var alph = "abcdefghijklmnopqrstuvwxyz";
                                alph = alph.split("");

                                var isThere = false;;
                                for (var a of alph) {
                                    if (String(l).endsWith(a)) {
                                        isThere = true;
                                    }
                                }

                                if (isThere === true) { return ""; }
                                else { return "px"; }

                            }

                        }

                    }

                }

            }

        }


    }

    //Makes the Bootstrap class for Background
    new NewClass("bgd-", "background").normal();
    new NewClass("bg-", "background-color").normal();


    //Makes the Bootstrap class for Color
    new NewClass("col-", "color").normal();


    //Makes the Bootstrap class for Margin
    new NewClass("mx-", "margin-left margin-right", true).normal();
    new NewClass("my-", "margin-bottom margin-top", true).normal();

    //Makes the Bootstrap class for Padding
    new NewClass("pad-", "padding", true).normal();
    changingPadding: {
        new NewClass("padx-", "padding-left padding-right", true).normal();
        new NewClass("pady-", "padding-bottom padding-top", true).normal();
    }

    //Makes the Bootstrap class for Letter-Spacing
    new NewClass("le-", "letter-spacing", true).normal();

    //Makes the Bootstrap class for Border Elements
    new NewClass("bord-", "border").normal();

    //Makes the Bootstrap class for Display
    new NewClass("disp-", "display").normal();

    //Makes the Bootstrap class for Position
    new NewClass("position-", "position").normal();
    childrenOfPosition: {
        new NewClass("top-", "top", true).normal();
        new NewClass("bottom-", "bottom", true).normal();
        new NewClass("left-", "left", true).normal();
        new NewClass("right-", "right", true).normal();
    }

    //Makes a Bootstrap class for Cursor
    new NewClass("cur-", "cursor").normal();

    //Makes a Bootstrap class for Font
    new NewClass("fon-", "font").normal();

    //Makes a Bootstrap class for Text
    new NewClass("txt-", "text").normal();

    //Makes Bootstrap class for Width and Height
    new NewClass("wid-", "width", true).normal();
    new NewClass("hei-", "height", true).normal();

    //Makes Bootstrap class for Overflow
    new NewClass("overf-", "overflow").normal();

});