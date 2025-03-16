function enterFullscreen() {
    document.documentElement.requestFullscreen();
    document.getElementById("fullscreen").style.display = "none";
    document.getElementById("exit-fullscreen").style.display = "block";
}

function enterPartialscreen() {
    document.exitFullscreen();
    document.getElementById("fullscreen").style.display = "block";
    document.getElementById("exit-fullscreen").style.display = "none";
}