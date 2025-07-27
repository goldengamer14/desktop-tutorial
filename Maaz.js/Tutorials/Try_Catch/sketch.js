const input = get("#numInput");

input.onchange = function () {

    const a = input.value;
    new Promise((resolve, reject) => {
        if (Number(a)) {

            if (Number(a) < 100 || Number(a) > 200) {
                reject("WTH?");
            } else {
                resolve("Okay, your work will be done");
            }

        } else {
            reject("Please Give a valid Number");
        }
    }).then(r => {
        console.log(r);
        get("#response").innerHTML = r;
        get("#response").style.color = "#ff0";
    }).catch(err => {
        const response = get("#response");
        response.innerHTML = err;
        response.style.color = "#f00";
        throw new Error(err);
    });

}

const anotherInput = get("#anotherInput");
const getData = get("#getData");
const sendData = get("#sendData");

getData.onclick = function () {
    new Promise(async (resolve, reject) => {

        try {
            const file = anotherInput.value;
            await fetch(file).then(response => response.json())
                .then(json => resolve(json.data));
        } catch (error) {
            reject(error.name + "Something went wrong in getData\n\t" + error.message);
        } finally {
            console.warn("Okay finally you got your data");
        }

    }).then(response => console.log(response))
        .catch(err => { throw new Error(err); });
}

sendData.onclick = async function () {
    try {
        const file = anotherInput.value;
        await fetch(file, {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: get("#param").innerHTML
        }).then(r => r.json()).then(r => console.log(r));
    } catch (error) {
        throw new Error(error.name + "Something went wrong in sendData:\n\t" + error.message);
    } finally {
        console.warn("Okay finally you sent your data");
    }
}