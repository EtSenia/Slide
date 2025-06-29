const slides = document.querySelectorAll(".slide");
const slider = document.querySelector(".slider");
const controls = document.querySelector(".controls");

let current = 0;

function showSlide(index, direction) {
  const currentSlide = slides[current];
  currentSlide.classList.remove("active");

  if (direction === "next") {
    currentSlide.classList.add("exit-left");
  } else {
    currentSlide.classList.add("exit-right");
  }

  setTimeout(() => {
    currentSlide.classList.remove("exit-left", "exit-right");
  }, 600);

  current = index;
  const nextSlide = slides[current];
  nextSlide.classList.add("active");
}

function nextSlide() {
  if (current === slides.length - 1) {
    return;
  }
  const next = (current + 1) % slides.length;
  showSlide(next, "next");
}

function prevSlide() {
  if (current === 0) {
    return;
  }
  const prev = (current - 1 + slides.length) % slides.length;
  showSlide(prev, "prev");
}

document.addEventListener("keydown", (event) => {
  if (event.key === "ArrowRight") {
    nextSlide();
  } else if (event.key === "ArrowLeft") {
    prevSlide();
  }
});

function adjustSlideSize() {
  const vw = window.innerWidth;
  const vh = window.innerHeight;
  let maxWidth = vw * 0.9;
  let maxHeight = vh - 75;
  if (document.fullscreenElement) {
    maxWidth = document.fullscreenElement.clientWidth;
    maxHeight = document.fullscreenElement.clientHeight;
  }

  const newScaleW = maxWidth / 1920;
  const newScaleH = maxHeight / 1080;
  const newScale = Math.min(newScaleW, newScaleH);

  slider.style.width = `${maxWidth}px`;
  for (let i = 0; i < slides.length; i++) {
    slides[i].style.transform = `scale(${newScale})`;
  }
  console.log(maxWidth);
}

function checkFullscreen() {
  if (document.fullscreenElement) {
    controls.style.display = "none";
  } else {
    controls.style.display = "block";
    adjustSlideSize();
  }
}

window.addEventListener("fullscreenchange", checkFullscreen);

function toggleFullscreen() {
  if (!document.fullscreenElement) {
    document.documentElement.requestFullscreen().catch((err) => {
      console.log("Error al intentar activar pantalla completa: " + err);
    });
  } else {
    if (document.exitFullscreen) {
      document.exitFullscreen().catch((err) => {
        console.log("Error al intentar salir de pantalla completa: " + err);
      });
    }
  }
}

window.addEventListener("resize", adjustSlideSize);
adjustSlideSize();
slides[0].classList.add("active");
