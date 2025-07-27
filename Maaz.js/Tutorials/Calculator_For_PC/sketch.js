
document.getElementById("calculate").onclick = function () {

    let equationStr = window.prompt("Enter the equation:");
    let equation;
    let nums = "1234567890";
    nums = nums.split("");
    let validEquation = false;
    let rejects = "abcdefghijklmnopqrstuvwxyz_=<>?.,: ";
    rejects = rejects.split("");

    for (let i = 0; i < nums.length; i++) {
        const n = nums[i];
        if (String(equationStr).includes(String(n))) {
            validEquation = true;
            break;
        }
    }
    validEquation === true ? equation = equationStr : equation = 0;

    if (validEquation === true) {
        for (const r of rejects) {
            equation = String(equation).replaceAll(r, "");
        }
        equation = eval(equation);

    } else { equation = 0; }

    window.alert("The Answer of your Equation is:\n" + equation);

}