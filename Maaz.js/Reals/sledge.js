const rp=document.createElement("p");
rp.setAttribute("id","rp");
rp.innerHTML="<br><br>"+typeof new Date()+" "+new Date();
document.body.appendChild(rp);
setInterval(function(){
var date=new Date();
rp.innerHTML="<br><br>"+typeof date+" "+date;
},1000);


const num=[40,50,60,80,100];
console.log(num,typeof num);

const str=num[0,2,3].toString();
console.log(str,typeof str);

const strn=String(num);
console.log(strn,typeof strn);

const numer="-45";
console.log(numer,typeof numer,numer.indexOf("5"));

const strnumer=Number(numer)
console.log(strnumer,typeof strnumer,String(strnumer).slice(0,2));
console.log(strnumer.toString().split(""));
console.log(strnumer.toString().replace("-","+").split(""));