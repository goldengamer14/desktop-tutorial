let regular = {};

// characters between double / are called regular expressions
regular.g = /Hello, World!/g; // This g is a flag that searches the string globally
regular.i = /heLlO, woRlD!/i; // This i is a flag that makes it case insensitive
regular.gi = /heLlO, woRlD!/gi; // The i and g flags can also be used together

regular.carat = /^hel/i; // This ^ is a symbol that finds if the string starts with the word or not
regular.dollar = /ng$/i; // This $ is a symbol that finds if the string ends with the word or not
regular.dot = /hel../gi; // This . is a symbol that will accept any character at its place.
//                          Even helf5 will work

regular.star = /he*lo/gi; // This * is a symbol will accept one or more character till it starts and ends with it.
//                       Even he3adfwr;l will work because it consists of he in start and l at the end

regular.question = /hel?lo/gi; // This ? is a sympbol that will accept the word even if the letter before ? is'nt there
//                             won't work with heglo, only works with hello and helo

regular.set = {};
regular.set["a-to-z"] = /hel[a-z]o/gi; // This [a-z] is a symbol that will accept any letter in between A and Z
//                                        Also works with 0-9
//                                        Note: Will need [a-zA-Z] as well separately if wants UpperCase as well
//                                        Without an i Flag
regular.set["abcghl*459"] = /hel[abcghl\*459]o/gi; // This [abcghl*459] is a symbol that will accept any letter from abcghl*459
regular.set["^abcghl*459"] = /hel[^abcghl\*459]o/gi; // This [^abcghl*459] is a symbol that won't accept any letter from abcghl*459
//                                                      Will work with any other character (deijkmnopqrstuvwxyz123678!@#$%^&()-_=+)
regular.set["hel{5}o"] = /hel{5}o/gi; // This {2} is a symbol that represents that there can be 2 'l' characters
//                                    Only hello will work as it has 2 'l' and all the others matching
//                                    we don't need to hellllllllllo; we can simply hel{10}o
regular.set[`hel{0,3}o`] = /hel{0,3}o/gi; // This {0,3} is a symbol that represents that there can
//                                             Be 'l' characters only between 0 & 3
//                                             Only someone from helllo or hello or helo or heo will work

regular.group = /(hello){3}/gi; // This (hello){3} is a symbol that represents that there can
//                                             Be only 3 'hello' words
//                                             Only 'hello hello hello ' will work
//                                             ([0-9]{3}cry){4} can also be used

regular.wordCharacter = {};
regular.wordCharacter["w"] = /\wello/gi; // This \w is a wordCharacter that'll only allow
//                                     an alphabet or number or _
//                                     E.g. only hello, 6ello, aello, _ello like words will be accepted
//                                     No need to waste space on '[a-zA-z-0-9_]', use \w instead

regular.wordCharacter["w+lo"] = /\w+lo/gi; // This \w+lo is a wordCharacter that'll allow
//                                     (an alphabet or number or _) + lo
//                                     E.g. fjilwrlo 4123lo 5dfa^e41lo like words will be accepted

regular.wordCharacter["W"] = /\Wlo/gi; // This \W is a wordCharacter that won't allow
//                                     (an alphabet or number or _) + lo
//                                     E.g. &lo, $lo, !lo like words will be accepted

regular.digit = {};
regular.digit["d"] = /91\+ \d{10}/gi; // This \d is a digitCharacter that'll only allow
//                               digits between 0 to 9
//                               E.g. You want a phone number

regular.digit["d+"] = /\d+/gi; // This \d+ is a digitCharacter that'll only allow
//                               digits between 0 to 9 & they can be infinite
//                               E.g. a random number

regular.digit["D"] = /\D{10}/gi; // This \D is a digitCharacter that won't allow
//                               digits between 0 to 9
//                               E.g. asqwtrjqiu'sajklzxjfre#!$%@%$ will be allowed as they ain't digits

regular.space = {};
regular.space["\s"] = /\schup/gi; // This \s is a spaceCharacter that'll allow
//                               All types of spaces
//                               E.g. " chup", "\tchup" like words will be allowed

regular.space["\s+"] = /\s+chup/gi; // This \s+ is a spaceCharacter that'll allow
//                               All types of spaces in infinite amount
//                               E.g. "   chup", "\t\t\t  chup" like words will be allowed

regular.space["\S"] = /\Schup/gi; // This \S is a spaceCharacter that won't allow / will deny
//                               All types of spaces
//                               E.g. "  chup", "\tchup" like words will be denied &
//                                    "chup", "dchup"will be allowed

regular.space["\S+"] = /\S+chup/gi; // This \S+ is a spaceCharacter that won't allow / will deny
//                               All types of spaces in infinite amount
//                               E.g. "  chup", "\t\t   chup" like words will be denied &
//                                    "fwec1234hup", "dadf4chup"will be allowed

regular.wordBoundary = /hello\b/gi; // This \b is a wordBoundary that'll allow the word only if
//                                     The word ends with it
//                                     E.g. 'hello world' will work but 'helloword' won't because:
//                                           The world 'hello' is continued with 'word' and does'nt end
//                                     We can use it as '/\bword\b/' if we want to find a separate word

regular.assertion = {};

regular.assertion["?="] = /hello(?= world)/gi; // This ?= is an assertion that'll only accept a word if
//                                          The ', world' exists at the place
//                                          E.g. 'hello, world' will be allowed while 'hello world' or
//                                                'hello' won't be allowed

regular.assertion["?!"] = /hello(?! world)/gi; // This ?! is an assertion that'll only accept a word if
//                                          The ', world' does'nt exists at the place
//                                          E.g. 'hello' or 'helloword' will be allowed while 'hello, world'
//                                                won't be allowed

for (let exp in regular) {

    if (typeof regular[exp].source == typeof "") {
        console.log(`The Regular Expression ${exp}:`, regular[exp],
            `\nThe source of Regular Expression ${exp}:`, regular[exp].source,
            `\nand the flags of the Regular Expression ${exp}:`, regular[exp].flags);
    } else {
        for (let ex in regular[exp]) {
            console.log(`The Regular Expression ${exp}:`, regular[exp][ex],
                `\nThe source of Regular Expression ${exp}:`, regular[exp][ex].source,
                `\nand the flags of the Regular Expression ${exp}:`, regular[exp][ex].flags);
        }
    }

}

document.getElementsByTagName("h1")[0].innerHTML += "<span style=\"color:#f33\">"
    + / Antything can be done here/.source + "</span>";

const str = "HeLlo world, This is just a 'Hello, World!' and to another Hello, World! hello, woRlD! string";

// Functions to match expressions

const result = {};

// Using exec() Function to find the word in the string
result.exec = {};

// 1. Using g Flag in exec() function
result.exec.g = [];

result["exec"].g.push(regular["g"].exec(str));
result["exec"].g.push(regular["g"].exec(str));
// This will return null as there's no third Hello, World! in the string
result["exec"].g.push(regular["g"].exec(str));

// 2. Using i Flag in exec() function
result.exec.i = [];
result.exec["i"].push(regular["i"].exec(str));

// 3. Using gi Flags Together in exec() function
result.exec.gi = [];

result.exec["gi"].push(regular["gi"].exec(str));
result.exec["gi"].push(regular["gi"].exec(str));

// Using test() Function to find if the word's there or not
//     True for the word's there and False for the word's not there in the string
result.test = [];

// Using the test() Function to know if there's Hello, World! in this string or not
result.test.push(regular["g"].test(str));
result.test.push(regular["g"].test(str));
result.test.push(regular["g"].test(str));

// Using the match Function
result["match"] = {};

// Using the match Function using g Flag
result.match.g = str.match(regular.g);

// Using the match Function using gi Flag
result.match.gi = str.match(regular.gi);

// Using the search() Function to search for the word
// Returns the index of the first word else -1
result.search = [];

result.search.push(str.search(regular.gi));
result.search.push(str.search(regular.gi));
result.search.push(str.search(regular.gi));

// This will return -1 if not found in the string
result.search.push(str.search(/hufopsda/));

// Using the replace() Function to replace the word and it'll also return the whole string
result.replace = {};
result.replace.g = [];
result.replace.i = [];
result.replace.gi = [];

// If global flag is'nt given, then will replace only the first one; else each of words
result.replace.g.push(str.replace(regular.g, /Replacementg/));
result.replace.i.push(str.replace(regular.i, /Replacementi/));
result.replace.gi.push(str.replace(regular.gi, /Replacementgi/));

// Using the '^' carat symbol of the regular expression to see if the string starts with the word
// Returns null if the string does'nt starts with the word
result.carat = regular.carat.exec(str)

// Using the '$' dollat symbol of the regular expression to see if the string ends with the word
// Returns null if the string does'nt ends with the word
result.dollar = regular.dollar.exec(str);

// Using the '.' dot symbol so any character there can be accepted
result.dot = [];

do {
    let string = "hel45" + str + "heldo";
    result.dot.push(regular.dot.exec(string));

    if (result.dot[result.dot.length - 1] != null) {
        continue;
    } else { break; }
} while (true);

// Using the '*' star symbol to match any characters that come in between
result.star = [];

do {
    let string = str + " and to goodbye, heasrt51213lo he5lo  he*lo";
    result.star.push(regular.star.exec(string));

    if (result.star[result.star.length - 1] != null) {
        continue;
    } else { break; }
} while (true);

// Using the '?' question mark to make a letter optional, even if its not there it'll work
result.question = [];

do {
    let string = "helo to" + str;
    result.question.push(regular.question.exec(string));

    if (result.question[result.question.length - 1] != null) {
        continue;
    } else { break; }
} while (true);

result.set = {};

// Using the '[a-z]' set to accept all the letters between A to Z
result.set["a-to-z"] = [];

do {
    let string = "helfo to" + str + "and for bye, helio";
    result.set["a-to-z"].push(regular.set["a-to-z"].exec(string));

    if (result.set["a-to-z"][result.set["a-to-z"].length - 1] != null) {
        continue;
    } else { break; }
} while (true);

// Using the '[abcghl*459]' set to accept any letter from abcghl*459
result.set["abcghl*459"] = [];
do {
    let string = "helfo and hel4o to" + str + "and for bye, helao and hel*o";
    result.set["abcghl*459"].push(regular.set["abcghl*459"].exec(string));

    if (result.set["abcghl*459"][result.set["abcghl*459"].length - 1] != null) {
        continue;
    } else { break; }
} while (true);

// Using the '[^abcghl*459]' set to ignore any letter from abcghl*459
result.set["^abcghl*459"] = [];
do {
    let string = "helko and hel%o to" + str + "and for bye, hel5o and hel0o";
    // The hel5o here will be ignored as it has 5 which is'nt acceptable
    result.set["^abcghl*459"].push(regular.set["^abcghl*459"].exec(string));

    if (result.set["^abcghl*459"][result.set["^abcghl*459"].length - 1] != null) {
        continue;
    } else { break; }
} while (true);

// Using the '{5}' set to allow 5 characters
result.set["hel{5}o"] = [];
do {
    let string = "helllllo and hel{5}o to" + str + "and for bye, helllllo and hel0o";
    // The hel0o and hel{5}o here will be ignored as they are'nt helllllo
    result.set["hel{5}o"].push(regular.set["hel{5}o"].exec(string));

    if (result.set["hel{5}o"][result.set["hel{5}o"].length - 1] != null) {
        continue;
    } else { break; }
} while (true);

// Using the '{0,3}' set to allow only 0,1,2 & 3 characters
result.set["hel{0,3}o"] = [];
do {
    let string = "heo and hel{0,3}o to" + str + "and for bye, helllo and hel0o for hellllllo";
    // The hel0o and hel{0,3}o here will be ignored as they are'nt helllo heo
    result.set["hel{0,3}o"].push(regular.set["hel{0,3}o"].exec(string));

    if (result.set["hel{0,3}o"][result.set["hel{0,3}o"].length - 1] != null) {
        continue;
    } else { break; }
} while (true);

// Using the '(hello){3}' set to allow only 3 'hello' words
result.group = [];
do {
    let string = "hellohello hello, helloHelLohELlo" + str + "hellohellohello";
    // The 'hellohello hello,' will be ignored
    result.group.push(regular.group.exec(string));

    if (result.group[result.group.length - 1] != null) {
        continue;
    } else { break; }
} while (true);

// Using the wordCharacters
result.wordCharacter = {};

// Using the '\w' set to allow an alphabet, or number, or _
result.wordCharacter["w"] = [];
do {
    let string = "gello, _ello 4ello" + str + "!ello #ello zello";
    // The '!ello' & '#ello' will be ignored
    result.wordCharacter["w"].push(regular.wordCharacter["w"].exec(string));

    if (result.wordCharacter["w"][result.wordCharacter["w"].length - 1] != null) {
        continue;
    } else { break; }
} while (true);

// using '\w+lo' to allow any wordCharacter(alphabet or number or '_') before 'lo'
result.wordCharacter["w+lo"] = [];
do {
    let string = "gello, _ello 4ello" + str + "rfq234llo 15adf54lo saxcsd!lo";
    // The 'saxcsd!lo' will be ignored
    result.wordCharacter["w+lo"].push(regular.wordCharacter["w+lo"].exec(string));

    if (result.wordCharacter["w+lo"][result.wordCharacter["w+lo"].length - 1] != null) {
        continue;
    } else { break; }
} while (true);

// using '\W' to allow any non-wordCharacter('!@#$%^&*()-+=') before 'lo'
result.wordCharacter["W"] = [];
do {
    let string = "gello, _ello 4ello" + str + "$lo @lo !lo";
    // The 'gello', '_ello' & '4ello' will be ignored
    result.wordCharacter["W"].push(regular.wordCharacter["W"].exec(string));

    if (result.wordCharacter["W"][result.wordCharacter["W"].length - 1] != null) {
        continue;
    } else { break; }
} while (true);

// Using the digitCharacters

// Using '\d' to only allow digits(0 to 9) after 91 for an Indian phone number
regularExpressionTest("digit", "91+ 9561179786, 91+ 9595401301, ", " 91+ dsaf4383%^, 91+ 93201E$1, 91+ 9823979991", ["d"]);
// Using '\d+' to allow infinite digits
regularExpressionTest("digit", "91+ 9561179786, 91+ 9595401301, ", " 91+ dsaf4383%^, 91+ 93201E$1, 91+ 9823979991", ["d+"]);
// Using '\D' to allow non-digitCharacters
regularExpressionTest("digit", "asfrrtqsfthwlk;d, Zoya_Zain@gmail.com ", " dsaf4383%*()#!^, 93201E$1, 4141508956752", ["D"]);

// Using the spaceCharacters

// Using '\s' to allow all types of spaces("\t", "   ")
regularExpressionTest("space", "abe chup ", " chal\tchUP, hojaChUp, 4141508956752", ["\s"]);
// Using '\s+' to allow infinite amount of all types of spaces("\t", "   ")
regularExpressionTest("space", "\t chupabe    chup ", " chal\t\t chUP, hojaChUp, 4141508956752", ["\s+"]);
// Using '\S' to deny all types of spaces("\t", "   ") & allow non-spaceCharacters
regularExpressionTest("space", "abe nikaljachup ", " chal\tchUP, hojaChUp, 4141508956752", ["\S"]);
// Using '\S+' to deny all types of spaces("\t", "   ") & allow infinite amount of non-spaceCharacters
regularExpressionTest("space", "abe nikaljachup ", " chal\tchUP, hojaChUp, hatChuP, 4141508956752", ["\S+"]);

// Using the wordBoundary '\b' to only allow the word if the word ends with it
regularExpressionTest("wordBoundary", "HElLO world, heLO, world, hello", "HELLO ");

// Using the assertion '?=' to only allow the word if the word exists there
regularExpressionTest("assertion", "HElLO world, helLO, world, hello", "HELLO ", ["?="]);

// Using the assertion '?!' to only allow the word if the word does'nt exists there
regularExpressionTest("assertion", "HElLO world, helLO, world, hello", "HELLO ", ["?!"]);

console.error(result);

function regularExpressionTest(obj, string1, string2, subObjs) {
    let res;
    let reg;
    if (Object.prototype.toString.call(regular[obj]) == "[object Object]") {

        Object.prototype.toString.call(result[obj]) == "[object Object]" ? null : result[obj] = {};

        res = result[obj][subObjs[0]] = [];
        reg = regular[obj][subObjs[0]];

        if (subObjs.length > 0) {
            for (let i = 1; i < subObjs.length; i++) {
                if (i != subObjs.length - 1) {
                    res = res[subObjs[i]] = {};
                } else {
                    res = res[subObjs[i]] = [];
                    reg = reg[subObjs[i]];
                }
            }
        }

    } else {
        res = result[obj] = [];
        reg = regular[obj];
    }

    do {
        let string = string1 + str + string2;
        res.push(reg.exec(string));

        if (res[res.length - 1] != null) {
            continue;
        } else { break; }
    } while (true);
}