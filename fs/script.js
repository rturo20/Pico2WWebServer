function handleClick() {
    alert('Button was clicked on Pico 2W!');
    document.getElementById('content').innerHTML += 
        '<p>Button was clicked at ' + new Date().toLocaleTimeString() + '</p>';
}
