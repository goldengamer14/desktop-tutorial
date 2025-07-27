let arr;

document.getElementById("doit").onclick = ()=> {
    const numbers = prompt("Enter the Numbers");
    let products = ascendDecend(numbers);

    document.getElementById("showIt").innerHTML =
        "<span class=\"decend\">Ascending Order: </span>" + JSON.stringify(products[0]);
    document.getElementById("showIt").innerHTML +=
        "<br><br><span class=\"ascend\">decending Order: </span>" + JSON.stringify(products[1]);
}

// function (string) {
//     arr = string.split(",");
//     arr.forEach((e, i, ar) => ar[i] = Number.parseFloat(e));

//     let decendingArr = [];

//     const minArr = [...arr];
//     for (let i = 0; i < arr.length; i++) {
//         const minValue = Math.min(...minArr);
//         const minIndex = minArr.indexOf(minValue);

//         decendingArr.push(minValue);
//         minArr.splice(minIndex, 1);
//     }


//     let ascendingArr = [...arr].map((e, i, arr) => {
//         const pushable = Math.max(...arr);
//         arr[arr.indexOf(pushable)] = "";
//         return pushable;
//     });


//     document.getElementById("showIt").innerHTML =
//         "<span class=\"decend\">Ascending Order: </span>" + JSON.stringify(decendingArr);
//     document.getElementById("showIt").innerHTML +=
//         "<br><br><span class=\"ascend\">decending Order: </span>" + JSON.stringify(ascendingArr);
//     console.log(decendingArr);
//     console.log(ascendingArr);
// }