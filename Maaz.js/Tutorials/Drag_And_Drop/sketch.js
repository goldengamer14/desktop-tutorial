console.log("Hello, World!");

// Declaring variables
const containers = document.getElementsByClassName("blankBox");
const images = document.getElementsByClassName("imgHolders");

Array.from(images).forEach(image => {
    // Event listener to trigger the function on starting to drag
    image.ondragstart = e => {
        // console.log("It's draggy time");
        setTimeout(() => e.target.classList.add("hide"), 0);
    };

    // Event listener to trigger the function after leaving drag
    image.ondragend = e => {
        // console.log("End of draggy time");
        setTimeout(() => e.target.classList.remove("hide"), 0);
    };
});

// My try:
Array.from(containers).forEach(e => {
    e.ondragover = f => {
        f.preventDefault();
        e.classList.add("draggedOver");
    };

    e.ondragenter = () => {
        e.classList.add("draggedOver");
    };

    e.ondragleave = () => {
        e.classList.remove("draggedOver");
    };

    e.ondrop = () => {
        // console.log("drop on", e);

        Array.from(containers).forEach(box => {
            box.firstElementChild.classList.contains("imgBox") ? box.firstElementChild.classList.remove("imgBox") : null
        });

        e.classList.remove("draggedOver");
        e.firstElementChild.classList.add("imgBox");
        e.firstElementChild.draggable = "true";

    };
});

console.log("The images:", images, "\nThe containers:", containers);