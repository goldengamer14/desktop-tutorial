const nav = document.getElementById("navbar");
const menuToggle = document.getElementById("menu-toggle");
const overlay = document.getElementById("overlay");

const media = window.matchMedia("(max-width: 768px)");

if (media.matches) {
    nav.classList.remove("active");
    overlay.style.display = "none";
    nav.setAttribute("aria-hidden", "true");
    nav.setAttribute("inert", "true");
    console.log("Initial load on mobile. Menu set to inactive.");
} else {
    nav.setAttribute("aria-hidden", "false");
    nav.setAttribute("hidden", "false");
    nav.removeAttribute("inert");
    console.log("Initial load on desktop. Menu set to inactive.");
}

menuToggle.addEventListener("click", toggleMenu);

overlay.addEventListener("click", toggleMenu);

function toggleMenu() {
    nav.classList.toggle("active", !nav.classList.contains("active"));
    overlay.style.display = nav.classList.contains("active") ? "block" : "none";
    console.log("Menu toggled. Current state:", nav.classList.contains("active") ? "Active" : "Inactive");

    menuToggle.setAttribute("aria-expanded", nav.classList.contains("active"));
    overlay.setAttribute("aria-hidden", !nav.classList.contains("active"));
    nav.setAttribute("aria-hidden", !nav.classList.contains("active"));
    nav.setAttribute("inert", !nav.classList.contains("active"));

    if (nav.classList.contains("active")) {
        nav.removeAttribute("inert");
    } else {
        nav.setAttribute("inert", "true");
    }

}

function updateMenuState(event) {
    const isMobile = event.matches;

    nav.classList.remove("active");
    overlay.style.display = "none";
    nav.setAttribute("aria-hidden", isMobile ? "false" : "true");
    menuToggle.setAttribute("aria-expanded", isMobile ? "false" : "true");
    nav.setAttribute("hidden", !isMobile ? "false" : "true");
    if (!isMobile) {
        nav.removeAttribute("inert");
    }
    console.log("Media query changed. Current matches:", event.matches);
}

media.addEventListener("change", e => updateMenuState(e));