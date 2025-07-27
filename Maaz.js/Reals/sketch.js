console.time(`starting`);
const custom = document.querySelector("#custom");
const select = document.querySelector("#select");
const customopt = document.querySelector("#customopt");

select.addEventListener("change", function () {
  document.body.style.backgroundColor = select.value;
  select.style.backgroundColor = select.value;
});

custom.addEventListener("input", function () {
  customopt.innerHTML = custom.value;
  customopt.value = custom.value;
  customopt.style.backgroundColor = custom.value;
  if (select.value == customopt.value) {
    select.style.backgroundColor = customopt.value;
    custom.style.color = custom.value;
    document.body.style.backgroundColor = customopt.value;
  }
});
const btn = document.createElement("button");
btn.setAttribute("id", "btn");
btn.innerHTML = "tester";
document.body.appendChild(btn);

btn.addEventListener("click", function () {
  console.log("it was just a test");
});

let x = document.querySelector("#x");
let y = document.querySelector("#y");
const sum = document.querySelector("#sum");

x.addEventListener("input", function () {
  let xn = parseInt(x.value);
  let yn = parseInt(y.value);
  if (x.value == "") {
    xn = 0;
  }
  sum.innerHTML = xn + yn;
})

y.addEventListener("input", function () {
  let xn = parseInt(x.value);
  let yn = parseInt(y.value);
  if (y.value == "") {
    yn = 0;
  }
  sum.innerHTML = xn + yn;
})

document.addEventListener("dblclick", function () {
  const pertester = document.querySelector("#pertester");
  pertester.innerHTML += "<b>s</b>";
});

document.addEventListener("dblclick", function () {
  let pertester = "this is not an html";
  pertester = pertester.concat("<b>s</b>");
  for (var i = 0; i < pertester.length; i++) {
    console.log(pertester, pertester.lastIndexOf("h"));//,pertester[i],pertester.charAt(i),pertester[i].includes(" "),pertester.substring(0,i));
  }
});

console.timeEnd(`starting`);