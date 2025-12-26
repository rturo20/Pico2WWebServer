var score = 0, count = 0;

balloon.addEventListener('click', clickedIt);

function clickedIt() {
    score ++;
    scoreBox.innerHTML = 'Score: ' + score;
    balloon.style.height = '1px';
}

function moveIt() {
    balloon.style.height = '160px';
    balloon.style.left = Math.random() * 800 + 'px';
    balloon.style.top = Math.random() * 600 + 'px';
    count ++;
    if (count == 20) {
        alert('Game Over');
        clearInterval(timer);
    }
}

timer = setInterval(moveIt, 1000);
