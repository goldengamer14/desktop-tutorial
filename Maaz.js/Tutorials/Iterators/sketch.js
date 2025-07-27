console.log("Hello, World!");

const array = ["Audi", "Bugatti", "Ford", "Swift"];
console.log(array);

function Iterator(arr) {
    let index = 0;
    return {
        all: () => {
            let returnValue = [];
            index < arr.length - 1 ? null : index = 0;
            while (index < arr.length) {
                returnValue.push({ value: arr[index] });
                index++;
            }
            index = 0;
            return returnValue;
        },

        next: () => {
            if (index < arr.length) {
                index++;
                return arr[index - 1];
            } else {
                return "OK, you extracted all the values from your array";
            }
        },

        previous: () => {
            if (index > 0) {
                index = index - 1;
                return arr[index];
            } else {
                return "You're trying to go beyond the array";
            }
        },

        reset: () => {
            index = 0;
            return "Ok, we're back to the start";
        }
    }
}

let IteratorOfCars = Iterator(array);
for (let i = 0; i < array.length + 1; i++) {
    console.log(IteratorOfCars.next());
}

console.log(IteratorOfCars.all());
IteratorOfCars.reset();