document.addEventListener("DOMContentLoaded", function () {


    //eve stands for Everything
    var eve = document.all;

    class NewClass {


        constructor(className, what) {

            this.className = className;
            this.what = what;

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

                        if (k[i].includes(`${this.className}`)) {

                            var abcd = String(k[i]);
                            abcd = String(abcd);

                            k = k[i].replace(`${this.className}`, "");
                            k = k.replaceAll("_", " ");

                            var inc = `.${abcd} { ${this.what}: ${k}; } `;

                            if (document.getElementsByTagName("style")[0]) {

                                if (document.getElementsByClassName(abcd)) {
                                    document.getElementsByTagName("style")[0].innerHTML += inc;
                                }

                            }
                            else {

                                let st = document.createElement("style");
                                st.innerHTML += inc;
                                document.body.appendChild(st);

                            }

                        }

                        else if (k[i].includes(`${tester[0]}`)) {

                            var abcd = String(k[i]);
                            abcd = String(abcd);

                            let hello = k[i].split("_");

                            let m = 1;
                            do {
                                hello[m] = hello[m].substring(0, hello[m].lastIndexOf("-"));
                                var what = this.what + "-" + hello[m];
                                m++;
                            }
                            while (m < hello.length)

                            var l = k[i].substring(k[i].indexOf("-") + 1, k[i].length);
                            l = l.replaceAll("_", " ");

                            var inc = `.${abcd} { ${what}: ${l}; } `;

                            if (document.getElementsByTagName("style")[0]) {

                                if (document.getElementsByClassName(abcd)) {
                                    document.getElementsByTagName("style")[0].innerHTML += inc;
                                }

                            }
                            else {

                                let st = document.createElement("style");
                                st.innerHTML += inc;
                                document.body.appendChild(st);

                            }

                        }

                    }

                }

            }

        }


        measurement() {

            var way;

            var e = Array.from(eve);
            for (let i = 0; i < e.length; i++) {

                if (e[i].getAttribute("class")) {

                    if (String(e[i].getAttribute("class")).includes(this.className)) {

                        let that = String(e[i].getAttribute("class"));
                        let index = that.indexOf(`${this.className}`);
                        let k = String(e[i].getAttribute("class")).substring(index, that.length);

                        k = k.split(" ");
                        for (let i = 0; i < k.length; i++) {
                            if (k[i].includes(`${this.className}`)) {

                                var abcd = String(k[i]);
                                abcd = String(abcd);

                                var meas = ["vw", "vh", "px"];
                                for (let j = 0; j < meas.length; j++) {
                                    if (abcd.includes(meas[j])) {
                                        way = meas[j];
                                    }
                                }


                                k = k[i].replace(`${this.className}`, "");
                                k = k.replaceAll("_", ".");

                            }
                        }


                        for (let j = 0; j < meas.length; j++) {
                            if (abcd.includes(way)) {
                                let content = "";
                                for (let i = 0; i < this.what.length; i++) {

                                    if (k.includes(way)) {
                                        k = k.replace(way, "");
                                    }

                                    if (Array.isArray(this.what)) {
                                        var what = this.what[i];
                                    } else {
                                        var what = this.what;
                                        i = this.what.length + 1;
                                    }

                                    content += `${what}: ${k}${way};`;
                                }

                                var inc = `.${abcd} { ${content} } `;
                            }
                            else {
                                let content = "";
                                way = "px";
                                for (let i = 0; i < this.what.length; i++) {

                                    if (k.includes(meas[j])) {
                                        k = k.replace(meas[j], "");
                                    }


                                    if (Array.isArray(this.what)) {
                                        var what = this.what[i];
                                    } else {
                                        var what = this.what;
                                        i = this.what.length + 1;
                                    }

                                    content += ` ${what}: ${k}${way}; `;


                                }

                                var inc = `.${abcd} { ${content}} `;


                            }
                        }


                        if (document.getElementsByTagName("style")[0]) {

                            if (String(document.getElementsByTagName("style")[0].innerHTML).includes(abcd) === false) {
                                document.getElementsByTagName("style")[0].innerHTML += inc;
                            }

                        }
                        else {

                            let st = document.createElement("style");
                            st.innerHTML += inc;
                            document.body.appendChild(st);

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
    new NewClass("mx-", "margin-left margin-right").measurement();
    new NewClass("my-", "margin-bottom margin-top").measurement();

    //Makes the Bootstrap class for Padding
    new NewClass("padx-", "padding-left padding-right").measurement();
    new NewClass("pady-", "padding-bottom padding-top").measurement();

    //Makes the Bootstrap class for Letter-Spacing
    new NewClass("le-", "letter-spacing").measurement();

    //Makes the Bootstrap class for Border Elements
    new NewClass("bord-", "border").normal();

    //Makes the Bootstrap class for Display
    new NewClass("disp-", "display").normal();

    //Makes the Bootstrap class for Position
    new NewClass("position-", "position").normal();
    childrenOfPosition: {
        new NewClass("top-", "top").measurement();
        new NewClass("bottom-", "bottom").measurement();
        new NewClass("left-", "left").measurement();
        new NewClass("right-", "right").measurement();
    }

    //Makes a Bootstrap class for Cursor
    new NewClass("cur-", "cursor").normal();

    //Makes a Bootstrap class for Font
    new NewClass("fon-", "font").normal();

    //Makes a Bootstrap class for Text
    new NewClass("txt-", "text").normal();

});