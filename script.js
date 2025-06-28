const slides = document.querySelectorAll('.slide');
let current = 0;

function showSlide(index, direction) {
  const currentSlide = slides[current];
  currentSlide.classList.remove('active');

  // Animar slide saliente
  if (direction === 'next') {
    currentSlide.classList.add('exit-left');
  } else {
    currentSlide.classList.add('exit-right');
  }

  setTimeout(() => {
    currentSlide.classList.remove('exit-left', 'exit-right');
  }, 600);

  current = index;
  const nextSlide = slides[current];
  nextSlide.classList.add('active');
}

function nextSlide() {
  const next = (current + 1) % slides.length;
  showSlide(next, 'next');
}

function prevSlide() {
  const prev = (current - 1 + slides.length) % slides.length;
  showSlide(prev, 'prev');
}

// 🔑 Manejar teclas de flechas
document.addEventListener('keydown', (event) => {
  if (event.key === 'ArrowRight') {
    nextSlide();
  } else if (event.key === 'ArrowLeft') {
    prevSlide();
  }
});
