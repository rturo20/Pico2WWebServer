function toggleServo() {
    const switchElement = document.getElementById('servoSwitch');
    const statusElement = document.getElementById('status');
    
    // Get the current page URL to determine the server address
    const baseUrl = window.location.origin;
    
    if (switchElement.checked) {
        // Send request to turn servo ON
        fetch(`${baseUrl}/servo?action=on`)
            .then(response => {
                if (response.ok) {
                    statusElement.textContent = 'Servo: ON';
                    statusElement.style.color = '#4CAF50';
                    console.log('Servo turned ON');
                } else {
                    console.error('Failed to turn servo ON');
                    switchElement.checked = false;
                }
            })
            .catch(error => {
                console.error('Error:', error);
                switchElement.checked = false;
                alert('Failed to communicate with Pico. Please check connection.');
            });
    } else {
        // Send request to turn servo OFF
        fetch(`${baseUrl}/servo?action=off`)
            .then(response => {
                if (response.ok) {
                    statusElement.textContent = 'Servo: OFF';
                    statusElement.style.color = '#333';
                    console.log('Servo turned OFF');
                } else {
                    console.error('Failed to turn servo OFF');
                    switchElement.checked = true;
                }
            })
            .catch(error => {
                console.error('Error:', error);
                switchElement.checked = true;
                alert('Failed to communicate with Pico. Please check connection.');
            });
    }
}
