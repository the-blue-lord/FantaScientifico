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

document.addEventListener("DOMContentLoaded", () => {
    const numStars = 100;

    for (let i = 0; i < numStars; i++) {
        let star = document.createElement("div");
        star.classList.add("star");

        var rand = Math.random();

        // Assign plus or minus class randomly
        star.innerHTML = rand < 0.5 ? "+" : "-";
        star.classList.add(rand < 0.5 ? "plus" : "minus");

        // Generate random offset values
        let startX = (Math.random() - 0.5) * 200;
        let startY = (Math.random() - 0.5) * 200;

        if(startX > 0 && startY > 0) continue;
        else if (startY - startX > 100) continue;
        else if (startX - startY > 100) continue;

        var endX = 100;
        var endY = 100;

        if(startX > startY) endY = endY + startY - startX;
        else endX = endX + startX - startY;

        let timingScale = Math.sqrt(((endX-startX)**2)+((endY-startY)**2))/(100*Math.sqrt(2));

        // Set CSS variables dynamically
        star.style.setProperty("--startX", startX + "vw");
        star.style.setProperty("--startY", startY + "vh");
        star.style.setProperty("--endX", endX + "vw");
        star.style.setProperty("--endY", endY + "vh");
        star.style.setProperty("--timingScale", timingScale);

        // Add to the document
        document.getElementById("stars").appendChild(star);
    }
});