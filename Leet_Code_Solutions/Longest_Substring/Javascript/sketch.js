let lengthOfLongestSubstring = function (s) {
    let subs = [""];
    // let index = 0;

    s.split("").forEach((e, ind, s) => {
        // for (let i = 0; i < s.length - ind; i++)
        // if (subs.length == 0) if (subs[0].includes(s[i]))
        let minus = 0;
        if (subs[subs.length - 1].includes(e)) { subs.push(e); minus++; }
        else subs[subs.length - 1] += e;

        console.log(e, subs[subs.length - 1 - minus].includes(e), "exists in ", subs[subs.length - 1 - minus]);
    });
    console.log(s.split(""));

    let subLengths = [];
    subs.forEach(e => subLengths.push(e.length));
    console.log("string: ", s, "subLengths: ", subLengths, "subs: ", subs);
    return String(subs[subLengths.indexOf(Math.max(...subLengths))]);
};

console.log(lengthOfLongestSubstring("abcabcbb"));
console.log(lengthOfLongestSubstring("bbbbb"));
console.log(lengthOfLongestSubstring("pwwkew"));